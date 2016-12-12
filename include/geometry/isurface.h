#ifndef ISURFACES_H
#define ISURFACES_H
#include "iprimitive.h"
#include "ishow.h"

bsPackage isurface  : iprimitive , ishow
{
    enum {PointMin_M = 2, PointMin_N = 2};
    bs_matrixMxN <fpoint3> ctrl;
    size_t der_m;
    size_t der_n;
    vec2 _tex_min;
    vec2 _tex_max;

    eAlgorithmTypes ca;
    float smfactor ;

    //!
    //! \brief is_valid
    //! \return true表示曲线顶点时有效的。
    //!
    virtual bool is_valid()const{return ctrl.rows >= PointMin_N && ctrl.cols >= PointMin_M;}
    //!
    //! \brief evaluate 评估曲面
    //! \param ca 算法类型
    //! \return
    //!
    virtual bool evaluate(eAlgorithmTypes ca = Algorithm_Bezier) = 0;
    //!
    //! \brief derived 曲面的精度
    //!
    void derived(size_t m, size_t n){der_m = m; der_n = n;}
    size_t derived_m()const{return der_m;}
    size_t derived_n()const{return der_n;}

    //!
    //! \brief coord 曲面的纹理范围
    //!
    void coord(const vec2 &min, const vec2 &max) {_tex_max = max; _tex_min = min;}
    vec2 coord_min()const {return _tex_min;}
    vec2 coord_max()const {return _tex_max;}

    //!
    //! \brief algorithm 算法类型
    //! \return
    //!
    eAlgorithmTypes algorithm()const{return ca;}

    //!
    //! \brief set 设置内部使用的算法，需要将类型设置为 Algorithm_User
    //! \param algo
    //!
    virtual void set(const pialgorithm &algo) = 0;

    //!
    //! \brief factor 平滑系数
    //!
    void factor(float f) {smfactor = f;}
    float factor()const {return smfactor;}

    //!
    //! \brief clean
    //! 清空内部计算完成的顶点数据
    //!
    virtual void clean() = 0;

    //!
    //! \brief control 控制点操作
    //!
    bs_matrixMxN <fpoint3> &control(){return ctrl;}
    bs_matrixMxN<fpoint3> control()const{return ctrl;}
    fpoint3 &control(size_t mi, size_t ni){return ctrl[mi][ni];}
    fpoint3 control(size_t mi, size_t ni)const{return ctrl[mi][ni];}
    fpoint3 *control(size_t mi){return ctrl[mi];}
    const fpoint3 *control(size_t mi)const{return ctrl[mi];}

    bs_matrixMxN <fpoint3> & operator ()(){return control();}
    bs_matrixMxN <fpoint3>  operator ()()const{return control();}
    fpoint3 *operator [](size_t mi){return control(mi);}
    const fpoint3 *operator [](size_t mi)const{return control(mi);}

};

#define IFace bs_ptr(isurface)
typedef IFace pisurface;
#undef IFace
#endif // ISURFACES_H
