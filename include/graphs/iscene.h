#ifndef IPAINT_H
#define IPAINT_H

#include "maths/bs_vec4.h"
#include "tools/bs_string.h"
#include "maths/bs_color.h"
#include "bs_event.h"
#include "bs_object.h"
#include "bs_ptr.h"

bsPackage isceneobject
{

};

//!
//! @brief The iscene class 场景接口
//! @class iscene
//!
bsPackage iscene :public bs_object
{
    bsObject(iscene)
    iscene(bs_object *parent = NULL): bs_object(parent){}
    virtual ~iscene(){}

    virtual void event(const bs_event &e) =0;

    virtual void initialize() = 0;
    virtual void paint(const isize &sz) = 0;

};
#define iPaint bs_ptr(iscene)
typedef iPaint  piscene;
#undef iPaint

#endif // IPAINT_H
