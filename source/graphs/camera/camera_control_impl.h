#ifndef CAMERA_CONTROL_IMPL_H
#define CAMERA_CONTROL_IMPL_H

#include "graphs/icamcontrol.h"

class camera_control_impl :public icamcontrol
{
protected:
    vec2          mouse;

public:
    //! 复位相机数据
    virtual void reset() ;

    //! 相机当前视图模式
    virtual void view(eCameraViews dir) ;

    //! 相机旋转
    virtual void rotation(vec2 mouse);
    virtual void rotation(const quat &quat);
    virtual void rotation(const vec3 &axis, float angle);
    virtual void rotation(float Hangle, float Vangle);
    virtual void rotation(eRotationModes mode, float angle);

    virtual void roll(eRollModes mode, float angle);
    virtual void zoom(float factor);
    virtual void zoom(vec2 mouse) ;

    //! 相机移动
    virtual void move(vec2 mouse) ;
    virtual void move(eMoveModes mode, float length);
    virtual void move(const vec3 &axis, float length);
    virtual void move(const vec3 &motionvector);

    virtual void event(const bs_event&) {}

    virtual vec3 project_sphere(const vec2 xy);
    virtual vec3 coord_transform(vec3 const axis);
    virtual vec2 scale_mouse(const vec2& mouse);

};

#endif // CAMERA_CONTROL_IMPL_H
