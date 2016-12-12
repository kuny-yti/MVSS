#ifndef bs_line3_H
#define bs_line3_H

#include "bs_vec3.h"

///This implements a 3D line(segment).
template <typename T>
class bs_line3 {
    bs_vector3<T> p_; /// position
    bs_vector3<T> d_; /// direction

public:
    /// trivial standard constructor
    bs_line3() {}

    /// construct a line from two points on a line
    bs_line3(const bs_vector3<T>& start, const bs_vector3<T>& end)
        : p_(start)
        , d_(end - start)
    {}

    /**
        * Interpolates between start and end
        * @param t Should be betweeen 0 and 1.
        * @return (start*(1-t) + end*t)
        */
    bs_vector3<T> interpolates(T t) const {
        return p_ + t * d_;
    }

    bs_vector3<T> start() const { return p_; }
    bs_vector3<T> end() const { return p_ + d_; }
    ///Set startingpoint to start, leaves end unchanged.
    void start(const bs_vector3<T>& s) { d_ = (p_ + d_) - s; p_ = s; }
    ///Set endpoint to end, leaves start unchanged.
    void end(const bs_vector3<T>& e) { d_ = (e - p_); }

    ///@return Vector from start to end
    bs_vector3<T> direction() const { return d_; }
    ///Set direction, changing endpoint, leaves start unchanged.
    void direction(const bs_vector3<T>& dir) { d_ = dir; }

    /**
         * intersect a linesegment with another linesegment
         * See RTR p618
         *
         * @param l the line to intersect this object with
         * @param v holds the result if an intersection took place,
         * otherwise it remains unchanged
         * @return true if these lines intersect
        */
    bool intersectSegments(const bs_line3<T>& l, bs_vector3<T>& v) const;

    /**
         * intersect a line with another line
         * See RTR p618
         *
         * @param l the line to intersect this object with
         * @param v holds the result if an intersection took place,
         * otherwise it remains unchanged
         * @return true if these lines intersect
        */
    bool intersect(const bs_line3<T>& l, bs_vector3<T>& v) const;
};

//------------------------------------------------------------------------------
//  non inline implementation
//------------------------------------------------------------------------------

template <typename T>
bool bs_line3<T>::intersectSegments(const bs_line3<T>& l, bs_vector3<T>& v) const {
    bs_vector3<T> c = cross(d_, l.d_);
    T q = lengthSq(c);

    if (std::fabs(q) < 0.00001f)
        return false;
    else {
        bs_vector3<T> o = l.p_ - p_;
        bs_vector3<T> d = l.d_;

        T t = o.x*d_.y*c.z + d_.x*c.y*o.z + c.x*o.y*d_.z
                - c.x*d_.y*o.z - d_.x*o.y*c.z - o.x*c.y*d_.z;

        T s = o.x*l.d_.y*c.z + l.d_.x*c.y*o.z + c.x*o.y*l.d_.z
                - c.x*l.d_.y*o.z - l.d_.x*o.y*c.z - o.x*c.y*l.d_.z;

        t /= q;
        s /= q;

        v = p_ + t*d_;
        if( (s >= 0.0) && (s <= 1.0)
                &&(t >= 0.0) && (t <= 1.0) )
            return true;
        else
            return false;
    }
}

template <typename T>
bool bs_line3<T>::intersect(const bs_line3<T>& l, bs_vector3<T>& v) const {
    bs_vector3<T> c = cross(d_, l.d_);
    T q = lengthSq(c);

    if (std::fabs(q) < 0.00001f)
        return false;
    else {
        bs_vector3<T> o = l.p_ - p_;
        bs_vector3<T> d = l.d_;

        T t = o.x*d_.y*c.z + d_.x*c.y*o.z + c.x*o.y*d_.z
                - c.x*d_.y*o.z - d_.x*o.y*c.z - o.x*c.y*d_.z;

        t /= q;
        v = p_ + t*d_;

        return true;
    }
}

template <typename T>
std::ostream& operator << (std::ostream& s, const bs_line3<T>& l) {
    return (s << "x = " << l.start() << " + t * " << l.direction());
}

typedef bs_line3<int>     iline3;
typedef bs_line3<float>   fline3;
typedef bs_line3<real>    dline3;


template<typename T>
bs_bstream &operator << (bs_bstream &in, const bs_line3<T> &v)
{
    in << v.start();
    in << v.direction();
    return in;
}

template<typename T>
bs_bstream &operator >> (bs_bstream &out, bs_line3<T> &v)
{
    bs_vector3<T> tmp;
    out >> tmp; v.start(tmp);
    out >> tmp; v.direction(tmp);
    return out;
}
template<typename T>
bs_tstream &operator << (bs_tstream &in, const bs_line3<T> &v)
{
    in << v.start();
    in << v.direction();
    return in;
}
template<typename T>
bs_tstream &operator >> (bs_tstream &out, bs_line3<T> &v)
{
    bs_vector3<T> tmp;
    out >> tmp; v.start(tmp);
    out >> tmp; v.direction(tmp);
    return out;
}
#endif // bs_line3_H
