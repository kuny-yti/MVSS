#ifndef IPIPELINE_H
#define IPIPELINE_H

#include "bs_define.h"
#include "maths/bs_vec4.h"
#include "maths/bs_color.h"
#include "maths/bs_mat4X4.h"
#include "bs_ptr.h"
#include "rdefine.h"

bsPackage ipipeline
{
    virtual ~ipipeline(){}

    //! 当前系统视口操作
    virtual void viewport(const ivec4 &) = 0;
    virtual ivec4 viewport() = 0;
    //! 视口裁剪
    virtual void scissor(const ivec4 &) = 0;

    //! 开启或关闭指定功能
    virtual void enable(eEnableModes mod) = 0;
    virtual void disable(eEnableModes mod) = 0;
    virtual bool is_enable(eEnableModes mod)const = 0;

    //! alpha 透明值
    virtual void alpha_value( float value, eAlphaFuncs func = Alpha_Greater) = 0;
    //! depth 深度
    virtual void depth_mode(eDepthModes mode) = 0;
    //! blending 混合
    virtual void blending_mode(eBlendingModes mode) = 0;

    //! masks 遮罩
    virtual void mask_color(bool r, bool g, bool b, bool a) = 0;
    virtual void mask_depth(bool depth) = 0;

    //! stencil 模板
    virtual void stencil_func(eStencilFuncs func, int ref=0) = 0;
    virtual void stencil_op(eStencilOps op) = 0;
/*
    //! fog 雾
    virtual void fog_color(const bs_color & color) = 0;
    virtual void fog_distance(const vec2&) = 0;*/

    //! cull face多边形剔除
    virtual void cull_mode(eCullModes mode) = 0;
    virtual void front_face(eFaceModes mode) = 0;

    //! 当前缓存的清理
    virtual void clear(int buffer) = 0;
    virtual void clear_color(const bs_color & color) = 0;

    //! 设置当前绘制模式
    virtual void polygon_mode(ePolygonModes mode, uint face = Face_Back | Face_Front) = 0;

    //! 当前系统矩阵操作
    virtual void matrix_identity(eMatrixModes mode = Matrix_Not) = 0;
    virtual void matrix_mode(eMatrixModes mode) = 0;
    virtual void matrix_push(eMatrixModes mode = Matrix_Not) = 0;
    virtual void matrix_pop(eMatrixModes mode = Matrix_Not) = 0;
    virtual void matrix_load(const mat4  &matrix) = 0;

    virtual void color(const bs_color &col) = 0;

    //! 返回当前系统的矩阵数据
    virtual mat4 matrix(eMatrixModes mode) = 0;
    virtual void get(mat4 &mat) = 0;

    //! 操作当前屏幕模式(从屏幕坐标转换为空间坐标，屏幕启动2D模式)
    virtual fpoint3 screen_pos(const ipoint &p, float &zNear)= 0;
    virtual void screen_start(eOrthogonalModes mode = Orthogonal_Viewport) = 0;
    virtual void screen_stop() = 0;
    virtual void ray_line(const ipoint &p, vec3 &_near, vec3 &_far) = 0;
};
#define iPipeline bs_ptr(ipipeline)
typedef iPipeline pipipeline;

#endif
