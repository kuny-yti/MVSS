#ifndef VERTEX_BUFFER_IMPL_H
#define VERTEX_BUFFER_IMPL_H

#include "render/ivao.h"
#include "render/ivbo.h"

class opengl_vertex_array : public ivao
{
private:
    piibo bind_ibo;
    pivbo bind_vbo;

public:
    virtual bool has_vao()const ;
    virtual bool has_vbo()const ;

    virtual void append(const pivbo &p, int /*attrib*/ = -1) ;

    virtual void remove(const pivbo &p, int /*attrib*/ = -1);


    virtual bool create ();
    virtual void destroy();

    virtual void bind(const piibo & = piibo());
    virtual void unbind();

    virtual void draw(eDrawModes drawMode, uint first = 0, uint total = 0);
    virtual void elements(eDrawModes drawMode, uint amt);
};

#endif // VERTEX_BUFFER_IMPL_H
