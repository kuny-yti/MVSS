#ifndef RAY_PICK_H
#define RAY_PICK_H

#include "maths/bs_vec3.h"

class ray_pick
{
    struct PlaneEqv
    {
        real a, b, c, d;

        /// <summary>
        /// 点法式方程
        /// </summary>
        PlaneEqv(const fpoint3 &m0, const fpoint3 &n)
        {
            a = n.x; b = n.y; c = n.z;
            d = -n.x * m0.x - n.y * m0.y - n.z * m0.z;
        }

    };
    struct LineEqv
    {
        real a1, b1, c1, d1;
        real a2, b2, c2, d2;

        //点向式方程
        LineEqv(){}
        LineEqv(const fpoint3 &m0, const fpoint3 & s);

        void TestPoint(const fpoint3 & p, real &res1, real &res2);
    };

    struct TriEqaResult
    {
        real x, y, z;
        bool hasSolution;

        TriEqaResult(const LineEqv &a, const PlaneEqv &c);
        bool SolveDEqv(real a1, real b1, real c1, real a2, real b2, real c2,
                       real &sx,  real &sy);

    };

    /// <summary>
    /// 转为单位向量
    /// </summary>
    fpoint3 Normalise(const fpoint3 &v)const;

    //向量积
    fpoint3 delivery(const fpoint3 &U, const fpoint3 &V) const;
    fpoint3 CalcPlaneNormal(const fpoint3 &P1,const fpoint3 &P2, const fpoint3 &P3)const;

    bool IsInTrg(const fpoint3 &vtxA, const fpoint3 &vtxB,const fpoint3 &vtxC,
                 const TriEqaResult &pHit);

    fpoint3 start;
    fpoint3 end;
public:
    ray_pick(const fpoint3 &rayStart, const fpoint3 &rayEnd);

    bool intersect(const fpoint3 &vtxA, const fpoint3 &vtxB, const fpoint3 &vtxC,
                   fpoint3 &pos);

    static bool pick (fpoint3 &pos, const fpoint3 &rayStart, const fpoint3 &rayEnd,
                      const fsize rect = fsize(0.02f,0.02f));
};

#endif // RAY_PICK_H
