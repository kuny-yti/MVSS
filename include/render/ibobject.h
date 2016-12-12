#ifndef IBUFFER_OBJECT_H
#define IBUFFER_OBJECT_H

#include "rdefine.h"
#include "bs_ptr.h"
#include "maths/bs_vec3.h"

bsPackage ibobject
{
    eTargetModes target;
    eAccessModes access;
    eUsageModes usage;
    uint id;
    size_t size;
    bool binding[Target_Count];
    bool mapping[Target_Count];

    ibobject():
        target(Target_PixelPack),
        access(Access_ReadWrite),
        usage(Usage_StreamDraw),
        id(0),
        size(0)
    {
        bs_memory::set_c(binding, 0, sizeof(bool)*5);
        bs_memory::set_c(mapping, 0, sizeof(bool)*5);
    }
    virtual ~ibobject(){destroy();}

    virtual bool create (size_t s = 0) {size = s;return true;}
    virtual void destroy()
    {
        target = (Target_PixelPack);
        access = (Access_ReadWrite);
        usage = (Usage_StreamDraw);
        id = (0);
        size = (0);
    }

    virtual void bind(eTargetModes t = Target_None)
    {
        if(t == Target_None)
            t = target;
        binding[t] = true;
    }
    virtual void *map(eAccessModes acce = Access_None, eTargetModes targ = Target_None)
    {
        if(targ == Target_None)
            targ = target;

        if(acce == Access_None)
        {
            if(targ == Target_PixelUnPack)
                acce = Access_WriteOnly;
            else if(targ ==Target_PixelPack)
                acce = Access_ReadOnly;
        }
        mapping[targ] = true;
        return NULL;
    }
    virtual void *map(ulong offset, uint len, eAccessModes acce = Access_None,
                      eTargetModes targ = Target_None)
    {
        if(targ == Target_None)
            targ = target;

        if(acce == Access_None)
        {
            if(targ == Target_PixelUnPack)
                acce = Access_WriteOnly;
            else if(targ ==Target_PixelPack)
                acce = Access_ReadOnly;
        }
        mapping[targ] = true;
        offset = offset;
        len = len;
        return NULL;
    }
    virtual void write(void* data) = 0;
    virtual void writesub(void* data, size_t size = 0, size_t offset = 0) = 0;

    virtual void unbind(eTargetModes t= Target_None)
    {
        if(t == Target_None)
            t = target;
        binding[t] = false;
    }
    virtual void unmap(eTargetModes targ= Target_None)
    {
        if(targ == Target_None)
            targ = target;
        mapping[targ] = false;
    }
    virtual bool is_bound(eTargetModes target= Target_None)
    {
        if (target > Target_None && target < Target_Count)
            return binding[target];
        else
            return false;
    }
    virtual bool is_mapped(eTargetModes target= Target_None)
    {
        if (target > Target_None && target < Target_Count)
            return mapping[target];
        else
            return false;
    }

};
#define iBo bs_ptr(ibobject)
typedef iBo pibobject;
#undef iBo

#endif // IBUFFER_OBJECT_H
