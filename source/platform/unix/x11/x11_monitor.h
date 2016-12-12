#ifndef X11_MONITOR_H
#define X11_MONITOR_H

#include "ext/x11_extension.h"
#include "interface/iplatform.h"

namespace impl {
struct monitor;
}
class x11_monitor
{
public:
    x11_monitor(x11_extension *ext);
    ~x11_monitor();

    virtual void enums() ;
    virtual bool change(const tMonitorInofs &mi, bool is_fullscreen = false);

private:
    impl::monitor *impl;
};

#endif // X11_MONITOR_H
