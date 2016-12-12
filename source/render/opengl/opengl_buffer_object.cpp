#include "opengl_buffer_object.h"
#include "opengl_mx.h"

static GLenum returnTarget (eTargetModes mode)
{
    switch ((int)mode)
    {
    case Target_Array:
        return GL_ARRAY_BUFFER;
    case Target_ElementArray:
        return GL_ELEMENT_ARRAY_BUFFER;
    case Target_PixelUnPack:
        return GL_PIXEL_UNPACK_BUFFER;
    case Target_PixelPack:
        return GL_PIXEL_PACK_BUFFER;
    case Target_Uniform:
            return GL_UNIFORM_BUFFER;
    }
    return GL_PIXEL_PACK_BUFFER;
}
static GLenum returnUsage(eUsageModes mode)
{
    switch ((int)mode)
    {
    case Usage_StaticDraw:
        return GL_STATIC_DRAW;
    case Usage_StaticRead:
        return GL_STATIC_READ;
    case Usage_StaticCopy:
        return GL_STATIC_COPY;
    case Usage_DynamicDraw:
        return GL_DYNAMIC_DRAW;
    case Usage_DynamicRead:
        return GL_DYNAMIC_READ;
    case Usage_DynamicCopy:
        return GL_DYNAMIC_COPY;
    case Usage_StreamDraw:
        return GL_STREAM_DRAW;
    case Usage_StreamRead:
        return GL_STREAM_READ;
    case Usage_StreamCopy:
        return GL_STREAM_COPY;
    }
    return GL_DYNAMIC_DRAW;
}
static GLenum returnAccess(eAccessModes mode)
{
    switch ((int)mode)
    {
    case Access_ReadOnly:
        return GL_READ_ONLY;
    case Access_WriteOnly:
        return GL_WRITE_ONLY;
    case Access_ReadWrite:
        return GL_READ_WRITE;
    }
    return GL_READ_WRITE;
}

bool opengl_buffer_object::create(size_t s)
{
    glGenBuffers(1, &id);
    glBindBuffer(returnTarget(target), id);

    glBufferData(returnTarget(target), s, NULL, returnUsage(usage));
    return ibobject::create(s);
}
void opengl_buffer_object::destroy()
{
    glDeleteBuffers(1, &id);
    ibobject::destroy();
}
void opengl_buffer_object::bind(eTargetModes targ)
{
    if(targ == Target_None)
        targ = target;

    glBindBuffer(returnTarget( targ), id);
    ibobject::bind(targ);
}
void *opengl_buffer_object::map(eAccessModes acce, eTargetModes targ)
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

    unmap(targ);
    bind(targ);
    ibobject::map(acce, targ);

    return glMapBuffer(returnTarget( targ), returnAccess( acce));
}
void *opengl_buffer_object::map(ulong offset, uint len, eAccessModes acce, eTargetModes targ)
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

    unmap(targ);
    bind(targ);
    ibobject::map(offset, len, acce, targ);

    return glMapBufferRange(returnTarget( targ), offset, len, returnAccess( acce));
}
void opengl_buffer_object::write(void* data)
{
    bind();
    glBufferData(returnTarget(target), size, data, returnUsage(usage));
}
void opengl_buffer_object::writesub(void* data, size_t s, size_t offset)
{
    bind();
    if (s)
        glBufferSubData(returnTarget(target),  offset, s, data);
    else
        glBufferSubData(returnTarget(target),  offset, size, data);
}
void opengl_buffer_object::unbind(eTargetModes target)
{
    glBindBuffer(returnTarget (target), 0);
    ibobject::unbind(target);
}
void opengl_buffer_object::unmap(eTargetModes target)
{
    if(is_mapped(target))
    {
        glUnmapBuffer(returnTarget(target));
        ibobject::unbind(target);
    }
    if (is_bound(target))
        unbind(target);
}

