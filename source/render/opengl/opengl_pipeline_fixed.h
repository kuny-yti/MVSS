#ifndef PIPELINE_FIXED_H
#define PIPELINE_FIXED_H

#include "render/ipipeline.h"

class pipeline_fixed :public ipipeline
{
public:
    pipeline_fixed(){}
    ~pipeline_fixed(){}

    int gluProject(real objx, real objy, real objz, const real modelMatrix[16], const real projMatrix[16],
                   const int viewport[4], real *winx, real *winy, real *winz);

    int gluUnProject(real winx, real winy, real winz, const real modelMatrix[16], const real projMatrix[16],
                     const int viewport[4], real *objx, real *objy, real *objz);

    int gluUnProject4(real winx, real winy, real winz, real clipw,
                      const real modelMatrix[16], const real projMatrix[16],
                      const int viewport[4], real nearVal, real farVal,
                      real *objx, real *objy, real *objz, real *objw);

    //! 当前系统视口操作
    virtual void viewport(const ivec4 &) ;
    virtual ivec4 viewport() ;
    //! 视口裁剪
    virtual void scissor(const ivec4 &) ;

    //! 开启或关闭指定功能
    virtual void enable(eEnableModes mod) ;
    virtual void disable(eEnableModes mod) ;
    virtual bool is_enable(eEnableModes mod)const ;

    //! alpha 透明值
    virtual void alpha_value( float value, eAlphaFuncs func = Alpha_Greater) ;
    //! depth 深度
    virtual void depth_mode(eDepthModes mode) ;
    //! blending 混合
    virtual void blending_mode(eBlendingModes mode) ;

    //! masks 遮罩
    virtual void mask_color(bool r, bool g, bool b, bool a) ;
    virtual void mask_depth(bool depth) ;

    //! stencil 模板
    virtual void stencil_func(eStencilFuncs func, int ref=0) ;
    virtual void stencil_op(eStencilOps op) ;
/*
    //! fog 雾
    virtual void fog_color(const bs_color & color) ;
    virtual void fog_distance(const vec2&) ;*/

    //! cull face多边形剔除
    virtual void cull_mode(eCullModes mode) ;

    //! 当前缓存的清理
    virtual void clear(int buffer) ;
    virtual void clear_color(const bs_color & color) ;

    //! 设置当前绘制模式
    virtual void polygon_mode(ePolygonModes mode, uint face = Face_Back | Face_Front) ;
    virtual void front_face(eFaceModes mode);

    //! 当前系统矩阵操作
    virtual void matrix_identity(eMatrixModes mode = Matrix_Not) ;
    virtual void matrix_mode(eMatrixModes mode) ;
    virtual void matrix_push(eMatrixModes mode = Matrix_Not) ;
    virtual void matrix_pop(eMatrixModes mode = Matrix_Not) ;
    virtual void matrix_load(const mat4  &matrix) ;

    virtual void color(const bs_color &col) ;

    //! 返回当前系统的矩阵数据
    virtual mat4 matrix(eMatrixModes mode) ;
    virtual void get(mat4 &mat);

    //! 操作当前屏幕模式(从屏幕坐标转换为空间坐标，屏幕启动2D模式)
    virtual fpoint3 screen_pos(const ipoint &p, float &zNear);
    virtual void screen_start(eOrthogonalModes mode = Orthogonal_Viewport) ;
    virtual void screen_stop() ;
    virtual void ray_line(const ipoint &p, vec3 &_near, vec3 &_far) ;

private:
    void enable_state_op(eEnableModes mod, bool is);
    friend class opengl_state;
};

#endif // PIPELINE_H
