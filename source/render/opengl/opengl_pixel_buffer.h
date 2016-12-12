#ifndef PIXEL_BUFFER_IMPL_H
#define PIXEL_BUFFER_IMPL_H

#include "render/ipbo.h"
#include "opengl_buffer_object.h"

class opengl_pixel_buffer :public opengl_buffer_object, public ipbo
{
    isize size;
public:
    opengl_pixel_buffer();

    bool create (size_t size = 0){return opengl_buffer_object::create(size);}
    void destroy(){opengl_buffer_object::destroy();}

    void bind(eTargetModes target= Target_None){opengl_buffer_object::bind(target);}
    void *map(eAccessModes access = Access_None, eTargetModes target = Target_None)
    {
        return opengl_buffer_object::map(access, target);
    }
    void *map(ulong offset, uint len, eAccessModes access = Access_None,
              eTargetModes target = Target_None)
    {
        return opengl_buffer_object::map(offset, len, access, target);
    }
    void write(void* data){ opengl_buffer_object::write(data);}
    void writesub(void* data, size_t size = 0, size_t offset = 0){opengl_buffer_object::writesub(data, size, offset);}

    void unbind(eTargetModes target){opengl_buffer_object::unbind(target);}
    void unmap(eTargetModes target){opengl_buffer_object::unmap(target);}

    virtual void create (const isize &size , eUsageModes usage) ;
    virtual void wirte (void *data, const isize &size = isize());
    virtual void copy(const pitexture &texture, const ivec4 &off = ivec4()) ;

    virtual void bind_pack() ;
    virtual void bind_unpack() ;

    virtual void unbind_pack() ;
    virtual void unbind_unpack() ;

};

#endif // PIXEL_BUFFER_IMPL_H
