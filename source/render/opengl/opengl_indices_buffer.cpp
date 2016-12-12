#include "opengl_indices_buffer.h"

void opengl_indices_buffer::write (void *dat, size_t total, bool is_up)
{
    if (!opengl_buffer_object::size < (total * byte * sizeOf))
    {
        if (buffer_size < total)
        {
            create(total);
            buffer_size = total;
            if (is_up)
            {
                write(dat);
                return ;
            }
        }
        else
            opengl_buffer_object::size = total * byte * sizeOf;
    }
    if (is_up)
        writesub(dat);
    else
        write(dat);
}
bool opengl_indices_buffer::create (size_t size)
{
    opengl_buffer_object::target = (eTargetModes)Target;
    return opengl_buffer_object::create(size * byte * sizeOf);
}
void opengl_indices_buffer::destroy()
{
    opengl_buffer_object::destroy();
}

void opengl_indices_buffer::bind(eTargetModes target)
{
    opengl_buffer_object::bind(target);
}
void *opengl_indices_buffer::map(eAccessModes access , eTargetModes target )
{
    return opengl_buffer_object::map(access, target);
}
void *opengl_indices_buffer::map(ulong offset, uint len, eAccessModes access ,
          eTargetModes target )
{
    return opengl_buffer_object::map(offset, len, access, target);
}
void opengl_indices_buffer::write(void* data)
{
    opengl_buffer_object::write(data);
}
void opengl_indices_buffer::writesub(void* data, size_t size, size_t offset )
{
    opengl_buffer_object::writesub(data, size, offset);
}

void opengl_indices_buffer::unbind(eTargetModes target)
{
    opengl_buffer_object::unbind(target);
}
void opengl_indices_buffer::unmap(eTargetModes target)
{
    opengl_buffer_object::unmap(target);
}
