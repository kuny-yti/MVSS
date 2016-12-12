#ifndef ISHOW_H
#define ISHOW_H
#include "bs_define.h"
#include "geomdefine.h"
#include "render/itexture.h"

typedef enum
{
    Show_VertexLine = 1,
    Show_VertexPoint = 1 << 1,
    Show_ControlPoint = 1 << 2,
    Show_ControlLine = 1 << 3,
    Show_Dashed =  1 << 4,

    Show_Texture = 1 << 5
}eShowContrls;

bsPackage ishow
{
    bool need_update;     ///< 是否需要更新
    int shows;            ///< 绘制信息 eShowContrls
    float ctrl_psize;     ///< 控制点大小
    float vert_psize;     ///< 顶点大小
    float ctrl_lsize;     ///< 控制线宽
    float vert_lsize;     ///< 顶点线宽
    bs_color ctrl_pcolor; ///< 控制点颜色
    bs_color vert_pcolor; ///< 顶点颜色
    bs_color ctrl_lcolor; ///< 控制线颜色
    bs_color vert_lcolor; ///< 顶点线颜色

    bs_color face_color;  ///< 面颜色
    pitexture face_tex;   ///< 面纹理

    ishow()
    {
        need_update = true;
        shows &= (~Show_Texture);
        ctrl_psize = 2;
        vert_psize = 1;
        ctrl_lsize = 1;
        vert_lsize = 1;
        ctrl_pcolor = bs_color(1.,1.,1.,1.);
        vert_pcolor = bs_color(1.,1.,1.,1.);
        ctrl_lcolor = bs_color(1.,1.,1.,1.);
        vert_lcolor = bs_color(1.,1.,1.,1.);
        face_color = bs_color(1.,1.,1.,1.);
    }
    virtual ~ishow(){}

    void update(bool up = true){need_update = up;}
    bool is_update()const {return need_update;}
    //!
    //! \brief show 绘制控制信息
    //! \param pl
    //!
    void show(int pl) {shows = pl;}
    int show()const {return shows;}

    //!
    //! \brief control_point 控制点大小和颜色
    //! \param psize
    //! \param pcol
    //!
    void control_point(float psize, const bs_color pcol = bs_color(1.,1.,1.,1.))
    {ctrl_psize = psize;ctrl_pcolor = pcol;}
    float size_control_point()const{return ctrl_psize;}
    bs_color color_control_point()const{return ctrl_pcolor;}

    //!
    //! \brief control_point 控制线宽和颜色
    //! \param psize
    //! \param pcol
    //!
    void control_line(float lsize, const bs_color pcol = bs_color(1.,1.,1.,1.))
    {ctrl_lsize = lsize;ctrl_lcolor = pcol;}
    float size_control_line()const{return ctrl_lsize;}
    bs_color color_control_line()const{return ctrl_lcolor;}

    //!
    //! \brief control_point 顶点大小和颜色
    //! \param psize
    //! \param pcol
    //!
    void vertex_point(float psize, const bs_color pcol = bs_color(1.,1.,1.,1.))
    {vert_psize = psize;vert_pcolor = pcol;}
    float size_vertex_point()const{return vert_psize;}
    bs_color color_vertex_point()const{return vert_pcolor;}

    //!
    //! \brief control_point 顶点线宽和颜色
    //! \param psize
    //! \param pcol
    //!
    void vertex_line(float lsize, const bs_color pcol = bs_color(1.,1.,1.,1.))
    {vert_lsize = lsize;vert_lcolor = pcol;}
    float size_vertex_line()const{return vert_lsize;}
    bs_color color_vertex_line()const{return vert_lcolor;}

    //!
    //! \brief face 面对象的颜色和纹理
    //! \param col
    //! \param tex
    //!
    void face(const bs_color &col) {face_color = col; shows &= (~Show_Texture);}
    void face(const pitexture &tex)
    {face_tex = tex; shows |= Show_Texture;}
    bs_color color()const {return face_color;}
    pitexture texture()const {return face_tex;}
};
#endif // ISHOW_H
