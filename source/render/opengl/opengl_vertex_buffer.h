#ifndef OPENGL_VERTEX_BUFFER_H
#define OPENGL_VERTEX_BUFFER_H

#include "render/ivbo.h"
#include "opengl_buffer_object.h"

class opengl_vertex_buffer :public ivbo, public opengl_buffer_object
{
    size_t total();
public:
    void write (void *dat, size_t total, bool is_up = false);
    virtual void draw (eDrawModes drawMode, uint first = 0, uint total = 0) ;
    virtual void elements(eDrawModes drawMode, tIndicess *index, uint amt = 0) ;

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

#endif // OPENGL_VERTEX_BUFFER_H
