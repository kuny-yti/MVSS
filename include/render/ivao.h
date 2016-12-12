#ifndef IVERTEX_ARRAY_H
#define IVERTEX_ARRAY_H
#include "iibo.h"
#include "ivbo.h"

bsPackage ivao
{
    uint id;
    bs_list <pivbo> vbo;
    bs_list <piibo> ibo;

    virtual bool create () {id = 0;return false;}
    virtual void destroy() {id = 0;}
    virtual bool has_vao()const {return false;}
    virtual bool has_vbo()const {return false;}

    virtual void append(const pivbo &p, int /*attrib*/ = -1) {if (!vbo.contains(p))vbo.append(p);}
    virtual void append(const piibo &p, int /*attrib*/ = -1){if (!ibo.contains(p))ibo.append(p);}

    virtual void remove(const pivbo &p, int /*attrib*/ = -1){int inx = vbo.find(p); if (inx != -1) vbo.remove(inx);}
    virtual void remove(const piibo &p, int /*attrib*/ = -1){int inx = ibo.find(p); if (inx != -1) ibo.remove(inx);}

    virtual void bind(const piibo &bo = piibo()) = 0;
    virtual void unbind() = 0;

    virtual void draw(eDrawModes drawMode, uint first = 0, uint total = 0)= 0;
    virtual void elements(eDrawModes drawMode, uint amt = 0)= 0;
};
#define iVao bs_ptr(ivao)
typedef iVao pivao;
#undef iVao
#endif // IVERTEX_ARRAY_H
