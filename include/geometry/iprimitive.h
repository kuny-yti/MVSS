#ifndef IPRIMITIVE_H
#define IPRIMITIVE_H

#include "ialgorithm.h"
#include "bs_ptr.h"

#define IPRIM bs_ptr(bsPackage iprimitive)
typedef IPRIM piprimitive;
#undef IPRIM



bsPackage iprimitive
{
    virtual ~iprimitive(){}

    //!
    //! \brief mirror 对象镜像
    //! \param axis   指定坐标
    //!
    virtual void mirror(const vec3 &axis) = 0;
    //!
    //! \brief transform 需要转换的几何体需要继承此函数(注：可以使用OpenGL的固定管线功能转换)
    //! \param xform
    //!
    virtual void transform(const bs_xform &xform) = 0;
    //!
    //! \brief draw 继承后可在OpenGL内部执行几何体渲染
    //!
    virtual void draw() =0;


    virtual piprimitive clone() const {return piprimitive();}

    //!
    //! \brief rotation 根据指定轴用弧度来旋转
    //! \param radian 弧度
    //! \param axis   指定坐标
    //!
    void rotation(int radian, const vec3 &axis) {transform(rotations(radian, axis));}
    //!
    //! \brief rotation 根据指定轴用角度来旋转
    //! \param angle 角度
    //! \param axis   指定坐标
    //!
    void rotation(float angle, const vec3 &axis){transform(rotations(angle, axis));}
    //!
    //! \brief scale 沿指定轴缩放
    //! \param sc
    //!
    void scale(const vec3 &sc){transform(scales(sc));}
    //!
    //! \brief translation 平移指定轴
    //! \param tr
    //!
    void translation(const vec3 &tr){transform(translations(tr));}

    //!
    //! \brief
    //!
    static bs_xform rotations(int radian, const vec3 &axis){return bs_xform::createRotation(bs_deg2rad((real)radian), axis);}
    static bs_xform rotations(float angle, const vec3 &axis){return bs_xform::createRotation(angle, axis);}
    static bs_xform scales(const vec3 &sc){return ( bs_xform::createScale(sc));}
    static bs_xform translations(const vec3 &tr){return( bs_xform::createTranslation(tr));}

    //!
    //! \brief transform 默认几何体转换(注：若转换后，几何体顶点数据会更新)
    //! \param vext
    //! \param xform
    //!
    static void transform(tQuads3f &vext, const bs_xform &xform)
    {
        transform(vext.t0, xform);
        transform(vext.t1, xform);
    }
    static void transform(tQuads2f &vext, const bs_xform &xform)
    {
        transform(vext.t0, xform);
        transform(vext.t1, xform);
    }
    static void transform(tTriangle2f &vext, const bs_xform &xform)
    {
        bs_xform::RowType rt;
        rt = bs_xform::RowType (vext.p0.x, vext.p0.y, 1.0, 1.0);
        rt = xform * rt;
        vext.p0 = rt.xy();

        rt = bs_xform::RowType (vext.p1.x, vext.p1.y, 1.0, 1.0);
        rt = xform * rt;
        vext.p1 = rt.xy();

        rt = bs_xform::RowType (vext.p2.x, vext.p2.y, 1.0, 1.0);
        rt = xform * rt;
        vext.p2 = rt.xy();
    }
    static void transform(tTriangle3f &vext, const bs_xform &xform)
    {
        bs_xform::RowType rt = bs_xform::RowType (vext.p0, 1.0);
        rt = xform * rt;
        vext.p0 = rt.xyz();

        rt = bs_xform::RowType (vext.p1, 1.0);
        rt = xform * rt;
        vext.p1 = rt.xyz();

        rt = bs_xform::RowType (vext.p2, 1.0);
        rt = xform * rt;
        vext.p2 = rt.xyz();
    }
    static void transform(bs_matrixMxN<fpoint3> &vext, const bs_xform &xform)
    {
        for (size_t i = 0; i < vext.cols; ++i)
            for (size_t j = 0; j < vext.rows; ++j)
            {
                bs_xform::RowType rt = bs_xform::RowType (vext[i][j], 1.0);
                rt = xform * rt;
                vext[i][j] = rt.xyz();
            }
    }
    static void transform(bs_vector<fpoint3> &vext, const bs_xform &xform)
    {
        for (size_t j = 0; j < vext.count(); ++j)
        {
            bs_xform::RowType rt = bs_xform::RowType (vext[j], 1.0);
            rt = xform * rt;
            vext[j] = rt.xyz();
        }
    }

};

#endif // IPRIMITIVE_H
