#ifndef IVERTEX_BUFFER_H
#define IVERTEX_BUFFER_H

#include "maths/bs_color.h"
#include "ibobject.h"
#include "tools/bs_vector.h"
#include "maths/bs_matMxN.h"

//! 开启vbo后，和shader交互时需要设置各自固定属性位置 (eAttribIndexs)

bsPackage ivbo : public ibobject
{
    int byte;
    size_t buffer_size;
    eAttribIndexs attrib;// color,vertex,normal,coord

    enum {byteVertex = 3, byteColor = 4, byteCoord = 2,Target = Target_Array, sizeOf = sizeof(float)};
    virtual void write (void *dat, size_t total, bool is_up = false) = 0;
    size_t count()const {return buffer_size;}

    virtual void draw (eDrawModes drawMode, uint first = 0, uint total = 0) = 0;
    virtual void elements(eDrawModes drawMode, tIndicess *index, uint amt = 0) = 0;
};
#define iVbo bs_ptr(ivbo)
typedef iVbo pivbo;
#undef iVbo

#endif // IVERTEX_BUFFER_H
