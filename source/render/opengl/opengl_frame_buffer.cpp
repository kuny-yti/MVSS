#include "opengl_frame_buffer.h"
#include "opengl_texture.h"
#include "render/rdefine.h"
#include "tools/bs_vector.h"
#include "opengl_mx.h"

namespace impl {
class opengl_fbo
{
public:
    uint rbo;
    bool valid;
    int curren_target;
    int back_fbo;
    bool is_scissor;
    ivec4 back_viewport;

    opengl_fbo():
        rbo(0),
        valid(true),
        curren_target(0),
        back_fbo(-1),
        is_scissor(false),
        back_viewport()
    {

    }

    GLuint returnDataFormat(int format)
    {
        switch (format)
        {
        case Data_RGB :
            return GL_RGB;
        case Data_RGBA :
            return GL_RGBA;
        case Data_Luminance:
            return GL_LUMINANCE;
        case Data_LuminanceAlpha:
            return GL_LUMINANCE_ALPHA;
        case Data_BGR:
            return GL_BGR;
        case Data_BGRA:
            return GL_BGRA;
        default:
            break;
        }
        return GL_RGBA;
    }
    GLuint returnDataDepth(int depth)
    {
        switch (depth)
        {
        case Data_Byte :
            return GL_BYTE;
        case Data_UByte:
            return GL_UNSIGNED_BYTE;
        case Data_Short:
            return GL_SHORT;
        case Data_UShort:
            return GL_UNSIGNED_SHORT;
        case Data_Int:
            return GL_INT;
        case Data_UInt:
            return GL_UNSIGNED_INT;
        case Data_Float:
            return GL_FLOAT;
        case Data_2Bytes :
            return GL_2_BYTES;
        case Data_3Bytes :
            return GL_3_BYTES;
        case Data_4Bytes:
            return GL_4_BYTES;
        case Data_Double:
            return GL_DOUBLE;

        default:
            break;
        }
        return GL_UNSIGNED_BYTE;
    }
    GLuint returnInternalFormat(int internal)
    {
        switch (internal)
        {
        case Internal_Red:
            return GL_RED;
        case Internal_Green:
            return GL_GREEN;
        case Internal_Blue:
            return GL_BLUE;
        case Internal_Alpha:
            return GL_ALPHA;
        case Internal_RGB:
            return GL_RGB;
        case Internal_RGBA:
            return GL_RGBA;
        case Internal_Luminance:
            return GL_LUMINANCE;
        case Internal_LuminanceAlpha:
            return GL_LUMINANCE_ALPHA;
        default:
            break;
        }
        return GL_RGBA;
    }
    GLenum returnTexFilterMode(eTextureParams mode)
    {
        switch((int)mode)
        {
        default:
        case Filter_Nearest:
            return GL_NEAREST;

        case Filter_NearestMipmapNearest:
            return GL_NEAREST_MIPMAP_NEAREST;

        case Filter_NearestMipmapLinear:
            return GL_NEAREST_MIPMAP_LINEAR;

        case Filter_Linear:
            return GL_LINEAR;

        case Filter_LinearMipmapNearest:
            return GL_LINEAR_MIPMAP_NEAREST;

        case Filter_LinearMipmapLinear:
            return GL_LINEAR_MIPMAP_LINEAR;
        }
    }
    GLenum attachmentColor(int i)
    {
        return GL_COLOR_ATTACHMENT0 + i;
    }
    uint attachment(eFrameBufferAttachs i)
    {
        if (i < Attach_Depth)
            return attachmentColor(i);
        else if (i == Attach_Depth)
            return GL_DEPTH_ATTACHMENT;
        else if (i == Attach_Stencil)
            return GL_STENCIL_ATTACHMENT;
        return GL_COLOR_ATTACHMENT0;
    }

    bool checkFramebufferStatus()
    {
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        switch(status)
        {
        case GL_NO_ERROR:
        case GL_FRAMEBUFFER_COMPLETE:
            return true;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            throw bs_string("opengl_frame_buffer: Unsupported framebuffer format.");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            throw bs_string("opengl_frame_buffer: Framebuffer incomplete attachment.");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            throw bs_string("opengl_frame_buffer: Framebuffer incomplete, missing attachment.");
            break;
    #ifdef GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT
        case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT:
            throw bs_string("opengl_frame_buffer: Framebuffer incomplete, duplicate attachment.");
            break;
    #endif
    #ifdef GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
            throw bs_string("opengl_frame_buffer: Framebuffer incomplete, attached images must have same dimensions.");
            break;
    #endif
    #ifdef GL_FRAMEBUFFER_INCOMPLETE_FORMATS
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
            throw bs_string("opengl_frame_buffer: Framebuffer incomplete, attached images must have same format.");
            break;
    #endif
    #ifdef GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            throw bs_string("opengl_frame_buffer: Framebuffer incomplete, missing draw buffer.");
            break;
    #endif
    #ifdef GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            throw bs_string("opengl_frame_buffer: Framebuffer incomplete, missing read buffer.");
            break;
    #endif
    #ifdef GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            throw bs_string("opengl_frame_buffer: Framebuffer incomplete, attachments must have same number of samples per pixel.");
            break;
    #endif
        default:
        {
            bs_string num;
            num.form(status);
            throw bs_string("opengl_frame_buffer: An undefined error has occurred: ")
                    +num ;
        }
            break;
        }
        return false;
    }

};
}


opengl_frame_buffer::opengl_frame_buffer():
    ifbo()
{
    impl = bsNew (impl::opengl_fbo());
}
opengl_frame_buffer::~opengl_frame_buffer()
{
    destroy();
    bsDelete (impl);
}
void opengl_frame_buffer::destroy()
{
    glDeleteFramebuffers(1, &id);
    if (impl->rbo)
        glDeleteRenderbuffers(1, &impl->rbo);
    impl->rbo = 0;
    ifbo::destroy();
}
void opengl_frame_buffer::create_rbo_t(int samples, const isize &size)
{
    if (impl->rbo)
        glDeleteRenderbuffers(1, &impl->rbo);

    glGenRenderbuffers(1, (uint*)&impl->rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, impl->rbo);
    if (samples)
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples,
                                         impl->returnInternalFormat(texture[impl->curren_target]->internal),
                                         size.width, size.height);
    else
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.width, size.height);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, impl->attachment(attachment),
                           GL_TEXTURE_2D, texture[texture.size() -1]->id, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, impl->attachmentColor(Attach_Depth),
                              GL_RENDERBUFFER, impl->rbo);
}
uint opengl_frame_buffer::create(const isize &s)
{
    if ((size != s) || (!id))
    {
        size = s;

        glGetIntegerv(GL_MAX_SAMPLES, &samples_max);
        samples = bs_bound(0, int(samples), int(samples_max));

        if (id)
            glDeleteFramebuffers(1, (uint*)&id);
        glGenFramebuffers(1, (uint*)&id);
        glBindFramebuffer(GL_FRAMEBUFFER, id);

        //if (samples() == 0)
        {
            if (texture.size() == 0)
                create_texture();

            create_rbo_t(this->samples, this->size);

            impl->valid = impl->checkFramebufferStatus();
            if (!impl->valid)
                texture.remove(texture.size() -1);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        //else
        {
            //mipmap() = false;
            /*
            glRenderbufferStorageMultisample(GL_RENDERBUFFER,
                    samples(),
                    returnInternalFormat(impl->texture->internal()),
                    size().width(), size().height());

            impl->valid = checkFramebufferStatus();
            if (impl->valid)
            {
                glGetRenderbufferParameteriv(GL_RENDERBUFFER,
                                                    GL_RENDERBUFFER_SAMPLES,
                                                    &impl->samples);
            }*/
        }

    }
    return ifbo::create(s);
}
void opengl_frame_buffer::create_texture(const pitexture &tex)
{
    if (tex.get())
    {
        if (!tex->id)
        {
            tex->filter_min = Filter_Nearest;
            tex->filter_mag = Filter_Nearest;
            tex->wrap_s = Wrap_Repeat;
            tex->wrap_t = Wrap_Repeat;
            if (mipmap)
                tex->level_max = 4;

            tex->update(size, NULL);
        }
        texture.append(tex);
        return ;
    }

    pitexture texa = bs_malloc (opengl_texture);

    texa->filter_min = Filter_Nearest;
    texa->filter_mag = Filter_Nearest;
    texa->wrap_s = Wrap_Repeat;
    texa->wrap_t = Wrap_Repeat;
    if (mipmap)
        texa->level_max = 4;

    texa->update(size, NULL);

    /*if (tex->level_max())
    {
        int width = size.width();
        int height = size.height();
        int level = 0;
        while (width > 1 || height > 1)
        {
            width = qMax(1, width >> 1);
            height = qMax(1, height >> 1);
            ++level;
            glTexImage2D(GL_TEXTURE_2D, level,
                         returnInternalFormat(
                             tex->internal()),
                         width, height,
                         0, returnDataFormat(tex->format()),
                         returnDataDepth(tex->depth()), 0);
        }
    }*/
    texa->unbind();
    texture.append(texa);
}
bool opengl_frame_buffer::has_fbo()
{
    return GLEW_ARB_framebuffer_object;
}
bool opengl_frame_buffer::has_fbo_blit()
{
    return false;
}
void opengl_frame_buffer::blit(const pifbo &source, const pifbo &target,
                               const ivec4 &targetRect, const ivec4 &sourceRect,
                               eBufferTypes buffers , eTextureParams filter )
{
    GLuint buff ;
    if (buffers == Buffer_Stencil)
        buff = GL_STENCIL_BUFFER_BIT;
    else if (buffers == Buffer_Depth)
        buff = GL_DEPTH_BUFFER_BIT;
    else
        buff = GL_COLOR_BUFFER_BIT;

    const int sh = source->size.height;
    const int th = target->size.height;

    const int sx0 = sourceRect.x;
    const int sx1 = sourceRect.x + sourceRect.width;
    const int sy0 = sh - (sourceRect.y + sourceRect.height);
    const int sy1 = sh - sourceRect.y;

    const int tx0 = targetRect.x;
    const int tx1 = targetRect.x + targetRect.width;
    const int ty0 = th - (targetRect.y + targetRect.height);
    const int ty1 = th - targetRect.y;

    glBindFramebuffer(GL_READ_FRAMEBUFFER, source->id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target->id);
    glBlitFramebuffer(sx0, sy0, sx1, sy1, tx0, ty0, tx1, ty1,
                         buff, impl->returnTexFilterMode(filter));
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
}

const bs_image opengl_frame_buffer::read()
{
    bs_image out (size.width, size.height, NULL, ImageData_RGBA);

    bind();
    glReadPixels(0, 0, size.width, size.height, GL_RGBA, GL_UNSIGNED_BYTE, out.bits());
    unbind();

    return out;
}

int opengl_frame_buffer::targetadd(const pitexture &tex)
{
    create_texture(tex);
    return ifbo::targetadd(tex);
}
void opengl_frame_buffer::target(int usedTarget)
{
    if ((size_t)usedTarget >= texture.size())
        return ;
    if (impl->curren_target != usedTarget)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, id);
        glFramebufferTexture2D(GL_FRAMEBUFFER, impl->attachment(attachment),
                               GL_TEXTURE_2D, texture[usedTarget]->id, 0);
        impl->curren_target = usedTarget;
    }
}

void opengl_frame_buffer::begin(int usedTarget)
{
    if ((size_t)usedTarget >= texture.size())
        return;

    glDisable(GL_SCISSOR_TEST);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    bind();
    target(usedTarget);

    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0,0, texture[usedTarget]->size.width, texture[usedTarget]->size.height);
}
void opengl_frame_buffer::end(int usedTarget)
{
    if ((size_t)usedTarget >= texture.size())
        return ;

    glPopAttrib();
    unbind();

    if (mipmap)
    {
        texture[usedTarget]->bind();
        texture[usedTarget]->genmipmap();
        texture[usedTarget]->unbind();
    }

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void opengl_frame_buffer::bind(void)
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &impl->back_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}
void opengl_frame_buffer::unbind(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER,  impl->back_fbo);
}

