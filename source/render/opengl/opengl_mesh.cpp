#include "opengl_mesh.h"
#include "irender.h"
#include "opengl_mx.h"

opengl_mesh::opengl_mesh(const bs_ptr(irender) &rctx)
{
    ctx = rctx;
}
opengl_mesh::~opengl_mesh()
{

}
void opengl_mesh::autoindices()
{
    indices.resize(count_vertex());

    for(int i = 0; i < count_vertex();i++)
        indices[i]=(tIndicess)i;
    update();
}

void opengl_mesh::draw (eDrawModes dm)
{
    imesh::draw(dm);

    if (!has_vertex())
        return ;
    if (vao.get() == NULL)
        vao = ctx->vertex_array();
    if (vbo.get() == NULL)
    {
        vbo = ctx->vertex_buffer();
        vbo->attrib = Attrib_Position;
    }
    if (has_coord() && cbo.get() == NULL)
    {
        cbo = ctx->vertex_buffer();
        cbo->attrib = Attrib_TextureCoord;
    }
    if (has_normal() && nbo.get() == NULL)
    {
        nbo = ctx->vertex_buffer();
        nbo->attrib = Attrib_Normal;
    }
    if (has_index() && ibo.get() == NULL)
        ibo = ctx->indices_buffer();

    if (is_update())
    {
        vbo->write(vertex.data(), count_vertex());
        if (has_coord())
            cbo->write(coord.data(), count_coord());
        if (has_normal())
            nbo->write(normal.data(), count_normal());
        if (has_index())
            ibo->write(indices.data(), count_indices());
        update(false);
    }

    if (!has_index())
    {
        if (has_coord())
        {
            cbo->bind();
            glEnableVertexAttribArray(cbo->attrib < 0 ? Attrib_TextureCoord: cbo->attrib);
            glVertexAttribPointer(cbo->attrib < 0 ? Attrib_TextureCoord: cbo->attrib, cbo->byte, GL_FLOAT, GL_FALSE, 0, 0);
        }
        if (has_normal())
        {
            nbo->bind();
            glEnableVertexAttribArray(nbo->attrib < 0 ? Attrib_Normal: nbo->attrib);
            glVertexAttribPointer(nbo->attrib < 0 ? Attrib_Normal: nbo->attrib, nbo->byte, GL_FLOAT, GL_FALSE, 0, 0);
        }
        vbo->bind();
        glEnableVertexAttribArray(vbo->attrib < 0 ? Attrib_Position: vbo->attrib);
        glVertexAttribPointer(vbo->attrib < 0 ? Attrib_Position: vbo->attrib, vbo->byte, GL_FLOAT, GL_FALSE, 0, 0);
        vbo->draw(mode);
    }
    else if (vao.get())
    {
        if (has_coord())
            vao->append(cbo);
        if (has_normal())
            vao->append(nbo);
        vao->append(vbo);
        vao->bind(ibo);
        vao->elements(mode);
    }
    else
    {
        if (has_coord())
        {
            cbo->bind();
            glEnableVertexAttribArray(cbo->attrib < 0 ? Attrib_TextureCoord: cbo->attrib);
            glVertexAttribPointer(cbo->attrib < 0 ? Attrib_TextureCoord: cbo->attrib, cbo->byte, GL_FLOAT, GL_FALSE, 0, 0);
        }
        if (has_normal())
        {
            nbo->bind();
            glEnableVertexAttribArray(nbo->attrib < 0 ? Attrib_Normal: nbo->attrib);
            glVertexAttribPointer(nbo->attrib < 0 ? Attrib_Normal: nbo->attrib, nbo->byte, GL_FLOAT, GL_FALSE, 0, 0);
        }
        vbo->bind();
        glEnableVertexAttribArray(vbo->attrib < 0 ? Attrib_Position: vbo->attrib);
        glVertexAttribPointer(vbo->attrib < 0 ? Attrib_Position: vbo->attrib, vbo->byte, GL_FLOAT, GL_FALSE, 0, 0);
        vbo->elements(mode, indices.data());
    }
}
