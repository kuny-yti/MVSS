#include "glx_context.h"
#include "interface/iwcontext.h"
#include "opengl/opengl_mx.h"

namespace impl {

#ifndef GLX_RGBA_FLOAT_TYPE
#define GLX_RGBA_FLOAT_TYPE 0x20B9
#endif

#ifndef GLX_RGBA_FLOAT_BIT
#define GLX_RGBA_FLOAT_BIT 0x00000004
#endif

namespace rctx {

Display *display(ircontext *rctx){return rctx->wcontext->parent<Display*>();}
Window root(ircontext *rctx) {return DefaultRootWindow(display(rctx));}
int screen(ircontext *rctx) {return DefaultScreen(display(rctx));}
int depth(ircontext *rctx) {return DefaultDepth(display(rctx), screen(rctx));}
XVisualInfo* config(ircontext *rctx){return rctx->config<XVisualInfo*>();}
GLXContext context(ircontext *rctx){return rctx->context<GLXContext>();}
Window handle(ircontext *rctx){return rctx->wcontext->handle<Window>();}

}
struct xserver
{
    bool init_glxew(Display *display, Window root, int screen)
    {
        bs_vector<int> attributes;
        attributes.append( GLX_RGBA );
        attributes.append( None );

        XVisualInfo *visualInfo = glXChooseVisual(display, screen, attributes.data());
        if( !visualInfo )
            return false;

        //context
        GLXContext context = glXCreateContext(display, visualInfo, 0, True );
        if( !context )
            return false;

        // window
        XSetWindowAttributes wa;
        wa.colormap = XCreateColormap(display, root, visualInfo->visual, AllocNone );
        wa.background_pixmap = None;
        wa.border_pixel = 0;
        Window drawable = XCreateWindow(display, root, 0, 0, 16, 16,
                                        0, visualInfo->depth, InputOutput, visualInfo->visual,
                                        CWBackPixmap | CWBorderPixel | CWColormap, &wa );
        if(!drawable)
            return false;

        XFree( visualInfo );
        XSync(display, False );

        glXMakeCurrent( display, drawable, context );

        const GLenum result = glxewInit();
        bool success = result == GLEW_OK;
        if( !success )
            return false;

        XSync(display, False );
        glXDestroyContext(display, context );
        XDestroyWindow(display, drawable );

        return success;
    }
};
struct window
{
    Window create_PBuffer( GLXFBConfig* fbConfig ,ircontext *rc)
    {
        // Create PBuffer
        const int attributes[] = { GLX_PBUFFER_WIDTH, rc->format.viewport.width,
                                   GLX_PBUFFER_HEIGHT, rc->format.viewport.height,
                                   GLX_LARGEST_PBUFFER, True,
                                   GLX_PRESERVED_CONTENTS, True,
                                   0 };

        return glXCreatePbuffer( rctx::display(rc), fbConfig[0], attributes );
    }
};
struct context : xserver, window
{
    uint glXNVSwapGroup;
    bool is_initglxew;

    context(ircontext *_self):
        xserver()
    {
        if (!_self->wcontext)
        {
            log_error("Rendering context No context window.");
            return ;
        }
        if (!_self->wcontext->parents)
        {
            log_error("X server not connection");
            return ;
        }

        is_initglxew = init_glxew(rctx::display(_self), rctx::root(_self), rctx::screen(_self));
        if (!is_initglxew)
        {
            log_error("Initialize GLXEW failed.");
            return ;
        }

        _self->wcontext->rcontext = _self;
    }

    GLXContext create (Display *displays, GLXFBConfig* fbConfig,GLXContext share, const tRenderFormats &rformat)
    {
        int type = GLX_RGBA_TYPE;
        if (rformat.target == Render_PBuffer &&
            (rformat.color_bits == Channel_RGBA16 || rformat.color_bits == Channel_RGBA32))
            type = GLX_RGBA_FLOAT_TYPE;

        return glXCreateNewContext(displays, fbConfig[0], type, share, True );
    }
    GLXFBConfig* chooseconfig(Display *displays, int screens, const tRenderFormats &ormat)
    {
        if (!displays)
            return NULL;

        // build attribute list
        bs_vector<int> attributes;
        tRenderFormats rformat = ormat;

        switch(rformat.target)
        {
          case Render_PBuffer:
            attributes.append ( GLX_DRAWABLE_TYPE );
            attributes.append ( GLX_PBUFFER_BIT );
            break;

          default:
            // no break;
          case Render_Window:
          case Render_Image: // needs fbConfig with visual for dummy window
          case Render_OffScreen: // No typo - FBO needs fbConfig with visual for dummy window
            attributes.append ( GLX_X_RENDERABLE );
            attributes.append ( True );
        }

        if (rformat.color_bits)
        {
            if (rformat.target == Render_OffScreen || rformat.target == Render_Image)
                rformat.color_bits = Channel_RGBA8;  // Create FBO dummy window with 8bpp

            else if (rformat.color_bits == Channel_RGBA16 || rformat.color_bits == Channel_RGBA32)
            {
                attributes.append (GLX_RENDER_TYPE);
                attributes.append (GLX_RGBA_FLOAT_BIT);
            }
            else
                rformat.color_bits = Channel_RGBA8;

            attributes.append( GLX_RED_SIZE );
            attributes.append( RedChannelBit(rformat.color_bits & Channel_RedMask) );
            attributes.append( GLX_GREEN_SIZE );
            attributes.append( GreenChannelBit(rformat.color_bits & Channel_GreenMask ));
            attributes.append( GLX_BLUE_SIZE );
            attributes.append( BlueChannelBit(rformat.color_bits & Channel_BlueMask) );

            if ((rformat.color_bits & Channel_AlphaMask) && rformat.has_alpha)
            {
                attributes.append( GLX_ALPHA_SIZE );
                attributes.append( AlphaChannelBit(rformat.color_bits & Channel_AlphaMask) );
            }
        }

        if (rformat.depth_bits)
        {
            attributes.append (GLX_DEPTH_SIZE );
            attributes.append (rformat.depth_bits );
        }
        if ( rformat.stencil_bits)
        {
            attributes.append (GLX_STENCIL_SIZE );
            attributes.append (rformat.stencil_bits );
        }

        if (rformat.accum_bits)
        {
            attributes.append( GLX_ACCUM_RED_SIZE );
            attributes.append( RedChannelBit(rformat.accum_bits & Channel_RedMask));
            attributes.append( GLX_ACCUM_GREEN_SIZE );
            attributes.append( GreenChannelBit(rformat.accum_bits & Channel_GreenMask) );
            attributes.append( GLX_ACCUM_BLUE_SIZE );
            attributes.append( BlueChannelBit(rformat.accum_bits & Channel_BlueMask) );
            if (rformat.has_alpha)
            {
                attributes.append( GLX_ACCUM_ALPHA_SIZE );
                attributes.append( AlphaChannelBit(rformat.accum_bits & Channel_AlphaMask) );
            }
        }

        if ( rformat.samples >= 0 && rformat.target != Render_OffScreen )
        {
            attributes.append( GLX_SAMPLE_BUFFERS );
            attributes.append( 1 );
            attributes.append( GLX_SAMPLES );
            attributes.append( rformat.samples );
        }
        if( rformat.has_stereo && rformat.enable_stereo && rformat.target == Render_Window)
        {
            attributes.append( GLX_STEREO );
            attributes.append( true );
        }
        if( rformat.swap == Swap_DoubleBuffer)
        {
            attributes.append( GLX_DOUBLEBUFFER );
            attributes.append( true );
        }
        if (rformat.has_debug)
        {
            attributes.append( GLX_CONTEXT_FLAGS_ARB );
            attributes.append( GLX_CONTEXT_DEBUG_BIT_ARB );
        }
        attributes.append( None );

        bs_vector<int> backoffAttributes;
        if ( rformat.target == Render_Window)
        {
            if( rformat.swap == Swap_DoubleBuffer )
                backoffAttributes.append( GLX_DOUBLEBUFFER );
        }

        if( rformat.stencil_bits)
            backoffAttributes.append( GLX_STENCIL_SIZE );

        int nConfigs = 0;
        GLXFBConfig* configs = glXChooseFBConfig(displays, screens, &attributes[0], &nConfigs );

        while (( nConfigs == 0 || !configs ) && !backoffAttributes.is_empty())
        {
            const int attribute = backoffAttributes.back();
            backoffAttributes.pop_back();

            int val = attributes.find(attribute);
            if (val < 0)
                return NULL;
            attributes.remove(val, 2);
            configs = glXChooseFBConfig(displays, screens, &attributes[0], &nConfigs );
        }

        return configs;
    }

    void initSwapSync(bool s, const tRenderFormats &rformat)
    {
        if(rformat.target != Render_Window)
            return;

        if (!rformat.swap_sync)
            return ;

        if (s)
        {
            if( GLXEW_SGI_swap_control )
                glXSwapIntervalSGI ( (rformat.swap_sync <= 0) ? 1 : rformat.swap_sync );
            else
                printf("GLX_SGI_swap_control not supported, ignoring window swapsync hint");
        }
    }

    int joinNVSwapBarrier(Display *displays, Window handles, int screens,  const uint group, const uint barrier)
    {
        if( group == 0 )
            return -1;

        if ( !GLXEW_NV_swap_group )
        {
            printf( "NV Swap group extension not supported \n");
            return -1;
        }

        uint32_t maxBarrier = 0;
        uint32_t maxGroup = 0;

        glXQueryMaxSwapGroupsNV(displays, screens, &maxGroup, &maxBarrier );

        if(group > maxGroup)
        {
            printf ("Failed to initialize GLX_NV_swap_group: requested group %d "
                    "greater than maxGroups (%d)\n", group, maxGroup);
            return -1;
        }

        if( barrier > maxBarrier )
        {
            printf( "Failed to initialize GLX_NV_swap_group: requested barrier %d "
                    "greater than maxBarriers (%d)\n", barrier, maxBarrier);
            return -1;
        }

        if( !glXJoinSwapGroupNV(displays, handles, group))
        {
            printf ("Failed to join swap group %d\n", group) ;
            return -1;
        }

        if( !glXBindSwapBarrierNV(displays, group, barrier ))
        {
            printf ("Failed to bind swap barrier %d\n", barrier) ;
            return -1;
        }
        return group;
    }

    void leaveNVSwapBarrier(Display *displays, Window handles, uint nvsg)
    {
        if (nvsg == 0 )
            return;

        glXBindSwapBarrierNV( displays, nvsg, 0 );
        glXJoinSwapGroupNV( displays, handles, 0 );
    }
};

}


bool glx_context::share(const pircontext &ctx)
{
    return ircontext::share(ctx);
}

bs_image glx_context::capture() const
{
    return bs_image();
}

bool glx_context::create(iwcontext *wctx, const tRenderFormats &rf)
{
    if (ircontext::create(wctx, rf))
    {
        format.has_debug = false;
        impl = bsNew (impl::context(this)); // init glxew

        GLXFBConfig *config = impl->chooseconfig(impl::rctx::display(this),
                                                 impl::rctx::screen(this), format);
        if (config == NULL)
        {
            log_error("GLX choose FBConfig failed.");
            return false;
        }
        ircontext *sctx = sharectx.get();
        GLXContext _ctx = impl->create(impl::rctx::display(this), config, sctx ? sctx->context<GLXContext>() :
                                                                                 NULL, format);
        if (_ctx == NULL)
        {
            log_error("GLX create new context failed.");
            return false;
        }
        XVisualInfo* ret = glXGetVisualFromFBConfig(impl::rctx::display(this), config[0]);
        XFree(config);
        if (!ret)
        {
            log_error("GLX: Failed to retrieve Visual for GLXFBConfig") ;
            return false;
        }
        ircontext::ctx = (ctxid)_ctx;
        ircontext::cfg = (cfgid)ret;

        if (format.target == Render_PBuffer)
        {
            Window pb = impl->create_PBuffer(config, this);
            if (!pb)
            {
                log_error("GLX create PBuffer failed.");
                return false;
            }
            XFlush (impl::rctx::display(this));
            ircontext::wcontext->handles = (winid)pb;

            uint width = 0;
            uint height = 0;
            glXQueryDrawable(impl::rctx::display(this), pb, GLX_WIDTH,  &width );
            glXQueryDrawable(impl::rctx::display(this), pb, GLX_HEIGHT, &height );
            format.viewport.width = width;
            format.viewport.height = height;
        }

        return true;

    }
    return false;
}

void glx_context::close()
{
    if (format.target == Render_PBuffer)
        glXDestroyPbuffer(impl::rctx::display(this), impl::rctx::handle(this));

    XFree(impl::rctx::config(this));
    glXDestroyContext (impl::rctx::display(this), impl::rctx::context(this));
    ircontext::ctx = NULL;
    ircontext::cfg = NULL;
    bsDelete (impl);
}

void glx_context::swap()
{
    if (!format.swap_sync)
        glXSwapBuffers(impl::rctx::display(this), impl::rctx::handle(this));
}
void glx_context::make()
{
    glXMakeCurrent(impl::rctx::display(this), impl::rctx::handle(this), impl::rctx::context(this));
}
void glx_context::done()
{
    glXMakeCurrent(impl::rctx::display(this), 0, 0 );
}

void glx_context::debugcall(fDebugCall func)
{
    glDebugMessageCallback(func, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
}
