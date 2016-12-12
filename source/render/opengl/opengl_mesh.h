#ifndef OPENGL_MESH_H
#define OPENGL_MESH_H

#include "render/imesh.h"

class opengl_mesh :public imesh
{
    bs_ptr(bsPackage irender) ctx;
public:
    opengl_mesh(const class bs_render *rctx);
    ~opengl_mesh();

    void autoindices();

    virtual void draw (eDrawModes dm) ;
};

#endif // OPENGL_MESH_H
