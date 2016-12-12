#ifndef IGUI_H
#define IGUI_H
#include "bs_define.h"
#include "bs_ptr.h"
#include "bs_event.h"
#include "maths/bs_vec4.h"
#include "iwidget.h"

struct uicontext
{

};
//!
//! @brief The igui class
//! @class igui
//!
struct igui :public bs_object
{
    bsObject(igui)
    igui(bs_object *parent = NULL): bs_object(parent){context = NULL;}
    virtual ~igui(){}

    virtual void event(const bs_event &e) =0;
    virtual void paint() = 0;

    void set(const ivec4 &vp){viewport = vp;}

    ivec4 viewport;
    uicontext *context;
};

#define G bs_ptr(igui)
typedef G pigui;
#undef G

#endif
