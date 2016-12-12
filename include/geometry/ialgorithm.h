#ifndef IALGORITHM_H
#define IALGORITHM_H

#include "bs_define.h"
#include "bs_ptr.h"
#include "maths/bs_mat4X4.h"
#include "maths/bs_matMxN.h"
#include "maths/bs_vec4.h"
#include "geomdefine.h"

using namespace primitive;
typedef mat4 bs_xform;
typedef enum
{
    Algorithm_Not,
    Algorithm_Bezier,
    Algorithm_CatmullRom,
    Algorithm_BSpline,
    Algorithm_NURBS,
    Algorithm_User
}eAlgorithmTypes;


bsPackage ialgorithm
{
    virtual ~ialgorithm(){}
    //!
    //! \brief evaluate_curve 曲线生成 (算法: deCasteljau, Catmull-Rom)
    //! \param der_count 衍生点的数量
    //! \param der_point 衍生的点
    //! \param ctrl_point 控制点
    //! \param factor 平滑系数
    //! \return true 成功
    //!
    virtual bool evaluate_curve(const bs_vector<fpoint3> &ctrl_point, bs_vector<fpoint3> &der_point,
                        const size_t der_count, float factor = 0.6) = 0;
    //!
    //! \brief evaluate_surface 曲面生成 (算法: deCasteljau, Catmull-Rom)
    //! \param der_mcount 衍生点的行数
    //! \param der_ncount 衍生点的列数
    //! \param der_point  衍生的点
    //! \param ctrl_point 控制点
    //! \return true 成功
    //!
    virtual bool evaluate_surface(const bs_matrixMxN<fpoint3> &ctrl_point, bs_matrixMxN<fpoint3> &der_point,
                          const size_t der_mcount, const size_t der_ncount, float factor = 0.6) = 0;

    //!
    //! \brief evaluate_texcoord 曲面纹理坐标计算
    //! \param min 纹理的左下角
    //! \param max 纹理的右上角
    //! \param der_mcount 网格m方向数量
    //! \param der_ncount 网格n方向数量
    //! \param texCoord 返回的纹理坐标
    //!
    virtual void evaluate_texcoord(const vec2 &min, const vec2 &max,
                           const size_t der_mcount, const size_t der_ncount,
                           bs_matrixMxN<vec2> &texCoord)= 0;

    //!
    //! \brief evaluate_indice 索引计算(绘制模式 Draw_TriangleStrip)
    //! \param der_mcount
    //! \param der_ncount
    //! \param mindex
    //! \param nindex
    //!
    virtual void evaluate_indice(const size_t der_mcount, const size_t der_ncount,
                         bs_matrixMxN<tMxNIndices> &index)= 0;
    //!
    //! \brief evaluate_indice 索引计算(绘制模式 Draw_Triangles)
    //! \param der_mcount
    //! \param der_ncount
    //! \param mindex
    //! \param nindex
    //!
    virtual void evaluate_indice(const size_t der_mcount, const size_t der_ncount,
                         bs_matrixMxN<tQuadIndices> &index)= 0;

    //!
    //! \brief evaluate_indice 曲面索引计算(绘制模式 Draw_LineStrip)
    //! \param der_mcount
    //! \param der_ncount
    //! \param mindex
    //! \param nindex
    //!
    virtual void evaluate_indice(const size_t der_mcount, const size_t der_ncount,
                                bs_matrixMxN<uint> &mindex, bs_matrixMxN<uint> &nindex) = 0;
    //!
    //! \brief convert 曲面顶点转化为面保存
    //! \param der_point  衍生的点
    //! \param der_surface 衍生的面
    //! \return true 成功
    //!
    virtual bool convert(const bs_matrixMxN<fpoint3> &der_point, bs_matrixMxN<tQuads3f> &der_surface)= 0;
    //!
    //! \brief convert 曲面纹理坐标转换为面纹理坐标
    //! \param tc_src 须转换的坐标
    //! \param tc_dst 转换完成的坐标
    //!
    virtual bool convert(const bs_matrixMxN<vec2> &tc_src, bs_matrixMxN<tQuads2f> &tc_dst)= 0;
};
#define IALGO bs_ptr(ialgorithm)
typedef IALGO pialgorithm;
#undef IALGO
#endif // IALGORITHM_H
