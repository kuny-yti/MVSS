#ifndef IINDEX_BUFFER_H
#define IINDEX_BUFFER_H
#include "ibobject.h"

bsPackage iibo : ibobject
{
    size_t buffer_size;
    enum {byte = 1, Target = Target_ElementArray, sizeOf = sizeof(tIndicess)};
    virtual void write (void *dat, size_t total, bool is_up = false) = 0;
    size_t count()const{return buffer_size;}
};
#define iVbo bs_ptr(iibo)
typedef iVbo piibo;
#undef iVbo
#endif // IINDEX_BUFFER_H
