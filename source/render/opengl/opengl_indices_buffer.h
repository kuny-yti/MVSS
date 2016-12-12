#ifndef OPENGL_INDEX_BUFFER_H
#define OPENGL_INDEX_BUFFER_H

#include "render/iibo.h"
#include "opengl_buffer_object.h"

class opengl_indices_buffer :public iibo, public opengl_buffer_object
{
public:
    void write (void *dat, size_t total, bool is_up = false);
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

#endif // OPENGL_INDEX_BUFFER_H
