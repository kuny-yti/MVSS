#ifndef IGRAPH_H
#define IGRAPH_H
#include "maths/bs_vec3.h"
#include "bs_event.h"
#include "tools/bs_string.h"

#include "iscene.h"
#include "icamera.h"
#include "icamcontrol.h"
#include "gui/igui.h"
#include "render/irender.h"
#include "interface/iplatform.h"

/*!
 * @brief The igraphs class 图形管理器
 * @class igraphs
 */
bsPackage igraphs
{
    virtual ~igraphs(){}

    //!
    //! \brief append 给图形管理器添加绘画对象、相机对象、相机控制器
    //! \param
    //!
    virtual void append(const piscene &pat) = 0;
    virtual void append(const picamera &cam) = 0;
    virtual void append(const picamcontrol &ctrl) = 0;
    virtual picamcontrol append(eCameraControls ctrl = Camera_Target) = 0;
    //!
    //! \brief append 给图形管理器删除绘画对象、相机对象、相机控制器
    //! \param
    //!
    virtual void remove(const piscene &pat) = 0;
    virtual void remove(const picamera &cam) = 0;
    virtual void remove(const picamcontrol &ctrl) = 0;

    //!
    //! \brief set 设置图形管理器的默认相机和控制器
    //! \param
    //!
    virtual void set(const pigui &) = 0;
    virtual void set(const picamera &) = 0;
    virtual void set(const picamcontrol &) = 0;

    virtual picamera camera() = 0;
    virtual picamcontrol control() =0;

    //!
    //! \brief initialize 初始化图形管理器
    //! \param window 平台窗口
    //! \param rbase 渲染器
    //!
    virtual void initialize(const piwindow &window, const pirender &rbase) = 0;

    virtual bool pick(const ipoint &p, fpoint3 &_src)= 0;

};
#define iGraphs bs_ptr(igraphs)
typedef iGraphs  pigraphs;
#undef iGraphs

#endif // IGRAPHIC_H
