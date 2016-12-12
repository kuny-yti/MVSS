#ifndef bs_aabb_H
#define bs_aabb_H
#include "bs_define.h"
#include "bs_mat4X4.h"
#include "bs_plane.h"
#include "tools/bs_stream.h"

// Axis-aligned bounding box
template<typename T>
class bs_aabb
{
public:
    typedef T tElemTypes;
public:
    bs_aabb() { }
    bs_aabb(const bs_vector3<T>& v): min_(v),max_(v){}

    bs_aabb(const bs_vector3<T>& v1, const bs_vector3<T>& v2):
        min_(v1),max_(v1) {append(v2);}

    void append(const bs_vector3<T>& v)
    {
        min_ = bs_min(min(), v);
        max_ = bs_max(max(), v);
    }

    void append(const bs_aabb& b) {
        append(b.min());
        append(b.max());
    }

    bs_vector3<T> min() const { return min_; }
    bs_vector3<T> max() const { return max_; }

    T width() const{return diagonal().x;}
    T height() const{return diagonal().y;}
    T depth() const{return diagonal().z;}

    bs_aabb& operator |= (const bs_aabb  &rhs)
    {
        min_ = ::min(min(), rhs.min());
        max_ = ::max(max(), rhs.max());
        return *this;
    }
    bs_aabb& operator &= (const bs_aabb  &rhs)
    {
        min_ = ::max(min(), rhs.min());
        max_ = ::min(max(), rhs.max());
        return *this;
    }
    bs_aabb& operator /= (T rhs)
    {
        return this->operator *= ((T)1.0 / rhs);
    }
    bs_aabb& operator *= (T rhs)
    {
        min_ *= rhs;
        max_ *= rhs;
        return *this;
    }
    bs_aabb& operator -= (const bs_vector3<T>& rhs)
    {
        min_ -= rhs;
        max_ -= rhs;
        return *this;
    }
    bs_aabb& operator += (const bs_vector3<T>& rhs)
    {
        min_ += rhs;
        max_ += rhs;
        return *this;
    }
    bs_aabb operator - () const
    {
        return bs_aabb(-this->max(), -this->min());
    }

    //!
    //! \brief center 返回中心点
    //! \return
    //!
    bs_vector3<T> center() const {return (diagonal() * 0.5f + min());}
    //!
    //! \brief diagonal 返回对角线尺寸
    //! \return
    //!
    bs_vector3<T> diagonal() const {return (max_ - min_);}
    //!
    //! \brief corner 返回角顶点
    //! \param index
    //! \return
    //!
    bs_vector3<T> corner(const bs_vector3<T> &index) const
    {
        bs_vector3<T> _corner;
        _corner.x = (index.x >0) ? max_.x : min_.x;
        _corner.y = (index.y >0) ? max_.y : min_.y;
        _corner.z = (index.z >0) ? max_.z : min_.z;
        return _corner;
    }
    //!
    //! \brief corners 返回8个角的顶点
    //! \param corners
    //! \return
    //!
    bool corners(bs_vector3<T> cs[8] ) const
    {
        int n = 0;
        if (is_valid())
        {
            bs_vector3<T> P;
            int i,j,k;
            for( i = 0; i < 2; i++ )
            {
                P.x = (i) ? max().x : min().x;
                for ( j = 0; j < 2; j++ )
                {
                    P.y = (j) ? max().y : min().y;
                    for ( k = 0; k < 2; k++ )
                    {
                        P.z = (k) ? max().z : min().z;
                        cs[n++] = P;
                    }
                }
            }
        }
        return (8 == n);
    }

    //!
    //! \brief volume 返回边界框的体积
    //! \return
    //!

    T volume() const {return fabs((min().x - max().x) * (min().y - max().y) * (min().z - max().z));}

    //!
    //! \brief distance 返回点v到边界框的最短距离
    //! \param v
    //! \return
    //!
    T distance (const bs_vector3<T>& v) const
    {
        return sqrt(distanceSq(v));
    }
    T distanceSq (const bs_vector3<T>& v)const
    {
        if (contains(v))
            return T(0);
        else
        {
            bs_vector3<T> maxDist(0,0,0);

            if (v.x < min().x)
                maxDist.x = min().x - v.x;
            else if (v.x > max().x)
                maxDist.x = v.x - max().x;

            if (v.y < min().y)
                maxDist.y = min().y - v.y;
            else if (v.y > max().y)
                maxDist.y = v.y - max().y;

            if (v.z < min().z)
                maxDist.z = min().z - v.z;
            else if (v.z > max().z)
                maxDist.z = v.z - max().z;

            return lengthSq(maxDist);
        }
    }

    //!
    //! \brief is_valid 返回边界框是否有效
    //! \return
    //!
    bool is_valid() const { return min() != max() && min() < max(); }

    //!
    //! \brief contains 是否包含p点
    //! \param p
    //! \return
    //!
    bool contains(const bs_vector3<T>& p)
    {
        return ( (p.x >= min().x) && (p.y >= min().y) && (p.z >= min().z) &&
                 (p.x <= max().x) && (p.y <= max().y) && (p.z <= max().z) );
    }

    //!
    //! \brief contains 是否包含b边界框
    //! \param b
    //! \return
    //!
    bool contains(const bs_aabb& b) {return ( contains(b.min()) && contains(b.max()) );}

    //!
    //! \brief intersects 返回边界框是否相交
    //! \param b
    //! \return
    //!
    bool intersects(const bs_aabb& b) const
    {
        if ((min().x > b.max().x) || (b.min().x > max().x))
            return false;
        if ((min().y > b.max().y) || (b.min().y > max().y))
            return false;
        if ((min().z > b.max().z) || (b.min().z > max().z))
            return false;

        return true;
    }
    //!
    //! \brief intersects 是否和平面p相交
    //! \param p
    //! \return
    //!
    bool intersects(const bs_plane<T>& p) const
    {
        bool pointsNeg = false;
        bool pointsPos = false;

        T d = p.distance(vec3(min().x, min().y, min().z));
        if(d < 0.0) pointsNeg = true; else if(d > 0.0) pointsPos = true;
        d = p.distance(bs_vector3<T>(max().x, min().y, min().z));
        if(d < 0.0) pointsNeg = true; else if(d > 0.0) pointsPos = true;
        d = p.distance(bs_vector3<T>(min().x, max().y, min().z));
        if(d < 0.0) pointsNeg = true; else if(d > 0.0) pointsPos = true;
        d = p.distance(bs_vector3<T>(min().x, min().y, max().z));
        if(d < 0.0) pointsNeg = true; else if(d > 0.0) pointsPos = true;
        d = p.distance(bs_vector3<T>(max().x, max().y, min().z));
        if(d < 0.0) pointsNeg = true; else if(d > 0.0) pointsPos = true;
        d = p.distance(bs_vector3<T>(min().x, max().y, max().z));
        if(d < 0.0) pointsNeg = true; else if(d > 0.0) pointsPos = true;
        d = p.distance(bs_vector3<T>(max().x, min().y, max().z));
        if(d < 0.0) pointsNeg = true; else if(d > 0.0f) pointsPos = true;
        d = p.distance(bs_vector3<T>(max().x, max().y, max().z));
        if(d < 0.0) pointsNeg = true; else if(d > 0.0) pointsPos = true;

        return (pointsNeg && pointsPos);
    }

    bool inside_xz(const bs_aabb& bound) const
    {
        bsASSERT(       is_valid (), "This Box ist not valid.");
        bsASSERT(bound.is_valid(), "Box b ist not valid.");

        bs_vector3<T> llfb = bound.min();
        bs_vector3<T> urbb = bound.max();

        T r0x = bs_min(min()[0], max()[0]);
        T r1x = bs_max(min()[0], max()[0]);
        T r0y = bs_min(min()[2], max()[2]);
        T r1y = bs_max(min()[2], max()[2]);
        T r2x = bs_min(llfb[0], urbb[0]);
        T r3x = bs_max(llfb[0], urbb[0]);
        T r2y = bs_min(llfb[2], urbb[2]);
        T r3y = bs_max(llfb[2], urbb[2]);

        return (r0x >= r2x) && (r0y >= r2y) && (r1x <= r3x) && (r1y <= r3y);
    }
    bool inside_xz(const vec3& v) const {
        bsASSERT(  is_valid(), "This Box ist not defined.");

        return (min()[0] <= v[0]) && (v[0] <= max()[0])
                && (min()[2] <= v[2]) && (v[2] <= max()[2]);
    }

    //!
    //! \brief inside 返回bound边界框是否在内部
    //! \param bound
    //! \return
    //!
    bool inside(const bs_aabb& bound) const {
        bsASSERT(       is_valid(), "This Box ist not defined.");
        bsASSERT(bound.is_valid(), "Box b ist not defined.");

        vec3 llfb = bound.min();
        vec3 urbb = bound.max();

        T r0x = bs_min(min()[0], max()[0]);
        T r1x = bs_max(min()[0], max()[0]);
        T r0y = bs_min(min()[1], max()[1]);
        T r1y = bs_max(min()[1], max()[1]);
        T r0z = bs_min(min()[2], max()[2]);
        T r1z = bs_max(min()[2], max()[2]);

        T r2x = bs_min(llfb[0], urbb[0]);
        T r3x = bs_max(llfb[0], urbb[0]);
        T r2y = bs_min(llfb[1], urbb[1]);
        T r3y = bs_max(llfb[1], urbb[1]);
        T r2z = bs_min(llfb[2], urbb[2]);
        T r3z = bs_max(llfb[2], urbb[2]);

        return (r0x >= r2x) && (r1x <= r3x) &&
                (r0y >= r2y) && (r1y <= r3y) &&
                (r0z >= r2z) && (r1z <= r3z);
    }

    //!
    //! \brief inside 返回v点是否在内部
    //! \param v
    //! \return
    //!
    bool inside(const bs_vector3<T>& v) const {
        bsASSERT(  is_valid(), "This Box ist not valid.");

        return (min()[0] <= v[0]) && (v[0] <= max()[0]) &&
                (min()[1] <= v[1]) && (v[1] <= max()[1]) &&
                (min()[2] <= v[2]) && (v[2] <= max()[2]);
    }

    bs_aabb transform(const bs_matrix4X4<T>& m) const
    {
        bs_aabb b;
        b.append(m * bs_vector3<T>(min().x, min().y, min().z));
        b.append(m * bs_vector3<T>(max().x, min().y, min().z));
        b.append(m * bs_vector3<T>(min().x, max().y, min().z));
        b.append(m * bs_vector3<T>(min().x, min().y, max().z));
        b.append(m * bs_vector3<T>(max().x, max().y, min().z));
        b.append(m * bs_vector3<T>(min().x, max().y, max().z));
        b.append(m * bs_vector3<T>(max().x, min().y, max().z));
        b.append(m * bs_vector3<T>(max().x, max().y, max().z));
        return b;
    }


    friend bs_bstream &operator << (bs_bstream &in, const bs_aabb &v)
    {
        in << v.min_;
        in << v.max_;
        return in;
    }
    friend bs_bstream &operator >> (bs_bstream &out, bs_aabb &v)
    {
        out >> v.min_;
        out >> v.max_;
        return out;
    }
    friend bs_tstream &operator << (bs_tstream &in, const bs_aabb &v)
    {
        in << v.min_;
        in << v.max_;
        return in;
    }
    friend bs_tstream &operator >> (bs_tstream &out, bs_aabb &v)
    {
        out >> v.min_;
        out >> v.max_;
        return out;
    }

private:
    bs_vector3<T> min_; //lower left front
    bs_vector3<T> max_; //upper right back
};

typedef bs_aabb<int> iaabb;
typedef bs_aabb<float> faabb;
typedef bs_aabb<real> daabb;
#endif // bs_aabb_H
