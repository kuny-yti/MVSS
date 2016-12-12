#ifndef UNIFORM_BUFFER_IMPL_H
#define UNIFORM_BUFFER_IMPL_H
#include "render/iubo.h"
#include "opengl_buffer_object.h"
#include "render/iglsl.h"


class opengl_uniform_buffer :public iubo, public opengl_buffer_object
{
private:
    int id_bak;

private:
    void create_ubo (int size) ;
    int binding_point()const;
public:
    opengl_uniform_buffer();
    ~opengl_uniform_buffer();

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

    int uniform(const piprogram &pro, const bs_string &block_name);
    void bind(const piprogram &pro, int binding);
    void bind(const piprogram &pro);
    void wirte (void *data, const bs_string &member = bs_string());
};

#endif // UNIFORM_BUFFER_IMPL_H
