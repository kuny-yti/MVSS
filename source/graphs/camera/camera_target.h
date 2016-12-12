#ifndef CAMERA_TRACKBALL_H
#define CAMERA_TRACKBALL_H

#include "camera_control_impl.h"

//! 根据目标位置调整相机 (轨迹球)
class camera_target :public camera_control_impl
{
private:

public:
    camera_target();

    //! 中心点到相机的距离
    float center_distance();
private:
    void zoom_absolute(float focallength);

protected:
    virtual void event(const bs_event&) ;
};

#endif // CAMERA_TRACKBALL_H
