#include "bs_render.h"

#include "opengl/opengl_capability.h"
#include "opengl/opengl_pipeline_fixed.h"
#include "opengl/opengl_context.h"
#include "opengl/opengl_state.h"
#include "opengl/opengl_mx.h"

/*
void bs_render::push_all()
{
    stateop()->enable_state_current.antialiasing = glIsEnabled(GL_LINE_SMOOTH);
    stateop()->enable_state_current.scissor = glIsEnabled(GL_SCISSOR_TEST);
    stateop()->enable_state_current.blend = glIsEnabled(GL_BLEND);
    stateop()->enable_state_current.stencil = glIsEnabled(GL_STENCIL_TEST);
    stateop()->enable_state_current.alpha = glIsEnabled(GL_ALPHA_TEST);
    stateop()->enable_state_current.depth = glIsEnabled(GL_DEPTH_TEST);
    stateop()->enable_state_current.cull = glIsEnabled(GL_CULL_FACE);
    stateop()->enable_state_current.fog = glIsEnabled(GL_FOG);
    stateop()->enable_state_current.debug = glIsEnabled(GL_DEBUG_OUTPUT);

    glGetIntegerv(GL_VIEWPORT, stateop()->viewport_current.elem);
    glGetIntegerv(GL_SCISSOR_BOX, stateop()->scissor_box.elem);
    glGetFloatv(GL_LINE_WIDTH, &stateop()->line_width);
    glGetIntegerv(GL_BLEND_SRC, &stateop()->blend_src);
    glGetIntegerv(GL_BLEND_DST, &stateop()->blend_dst);
}
void bs_render::pop_all()
{
    if (stateop()->enable_state_current.antialiasing)
        glEnable(GL_LINE_SMOOTH);
    else
        glDisable(GL_LINE_SMOOTH);

    if (stateop()->enable_state_current.scissor)
        glEnable(GL_SCISSOR_TEST);
    else
        glDisable(GL_SCISSOR_TEST);

    if (stateop()->enable_state_current.blend)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);

    if (stateop()->enable_state_current.stencil)
        glEnable(GL_STENCIL_TEST);
    else
        glDisable(GL_STENCIL_TEST);

    if (stateop()->enable_state_current.alpha)
        glEnable(GL_ALPHA_TEST);
    else
        glDisable(GL_ALPHA_TEST);

    if (stateop()->enable_state_current.depth)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    if (stateop()->enable_state_current.cull)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);

    if (stateop()->enable_state_current.fog)
        glEnable(GL_FOG);
    else
        glDisable(GL_FOG);

    if (stateop()->enable_state_current.debug)
        glEnable (GL_DEBUG_OUTPUT);
    else
        glDisable (GL_DEBUG_OUTPUT);

    bs_rect vp = stateop()->viewport_current;
    glViewport(vp.x, vp.y, vp.width(), vp.height());
    vp = stateop()->scissor_box;
    glScissor(vp.x, vp.y, vp.width(), vp.height());
    glLineWidth(stateop()->line_width);
    glAlphaFunc(stateop()->blend_src, stateop()->blend_dst);
}
*/

namespace impl {
struct render : public tStateOpes
{
    bs_ptr(opengl_capability) caps;
};
}
bs_render::bs_render()
{
    impl = bsNew (impl::render);
    is_initialize = false;
}
bs_render::~bs_render()
{
    bsDelete (impl);
}

bool bs_render::create(iwcontext *wctx, const tRenderFormats &rf)
{
    if (!context && wctx != NULL)
    {
        context = bs_malloc(opengl_context);
        if (!context->create(wctx, rf))
            return false;
        return true;
    }
    else if (context)
        return true;

    return false;
}
void bs_render::close()
{
    if (context)
        context->close();
}
static void gl_debug(uint source, uint type, uint id, uint severity, int length, const char* message, const void*)
{
    log_informations("OpenGL Logs: %d %d %d %d %s", source, type, id, severity, length, message);
}

bool bs_render::initialize()
{
    if (is_initialize)
        return true;
    if (!context)
        return false;

    if (glewInit() != GLEW_OK)
        return false;

    if (!impl->caps)
    {
        impl->caps = bs_malloc (opengl_capability);
        impl->caps->initialize();
    }

    if (!pipeline)
        pipeline = bs_malloc (pipeline_fixed);

    if (context->format.has_debug)
    {
        pipeline->enable(Enable_Debug);
        context->debugcall(gl_debug);
    }
    is_initialize = true;
    return initialize();
}

picapability bs_render::capability()const
{
    return impl->caps;
}


#include "opengl/opengl_texture.h"
pitexture bs_render::texture()
{
    return bs_malloc (opengl_texture);
}

#include "opengl/opengl_frame_buffer.h"
pifbo bs_render::fbo()
{
    return bs_malloc ( opengl_frame_buffer);
}

#include "opengl/opengl_buffer_object.h"
pibobject bs_render::bobject()
{
    return bs_malloc ( opengl_buffer_object);
}

#include "opengl/opengl_pixel_buffer.h"
pipbo bs_render::pbo()
{
    return bs_malloc ( opengl_pixel_buffer);
}

#include "opengl/opengl_shader_program.h"
pishader bs_render::shader()
{
    return bs_malloc ( opengl_shader);
}
piprogram bs_render::program()
{
    return bs_malloc ( opengl_program);
}


#include "opengl/opengl_vertex_array.h"
pivao bs_render::vao()
{
    return bs_malloc (opengl_vertex_array);
}
#include "opengl/opengl_vertex_buffer.h"
pivbo bs_render::vbo()
{
    return bs_malloc (opengl_vertex_buffer);
}
#include "opengl/opengl_indices_buffer.h"
piibo bs_render::ibo()
{
    return bs_malloc (opengl_indices_buffer);
}

/*#include "opengl/opengl_mesh.h"
pimesh bs_render::mesh()
{
    return s_malloc ( opengl_mesh, this);
}*/

#include "opengl/opengl_uniform_buffer.h"
piubo bs_render::ubo()
{
    return bs_malloc ( opengl_uniform_buffer);
}
