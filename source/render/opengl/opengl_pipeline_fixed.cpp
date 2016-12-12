
#include "opengl_pipeline_fixed.h"
#include "bs_render.h"
#include "opengl_mx.h"
#include "opengl_state.h"

#define opengl_state() ((tStateOpes*)bs_render::instance ()->impl)

// viewport
void pipeline_fixed::viewport(const ivec4 &rect)
{
    opengl_state()->viewport_current = rect;
    glViewport(rect.x, rect.y, rect.width, rect.height);
}

// clear
void pipeline_fixed::clear(int buffer)
{
    switch(buffer)
    {
    case Buffer_Color:
        glClear(GL_COLOR_BUFFER_BIT);
        return;

    case Buffer_Depth:
        glClear(GL_DEPTH_BUFFER_BIT);
        return;

    case Buffer_Stencil:
        glClear(GL_STENCIL_BUFFER_BIT);
        return;

    case Buffer_Color | Buffer_Depth:
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return;

    case Buffer_Color | Buffer_Stencil:
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        return;

    case Buffer_Color | Buffer_Depth | Buffer_Stencil:
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        return;

    case Buffer_Depth | Buffer_Stencil:
        glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        return;
    }
}

static GLenum returnMode(ePolygonModes mo)
{
    switch ((int)mo)
    {
    case Polygon_Point:
        return GL_POINT;
    case Polygon_Wireframe:
        return GL_LINE;
    case Polygon_Fill:
        return GL_FILL;
    }
    return GL_FILL;
}
void pipeline_fixed::front_face(eFaceModes mode)
{
    if (mode == Face_CCW)
        glFrontFace(GL_CCW);
    else
        glFrontFace(GL_CW);
}
void pipeline_fixed::polygon_mode(ePolygonModes mode, uint face)
{
    switch (face)
    {
    case Face_Front:
        glPolygonMode(GL_FRONT, returnMode(mode));
        return;
    case Face_Back:
        glPolygonMode(GL_BACK, returnMode(mode));
        return;
    case Face_Left :
        glPolygonMode(GL_LEFT, returnMode(mode));
        return;
    case Face_Right:
        glPolygonMode(GL_RIGHT, returnMode(mode));
        return;
    case Face_Front | Face_Back:
        glPolygonMode(GL_FRONT_AND_BACK, returnMode(mode));
        return;
    case Face_Right | Face_Back:
        glPolygonMode(GL_BACK_RIGHT, returnMode(mode));
        return;
    case Face_Left | Face_Back:
        glPolygonMode(GL_BACK_LEFT, returnMode(mode));
        return;
    case Face_Left | Face_Front:
        glPolygonMode(GL_FRONT_LEFT, returnMode(mode));
        return;
    case Face_Right | Face_Front:
        glPolygonMode(GL_FRONT_RIGHT, returnMode(mode));
        return;
    }
    glPolygonMode(GL_FRONT_AND_BACK, returnMode(mode));
}

void pipeline_fixed::clear_color(const bs_color &color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}


// matrix
void pipeline_fixed::matrix_identity(eMatrixModes matrixMode)
{
    matrix_mode(matrixMode);
    glLoadIdentity();
    switch((int)matrixMode)
    {
    case Matrix_ModelView:
        opengl_state()->model_view_matrix_current = mat4::identity;
        return;

    case Matrix_Projection:
        opengl_state()->projection_matrix_current = mat4::identity;
        return;

    case Matrix_Texture:
        opengl_state()->texture_matrix_current = mat4::identity;
        return;
    }
}

void pipeline_fixed::matrix_mode(eMatrixModes matrixMode)
{
    opengl_state()->matrix_mode_current = matrixMode;
    switch((int)matrixMode)
    {
    case Matrix_ModelView:
        glMatrixMode(GL_MODELVIEW);
        return;

    case Matrix_Projection:
        glMatrixMode(GL_PROJECTION);
        return;

    case Matrix_Texture:
        glMatrixMode(GL_TEXTURE);
        return;
    }
}

void pipeline_fixed::matrix_push(eMatrixModes matrixMode)
{
    if (matrixMode != Matrix_Not)
        matrix_mode(matrixMode);
    glPushMatrix();
    switch((int)matrixMode)
    {
    case Matrix_ModelView:
        opengl_state()->push_model_view_matrix();
        return;

    case Matrix_Projection:
        opengl_state()->push_projection_matrix();
        return;

    case Matrix_Texture:
        opengl_state()->push_texture_matrix();
        return;
    }
}
void pipeline_fixed::matrix_pop(eMatrixModes matrixMode)
{
    if (matrixMode != Matrix_Not)
        matrix_mode(matrixMode);
    glPopMatrix();
    switch((int)matrixMode)
    {
    case Matrix_ModelView:
        opengl_state()->pop_model_view_matrix();
        return;

    case Matrix_Projection:
        opengl_state()->pop_projection_matrix();
        return;

    case Matrix_Texture:
        opengl_state()->pop_texture_matrix();
        return;
    }
}
void pipeline_fixed::matrix_load(const mat4 &matrix)
{
    glLoadMatrixf(matrix.elem);
    opengl_state()->load() = matrix;
}

void pipeline_fixed::color(const bs_color &col)
{
    glColor4f (col.r, col.g, col.b, col.a);
    opengl_state()->color_current = col;
}
ivec4 pipeline_fixed::viewport()
{
    return opengl_state()->viewport_current;
}
mat4 pipeline_fixed::matrix(eMatrixModes mode)
{
    switch((int)mode)
    {
    case Matrix_ModelView:
        return opengl_state()->model_view_matrix_current;

    case Matrix_Projection:
        return opengl_state()->projection_matrix_current;

    case Matrix_Texture:
        return opengl_state()->texture_matrix_current;
    }

    return mat4();
}
void pipeline_fixed::get(mat4 &mat)
{
    GLfloat ModelMatrix[16], ProjMatrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX,  ModelMatrix);
    glGetFloatv(GL_PROJECTION_MATRIX, ProjMatrix);

    mat = mat4(ModelMatrix) * mat4(ProjMatrix);
}
void pipeline_fixed::ray_line(const ipoint &p,
                              vec3 &_near,
                              vec3 &_far)
{
    real o1x, o1y, o1z;
    real o2x, o2y, o2z;

    //更新矩阵
    dmat4 mv = matrix(Matrix_ModelView);
    dmat4 prj =matrix(Matrix_Projection);

    ivec4 viewPort = viewport();

    gluUnProject((real)p.x, (viewPort.height - (real)p.y - 1), 0.0,
                        mv.elem,prj.elem, viewPort.elem, &o1x, &o1y, &o1z);
    gluUnProject((real)p.x, (viewPort.height - (real)p.y - 1), 1.0,
                        mv.elem,prj.elem, viewPort.elem, &o2x, &o2y, &o2z);

    _near = bs_vector3f(o1x, o1y, o1z);
    _far = bs_vector3f(o2x, o2y, o2z);
}

fpoint3 pipeline_fixed::screen_pos(const ipoint &p, float &zNear)
{
    GLint *viewport;
    GLfloat *modelview;
    GLfloat *projection;
    GLdouble winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glEnable(GL_DEPTH_TEST);

    modelview = opengl_state()->model_view_matrix_current.elem;
    projection = opengl_state()->projection_matrix_current.elem;
    viewport = opengl_state()->viewport_current.elem;

    winX = (float)p.x;
    winY = (float)viewport[3] - (float)p.y;
    if ((zNear < 0.00001))
    {
        glReadPixels(p.x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
        zNear = winZ;
    }

    gluUnProject(winX, winY, zNear, (GLdouble*)modelview, (GLdouble*)projection,
                 viewport, &posX, &posY, &posZ);

    return fpoint3(posX, posY, posZ);
}

void pipeline_fixed::screen_start(eOrthogonalModes mode)
{
    mat4 ortho;
    ivec4 view = viewport();

    matrix_push(Matrix_Projection);
    matrix_identity(Matrix_Projection);

    switch ((int)mode)
    {
    case Orthogonal_Viewport:
        ortho = mat4::createOrtho(0.0f, (float)view.width, (float)view.height, 0.0f, 0.0f, 1.0f);
        break;
    case Orthogonal_ViewportUpward:
        ortho = mat4::createOrtho(0.0f, (float)view.width, 0.0f, (float)view.height, 0.0f, 1.0f);
        break;
    case Orthogonal_One:
        ortho = mat4::createOrtho(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
        break;
    case Orthogonal_OneUpward:
        ortho = mat4::createOrtho(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
        break;
    }

    ortho = transpose(ortho);
    matrix_load(ortho);

    matrix_push(Matrix_ModelView);
    matrix_identity(Matrix_ModelView);
}
void pipeline_fixed::screen_stop()
{
    matrix_pop(Matrix_Projection);
    matrix_pop(Matrix_ModelView);
}

static GLenum return_enable(eEnableModes mod)
{
    GLenum mode = 0;
    switch (int(mod))
    {
    case Enable_Scissor:
        mode = GL_SCISSOR_TEST;
    break;
    case Enable_Alpha:
        mode = GL_ALPHA_TEST;
        break;
    case Enable_Blend:
        mode = GL_BLEND;
        break;
    case Enable_Depth:
         mode = GL_DEPTH_TEST;
        break;
    case Enable_Stencil:
        mode = GL_STENCIL_TEST;
        break;
    case Enable_Cull:
        mode = GL_CULL_FACE;
        break;
    case Enable_Fog:
        mode = GL_FOG;
        break;
    case Enable_Antialiasing:
        mode = GL_LINE_SMOOTH;
        break;
    case Enable_Debug:
        mode = GL_DEBUG_OUTPUT;
        break;
    }
    return mode;
}
void pipeline_fixed::enable(eEnableModes mod)
{
    glEnable (return_enable (mod));
    if (mod == Enable_Debug)
        glEnable (GL_DEBUG_OUTPUT_SYNCHRONOUS);
    enable_state_op(mod, true);
}
void pipeline_fixed::disable(eEnableModes mod)
{
    glDisable (return_enable (mod));
    if (mod == Enable_Debug)
        glDisable (GL_DEBUG_OUTPUT_SYNCHRONOUS);
    enable_state_op(mod, false);
}
bool pipeline_fixed::is_enable(eEnableModes mod)const
{
    return glIsEnabled(return_enable (mod));
}
void pipeline_fixed::enable_state_op(eEnableModes mod, bool is)
{
    switch (int(mod))
    {
    case Enable_Scissor:
        opengl_state()->enable_state_current.scissor = is;
        break;
    case Enable_Alpha:
        opengl_state()->enable_state_current.alpha = is;
        break;
    case Enable_Blend:
        opengl_state()->enable_state_current.blend = is;
        break;
    case Enable_Depth:
        opengl_state()->enable_state_current.depth = is;
        break;
    case Enable_Stencil:
        opengl_state()->enable_state_current.stencil = is;
        break;
    case Enable_Cull:
        opengl_state()->enable_state_current.cull = is;
        break;
    case Enable_Fog:
        opengl_state()->enable_state_current.fog = is;
        break;
    case Enable_Antialiasing:
        opengl_state()->enable_state_current.antialiasing = is;
        break;
    case Enable_Debug:
        opengl_state()->enable_state_current.debug = is;
        break;
    }
}
//! 视口裁剪
void pipeline_fixed::scissor(const ivec4 &rect)
{
    glScissor(rect.x, rect.y, rect.width, rect.height);
}

GLenum return_alpha( eAlphaFuncs func)
{
    GLenum mode = 0;
    switch ((int)func)
    {
    case Alpha_Never :
        mode = GL_NEVER;
        break;
    case Alpha_Less :
        mode = GL_LESS;
        break;
    case Alpha_Equal :
        mode = GL_EQUAL;
        break;
    case Alpha_LEqual :
        mode = GL_LEQUAL;
        break;
    case Alpha_Greater:
        mode = GL_GREATER;
        break;
    case Alpha_NotEqual :
         mode = GL_NOTEQUAL;
        break;
    case Alpha_GEqual:
        mode = GL_GEQUAL;
        break;
    case Alpha_Always:
        mode = GL_ALWAYS;
        break;
    }
    return mode;
}
//! alpha 透明值
void pipeline_fixed::alpha_value(float value, eAlphaFuncs func)
{
    glAlphaFunc(return_alpha(func), value);
}
GLenum return_depth(eDepthModes func)
{
    GLenum mode = 0;
    switch ((int)func)
    {
    case Depth_Always :
        mode = GL_ALWAYS;
        break;
    case Depth_Less:
        mode = GL_LESS;
        break;
    case Depth_Greater :
        mode = GL_GREATER;
        break;
    case Depth_Equal:
        mode = GL_EQUAL;
        break;
    case Depth_LEqual:
        mode = GL_LEQUAL;
        break;
    case Depth_GEqual :
         mode = GL_GEQUAL;
        break;
    case Depth_NotEqual:
        mode = GL_NOTEQUAL;
        break;
    case Depth_None:
        mode = GL_NONE;
        break;
    }
    return mode;
}
//! depth 深度
void pipeline_fixed::depth_mode(eDepthModes mode)
{
    glDepthFunc(return_depth (mode));
}
//! blending 混合
void pipeline_fixed::blending_mode(eBlendingModes mode)
{
    switch((int)mode)
    {
    case Blending_None:
        glBlendFunc(GL_ONE, GL_ZERO);
        break;
    case Blending_Alpha:
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        break;
    case Blending_Add:
        glBlendFunc(GL_ONE, GL_ONE);
        break;
    case Blending_Product:
        glBlendFunc(GL_ZERO, GL_SRC_COLOR);
        break;
    case Blending_Light:
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
        break;
    }
}

//! masks 遮罩
void pipeline_fixed::mask_color(bool r, bool g, bool b, bool a)
{
    glColorMask(r, g, b, a);
}
void pipeline_fixed::mask_depth(bool depth)
{
    glDepthMask(depth);
}

//! stencil 模板
void pipeline_fixed::stencil_func(eStencilFuncs func, int ref)
{
    switch((int)func)
    {
        default:
        case Stencil_Always:
            glStencilFunc(GL_ALWAYS, ref, ~0);
            break;
        case Stencil_Equal:
            glStencilFunc(GL_EQUAL, ref, ~0);
            break;
        case Stencil_NotEqual:
            glStencilFunc(GL_NOTEQUAL, ref, ~0);
            break;
        case Stencil_Never:
            glStencilFunc(GL_NEVER, ref, ~0);
            break;
        case Stencil_Less:
            glStencilFunc(GL_LESS, ref, ~0);
            break;
        case Stencil_LEqual:
            glStencilFunc(GL_LEQUAL, ref, ~0);
            break;
        case Stencil_Greater:
            glStencilFunc(GL_GREATER, ref, ~0);
            break;
        case Stencil_GEqual:
            glStencilFunc(GL_GEQUAL, ref, ~0);
            break;
    }
}
void pipeline_fixed::stencil_op(eStencilOps op)
{
    switch((int)op)
    {
        default:
        case Stencil_Keep:
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            break;
        case Stencil_Invert:
            glStencilOp(GL_KEEP, GL_INVERT, GL_INVERT);
            break;
        case Stencil_Decr:
            glStencilOp(GL_KEEP, GL_DECR, GL_DECR);
            break;
        case Stencil_Incr:
            glStencilOp(GL_KEEP, GL_INCR, GL_INCR);
            break;
        case Stencil_Replace:
            glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
            break;
    }
}
/*
//! fog 雾
void pipeline_fixed::fog_color(const bs_color & color)
{
    float dat[4] = {color.r, color.g, color.b, color.a};
    glFogfv(GL_FOG_COLOR, dat);
}
void pipeline_fixed::fog_distance(const vec2 &d)
{
    glFogf(GL_FOG_START, d.x);
    glFogf(GL_FOG_END,   d.y);
}
*/

//! cull face多边形剔除
void pipeline_fixed::cull_mode(eCullModes mode)
{
    switch((int)mode)
    {
    case Cull_Front:
        glCullFace(GL_FRONT);
        return;

    case Cull_Back:
        glCullFace(GL_BACK);
        return;

    case Cull_FrontBack:
        glCullFace(GL_FRONT_AND_BACK);
        return;
    }
}
