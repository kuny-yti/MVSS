#ifndef bs_line2_H
#define bs_line2_H
#include "bs_vec2.h"

///This implements a 2D line(segment).
template <typename T>
class bs_line2 {
    bs_vector2<T> p_; /// position
    bs_vector2<T> d_; /// direction

public:
    /// trivial standard constructor
    bs_line2() {}

    /// construct a line from starting and end point
    bs_line2(const bs_vector2<T>& start, const bs_vector2<T>& end)
        : p_(start)
        , d_(end - start)
    {}

    /**
        * Interpolates between start and end
        * @param t Should be betweeen 0 and 1 for line segments
        * @return (start*(1-t) + end*t)
        */
    bs_vector2<T> interpolates(T t) {
        return p_ + t * d_;
    }

    bs_vector2<T> start() const { return p_; }
    bs_vector2<T> end() const { return p_ + d_; }
    ///Set startingpoint to start, leaves end unchanged.
    void start(const bs_vector2<T>& s) { d_ = (p_ + d_) - s; p_ = s; }
    ///Set endpoint to end, leaves start unchanged.
    void end(const bs_vector2<T>& e) { d_ = (e - p_); }

    ///@return Vector from start to end
    bs_vector2<T> direction() const { return d_; }
    ///Set direction, changing endpoint, leaves start unchanged.
    void direction(const bs_vector2<T>& dir) { d_ = dir; }

    /**
        * intersect a linesegment with another linesegment
        * See RTR p617.
        *
        * @param l the line to intersect this object with
        * @param v holds the result if an intersection took place,
        * otherwise it remains unchanged
        * @return true if these lines intersect, false otherwise
        */
    bool intersectSegments(const bs_line2<T>& l, bs_vector2<T>& v) const;

    /**
        * intersect a line with another line
        *
        * @param l the line to intersect this object with
        * @param v holds the result if an intersection took place,
        * otherwise it remains unchanged
        * @return true if these lines intersect, false otherwise
        */
    bool intersect(const bs_line2<T>& l, bs_vector2<T>& v) const;
};

// non inline implementation

template <typename T>
bool bs_line2<T>::intersectSegments(const bs_line2<T>& l, bs_vector2<T>& v) const {
    T f = dot(l.d_, bs_vector2<T>(-d_.y, d_.x));

    T e = dot((p_-l.p_), bs_vector2<T>(-d_.y, d_.x));
    T d = dot((p_-l.p_), bs_vector2<T>(-l.d_.y, l.d_.x));

    //     std::cout << "d: " << d << " e: "<< e << " f: " << f << "\n";

    //test if s and t are in range [0, 1]:
    if(f > 0.0) {
        if((d < 0.0) || (d > f))
            return false;
    }
    else {
        if((d > 0.0) || (d < f))
            return false;
    }

    if(f > 0.0) {
        if((e < 0.0) || (e > f))
            return false;
    }
    else {
        if((e > 0.0) || (e < f))
            return false;
    }

    //     T t = e/f;
    T s = d/f;
    //     std::cout << "s: " << s << " t: "<< t << "\n";

    //     v = l.p_ + t * l.d_;
    //     std::cout << "v: " << v << "\n";
    v = p_ + (s * d_);
    //     std::cout << "v: " << v << "\n";
    return true;
}


template <typename T>
bool bs_line2<T>::intersect(const bs_line2<T>& l, bs_vector2<T>& v) const {
    // check whether these lines interesect
    T q = d_.y * l.d_.x - d_.x * l.d_.y;

    if (q == 0.f)
        return false; // these lines a collinear or stack

    // calculate paramter for this line where the lines intersect
    T t = -( l.d_.x * (p_.y - l.p_.y) + l.d_.y * l.p_.x - l.d_.y * p_.x ) / q;
    v = p_ + t * d_;

    return true;
}

template <typename T>
std::ostream& operator << (std::ostream& s, const bs_line2<T>& l) {
    return (s << "x = " << l.start() << " + t * " << l.direction());
}

typedef bs_line2<int>     iline2;
typedef bs_line2<float>   fline2;
typedef bs_line2<real>    dline2;


template<typename T>
bs_bstream &operator << (bs_bstream &in, const bs_line2<T> &v)
{
    in << v.start();
    in << v.direction();
    return in;
}

template<typename T>
bs_bstream &operator >> (bs_bstream &out, bs_line2<T> &v)
{
    bs_vector2<T> tmp;
    out >> tmp; v.start(tmp);
    out >> tmp; v.direction(tmp);
    return out;
}

template<typename T>
bs_tstream &operator << (bs_tstream &in, const bs_line2<T> &v)
{
    in << v.start();
    in << v.direction();
    return in;
}
template<typename T>
bs_tstream &operator >> (bs_tstream &out, bs_line2<T> &v)
{
    bs_vector2<T> tmp;
    out >> tmp; v.start(tmp);
    out >> tmp; v.direction(tmp);
    return out;
}
#endif // bs_line2_H
