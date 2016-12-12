#ifndef X11_WINDOW_H
#define X11_WINDOW_H

#include "interface/iplatform.h"
#include "interface/iwcontext.h"
#include "interface/ircontext.h"

namespace impl {
struct window;
}
class x11_window : public iplatform::iwindow
{
    bsObject(x11_window)
public:
    x11_window(iwindow *parent = NULL);
    ~x11_window();

    virtual void fullscreen(bool s = true);
    /// 置顶窗口
    virtual void top(bool s = true) ;
    /// 关闭窗口
    /// 最小化
    virtual void minimum() ;
    /// 最大化
    virtual void maximun();
    /// 正常显示
    virtual void normal();

    /// 设置窗口是否为开启状态
    virtual void enable(bool s = true);
    virtual void disable(bool s = true);

    /// 设置窗口是否为隐藏状态
    virtual void hide(bool s = true) ;
    virtual void show(bool s = true);

    /// 窗口的尺寸
    virtual void size(const ivec2 &rect) ;
    virtual void resize(const ivec2 &rect) ;

    /// 窗口的位置
    virtual void pos(const ivec2 &p) ;
    virtual void move(const ivec2 &p) ;

    /// 窗口内的客户区域几何大小
    virtual void geometry(const ivec4 &rect);
    virtual void title(const bs_string &til) ;

    virtual bool create(ircontext *rctx, const tWindowFormats &wf) ;
    virtual void close();

    virtual void callback(tObjectNexus *param);
    virtual int dispatch();
    virtual void post(const tWindowNexus &evt);

    void *ext();
private:
    impl::window *impl;
};

#endif
