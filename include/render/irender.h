#ifndef READER_BASE_H
#define READER_BASE_H

#include "maths/bs_vec4.h"
#include "maths/bs_color.h"
#include "maths/bs_mat4X4.h"
#include "maths/bs_mat3x3.h"
#include "maths/bs_mat2x2.h"
#include "tools/bs_string.h"

#include "icapability.h"
#include "ipipeline.h"
#include "rdefine.h"
#include "interface/ircontext.h"
#include "itexture.h"
#include "ifbo.h"
#include "ibobject.h"
#include "ipbo.h"
#include "iglsl.h"
#include "ivbo.h"
#include "ivao.h"
#include "iibo.h"
#include "iubo.h"
//#include "imesh.h"

//! 1.矩阵操作
//! 2.视口操作
//! 3.透明度
//! 4.深度
//! 5.混合
//! 6.遮罩
//! 7.模板
//! 8.雾
//! 9.多边形剔除
//! 10.对象绘制操作
//! 11.纹理对象
//! 12.帧缓存对象
//! 13.像素缓存对象
//! 14.着色器对象
//! 15.顶点缓存对象
//! 16.统一变量缓存对象
//! 17.灯光对象
//! 18.材质对象
//! 19.几何网格对象
//! 20.2D画板绘制


bsPackage irender
{
    pircontext context;
    pipipeline pipeline;
    bool is_initialize;

    virtual ~irender(){}
    //!
    //! \brief create 创建渲染上下文
    //! \param wctx 窗口上下文
    //! \param rf 渲染格式指定
    //! \return true 创建成功
    //!
    virtual bool create(iwcontext *wctx, const tRenderFormats &rf) = 0;
    //!
    //! \brief close 销毁创建的上下文
    //!
    virtual void close() =0;
    //!
    //! \brief initialize 上下文进行初始化
    //! \return true 初始化成功
    //!
    virtual bool initialize() {return is_initialize;}

    //! 当前系统支持的硬件功能
    bs_string version()const{return capability()->version;}
    bs_string vendor()const{return capability()->vendor;}
    bs_string renderer()const{return capability()->renderer;}
    bs_string version_glsl()const{return capability()->version_glsl;}
    bool is_support(const bs_string &ext) {return capability()->is_support(ext);}
    bool is_support(eCapabilityExtensions ext) {return capability()->is_support(ext);}

    //!是渲染器内部对象，其他地方只是引用。
    virtual picapability capability() const= 0;
    //! 对象
    virtual pitexture texture() = 0;
    virtual pifbo fbo() = 0;
    virtual pibobject bobject() = 0;
    virtual pipbo pbo() = 0;

    virtual pishader shader() = 0;
    virtual piprogram program() = 0;
    virtual pivbo vbo() = 0;
    virtual pivao vao() = 0;
    virtual piibo ibo() = 0;

    //virtual pimesh mesh() = 0;
    virtual piubo ubo() =0;

};
#define iRender bs_ptr(irender)
typedef iRender  pirender;

#endif // RENDER_CONTEXT_H
