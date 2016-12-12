#include "opengl_vertex_buffer.h"
#include "opengl_mx.h"

namespace impl {
static GLenum returnGLType(eDataTypes type)
{
    switch((int)type)
    {
    case Type_Bool:
        return GL_BOOL;

    case Type_Byte:
        return GL_BYTE;

    case Type_UByte:
        return GL_UNSIGNED_BYTE;

    case Type_Short:
        return GL_SHORT;

    case Type_UShort:
        return GL_UNSIGNED_SHORT;

    case Type_Int:
        return GL_INT;

    case Type_UInt:
        return GL_UNSIGNED_INT;

    case Type_Float:
        return GL_FLOAT;

    case Type_Double:
        return GL_DOUBLE;
    }
    return GL_NONE;
}
static GLenum returnPrimitiveType(eDrawModes type)
{
    switch((int)type)
    {
    case Draw_Lines:
        return GL_LINES;

    case Draw_LineLoop:
        return GL_LINE_LOOP;

    case Draw_LineStrip:
        return GL_LINE_STRIP;

    case Draw_Triangles:
        return GL_TRIANGLES;

    case Draw_TriangleStrip:
        return GL_TRIANGLE_STRIP;

    case Draw_TriangleFan:
        return GL_TRIANGLE_FAN;
    }
    return GL_NONE;
}

}
size_t opengl_vertex_buffer::total()
{
    if (attrib == Attrib_Color)
        byte = byteColor;
    else if (attrib == Attrib_TextureCoord)
        byte = byteCoord;
    else
        byte = byteVertex;
    return byte * sizeOf;
}
void opengl_vertex_buffer::write (void *dat, size_t s, bool is_up )
{
    if (!opengl_buffer_object::size < (s * total()))
    {
        if (buffer_size < s)
        {
            create(s* total());
            buffer_size = s;
            if (is_up)
            {
                write(dat);
                return ;
            }
        }
        else
            opengl_buffer_object::size = s * total();
    }
    if (is_up)
        writesub(dat);
    else
        write(dat);
}
void opengl_vertex_buffer::draw (eDrawModes drawMode, uint first, uint total)
{
    if (opengl_buffer_object::size)
    {
        int s = 0;
        int t = this->count();
        if(first)
            s = first;

        if (total)
            t = total;

        glDrawArrays(impl::returnPrimitiveType(drawMode), s, t);
    }
}
void opengl_vertex_buffer::elements(eDrawModes drawMode, tIndicess *index, uint amt)
{
    glDrawElements(impl::returnPrimitiveType(drawMode), amt > 0 ? amt : this->count(),
                   impl::returnGLType(Type_UInt), index);
}
bool opengl_vertex_buffer::create (size_t size)
{
    opengl_buffer_object::target = (eTargetModes)Target;
    opengl_buffer_object::usage = Usage_StaticDraw;
    return opengl_buffer_object::create(size);
}
void opengl_vertex_buffer::destroy()
{
    opengl_buffer_object::destroy();
}

void opengl_vertex_buffer::bind(eTargetModes target)
{
    opengl_buffer_object::bind(target);
}
void *opengl_vertex_buffer::map(eAccessModes access , eTargetModes target )
{
    return opengl_buffer_object::map(access, target);
}
void *opengl_vertex_buffer::map(ulong offset, uint len, eAccessModes access,
          eTargetModes target)
{
    return opengl_buffer_object::map(offset, len, access, target);
}
void opengl_vertex_buffer::write(void* data)
{
    opengl_buffer_object::write(data);
}
void opengl_vertex_buffer::writesub(void* data, size_t size , size_t offset)
{
    opengl_buffer_object::writesub(data, size, offset);
}

void opengl_vertex_buffer::unbind(eTargetModes target)
{
    opengl_buffer_object::unbind(target);
}
void opengl_vertex_buffer::unmap(eTargetModes target)
{
    opengl_buffer_object::unmap(target);
}
