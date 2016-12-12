#ifndef GEOMETRY_DEFINE_H
#define GEOMETRY_DEFINE_H
#include "bs_define.h"
#include "maths/bs_vec4.h"
#include "maths/bs_line2.h"
#include "maths/bs_line3.h"
#include "maths/bs_color.h"

namespace primitive {

template<typename T>
struct tLine2
{
    enum {size = 2};
    union
    {
        struct
        {
            bs_vector2<T> p0;
            bs_vector2<T> p1;
        };
        bs_vector2<T> elem[size];
    };
    tLine2(){}
    tLine2(const tLine2 &tt)
    {
        p0 = tt.p0;
        p1 = tt.p1;
    }
    tLine2(const bs_vector<T> &P0, const bs_vector<T> &P1)
    {
        p0 = P0;
        p1 = P1;
    }
    ~tLine2(){}

    tLine2(const bs_line2<T> &l2)
    {
        p0 = l2.start();
        p1 = l2.end();
    }
    bs_line2<T> to_line()
    {
        return bs_line2<T>(p0, p1);
    }
};
template<typename T>
struct tLine3
{
    enum {size = 2};
    union
    {
        struct
        {
            bs_vector3<T> p0;
            bs_vector3<T> p1;
        };
        bs_vector3<T> elem[size];
    };
    tLine3(){}
    tLine3(const tLine3 &tt)
    {
        p0 = tt.p0;
        p1 = tt.p1;
    }
    tLine3(const bs_vector3<T> &P0, const bs_vector3<T> &P1)
    {
        p0 = P0;
        p1 = P1;
    }

    ~tLine3(){}
    tLine3(const bs_line3<T> &l3)
    {
        p0 = l3.start();
        p1 = l3.end();
    }
    bs_line3<T> to_line()
    {
        return bs_line3<T>(p0, p1);
    }
};

template<typename T>
struct tTriangle2
{
    enum {size = 3};
    union
    {
        struct
        {
            bs_vector2<T> p0;
            bs_vector2<T> p1;
            bs_vector2<T> p2;
        };
        bs_vector2<T> elem[size];
    };
    tTriangle2(){}
    tTriangle2(const tTriangle2 &tt)
    {
        p0 = tt.p0;
        p1 = tt.p1;
        p2 = tt.p2;
    }
    tTriangle2(const bs_vector<T> &P0,const bs_vector<T> &P1,const bs_vector<T> &P2)
    {
        p0 = P0;
        p1 = P1;
        p2 = P2;
    }
    ~tTriangle2(){}
};
template<typename T>
struct tQuads2
{
    enum {size = 2};
    union
    {
        struct
        {
            tTriangle2<T> t0;
            tTriangle2<T> t1;
        };
        tTriangle2<T> elem[size];
    };
    tQuads2(){}
    tQuads2(const tQuads2 &tq)
    {
        t0 = tq.t0;
        t1 = tq.t1;
    }
    tQuads2(const tTriangle2<T> &T0, const tTriangle2<T> &T1)
    {
        t0 = T0;
        t1 = T1;
    }
    ~tQuads2(){}
};

template<typename T>
struct tTriangle3
{
    enum{size = 3};
    union
    {
        struct
        {
            bs_vector3<T> p0;
            bs_vector3<T> p1;
            bs_vector3<T> p2;
        };
        bs_vector3<T> elem[size];
    };
    tTriangle3(){}
    tTriangle3(const tTriangle3 &tt){p0 = tt.p0;p1 = tt.p1;p2 = tt.p2;}
    tTriangle3(const bs_vector3<T> &P0, const bs_vector3<T> &P1, const bs_vector3<T> &P2)
    {p0 = P0;p1 = P1;p2 = P2;}
    ~tTriangle3(){}
};
template<typename T>
struct tQuads3
{
    enum {size = 2};
    union
    {
        struct
        {
            tTriangle3<T> t0;
            tTriangle3<T> t1;
        };
        tTriangle3<T> elem[size];
    };
    tQuads3(){}
    tQuads3(const tQuads3 &tt){t0 = tt.t0;t1 = tt.t1;}
    tQuads3(const tTriangle3<T> &T0, const tTriangle3<T> &T1){t0 = T0;t1 = T1;}
    ~tQuads3(){}
};

template<typename T>
struct tPrimitive2
{
    bs_vector2<T> vertex;
    bs_vector2<T> normal;
    bs_vector2<T> coord;
    bs_color color;
    tPrimitive2(){}
    tPrimitive2(const tPrimitive2 &tt)
    {
        vertex = tt.vertex;
        normal = tt.normal;
        coord = tt.coord;
        color = tt.color;
    }
    ~tPrimitive2(){}
};
template<typename T>
struct tPrimitive3
{
    bs_vector3<T> vertex;
    bs_vector3<T> normal;
    bs_vector2<T> coord;
    bs_color color;
    tPrimitive3(){}
    tPrimitive3(const tPrimitive3 &tt)
    {
        vertex = tt.vertex;
        normal = tt.normal;
        coord = tt.coord;
        color = tt.color;
    }
    ~tPrimitive3(){}
};

template<typename T>
struct tPrimitiveTriangle2
{
    enum {size = 2};
    union
    {
        struct
        {
            tPrimitive2<T> p0;
            tPrimitive2<T> p1;
        };
        tPrimitive2<T> elem[size];
    };
    tPrimitiveTriangle2(){}
    tPrimitiveTriangle2(const tPrimitiveTriangle2 &tt)
    {
        p0 = tt.p0;
        p1 = tt.p1;
    }
    ~tPrimitiveTriangle2(){}
};
template<typename T>
struct tPrimitiveTriangle3
{
    enum{size = 3};
    union
    {
        struct
        {
            tPrimitive3<T> p0;
            tPrimitive3<T> p1;
            tPrimitive3<T> p2;
        };
        tPrimitive3<T> elem[size];
    };
    tPrimitiveTriangle3(){}
    tPrimitiveTriangle3(const tPrimitiveTriangle3 &tt)
    {
        p0 = tt.p0;
        p1 = tt.p1;
        p2 = tt.p2;
    }
    ~tPrimitiveTriangle3(){}
};

template<typename T>
struct tPrimitiveQuads2
{
    enum {size = 2};
    union
    {
        struct
        {
            tPrimitiveTriangle2<T> t0;
            tPrimitiveTriangle2<T> t1;
        };
        tPrimitiveTriangle2<T> elem[size];
    };
    tPrimitiveQuads2(){}
    tPrimitiveQuads2(const tPrimitiveQuads2 &tt)
    {
        t0 = tt.t0;
        t1 = tt.t1;
    }
    ~tPrimitiveQuads2(){}
};
template<typename T>
struct tPrimitiveQuads3
{
    enum {size = 2};
    union
    {
        struct
        {
            tPrimitiveTriangle3<T> t0;
            tPrimitiveTriangle3<T> t1;
        };
        tPrimitiveTriangle3<T> elem[size];
    };
    tPrimitiveQuads3(){}
    tPrimitiveQuads3(const tPrimitiveQuads3 &tt)
    {
        t0 = tt.t0;
        t1 = tt.t1;
    }
    ~tPrimitiveQuads3(){}
};

typedef tLine2<int> tLine2i;
typedef tLine2<float> tLine2f;
typedef tLine2<real> tLine2d;

typedef tTriangle2<int> tTriangle2i;
typedef tTriangle2<float> tTriangle2f;
typedef tTriangle2<real> tTriangle2d;

typedef tQuads2<int> tQuads2i;
typedef tQuads2<float> tQuads2f;
typedef tQuads2<real> tQuads2d;

typedef tLine3<int> tLine3i;
typedef tLine3<float> tLine3f;
typedef tLine3<real> tLine3d;

typedef tTriangle3<int> tTriangle3i;
typedef tTriangle3<float> tTriangle3f;
typedef tTriangle3<real> tTriangle3d;

typedef tQuads3<int> tQuads3i;
typedef tQuads3<float> tQuads3f;
typedef tQuads3<real> tQuads3d;

typedef tPrimitive2<int> tPrimitive2i;
typedef tPrimitive2<float> tPrimitive2f;
typedef tPrimitive2<real> tPrimitive2d;

typedef tPrimitive3<int> tPrimitive3i;
typedef tPrimitive3<float> tPrimitive3f;
typedef tPrimitive3<real> tPrimitive3d;

typedef tPrimitiveTriangle2<int> tPrimitiveTriangle2i;
typedef tPrimitiveTriangle2<float> tPrimitiveTriangle2f;
typedef tPrimitiveTriangle2<real> tPrimitiveTriangle2d;

typedef tPrimitiveTriangle3<int> tPrimitiveTriangle3i;
typedef tPrimitiveTriangle3<float> tPrimitiveTriangle3f;
typedef tPrimitiveTriangle3<real> tPrimitiveTriangle3d;

typedef tPrimitiveQuads2<int> tPrimitiveQuads2i;
typedef tPrimitiveQuads2<float> tPrimitiveQuads2f;
typedef tPrimitiveQuads2<real> tPrimitiveQuads2d;

typedef tPrimitiveQuads3<int> tPrimitiveQuads3i;
typedef tPrimitiveQuads3<float> tPrimitiveQuads3f;
typedef tPrimitiveQuads3<real> tPrimitiveQuads3d;

}

struct tTriangleIndices
{
    enum {size = 3};
    union
    {
        struct {
            uint P0;
            uint P1;
            uint P2;
        };
        uint elem[size];
    };

    tTriangleIndices(uint p0, uint p1, uint p2)
    {
        P0 = p0; P1 = p1; P2 = p2;
    }
    explicit tTriangleIndices(uint *pi)
    {
        P0 = pi[0]; P1 = pi[1]; P2 = pi[2];
    }
    tTriangleIndices(const tTriangleIndices &ti)
    {
        *this = ti;
    }
    tTriangleIndices &operator =(const tTriangleIndices &ti)
    {
        P0 = ti.P0;
        P1 = ti.P1;
        P2 = ti.P2;
        return *this;
    }
    tTriangleIndices &operator =(uint *pi)
    {
        P0 = pi[0]; P1 = pi[1]; P2 = pi[2];
        return *this;
    }
    uint &operator [](int i)
    {
        bsASSERT(i >= size, "");
        return elem[i];
    }
    const uint &operator [](int i)const
    {
        bsASSERT(i >= size, "");
        return elem[i];
    }
};
struct tQuadIndices
{
    enum {col = 2, row = 3};
    union
    {
        struct{
            uint T0[row];
            uint T1[row];
        };
        uint elem[col][row];
    };

    tQuadIndices(tTriangleIndices t0, tTriangleIndices t1)
    {
        for (int i = 0; i < row; ++i)
        {
            T0[i] = t0.elem[i];
            T1[i] = t1.elem[i];
        }
    }
    tQuadIndices(uint t0p0, uint t0p1, uint t0p2,
                 uint t1p0, uint t1p1, uint t1p2)
    {
        T0[0] = t0p0;
        T1[0] = t1p0;

        T0[1] = t0p1;
        T1[1] = t1p1;

        T0[2] = t0p2;
        T1[2] = t1p2;
    }
    tQuadIndices(uint *t0pi, uint *t1pi)
    {
        for (int i = 0; i < row; ++i)
        {
            T0[i] = t0pi[i];
            T1[i] = t1pi[i];
        }
    }
    explicit tQuadIndices(uint *tipi)
    {
        *this = tipi;
    }
    tQuadIndices(const tQuadIndices &quad)
    {
        *this = quad;
    }
    tQuadIndices &operator =(uint *tipi)
    {
        for (int i = 0; i < row; ++i)
        {
            T0[i] = tipi[i];
            T1[i] = tipi[i+row];
        }
        return *this;
    }
    tQuadIndices &operator =(const tQuadIndices &quad)
    {
        for (int i = 0; i < row; ++i)
        {
            T0[i] = quad.T0[i];
            T1[i] = quad.T1[i];
        }
        return *this;
    }

    uint *operator [](int i)
    {
        bsASSERT(i >= col, "");
        return elem[i];
    }
    const uint *operator [](int i)const
    {
        bsASSERT(i >= col, "");
        return elem[i];
    }
};

struct tMxNIndices
{
    enum {col = 2, row = 2};
    union
    {
        struct{
            uint T0[row];
            uint T1[row];
        };
        uint elem[col][row];
    };
    tMxNIndices(uint i0, uint i1, uint i2, uint i3)
    {
        T0[0] = i0;
        T0[1] = i1;
        T1[0] = i2;
        T1[1] = i3;
    }
};
#endif // GEOMETRY_DEFINE_H
