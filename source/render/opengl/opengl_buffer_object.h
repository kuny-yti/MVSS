#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H

#include "render/ibobject.h"

class opengl_buffer_object :public ibobject
{
public:
    bool create (size_t size = 0);
    void destroy();

    void bind(eTargetModes target= Target_None);
    void *map(eAccessModes access = Access_None, eTargetModes target = Target_None);
    void *map(ulong offset, uint len, eAccessModes access = Access_None,
              eTargetModes target = Target_None);
    void write(void* data);
    void writesub(void* data, size_t size = 0, size_t offset = 0);

    void unbind(eTargetModes target);
    void unmap(eTargetModes target);
};

#endif // BUFFER_OBJECT_H
