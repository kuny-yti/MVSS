#ifndef IFRUSTUM_H
#define IFRUSTUM_H

#include "maths/bs_vec3.h"
#include "maths/bs_aabb.h"
#include "bs_ptr.h"


#define iFrustum bs_ptr(bsPackage ifrustum)
typedef iFrustum  pifrustum;
#undef iFrustum
/*!
 * @brief The icamera struct 相机的视锥
 * @class icamera
 */
bsPackage ifrustum
{
    float lefts;
    float rights;
    float bottoms;
    float tops;
    float nears;
    float fars;

    virtual ~ifrustum() {}
    //!
    //! \brief near
    //! \param nearDist
    //!
    void near(float nearDist) {nears = nearDist;}
    //!
    //! \brief far
    //! \param farDist
    //!
    void far(float farDist)  {fars = farDist;}
    //!
    //! \brief left
    //! \param v
    //!
    void left(float v)  {lefts = v;}
    //!
    //! \brief right
    //! \param v
    //!
    void right(float v)  {rights = v;}
    //!
    //! \brief top
    //! \param v
    //!
    void top(float v)  {tops = v;}
    //!
    //! \brief bottom
    //! \param v
    //!
    void bottom(float v)  {bottoms = v;}

    //!
    //! \brief is_culled_xz 是否能被裁剪
    //! \param bound
    //! \return
    //!
    virtual bool is_culled_xz(const faabb& bound) const = 0;
    //!
    //! \brief is_culled 是否能被裁剪
    //! \param b
    //! \return
    //!
    virtual bool is_culled(const faabb& b) const = 0;
    //!
    //! \brief is_culled 是否能被裁剪
    //! \param v
    //! \return
    //!
    virtual bool is_culled(const vec3& v) const = 0;
    //!
    //! \brief update 使用相机进行视锥更新
    //! \param c
    //!
    virtual void update(const bs_ptr(bsPackage icamera) &c) = 0;
    //!
    //! \brief fovy
    //! \return
    //!
    virtual float fovy() const = 0;
    //!
    //! \brief ratio
    //! \return
    //!
    virtual float ratio() const = 0;
    //!
    //! \brief fovy
    //! \param fovy
    //!
    virtual void fovy(float fovy) = 0;
    //!
    //! \brief ratio
    //! \param ratio
    //!
    virtual void ratio(float ratio) = 0;

    //!
    //! \brief leftn
    //! \return
    //!
    virtual const vec3& leftn() const = 0;
    //!
    //! \brief rightn
    //! \return
    //!
    virtual const vec3& rightn() const = 0;
    //!
    //! \brief bottomn
    //! \return
    //!
    virtual const vec3& bottomn() const = 0;
    //!
    //! \brief topn
    //! \return
    //!
    virtual const vec3& topn() const = 0;
    //!
    //! \brief nearn
    //! \return
    //!
    virtual const vec3& nearn() const = 0;
    //!
    //! \brief farn
    //! \return
    //!
    virtual const vec3& farn() const = 0;
    //!
    //! \brief nearp
    //! \return
    //!
    virtual const vec3& nearp() const = 0;
    //!
    //! \brief farp
    //! \return
    //!
    virtual const vec3& farp() const = 0;
    //!
    //! \brief campos
    //! \return
    //!
    virtual const vec3& campos() const = 0;
    //!
    //! \brief normal
    //! \param num
    //! \return
    //!
    virtual const vec3& normal(int num) const = 0;
    virtual bool operator == (const pifrustum &rhs)  = 0;
    virtual void operator = (const pifrustum &rhs)  = 0;

    //!
    //! \brief vertex
    //! \param num
    //! \return
    //!
    virtual const vec3& vertex(int num)const =0;
};


#endif // IFRUSTUM_H
