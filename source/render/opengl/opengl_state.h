#ifndef OPENGL_STATE_H
#define OPENGL_STATE_H
#include "bs_ptr.h"
#include "tools/bs_stack.h"
#include "maths/bs_mat4X4.h"
#include "maths/bs_color.h"
#include "render/ifbo.h"

union uEnableStates{
    struct
    {
        bool scissor;
        bool alpha;
        bool blend;
        bool depth;
        bool stencil;
        bool cull;
        bool fog;
        bool antialiasing;
        bool debug;
    };
    uint state;
};

struct tStateOpes
{
    bs_color color_current;
    ivec4 viewport_current;
    mat4 model_view_matrix_current;
    mat4 projection_matrix_current;
    mat4 texture_matrix_current;
    eMatrixModes matrix_mode_current;
    pifbo fbo_current;
    uEnableStates enable_state_current;

    bs_stack<mat4> model_view_matrix_stack;
    bs_stack<mat4> projection_matrix_stack;
    bs_stack<mat4> texture_matrix_stack;

    bs_stack<ivec4> viewport_stack;
    bs_stack<pifbo> fbo_stack;

    ivec4 scissor_box;
    float line_width;
    int blend_src;
    int blend_dst;

    mat4 &load()
    {
        switch((int)matrix_mode_current)
        {
        case Matrix_ModelView:
            return model_view_matrix_current;

        case Matrix_Projection:
            return projection_matrix_current;

        case Matrix_Texture:
            return texture_matrix_current;
        }
        return model_view_matrix_current;
    }

    size_t push_viewport()
    {
        viewport_stack.push(viewport_current);
        return viewport_stack.count();
    }
    size_t pop_viewport()
    {
        viewport_current = viewport_stack.pop();
        return viewport_stack.count();
    }

    size_t push_model_view_matrix()
    {
        model_view_matrix_stack.push(model_view_matrix_current);
        return model_view_matrix_stack.count();
    }
    size_t pop_model_view_matrix()
    {
        model_view_matrix_current = model_view_matrix_stack.pop();
        return model_view_matrix_stack.count();
    }

    size_t push_projection_matrix()
    {
        projection_matrix_stack.push(projection_matrix_current);
        return projection_matrix_stack.count();
    }
    size_t pop_projection_matrix()
    {
        projection_matrix_current = projection_matrix_stack.pop();
        return projection_matrix_stack.count();
    }

    size_t push_texture_matrix()
    {
        texture_matrix_stack.push(texture_matrix_current);
        return texture_matrix_stack.count();
    }
    size_t pop_texture_matrix()
    {
        texture_matrix_current = texture_matrix_stack.pop();
        return texture_matrix_stack.count();
    }
};

#endif // OPENGL_STATE_H
