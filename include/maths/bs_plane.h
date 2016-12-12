#ifndef bs_plane_H
#define bs_plane_H
#include "bs_vec4.h"
#include "bs_line3.h"

/**
 * @brief a bs_plane class
 * The bs_plane is stored in hessian normal form: <br>
 * n*x + d = 0; <br>
 * n is the unit normal of this bs_plane pointing from the origin to this bs_plane <br>
 * (-d) >= 0 is the distance from the origin. <br>
 * <br>
 * Note: You can transform this bs_plane like this: <br>
 * mybs_plane.toVec4() = myMatrix4 * mybs_plane.toVec4(); <br>
 * <br>
 * Perhaps you want to do this: <br>
 * mat4 mv = tgt::getModelViewMatrix(); <br>
 * mat4 inv; <br>
 * mv.invert(inv); <br>
 * mybs_plane.toVec4() = inv * mybs_plane.toVec4(); <br>
*/
template<typename T>
struct bs_plane {
    typedef T ElemType;

    /// The normal of the bs_plane.
    bs_vector3<T> n;
    /// The negative distance from the orign.
    T d;

    /// trivial default constructor
    bs_plane() {}

    /// init with unit normal [a, b, c] and distance _d <br>
    /// BEWARE: It is your job to ensure that [a, b, c] is a unit vector and that d <= 0
    bs_plane(T a, T b, T c, T _d)
        : n(a, b, c)
        , d(_d)
    {}

    /// creates a bs_plane with a unit length normal and a distance <br>
    /// BEWARE: It is your job to ensure that [a, b, c] is a unit vector and that d <= 0
    bs_plane(const bs_vector3<T>& _n, T _d)
        : n(_n)
        , d(_d)
    {}

    /**
     * Creates a bs_plane defined by three points.
     */
    bs_plane(const bs_vector3<T>& v1, const bs_vector3<T>& v2, const bs_vector3<T>& v3) {
        n = normalize( cross(v1 - v2, v2 - v3) );
        d = -dot(v1, n);

        // correct sign of d
        if (d > T(0)) {
            n = -n;
            d = -d;
        }
    }

    /// creates a bs_plane with one from another type
    template<typename U>
    bs_plane(const bs_plane<U>& p)
        : n(p.n)
        , d(T(p.d))
    {}

    const bs_vector4<T>& to_vec4() const {
        return *reinterpret_cast<bs_vector4<T>*>(const_cast<bs_plane<T>*>(this)->n.elem);
    }

    bs_vector4<T>& to_vec4() {
        return *reinterpret_cast<bs_vector4<T>*>(n.elem);
    }

    /**
     * Returns the signed distance to this bs_plane to a given point. <br>
     * If result < 0 -> v lies in the same half space of the bs_plane like the origin. <br>
     * If result > 0 -> v lies in the other half space of the bs_plane like the origin.
     * @return the signed distance from the bs_plane to the point
    */
    T distance(const bs_vector3<T>& v) const {
        return dot(n, v) + d;
    }

    /**
     * Returns whether this bs_plane intersects with line \p l.
     * When there is an intersection the parameter to compute
     * the point will be returned in \p f.
     * If \p l represents an edge between v1 and v2 that were given to \p l
     * via the constructor an intersection takes place when \p f is in [0, 1]
    */
    bool intersect(const bs_line3<T>& l, T& f) const;

    /**
     * Returns whether this bs_plane intersects with \p p.
     * When there is an intersection it will be computed in \p l
    */
    bool intersect(const bs_plane<T>& p, bs_line3<T>& l) const;

    /**
     * clips a given polygon to this bs_plane
     * @param vin the input polygon
     * @param vout the output polygon
    */
    void clipPolygon(const bs_vector< bs_vector3<T> >& vin, bs_vector< bs_vector3<T> >& vout) const;

    /**
     * Clips a given polygon to this bs_plane.
     * This is a second version with c-arrays. Can be faster, but beware:
     * Buffer overruns are likly!!!
     * @param vin the input polygon
     * @param numIn number of input vertices
     * @param vout the output polygon
     * @param numOut number of output vertices
    */
    void clipPolygon(const bs_vector3<T>* vin, size_t numIn, bs_vector3<T>* vout, size_t& numOut) const;

    static void createCubeVertices(const bs_vector3<T>& llf, const bs_vector3<T>& urb, bs_vector3<T> cubeVertices[8]);

    static void createCubebs_planes(const bs_vector3<T> cv[8], bs_plane<T> cubebs_planes[6]);

    /**
     * Creates bs_planes in the aabbs_planes array from a axis aligned box given via llf and urb. The normal points to inside.
     * @param llf lower left front of the input axis aligned box
     * @param urb upper right back of the input axis aligned box
     * @param cubebs_planes the result is written to this array which must be able to hold at least 6 bs_planes
     */
    static void createAABbs_planes(const bs_vector3<T>& llf, const bs_vector3<T>& urb, bs_plane<T> aabbs_planes[6]) {
        bs_vector3<T> cv[8];// cube vertices
        createCubeVertices(llf, urb, cv);

        // cube bs_planes
        createCubebs_planes(cv, aabbs_planes);
    }

    /**
     * This method creates a polygon from this bs_plane.
     * It is ensured that this polygon is huge enough
     * so it may be clipped later against the axis aligned box
     * defined by llf and urb. <br>
     * Note that you can use this method with Vector, too: <br>
     * Vector\<vec3\> vout(4);
     * createQuad(llf, urb, &vout[0]);
     * @param llf lower left front of the input axis aligned box
     * @param urb upper right back of the input axis aligned box
     * @param v the result is written to this array which must be able to hold at least 4 vertices
     */
    void createQuad(bs_vector3<T> llf, bs_vector3<T> urb, bs_vector3<T>* v) const;

    /**
     * This routine clips a polygon against 6 bs_planes
     * @param aabbs_planes against these 6 bs_planes is clipped
     * @param vin Points to the input vertices. <br>
     *      BEWARE: vin is changed after this call. <br>
     *      AND EVEN WORSE: The size should be enough. Buffer overruns are likely!!!
     * @param numIn number of vertices in vin
     * @param vout Points to the output vertices. This is the result. <br>
     *      BEWARE: The size should be enough. Buffer overruns are likely!!!
     * @param numOut Number of output vertices. This a result, too.
     */
    static void clipAAB(bs_plane<T> aabbs_planes[6], bs_vector3<T>* vin, size_t numIn, bs_vector3<T>* vout, size_t& numOut);

    /**
     * This routine clips a polygon against 6 bs_planes
     * @param aabbs_planes against these 6 bs_planes is clipped
     * @param vin The input vertices. <br>
     *      BEWARE: vin is changed after this call.
     * @param vout Points to the output vertices. This is the result,
     */
    static void clipAAB(bs_plane<T> aabbs_planes[6], bs_vector< bs_vector3<T> >& vin, bs_vector< bs_vector3<T> >& vout);

    /**
     * This method cilps this bs_plane against an axis aligned box defined by llf and urb.
     * The output is written to vout. <br>
     * BEWARE: It is likely that you can gain performance by precomputing the bs_planes of the AAB once,
     * then calculate a proper polygon with createQuad and finally call a static clipAAB version
     * which takes the bs_planes an the polygon as input.
     * @param llf lower left front of the input axis aligned box
     * @param urb upper right back of the input axis aligned box
     * @param vout the result polygon
     */
    void clipAAB(const bs_vector3<T>& llf, const bs_vector3<T>& urb, bs_vector< bs_vector3<T> >& vout) const;
};

// non inline implementation

template<typename T>
bool bs_plane<T>::intersect(const bs_line3<T> &l, T& f) const {
    T f0 = distance(l.start());
    T f1 = -dot(n, l.direction());

    if (std::fabs(f1) < 0.00001f)
        return false;
    else {
        f = f0 / f1;
        return true;
    }
}

template<typename T>
bool bs_plane<T>::intersect(const bs_plane<T>& p, bs_line3<T> &l) const {
    bs_vector3<T> pn = p.n;
    T n00 = dot( n,  n);
    T n01 = dot( n, pn);
    T n11 = dot(pn, pn);
    T det = n00 * n11 - n01 * n01;

    if (std::fabs(det) < 0.00001f)
        return false;
    else {
        T inv_det = 1.0f/det;
        T c0 = (-n11 *   -d + n01 * -p.d) * inv_det;
        T c1 = (-n00 * -p.d + n01 *   -d) * inv_det;
        l.start(n * c0 + pn * c1);
        l.direction(cross(n, pn));
        return true;
    }
}

template<typename T>
void bs_plane<T>::clipPolygon(const bs_vector< bs_vector3<T> >& vin, bs_vector< bs_vector3<T> >& vout) const {
    // temporary array for distances
    bs_vector<T> vd;
    vd.reserve(vin.size());

    for (size_t i = 0; i < vin.size(); ++i)
        vd.push_back( distance(vin[i]) );

    for (size_t i1 = 0, i2 = 1; i1 < vin.size(); ++i1, ++i2) {
        // wrap i2 around
        i2 = ( i2 == vin.size() ) ? 0 : i2;

        // inside-inside
        if (vd[i1] < T(0) && vd[i2] < T(0))
            vout.push_back( vin[i2] );
        // inside-outside OR outside-inside
        else if (vd[i1] * vd[i2] < 0.f) {
            T scale = vd[i1] / (vd[i1] - vd[i2]);
            // interpolate vertex
            bs_vector3<T> newv = vin[i1] + (vin[i2] - vin[i1]) * scale;
            vout.push_back(newv);

            // outside-inside
            if (vd[i1] > T(0) && vd[i2] < T(0))
                vout.push_back( vin[i2] );
        }
        // do nothing for outside-outside
    }
}

template<typename T>
void bs_plane<T>::clipPolygon(const bs_vector3<T>* vin, size_t numIn, bs_vector3<T>* vout, size_t& numOut) const {
    // temporary array for distances
    T* vd = new T[numIn];
    numOut = 0;

    for (size_t i = 0; i < numIn; ++i)
        vd[i] = distance(vin[i]);

    for (size_t i1 = 0, i2 = 1; i1 < numIn; ++i1, ++i2) {
        // wrap i2 around
        i2 = ( i2 == numIn ) ? 0 : i2;

        // inside-inside
        if (vd[i1] < T(0) && vd[i2] < T(0)) {
            vout[numOut] = vin[i2];
            ++numOut;
        }
        // inside-outside OR outside-inside
        else if (vd[i1] * vd[i2] < 0.f) {
            T scale = vd[i1] / (vd[i1] - vd[i2]);
            // interpolate vertex
            vout[numOut] = vin[i1] + (vin[i2] - vin[i1]) * scale;
            ++numOut;

            // outside-inside
            if (vd[i1] > T(0) && vd[i2] < T(0)) {
                vout[numOut] = vin[i2];
                ++numOut;
            }
        }
        // do nothing for outside-outside
    }

    // clean up
    delete[] vd;
}

template<typename T>
void bs_plane<T>::createCubeVertices(const bs_vector3<T>& llf, const bs_vector3<T>& urb, bs_vector3<T> cubeVertices[8]) {
    cubeVertices[0] = bs_vector3<T>(llf.x, llf.y, urb.z);// llb 0
    cubeVertices[1] = bs_vector3<T>(urb.x, llf.y, urb.z);// lrb 1
    cubeVertices[2] = bs_vector3<T>(urb.x, urb.y, urb.z);// urb 2
    cubeVertices[3] = bs_vector3<T>(llf.x, urb.y, urb.z);// ulb 3

    cubeVertices[4] = bs_vector3<T>(llf.x, llf.y, llf.z);// llf 4
    cubeVertices[5] = bs_vector3<T>(urb.x, llf.y, llf.z);// lrf 5
    cubeVertices[6] = bs_vector3<T>(urb.x, urb.y, llf.z);// urf 6
    cubeVertices[7] = bs_vector3<T>(llf.x, urb.y, llf.z);// ulf 7
}

template<typename T>
void bs_plane<T>::createCubebs_planes(const bs_vector3<T> cv[8], bs_plane<T> cubebs_planes[6]) {
    cubebs_planes[0] = bs_plane<T>( cv[2], cv[1], cv[0] );// front
    cubebs_planes[1] = bs_plane<T>( cv[0], cv[3], cv[7] );// left
    cubebs_planes[2] = bs_plane<T>( cv[4], cv[5], cv[6] );// back
    cubebs_planes[3] = bs_plane<T>( cv[6], cv[2], cv[1] );// right
    cubebs_planes[4] = bs_plane<T>( cv[5], cv[1], cv[0] );// bottom
    cubebs_planes[5] = bs_plane<T>( cv[7], cv[3], cv[2] );// top
}

template<typename T>
void bs_plane<T>::createQuad(bs_vector3<T> llf, bs_vector3<T> urb, bs_vector3<T>* v) const {
// copy of llf and urb is needed here
    T param;
    bs_line3<T> l;

    // scale to ensure that a polygon is created that is huge enough
    urb.x += T(5);
    urb.y += T(5);
    urb.z -= T(5);
    llf.x -= T(5);
    llf.y -= T(5);
    llf.z += T(5);

    // check whether the bs_plane's normal lies in the x-y bs_plane
    if (abs(n).z < T(0.000001)) {
        bs_vector3<T> llb(llf.x, llf.y, urb.z);
        bs_vector3<T> urf(urb.x, urb.y, llf.z);

        // each found vector must be scaled hence a overlapping polygon is guaranteed
        l = bs_line3<T>(urb, urb + bs_vector3<T>(T(1), T(0), T(0)));
        intersect(l, param);
        v[0] = l.interpolates (param);

        l = bs_line3<T>(urf, urf + bs_vector3<T>(T(1), T(0), T(0)));
        intersect(l, param);
        v[1] = l.interpolates(param);

        l = bs_line3<T>(llf, llf + bs_vector3<T>(T(1), T(0), T(0)));
        intersect(l, param);
        v[2] = l.interpolates(param);

        l = bs_line3<T>(llb, llb + bs_vector3<T>(T(1), T(0), T(0)));
        intersect(l, param);
        v[3] = l.interpolates(param);
    }
    else {
        bs_vector3<T> lrf(urb.x, llf.y, llf.z);
        bs_vector3<T> ulb(llf.x, urb.y, urb.z);

        // each found vector must be scaled hence a overlapping polygon is guaranteed
        l = bs_line3<T>(ulb, ulb + bs_vector3<T>(T(0), T(0), T(1)));
        intersect(l, param);
        v[0] = l.interpolates(param);

        l = bs_line3<T>(urb, urb + bs_vector3<T>(T(0), T(0), T(1)));
        intersect(l, param);
        v[1] = l.interpolates(param);

        l = bs_line3<T>(lrf, lrf + bs_vector3<T>(T(0), T(0), T(1)));
        intersect(l, param);
        v[2] = l.interpolates(param);

        l = bs_line3<T>(llf, llf + bs_vector3<T>(T(0), T(0), T(1)));
        intersect(l, param);
        v[3] = l.interpolates(param);
    }
}

template<typename T>
void bs_plane<T>::clipAAB(bs_plane<T> aabbs_planes[6], bs_vector3<T>* vin, size_t numIn, bs_vector3<T>* vout, size_t& numOut) {
    for (size_t i = 0; i < 6; ++i) {
        aabbs_planes[i].clipPolygon(vin, numIn, vout, numOut);

        if (i == 5)
            return;

        // swap
        bs_vector3<T>* tmp = vin;
        vin = vout;
        vout = tmp;
        numIn = numOut;
    }
}

template<typename T>
void bs_plane<T>::clipAAB(bs_plane<T> aabbs_planes[6], bs_vector<bs_vector3<T> > &vin, bs_vector<bs_vector3<T> > &vout) {
    for (size_t i = 0; i < 6; ++i) {
        aabbs_planes[i].clipPolygon(vin, vout);

        if (i == 5)
            return;

        bs_swap(vin, vout);
        vout.clear();
    }
}

template<typename T>
void bs_plane<T>::clipAAB(const bs_vector3<T>& llf, const bs_vector3<T>& urb, bs_vector<bs_vector3<T> > &vout) const {
    // 1. compute AAB bs_planes
    bs_plane<T> aabbs_planes[6];
    createAABbs_planes(llf, urb, aabbs_planes);

    // 2. calculate a polygon which is huge enough
    bs_vector< bs_vector3<T> > vin(4);
    createQuad(llf, urb, &vin[0]);

    // 3. clip against all 6 bs_planes of the AAB
    clipAAB(aabbs_planes, vin, vout);
}

/// ostream-operator
template <typename T>
std::ostream& operator << (std::ostream& s, const bs_plane<T>& p) {
    return (s << p.n << " * x + " << p.d << " = 0");
}

// typedefs for easy usage
// defined types with a small 'p' like other absolute math typenamees in tgt
typedef bs_plane<int>      iplane;
typedef bs_plane<float>    fplane;
typedef bs_plane<real>     dplane;


template<typename T>
bs_bstream &operator << (bs_bstream &in, const bs_plane<T> &v)
{
    in << v.d;
    in << v.n;
    return in;
}

template<typename T>
bs_bstream &operator >> (bs_bstream &out, bs_plane<T> &v)
{
    out >> v.d;
    out >> v.n;
    return out;
}
template<typename T>
bs_tstream &operator << (bs_tstream &in, const bs_plane<T> &v)
{
    in << v.d;
    in << v.n;
    return in;
}

template<typename T>
bs_tstream &operator >> (bs_tstream &out, bs_plane<T> &v)
{
    out >> v.d;
    out >> v.n;
    return out;
}
#endif // bs_plane_H
