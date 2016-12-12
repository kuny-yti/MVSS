#include "opengl_vertex_array.h"
#include "opengl_buffer_object.h"
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

bool opengl_vertex_array::create ()
{
    if (has_vao())
    {
        if (!id)
            glGenVertexArrays(1, (uint*)&id);
    }
    return id;
}
void opengl_vertex_array::destroy()
{
    if (id)
        glDeleteVertexArrays(1, (uint*)&id);
}
bool opengl_vertex_array::has_vao()const
{
    return GLEW_ARB_vertex_array_object;
}
bool opengl_vertex_array::has_vbo()const
{
    return GLEW_ARB_vertex_buffer_object;
}

void opengl_vertex_array::append(const pivbo &p, int attrib)
{
    ivao::append(p, attrib);

    if (p->attrib == Attrib_Position)
        bind_vbo = p;
    bind();
    glEnableVertexAttribArray(attrib < 0 ? p->attrib: attrib);
    p->bind();
    glVertexAttribPointer(attrib < 0 ? p->attrib: attrib, p->byte, GL_FLOAT, GL_FALSE, 0, 0);
}

void opengl_vertex_array::remove(const pivbo &p, int attrib)
{
    ivao::remove(p, attrib);
    glDisableVertexAttribArray(attrib < 0 ? p->attrib: attrib);
    p->unbind();
}

void opengl_vertex_array::bind(const piibo &ibo)
{
    if (has_vao() && bind_vbo.get())
    {
        if (!id)
            create();
        glBindVertexArray(id);
        if (ibo.get() != NULL)
        {
            bind_ibo = ibo;
            bind_ibo->bind();
        }
    }
}
void opengl_vertex_array::unbind()
{
    if (bind_ibo.get())
        bind_ibo->unbind();
    if (has_vao())
        glBindVertexArray(0);
}

void opengl_vertex_array::draw(eDrawModes drawMode, uint first, uint total)
{
    if(bind_vbo.get())
    {
        int s = 0;
        int t = bind_vbo->count();
        if(first)
            s = first;

        if (total)
            t = total;

        glDrawArrays(impl::returnPrimitiveType(drawMode), s, t);
    }
}
void opengl_vertex_array::elements(eDrawModes drawMode, uint amt)
{
    if (bind_ibo.get())
    glDrawElements(impl::returnPrimitiveType(drawMode), amt > 0 ? amt : bind_ibo->count(),
                   impl::returnGLType(Type_UInt), NULL);
}

