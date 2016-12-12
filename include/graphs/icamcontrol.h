#ifndef ICAMERA_CONTROL_H
#define ICAMERA_CONTROL_H

#include "icamera.h"
#include "maths/bs_vec3.h"
#include "bs_event.h"
#include "bs_ptr.h"

//! 控制相机的视图方向
typedef enum
{
    View_Left,
    View_Right,
    View_Top,
    View_Bottom,
    View_Front,
    View_Rear,
    View_Not
}eCameraViews;

//! 控制相机的滚动方向
typedef enum
{
    Roll_Horizontal,
    Roll_Vertical
}eRollModes;

//! 控制相机的旋转方向
typedef enum
{
    Rotation_Horizontal,
    Rotation_Vertical
}eRotationModes;

//! 控制相机的移动方向
typedef enum
{
    Move_Forward,
    Move_Backward,
    Move_Left,
    Move_Right,
    Move_Up,
    Move_Down
}eMoveModes;

//! 控制相机的状态
typedef enum
{
    Camera_Not,
    Camera_Eye,//! 眼睛位置控制状态(//!相机眼睛位置)
    Camera_Target, //!Camera_TrackBall,//! 轨迹球控制状态(//!相机目标位置)
    Camera_Fly,//! 飞行控制状态
    Camera_Pick//! 拾取状态
}eCameraControls;


bsPackage icamcontrol
{
    //! 当前控制器的名称
    bs_string name;
    picamera  cameras;
    eCameraControls panning;
    vec3          focus;
    float         radiuss;
    eCameraViews  views;

    icamcontrol():
        name("CameraControl"),
        cameras(),
        panning(Camera_Not),
        focus(0,0,0),
        radiuss(1.),
        views(View_Not){}
    virtual ~icamcontrol() {}
    //! 当前使用的相机对象
    void camera(const picamera &cam) {cameras = cam;}
    //! 控制器移动相机使用的中心点
    void target(const vec3& cen) {focus = cen;}
    const vec3& target() const {return focus;}

    //! 复位相机数据为默认状态
    virtual void reset() = 0;

    //! 相机当前视图模式
    virtual void view(eCameraViews dir) {views = dir;}

    //! 轨迹球半径
    virtual void radius(float r) {radiuss = r;}

    //! 相机旋转
    virtual void rotation(vec2 mouse) = 0;
    virtual void rotation(const quat &quat) = 0;
    virtual void rotation(const vec3 &vector, float angle) = 0;
    virtual void rotation(float Hangle, float Vangle) = 0;
    virtual void rotation(eRotationModes mode, float angle) = 0;

    //! 相机滚动
    virtual void roll(eRollModes mode, float angle) = 0;

    //! 相机缩放
    virtual void zoom(float factor) = 0;
    virtual void zoom(vec2 mouse) = 0;

    //! 相机移动
    virtual void move(vec2 mouse) = 0;
    virtual void move(eMoveModes mode, float length) = 0;
    virtual void move(const vec3 &axis, float length) = 0;
    virtual void move(const vec3 &motionvector) = 0;

    //! 控制器的事件
    virtual void event(const bs_event&) = 0;

    //! 将坐标转换到球面
    virtual vec3 project_sphere(const vec2 xy)= 0;
    //! 坐标转换
    virtual vec3 coord_transform(vec3 const axis)= 0;

    //! 坐标进行缩放
    virtual vec2 scale_mouse(const vec2& mouse)= 0;

    float  zoom_factor(const float& acuteness, const bool& zoomIn)
    {
        if (zoomIn) {
            return 1.f + 1.f/acuteness;
        } else {
            return 1.f - 1.f/acuteness;
        }
    }
    float  roll_angle(const float& acuteness, const bool& left)
    {
        if (left) {
            return - 1.f / acuteness;
        } else {
            return 1.f / acuteness;
        }
    }
    float  rotation_angle(const float& acuteness)
    {
        return 1.f / acuteness;
    }
    float  move_length(const float& acuteness, float size)  {
        return size / acuteness;
    }
};
#define iCameraCtrl bs_ptr(icamcontrol)
typedef iCameraCtrl  picamcontrol;

#endif // ICAMERA_CONTROL_H
