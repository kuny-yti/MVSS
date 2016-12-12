#ifndef CAMERA_H
#define CAMERA_H
#include "graphs/icamera.h"

class camera_impl : public icamera
{
public:
    camera_impl(const fpoint3& position =  CAMERA_POSITION_DEFAULT,
                const vec3& focus       =  CAMERA_FOCUS_DEFAULT,
                const vec3& up          =  CAMERA_UP_DEFAULT,
                float fovy              =  CAMERA_FOVY_DEFAULT,
                float ratio             =  CAMERA_RATION_DEFAULT,
                float distn             =  CAMERA_NEAR_DEFAULT,
                float distf             =  CAMERA_FAR_DEFAULT,
                eProjectionModes pm     =  CAMERA_MODE_DEFAULT,
                isize size            =  CAMERA_WINDOW_SIZE_DEFAULT);

    picamera clone() const;

    void position(const fpoint3& pos)
    {
        icamera::position(pos);
        invalidateVM();
    }
    void focus(const vec3& foc) {
        icamera::focus(foc);
        invalidateVM();
    }
    void up(const vec3& up_) {
        icamera::up(normalize(up_));
        invalidateVM();
    }

    void fovy(float fovy_) {
        if(modes == Mode_Perspective) {
            float oldRatio = frustums->ratio();
            frustums->fovy(fovy_);
            frustums->ratio(oldRatio);
        }
    }
    void ratio(float ratio_) {
        if(modes == Mode_Perspective)
            frustums->ratio(ratio_);
    }
    void near(float neardist) {
        if(modes == Mode_Perspective) {
            float oldFovy = frustums->fovy();
            float oldRatio = frustums->ratio();
            frustums->near(neardist);
            frustums->fovy(oldFovy);
            frustums->ratio(oldRatio);
        } else
            frustums->near(neardist);
    }
    void far(float fardist) {
        frustums->far(fardist);
    }
    void left(float v) {
        frustums->left(v);
        if(modes == Mode_Perspective)
            frustums->right(-v);
    }
    void right(float v) {
        frustums->right(v);
        if(modes == Mode_Perspective)
            frustums->left(-v);
    }
    void top(float v) {
        frustums->top(v);
        if(modes == Mode_Perspective)
            frustums->bottom(-v);
    }
    void bottom(float v) {
        frustums->bottom(v);
        if(modes == Mode_Perspective)
            frustums->top(-v);
    }

    quat quaternion() const {
        updateVM();
        return generateQuatFromMatrix(viewMatrix_.getRotationalPart());
    }

    void mode(eProjectionModes pm) {
        icamera::mode(pm);
        if(pm == Mode_Perspective) {
            right(-icamera::left());
            top(-icamera::bottom());
        }
    }

    //! void look();
    mat4 matrix_view() const;
    void matrix_view(const mat4& mvMat);

    mat4 matrix_view_inverse() const;
    mat4 matrix_rotate() const;

    virtual mat4 matrix_frustum() const;
    virtual mat4 matrix_projection() const;

    bool operator == (const picamera& rhs) ;
    bool operator != (const picamera& rhs) ;

protected:
    void updateFR() const
    {
        frustums->update (_this_);
    }
    void updateVM() const {
        if (!viewMatrixValid_)
            viewMatrix_ = mat4::createLookAt(positions, focuss, ups);
        viewMatrixValid_ = true;
    }

    void invalidateVM() const {
        viewMatrixValid_ = false;
    }

    /// This is the actual matrix that holds the current orientation and position of the Camera.
    mutable mat4 viewMatrix_;
    mutable bool viewMatrixValid_; /// if the model-view matrix is up-to-date

    picamera _this_;
};

#endif // CAMERA_H
