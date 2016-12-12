#include "camera_control_impl.h"

//! 复位相机数据
void camera_control_impl::reset()
{
    cameras->reset (CAMERA_POSITION_DEFAULT, CAMERA_FOCUS_DEFAULT, CAMERA_UP_DEFAULT);
}

//! 相机当前视图模式
void camera_control_impl::view(eCameraViews cv)
{
    switch ((int)cv) {
    case View_Left:

        break;
    case View_Right:

        break;
    case View_Top:

        break;
    case View_Bottom:

        break;
    case View_Front:
        cameras->position(mat4::createRotation(0, vec3(0,1,0)) * CAMERA_POSITION_DEFAULT);
        break;
    case View_Rear:

        break;
    default:
        break;
    }
}

//! 相机旋转
void camera_control_impl::rotation(vec2 newMouse)
{
    if (newMouse == mouse)
        return;

    /* First, figure out z-coordinates for projection of P1 and P2 to deformed sphere */
    vec3 p1 = project_sphere(mouse);
    vec3 p2 = project_sphere(newMouse);

    /* Now, find the axis we are going to rotate about*/
    vec3 axis = cross(p2, p1);

    /*... and calculate the angle phi between the two vectors which is the
      angle we need to rotate about*/
    vec3 d = p1 - p2;
    float t = length(d) / (2.0f * radiuss);

    /* avoid problems with out-of-control values... */
    if (t > 1.0f) t = 1.0f;
    if (t < -1.0f) t = -1.0f;

    float phi = 2.f * asinf(t);

    rotation(axis, phi);
}
void camera_control_impl::rotation(const quat &q)
{
    vec3 position = cameras->positions;
    position -= focus;
    position = quat::rotate(position, q);
    position += focus;

    vec3 focus_ = cameras->focuss;
    focus_ -= focus;
    focus_ = quat::rotate(focus_, q);
    focus_ += focus;

    vec3 upVector = cameras->ups;
    upVector = quat::rotate(upVector, q);

    cameras->reset(position, focus_, upVector);
}
void camera_control_impl::rotation(const vec3 &axis, float angle)
{
    if (panning == Camera_Target)
    {
        rotation(quat::createQuat(angle, coord_transform(axis)));
    }
    else if (panning == Camera_Eye)
    {
        vec3 look = normalize( quat::rotate(cameras->look(), angle, axis) );

        if ( fabs(dot( look, cameras->ups)) > .995f )
            return;

        cameras->focus(cameras->positions + cameras->focal_length()*look);
    }
}
void camera_control_impl::rotation(float Hangle, float Vangle)
{
    quat horz = quat::createQuat(Hangle, cameras->ups);
    quat vert = quat::createQuat(Vangle, cameras->strafe());

    // this way, we save calculations by multiplying the quaternions representing
    // the horizontal and vertical rotations instead of changing the view twice, once
    // for each angle and axis
    vec3 look = normalize( quat::rotate(cameras->look(), vert * horz) );

    // we do not allow views that are too similar to the up-vector since this results
    // in "flickering"
    if (fabs(dot(look, cameras->ups)) > .995f)
        return;

    // Set the new focus-point
    cameras->focus(cameras->positions + cameras->focal_length()*look);
}
void camera_control_impl::rotation(eRotationModes mode, float angle)
{
    if (mode == Rotation_Horizontal)
        rotation(cameras->ups, angle);
    else if (mode == Rotation_Vertical)
        rotation(cameras->strafe(), angle);
}

void camera_control_impl::roll(eRollModes mode, float angle)
{
    if (mode == Roll_Horizontal)
    {
        vec3 up = normalize( quat::rotate(cameras->ups, angle, cameras->look()) );
        cameras->up(up);
    }
    else if (mode == Roll_Vertical)
    {
        vec3 up = normalize( quat::rotate(cameras->ups, angle, cameras->strafe()) );
        cameras->up(up);
        vec3 look = cross(up, cameras->strafe());
        cameras->focus(cameras->positions + look);
    }
}

void camera_control_impl::move(eMoveModes mode, float length)
{
    switch ((int)mode) {
    case Move_Forward:
    {
        vec3 motionvector = length*cameras->look();
        cameras->position( cameras->positions + motionvector );
        cameras->focus( cameras->focuss + motionvector );
    }
        break;
    case Move_Backward:
    {
        vec3 motionvector = (-length)*cameras->look();
        cameras->position(cameras->positions + motionvector );
        cameras->focus(cameras->focuss + motionvector );
    }
        break;
    case Move_Up:
    {
        vec3 motionvector = length*cameras->ups;
        cameras->position( cameras->positions + motionvector );
        cameras->focus( cameras->focuss + motionvector );
    }
        break;
    case Move_Down:
    {
        vec3 motionvector = (-length)*cameras->ups;
        cameras->position( cameras->positions + motionvector );
        cameras->focus( cameras->focuss + motionvector );
    }
        break;
    case Move_Left:
    {
        vec3 motionvector = (length)*cameras->strafe ();
        cameras->position( cameras->positions + motionvector );
        cameras->focus( cameras->focuss + motionvector );
    }
        break;
    case Move_Right:
    {
        vec3 motionvector = (-length)*cameras->strafe ();
        cameras->position( cameras->positions + motionvector );
        cameras->focus( cameras->focuss + motionvector );
    }
        break;
    }
}
void camera_control_impl::move(const vec3 &ax, float length)
{
    vec3 axis = ax;
    if (panning == Camera_Target)
    {
        float frustFactor = cameras->focal_length() / cameras->frustums->nears;
        float frustWidth  = ( cameras->frustums->rights - cameras->frustums->lefts );
        float frustHeight = ( cameras->frustums->tops - cameras->frustums->bottoms );
        axis = normalize(axis) * length * frustFactor;
        axis.x *= frustWidth;
        axis.y *= frustHeight;

        // find axis in world coordinates according to the axis given in camera coordinates
        axis = coord_transform(axis);

        move(-axis);
        focus -= axis;
    }
    else if (panning == Camera_Eye)
    {
        vec3 motionvector = length * normalize(axis);
        move(motionvector);
    }

}
void camera_control_impl::move(const vec3 &motionvector)
{
    cameras->position( cameras->positions + motionvector );
    cameras->focus( cameras->focuss + motionvector );
}

void camera_control_impl::zoom(float factor)
{
    factor = 1.f / factor;
    cameras->position( (1.f-factor) * cameras->focuss + factor * cameras->positions);
}
void camera_control_impl::zoom(vec2 newMouse)
{
    zoom( 1 + dot( (mouse - newMouse), vec2( 0.f, 1.f)) );
}

void camera_control_impl::move(vec2 newMouse)
{
    vec2 mouseMotion;
    if (panning == Camera_Target)
        mouseMotion = newMouse - mouse;
    else if (panning == Camera_Eye)
        mouseMotion =  mouse - newMouse;
    vec3 axis;
    axis.x = mouseMotion.x;
    axis.y = mouseMotion.y;
    axis.z = 0;

    move(axis, length(mouseMotion));
}

vec3 camera_control_impl::project_sphere(const vec2 xy)
{
    static const float sqrt2 = sqrtf(2.f);
    vec3 result;

    float d = length(xy);
    if (d < radiuss * sqrt2 / 2.f)
    {
        result.z = sqrtf(radiuss * radiuss - d*d);
    } else {
        float t = radiuss / sqrt2;
        result.z = t*t / d;
    }

    result.x = xy.x;
    result.y = xy.y;
    return normalize(result);
}
vec3 camera_control_impl::coord_transform(vec3 const axis)
{
    mat4 rotation;
    cameras->matrix_rotate().invert(rotation);
    return rotation * axis;
}
vec2 camera_control_impl::scale_mouse(const vec2& mouse)
{
    isize s = cameras->window_sizes;
    return vec2( (mouse.x*2.f) / (s.width ) - 1.f, 1.f - (mouse.y*2.f) / (s.height ) );
}


