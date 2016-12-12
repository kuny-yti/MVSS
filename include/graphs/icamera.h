#ifndef ICAMERA_H
#define ICAMERA_H

#include "maths/bs_vec3.h"
#include "maths/bs_quaternion.h"
#include "tools/bs_string.h"
#include "ifrustum.h"
#include "bs_ptr.h"

/*!
 * @brief The eProjectionModes enum 投影模式
 * @enum eProjectionModes
 */
typedef enum  {
    Mode_Orthographic, ///< 正交投影
    Mode_Perspective,  ///< 透视投影
    Mode_Frustum       ///< 视锥投影
}eProjectionModes;

/*! 默认相机窗口尺寸 */
const isize CAMERA_WINDOW_SIZE_DEFAULT = isize(1280, 720);
/*! 默认相机宽高比 */
const float CAMERA_RATION_DEFAULT   = 1280.f / 720.f;
/*! 默认相机视角的大小 */
const float CAMERA_FOVY_DEFAULT     = 45.f;
/*! 默认相机近距离 */
const float CAMERA_NEAR_DEFAULT     = 0.1f;
/*! 默认相机远距离 */
const float CAMERA_FAR_DEFAULT      = 1000.f;
/*! 默认相机位置 */
const fpoint3  CAMERA_POSITION_DEFAULT = fpoint3(0.f, .5f, 2.f);
/*! 默认相机焦点位置(目标位置) */
const vec3  CAMERA_FOCUS_DEFAULT    = vec3(0.f, 0.f, 0.f);
/*! 默认相机方向矢量 */
const vec3  CAMERA_UP_DEFAULT       = vec3(0.f, 1.f,  0.f);
/*! 默认相机投影模式 */
const eProjectionModes CAMERA_MODE_DEFAULT = Mode_Perspective;

#define iCamera bs_ptr(bsPackage icamera)
typedef iCamera  picamera;
#undef iCamera
/*!
 * @brief The icamera struct 相机接口
 * @class icamera
 */
bsPackage icamera
{
    bs_string        name;          ///< 相机名称
    eProjectionModes modes;         ///< 相机投影模式
    fpoint3          positions;     ///< 相机位置
    vec3             focuss;        ///< 相机焦点位置(目标位置)
    vec3             ups;           ///< 相机方向矢量
    pifrustum        frustums;      ///< 相机名称
    isize            window_sizes;  ///< 相机窗口尺寸
    float            window_ratios; ///< 相机宽高比

    virtual ~icamera() {}

    //!
    //! \brief clone 克隆一个相机
    //! \return
    //!
    virtual picamera clone() const = 0;

    //!
    //! \brief position 设置相机眼睛位置
    //! \param pos 位置
    //!
    virtual void position(const fpoint3& pos) {positions = pos;}
    //!
    //! \brief focus 设置相机焦点位置
    //! \param foc 位置
    //!
    virtual void focus(const vec3& foc)  {focuss = foc;}
    //!
    //! \brief up 设置相机方向矢量
    //! \param up 位置
    //!
    virtual void up(const vec3& up)  {ups = up;}

    //!
    //! \brief frustum 设置相机产生的视锥体
    //! \param frust
    //!
    virtual void frustum(const pifrustum &frust) {frustums = frust;}
    //!
    //! \brief look
    //! \return
    //!
    vec3 look()const { return normalize(focuss - positions); }
    //!
    //! \brief strafe
    //! \return
    //!
    vec3 strafe()const { return normalize(cross(look(), ups)); }

    //!
    //! \brief fovy 视锥的视角的大小
    //! \return
    //!
    float fovy() const { return frustums->fovy(); }
    //!
    //! \brief ratio 视锥的宽高比
    //! \return
    //!
    float ratio() const { return frustums->ratio(); }
    //!
    //! \brief near
    //! \return
    //!
    float near() const { return frustums->nears; }
    //!
    //! \brief far
    //! \return
    //!
    float far() const { return frustums->fars; }
    //!
    //! \brief left
    //! \return
    //!
    float left() const { return frustums->lefts; }
    //!
    //! \brief right
    //! \return
    //!
    float right() const { return frustums->rights; }
    //!
    //! \brief top
    //! \return
    //!
    float top() const { return frustums->tops; }
    //!
    //! \brief bottom
    //! \return
    //!
    float bottom() const { return frustums->bottoms; }

    //!
    //! \brief fovy 设置相机视角的大小
    //! \param fovy_
    //!
    virtual void fovy(float fovy_)  = 0;

    //!
    //! \brief ratio 设置相机宽高比
    //! \param ratio_
    //!
    virtual void ratio(float ratio_)  = 0;
    //!
    //! \brief near
    //! \param neardist
    //!
    virtual void near(float neardist)  = 0;
    //!
    //! \brief far
    //! \param fardist
    //!
    virtual void far(float fardist)  = 0;
    //!
    //! \brief left
    //! \param v
    //!
    virtual void left(float v)  = 0;
    //!
    //! \brief right
    //! \param v
    //!
    virtual void right(float v)  = 0;
    //!
    //! \brief top
    //! \param v
    //!
    virtual void top(float v)  = 0;
    //!
    //! \brief bottom
    //! \param v
    //!
    virtual void bottom(float v)  = 0;

    //!
    //! \brief focal_length 设置相机的焦距
    //! \param f
    //!
    void focal_length(float f) {focus(positions + f * look());}
    //!
    //! \brief focal_length 相机的焦距
    //! \return
    //!
    float focal_length() const{return distance(focuss, positions);}

    //!
    //! \brief quaternion 控制相机的四元数
    //! \return
    //!
    virtual quat quaternion() const  = 0;

    //!
    //! \brief reset 重置相机位置
    //! \param pos
    //! \param focus_
    //! \param up_
    //!
    void reset(const fpoint3& pos = CAMERA_POSITION_DEFAULT,
               const vec3& focus_ = CAMERA_FOCUS_DEFAULT,
               const vec3& up_ = CAMERA_UP_DEFAULT)
    {
        position(pos);
        focus(focus_);
        up(up_);
    }

    //!
    //! \brief mode 设置当前相机模式
    //! \param pm
    //!
    void mode(eProjectionModes pm)  {modes = pm;}

    //!
    //! \brief window_size 设置相机目标窗口宽高比和窗口大小
    //! \param s
    //!
    virtual void window_size(const isize &s)
    {
        if (s == window_sizes)
            return ;
        window_sizes = s;
        ratio ((float)s.width / (float)s.height );
    }

    //!
    //! \brief matrix_view 相机视图矩阵
    //! \return
    //!
    virtual mat4 matrix_view() const = 0;
    //!
    //! \brief matrix_view 设置相机视图矩阵
    //! \param mvMat
    //!
    virtual void matrix_view(const mat4& mvMat) = 0;
    //!
    //! \brief matrix_view_inverse 相机逆向视图矩阵
    //! \return
    //!
    virtual mat4 matrix_view_inverse() const = 0;

    //!
    //! \brief matrix_rotate 相机旋转矩阵
    //! \return
    //!
    virtual mat4 matrix_rotate() const = 0;
    //!
    //! \brief matrix_frustum 相机视锥矩阵
    //! \return
    //!
    virtual mat4 matrix_frustum() const = 0;
    //!
    //! \brief matrix_projection 相机投影矩阵
    //! \return
    //!
    virtual mat4 matrix_projection() const = 0;

    virtual bool operator==(const picamera &rhs)  = 0;
    virtual bool operator!=(const picamera &rhs)  = 0;
};

#endif // ICAMERA_H
