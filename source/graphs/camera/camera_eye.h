#ifndef CAMERA_EYE_H
#define CAMERA_EYE_H

#include "camera_control_impl.h"

//! 根据眼睛位置调整相机
class camera_eye :public camera_control_impl
{
public:
    camera_eye();
protected:
    virtual void event(const bs_event&) ;
};

#endif // CAMERA_EYE_H
