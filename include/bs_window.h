
#ifndef BS_WINDOW_H
#define BS_WINDOW_H

#include "interface/iplatform.h"

namespace impl {
struct window;
}
class bs_window
{
public:
    bs_window (_in_ piwindow &iw);
    ~bs_window();

    static bs_window *instance();

    void fullscreen(bool s = true) ;
    /// 置顶窗口
    void top(bool s = true) ;
    /// 关闭窗口
    /// 最小化
    void minimum();
    /// 最大化
    void maximun();
    /// 正常显示
    void normal();

    /// 设置窗口是否为开启状态
    void enable(bool s = true) ;
    void disable(bool s = true) ;

    /// 设置窗口是否为隐藏状态
    void hide(bool s = true) ;
    void show(bool s = true) ;

    /// 窗口的尺寸
    void size(const vec2 &rect) ;
    void resize(const vec2 &rect);

    /// 窗口的位置
    void pos(const vec2 &p) ;
    void move(const vec2 &p) ;

    /// 窗口内的客户区域几何大小
    void geometry(const vec4 &rect) ;
    void title(const bs_string &til) ;

public:
    static piwindow create();

private:
    impl::window *impl;
};

#endif
