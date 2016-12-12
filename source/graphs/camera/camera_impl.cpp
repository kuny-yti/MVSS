#include "camera_impl.h"
#include "frustum_impl.h"

camera_impl::camera_impl(const fpoint3 &position,
                         const vec3& focus,
                         const vec3& up,
                         float fovy,
                         float ratio,
                         float distn,
                         float distf,
                         eProjectionModes pm, isize size)
    : icamera()
{
    positions = (position);
    focuss = (focus);
    ups = (normalize(up));
    frustums = (bs_malloc(frustum_impl, fovy, ratio, distn, distf));
    window_ratios = (1.0f);
    window_sizes = (size);
    modes = (pm);
    viewMatrix_ = mat4::createLookAt(position, focus, up);
    _this_ = picamera(this, bs_ref::refOne);
}


picamera camera_impl::clone() const
{
    picamera cam = bs_malloc( camera_impl, positions, focuss, ups, frustums->fovy(),
                              frustums->ratio(), frustums->nears, frustums->fars);
    cam->window_size(window_sizes);
    return cam;
}

bool camera_impl::operator == (const picamera& rhs)  {
    return (rhs->positions == positions) &&
            (rhs->focuss == focuss) &&
            (rhs->ups == ups) &&
            ((rhs->frustums) == (frustums)) &&
            (rhs->window_ratios == window_ratios) &&
            (rhs->modes == modes) &&
            (rhs->window_sizes == window_sizes);
}

bool camera_impl::operator!=(const picamera& rhs)  {
    return !(*this == rhs);
}

mat4 camera_impl::matrix_view() const {
    updateVM();
    return viewMatrix_;
}

void camera_impl::matrix_view(const mat4& mvMat) {
    mat4 inv;
    if (mvMat.invert(inv)) {
        // preserve the focallength
        float focallength = focal_length();// length(focuss - positions);

        // calculate world-coordinates
        vec4 pos   = (inv * vec4(0.f, 0.f,  0.f, 1.f));
        vec4 look  = (inv * vec4(0.f, 0.f, -1.f, 0.f));
        vec4 focus = pos + focallength * look;
        vec4 up    = (inv * vec4(0.f, 1.f,  0.f, 0.f));

        reset(pos.xyz(), focus.xyz(), up.xyz());

        viewMatrix_ = mvMat;
    }
}

mat4 camera_impl::matrix_rotate() const {
    updateVM();
    return viewMatrix_.getRotationalPart();
}

mat4 camera_impl::matrix_view_inverse() const {
    updateVM();
    mat4 inv;
    if (viewMatrix_.invert(inv))
        return inv;
    else
        return mat4::identity;
}

mat4 camera_impl::matrix_frustum() const {
    updateFR();
    return mat4::createFrustum(frustums->lefts * window_ratios,
                               frustums->rights * window_ratios,
                               frustums->bottoms, frustums->tops,
                               frustums->nears, frustums->fars);
}

mat4 camera_impl::matrix_projection() const {
    if(modes == Mode_Orthographic) {
        if(window_ratios > 1.0f)
            return mat4::createOrtho(frustums->lefts * window_ratios,
                                     frustums->rights * window_ratios,
                                     frustums->tops, frustums->bottoms,
                                    -frustums->nears, frustums->fars);
        else
            return mat4::createOrtho(frustums->lefts, frustums->rights,
                                     frustums->tops * (1.0f/window_ratios),
                                     frustums->bottoms * (1.0f/window_ratios),
                                    -frustums->nears, frustums->fars);
    } else if(modes == Mode_Perspective) {
        float fovy = frustums->fovy();
        if(fovy < 6.f)
            fovy = 6.f;
        if(fovy > 175.f)
            fovy = 175.f;

        if (window_ratios >= 1.f)
            return mat4::createPerspective(bs_deg2rad(fovy), frustums->ratio() * window_ratios,
                                       frustums->nears, frustums->fars);
        else
            return mat4::createPerspective(atan(tan(bs_deg2rad(fovy/2.f))/(frustums->ratio() * window_ratios))*2,
                                           frustums->ratio() * window_ratios , frustums->nears, frustums->fars);
    }

    return matrix_frustum();
}
