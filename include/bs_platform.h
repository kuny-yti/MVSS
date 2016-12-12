#ifndef BS_PLATFORM_H
#define BS_PLATFORM_H

#include "bs_utils.h"
#include "interface/iplatform.h"

class bs_platform :public bs_singleton<bs_platform>, public iplatform
{
public:
    bs_platform(const piwindow &pf = piwindow());
    ~bs_platform();

    void set(const piwindow &pf);
    piwindow get();
    piwindow window() {return get();}
    bs_platform &operator = (const piwindow &pf){set(pf);return *this;}
    piwindow operator ()() {return get();}

    virtual int dispatch() ;
    virtual void post(const tWindowNexus &evt) ;

    // monitor
    //!
    //! \brief change 更改监视器的设置信息
    //!
    virtual bool monitor_change(const tMonitorInofs &mi, bool is_fullscreen = false) ;
    virtual tMonitorInofs monitor(const int idx = 0) ;
    virtual int monitor_count()const ;

    // mouse
    virtual bool cursor_set(const bs_image &) ;
    virtual bs_image cursor_get() const;
    virtual void cursor_set(eCursorTypes ct) ;
    virtual void cursor_show(bool state = true) ;
    virtual void cursor_position(const ivec2 &s) ;
    virtual ivec2 cursor_position() const ;
    virtual void cursor_event(const bs_event &e) ;

public:
    //!
    //! \brief create 创建窗口
    //! \return
    //!
    static piwindow create();
};


#endif
