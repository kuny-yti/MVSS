#ifndef BS_GRAPHS_H
#define BS_GRAPHS_H

#include "bs_utils.h"
#include "graphs/igraphs.h"

namespace impl {
class graphs_window;
}
class bs_graphs :public igraphs, public bs_singleton<bs_graphs>
{
public:
    bs_graphs();
    ~bs_graphs();

    virtual void initialize(const piwindow &window, const pirender &rbase);

public:
    virtual void append(const piscene &pat);
    virtual void append(const picamera&);
    virtual void append(const picamcontrol &);
    virtual picamcontrol append(eCameraControls ctrl = Camera_Target);

    virtual void remove(const piscene &pat);
    virtual void remove(const picamera&);
    virtual void remove(const picamcontrol &);

    virtual void set(const pigui &) ;
    virtual void set(const picamera &);
    virtual void set(const picamcontrol &);

    virtual picamera camera();
    virtual picamcontrol control();

    virtual bool pick(const ipoint &p, fpoint3 &_src);
private:
    impl::graphs_window *impl;
};

#endif
