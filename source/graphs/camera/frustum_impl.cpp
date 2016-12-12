#include "frustum_impl.h"
#include "camera_impl.h"

frustum_impl::frustum_impl()
{
    nears = (CAMERA_NEAR_DEFAULT);
    fars = (CAMERA_FAR_DEFAULT);
    float halfheight = tanf(0.5f * bs_deg2rad(CAMERA_FOVY_DEFAULT)) * nears;
    tops =  halfheight;
    bottoms = -halfheight;
    ratio(CAMERA_RATION_DEFAULT);
}
frustum_impl::frustum_impl(float fovy, float _ratio, float _near, float _far)
{
    nears = (_near);
    fars = (_far);
    float halfheight = tanf(0.5f * bs_deg2rad(fovy)) * nears;
    tops =  halfheight;
    bottoms = -halfheight;
    ratio(_ratio);
}


frustum_impl::frustum_impl(float left, float right, float bottom, float top,
                           float _near, float _far)
{
    lefts = (left);
    rights = (right);
    bottoms = (bottom);
    tops = (top);
    nears = (_near);
    fars = (_far);
}

bool frustum_impl::operator==(const pifrustum & rhs)
{
    return  (rhs->lefts == lefts) &&
            (rhs->rights == rights) &&
            (rhs->bottoms == bottoms) &&
            (rhs->tops == tops) &&
            (rhs->nears == nears) &&
            (rhs->fars == fars);
}
void frustum_impl::operator = (const pifrustum& rhs)
{
    this->lefts = rhs->lefts;
    this->rights = rhs->rights;
    this->bottoms = rhs->bottoms;
    this->tops = rhs->tops;
    this->nears = rhs->nears;
    this->fars = rhs->fars;

    for (int i = 0; i < NORMALS_NUM; ++i)
        this->normals_[i] = rhs->normal (i);

    this->nearp_ = rhs->nearp ();
    this->farp_ = rhs->farp ();
    this->campos_ = rhs->campos ();
}
void frustum_impl::update(const picamera &c)
{
    campos_ = c->positions;

    normals_[FARs]  = c->look();
    normals_[NEARs] = -1.f * normals_[FARs];

    vec3 cam2near = normals_[FARs] * nears;

    nearp_ = campos_ + cam2near;
    farp_  = campos_ + normals_[FARs] * fars;

    vec3 strafe = c->strafe();
    vec3 tmp;

    tmp = cam2near + tops * cross(strafe, c->look());
    normals_[TOP]    = normalize(cross(strafe, tmp));

    tmp = cam2near + bottoms * cross(strafe, c->look());
    normals_[BOTTOM] = normalize(cross(tmp, strafe));

    tmp = cam2near + lefts * strafe;
    normals_[LEFT]   = normalize(cross(cross(strafe, c->look()), tmp));

    tmp = cam2near + rights * strafe;
    normals_[RIGHT]  = normalize(cross(tmp, cross(strafe, c->look())));

    calc_vertex(c);
}

// Is this bounding-box culled?
bool frustum_impl::is_culled_xz(const faabb &bound) const
{
    vec3 urb = bound.max();
    vec3 llf = bound.min();

    vec3 points[4];
    points[0] = vec3(llf.x, 0.f, llf.z);
    points[1] = vec3(urb.x, 0.f, llf.z);
    points[2] = vec3(urb.x, 0.f, urb.z);
    points[3] = vec3(llf.x, 0.f, urb.z);

    for (size_t i = 0; i < 6; i++) {
        int outside = 0;
        for (size_t j = 0; j < 4; j++) {
            vec3 pos = (i < 4) ? campos_
                : ((i == 4) ? nearp() : farp());
            if (dot(normal(static_cast<int>(i)), points[j] - pos) >= 0.f)
                ++outside;
        }
        if (outside == 4)
            return true;
    }
    return false;
}

// Note: Seems to work, but not really tested.
bool frustum_impl::is_culled(const faabb &bound) const {
    vec3 urb = bound.max();
    vec3 llf = bound.min();

    vec3 points[8];
    points[0] = vec3(llf.x, llf.y, llf.z);
    points[1] = vec3(urb.x, llf.y, llf.z);
    points[2] = vec3(urb.x, llf.y, urb.z);
    points[3] = vec3(llf.x, llf.y, urb.z);
    points[4] = vec3(llf.x, urb.y, llf.z);
    points[5] = vec3(urb.x, urb.y, llf.z);
    points[6] = vec3(urb.x, urb.y, urb.z);
    points[7] = vec3(llf.x, urb.y, urb.z);

    for (size_t i = 0; i < 6; i++) {
        int outside = 0;
        for (size_t j = 0; j < 8; j++) {
            vec3 pos = (i < 4) ? campos_
                : ((i == 4) ? nearp() : farp());

            if (dot(normal(static_cast<int>(i)), points[j] - pos) >= 0.f)
                ++outside;
        }
        if (outside == 8)
            return true;
    }

    return false;

}

// Is this point culled?
bool frustum_impl::is_culled(const vec3& v) const
{
    vec3 dist = v - campos_;

    if ((dot(leftn(),   dist) >= 0.f) ||
        (dot(rightn(),  dist) >= 0.f) ||
        (dot(bottomn(), dist) >= 0.f) ||
        (dot(topn(),    dist) >= 0.f) ||
        (dot(farn(),    v - farp()) >= 0.f) ||
        (dot(nearn(),   v - nearp() ) >= 0.f))
    {
        return true;
    } else {
        // all tests survived? then the point is visible
        return false;
    }
}

void frustum_impl::fovy(float fovy)
{
    float halfheight = tanf(0.5f * bs_deg2rad(fovy)) * nears;
    //float oldRatio = getRatio();
    tops =  halfheight;
    bottoms = -halfheight;

    // update left and right to reflect previous ratio
    //setRatio(oldRatio);
}

void frustum_impl::ratio(float ratio_)
{
    float halfwidth = 0.5f * (tops - bottoms) * ratio_;
    lefts = -halfwidth;
    rights =  halfwidth;
    bottoms = -halfwidth / ratio_;
    tops =  halfwidth / ratio_;
}

const vec3& frustum_impl::campos() const {
    return campos_;
}

const vec3& frustum_impl::farp() const {
    return farp_;
}

const vec3& frustum_impl::nearp() const {
    return nearp_;
}

const vec3& frustum_impl::farn() const {
    return normals_[FARs];
}

const vec3& frustum_impl::nearn() const {
    return normals_[NEARs];
}

const vec3& frustum_impl::topn() const {
    return normals_[TOP];
}

const vec3& frustum_impl::bottomn() const {
    return normals_[BOTTOM];
}

const vec3& frustum_impl::rightn() const {
    return normals_[RIGHT];
}

const vec3& frustum_impl::leftn() const {
    return normals_[LEFT];
}


float frustum_impl::fovy() const {
    return bs_rad2deg(atanf(tops/nears) - atanf(bottoms/nears));
}

float frustum_impl::ratio() const {
    return (rights - lefts)/(tops - bottoms);
}

const vec3& frustum_impl::normal(int num) const
{
    return normals_[num];
}
const vec3& frustum_impl::vertex(int num)const
{
    return vertex_[num];
}
void frustum_impl::calc_vertex(const picamera &cam)
{
    float ratio;
    float farLeft;
    float farRight;
    float farBottom;
    float farTop;

    // perspective mode
    if(cam->modes== Mode_Perspective)
        ratio     = this->fars/ this->nears;
    // orthographic mode
    else if (cam->modes == Mode_Orthographic)
        ratio = 1;
    else if (cam->modes == Mode_Frustum)
    {
        ratio     = this->fars/ this->nears;
    }

    farLeft   = lefts * ratio;
    farRight  = rights * ratio;
    farBottom = bottoms * ratio;
    farTop    = tops * ratio;

    // compute 8 vertices of the frustum
    // near top right
    vertex_[RTN].x = rights;
    vertex_[RTN].y = tops;
    vertex_[RTN].z = nears;
    vertex_[RTN] += cam->positions;

    // near top left
    vertex_[LTN].x = lefts;
    vertex_[LTN].y = tops;
    vertex_[LTN].z = nears;
    vertex_[LTN] += cam->positions;

    // near bottom left
    vertex_[LBN].x = lefts;
    vertex_[LBN].y = bottoms;
    vertex_[LBN].z = nears;
    vertex_[LBN] += cam->positions;

    // near bottom right
    vertex_[RBN].x = rights;
    vertex_[RBN].y = bottoms;
    vertex_[RBN].z = nears;
    vertex_[RBN] += cam->positions;

    // far top right
    vertex_[RTF].x = farRight;
    vertex_[RTF].y = farTop;
    vertex_[RTF].z = fars;
    vertex_[RTF] += cam->positions;

    // far top left
    vertex_[LTF].x = farLeft;
    vertex_[LTF].y = farTop;
    vertex_[LTF].z = fars;
    vertex_[LTF] += cam->positions;

    // far bottom left
    vertex_[LBF].x = farLeft;
    vertex_[LBF].y = farBottom;
    vertex_[LBF].z = fars;
    vertex_[LBF] += cam->positions;

    // far bottom right
    vertex_[RBF].x = farRight;
    vertex_[RBF].y = farBottom;
    vertex_[RBF].z = fars;
    vertex_[RBF] += cam->positions;
}
