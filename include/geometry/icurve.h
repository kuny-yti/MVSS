#ifndef ICURVE_H
#define ICURVE_H

#include "iprimitive.h"
#include "ishow.h"

bsPackage icurve : iprimitive , ishow
{
    enum {PointMin = 2};
    bs_vector <fpoint3> ctrl;
    eAlgorithmTypes ca;
    size_t dernum ;
    float smfactor ;

    virtual bool is_valid()const{return ctrl.count() >= PointMin;}
    //!
    //! \brief evaluate 评估曲线
    //! \param ca 算法类型
    //! \return
    //!
    virtual bool evaluate(eAlgorithmTypes ca = Algorithm_Bezier) = 0;
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
    //! \brief derived 曲线的精度
    //!
    void derived(size_t der){dernum = der;}
    size_t derived()const{return dernum;}

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
    bs_vector <fpoint3> &control() {return ctrl;}
    bs_vector <fpoint3> control()const {return ctrl;}
    fpoint3 &control(size_t inx) {return ctrl[inx];}
    fpoint3 control(size_t inx)const {return ctrl[inx];}
    bs_vector <fpoint3> & operator ()(){return control();}
    bs_vector <fpoint3>  operator ()()const{return control();}
    fpoint3 &operator [](size_t index){return control(index);}
    fpoint3 operator [](size_t index)const{return control(index);}
};

#define ICURVE bs_ptr(icurve)
typedef ICURVE picurve;
#undef ICURVE

#endif // ICURVE_H
