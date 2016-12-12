#ifndef IGEOMETRY_H
#define IGEOMETRY_H
#include "bs_ptr.h"
#include "ialgorithm.h"
#include "iprimitive.h"
#include "bs_debug.h"

#define IGEOM bs_ptr(bsPackage igeometry)
typedef IGEOM pigeometry;
#undef IGEOM

typedef enum
{
    Draw_Array = 1 << 1,
    Draw_VBO  = 1 << 2,
    Draw_Indice = 1 << 3
}eDrawMethods;

typedef enum
{
    Primitive_Line,     ///< 直线
    Primitive_Circle,   ///< 圆圈
    Primitive_Curve,    ///< 曲线
    Primitive_Surface,  ///< 曲面

    Primitive_Box,      ///<
    Primitive_Plane,    ///< 平面
    Primitive_Cylinder, ///< 圆柱面
    Primitive_Disc,     ///< 圆盘面
    Primitive_Cone,     ///< 椎体
    Primitive_Sphere,   ///< 球面

    Primitive_Arrow,    ///< 箭头
    Primitive_Axis,     ///< 坐标轴

}ePrimitiveTypes;

bsPackage igeometry
{
    virtual ~igeometry(){}

    virtual bool initialize(const pirender &) = 0;
    virtual piprimitive create(ePrimitiveTypes t) = 0;
    virtual piprimitive create(const bs_string &file) = 0;

    virtual eDrawModes dmode() const = 0;
    virtual ePolygonModes pmode() const = 0;
    virtual int dmethod()const = 0;
    virtual pirender irender() = 0;

    virtual void draw (const piprimitive&, eDrawModes dm ,int met = Draw_Array, ePolygonModes pm = Polygon_Fill) = 0;
};
#endif // IGEOMETRY_H
