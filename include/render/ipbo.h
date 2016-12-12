#ifndef IPIXEL_BUFFER_H
#define IPIXEL_BUFFER_H

#include "maths/bs_vec4.h"
#include "itexture.h"
#include "ibobject.h"

bsPackage ipbo : ibobject
{
    virtual ~ipbo(){}
    virtual void create (const isize &size , eUsageModes usage) = 0;
    virtual void wirte (void *data, const isize &size = isize()) = 0;
    virtual void copy(const pitexture &texture, const ivec4 &off = ivec4()) = 0;

    virtual void bind_pack() = 0;
    virtual void bind_unpack() = 0;

    virtual void unbind_pack() = 0;
    virtual void unbind_unpack() = 0;
};
#define iPbo bs_ptr(ipbo)
typedef iPbo pipbo;

#endif // IPIXEL_BUFFER_H
