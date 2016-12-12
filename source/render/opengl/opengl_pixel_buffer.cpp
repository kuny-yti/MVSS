#include "opengl_pixel_buffer.h"
#include "opengl_buffer_object.h"
#include "opengl_mx.h"

static int next_multiple_of_4(int x)
{
    return (x / 4 + (x % 4 == 0 ? 0 : 1)) * 4;
}
static GLuint returnDataFormat(eDataFormats format)
{
    switch ((int)format)
    {
    case Data_RGB :
        return GL_RGB;
    case Data_RGBA :
        return GL_RGBA;
    case Data_Luminance:
        return GL_LUMINANCE;
    case Data_LuminanceAlpha:
        return GL_LUMINANCE_ALPHA;
    case Data_BGR:
        return GL_BGR;
    case Data_BGRA:
        return GL_BGRA;
    default:
        break;
    }
    return GL_RGBA;
}
static GLuint returnDataDepth(eDataDepths depth )
{
    switch ((int)depth)
    {
    case Data_Byte :
        return GL_BYTE;
    case Data_UByte:
        return GL_UNSIGNED_BYTE;
    case Data_Short:
        return GL_SHORT;
    case Data_UShort:
        return GL_UNSIGNED_SHORT;
    case Data_Int:
        return GL_INT;
    case Data_UInt:
        return GL_UNSIGNED_INT;
    case Data_Float:
        return GL_FLOAT;
    case Data_2Bytes:
        return GL_2_BYTES;
    case Data_3Bytes :
        return GL_3_BYTES;
    case Data_4Bytes:
        return GL_4_BYTES;
    case Data_Double:
        return GL_DOUBLE;

    default:
        break;
    }
    return GL_UNSIGNED_BYTE;
}

opengl_pixel_buffer::opengl_pixel_buffer():
    opengl_buffer_object()
{

}


void opengl_pixel_buffer::create (const isize &_size , eUsageModes usage)
{
    if (size != _size) size = _size;

    uint row_size = next_multiple_of_4(size.width * 4);

    opengl_buffer_object::usage = usage;
    opengl_buffer_object::create(row_size *size.height);
}
void opengl_pixel_buffer::wirte (void *data, const isize &_size)
{
    if ((_size.width > 0) && (_size.height > 0))
    {
        if (_size != size)
        {
            eUsageModes us = opengl_buffer_object::usage;
            create(_size, us);
        }
        uint row_size = next_multiple_of_4(size.width * 4);

        void *buf = opengl_buffer_object::map(Access_WriteOnly, Target_PixelPack);
        bs_memory::copy_sse(buf, data, row_size* size.height);
        opengl_buffer_object::unmap(Target_PixelPack);
    }
}
void opengl_pixel_buffer::copy(const pitexture &texture, const ivec4 &off)
{
    int w = 0;
    int h = 0;
    if (size != texture->size) return;

    if(off.width <= 0)
        w = texture->size.width;
    if(off.height <= 0)
        h = texture->size.height;

    bind_unpack();
    glPixelStorei(GL_UNPACK_ROW_LENGTH, texture->size.width);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    texture->bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, off.x, off.y, w, h, returnDataFormat(texture->format),
                    returnDataDepth(texture->depth), 0);
    texture->unbind();
    unbind_unpack();
}

void opengl_pixel_buffer::bind_pack()
{
    opengl_buffer_object::bind(Target_PixelPack);
}
void opengl_pixel_buffer::bind_unpack()
{
    opengl_buffer_object::bind(Target_PixelUnPack);
}

void opengl_pixel_buffer::unbind_pack()
{
    opengl_buffer_object::unbind(Target_PixelPack);
}
void opengl_pixel_buffer::unbind_unpack()
{
    opengl_buffer_object::unbind(Target_PixelUnPack);
}

