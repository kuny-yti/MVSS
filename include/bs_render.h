#ifndef BS_RENDER_H
#define BS_RENDER_H

#include "bs_utils.h"
#include "maths/bs_vec4.h"
#include "maths/bs_color.h"
#include "maths/bs_mat4X4.h"
#include "tools/bs_string.h"

#include "render/icapability.h"
#include "render/ipipeline.h"
#include "render/rdefine.h"
#include "interface/ircontext.h"
#include "render/itexture.h"
#include "render/iglsl.h"
#include "render/ibobject.h"
#include "render/ifbo.h"
#include "render/ipbo.h"
#include "render/ivbo.h"
#include "render/ivao.h"
#include "render/iibo.h"
#include "render/iubo.h"
#include "render/irender.h"
//#include "render/imesh.h"

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

namespace impl {
struct render;
}
class bs_render :public irender, public bs_singleton<bs_render>
{
public:
    bs_render();
    virtual ~bs_render();

    //!
    //! \brief create 创建渲染上下文
    //! \param wctx 窗口上下文
    //! \param rf 渲染格式指定
    //! \return true 创建成功
    //!
    virtual bool create(iwcontext *wctx, const tRenderFormats &rf) ;
    //!
    //! \brief close 销毁创建的上下文
    //!
    virtual void close() ;
    //!
    //! \brief initialize 上下文进行初始化
    //! \return true 初始化成功
    //!
    virtual bool initialize() ;

    //!是渲染器内部对象，其他地方只是引用。
    virtual picapability capability() const;
    //! 对象
    virtual pitexture texture() ;
    virtual pibobject bobject() ;
    virtual pifbo fbo() ;
    virtual pipbo pbo() ;
    virtual pivbo vbo() ;
    virtual pivao vao();
    virtual piibo ibo() ;
    virtual piubo ubo() ;

    virtual pishader shader();
    virtual piprogram program() ;
    //virtual pimesh mesh();

    impl::render *impl;
};

#endif
