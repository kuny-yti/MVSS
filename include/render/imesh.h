#ifndef IMESH_H
#define IMESH_H

#include "ivao.h"
#include "ivbo.h"
#include "iibo.h"

#include "geometry/geomdefine.h"

#define iMesh bs_ptr(bsPackage imesh)
typedef iMesh pimesh;
#undef iMesh

using namespace primitive;
bsPackage imesh
{
    bool is_up;
    bs_vector <fpoint3>    vertex;
    bs_vector <vec3>       normal;
    bs_vector <vec2>       coord;
    bs_vector <tIndicess>  indices;

    pivao  vao;
    pivbo  vbo;
    pivbo  cbo;
    pivbo  nbo;
    piibo  ibo;

    eDrawModes mode;

    imesh()
    {
        is_up = true;
        mode = Draw_None;
    }
    virtual ~imesh (){}

    void update(bool s = true) {is_up = s;}
    bool is_update()const {return is_up;}
    //!
    //! \brief append 添加顶点、法线、纹理坐标、索引
    //! \param v
    //!
    void append(const fpoint3 &v){vertex.append(v);update();}
    void append(const vec3 &v){normal.append(v);update();}
    void append(const vec2 &v){coord.append(v);update();}
    void append(const tIndicess &v){indices.append(v);update();}

    //!
    //! \brief append 添加三角形顶点、法线、纹理坐标、索引
    //! \param 0
    //! \param 1
    //! \param 2
    //!
    void append(const fpoint3 &p0, const fpoint3 &p1, const fpoint3 &p2)
    {vertex.append(p0);vertex.append(p1);vertex.append(p2);update();}
    void append(const vec3 &n0,const vec3 &n1,const vec3 &n2)
    {normal.append(n0);normal.append(n1);normal.append(n2);update();}
    void append(const vec2 &c0, const vec2 &c1, const vec2 &c2)
    {coord.append(c0);coord.append(c1);coord.append(c2);update();}
    void append(const tIndicess &i0, const tIndicess &i1, const tIndicess &i2)
    {indices.append(i0);indices.append(i1);indices.append(i2);update();}

    void append(fpoint3 *p, size_t len)
    {vertex.append(p, len);update();}
    void append(vec3 *p, size_t len)
    {normal.append(p, len);update();}
    void append(vec2 *p, size_t len)
    {coord.append(p, len);update();}
    void append(tIndicess *p, size_t len)
    {indices.append(p, len);update();}

    void append(tTriangleIndices *p, size_t len)
    {append((tIndicess*)p->elem, len);update();}
    void append(tQuadIndices *p, size_t len)
    {append((tIndicess*)p->elem, len * p->col * p->row);update();}
    void append(tMxNIndices *p, size_t len)
    {append((tIndicess*)p->elem, len* p->col * p->row);update();}

    void append (tLine3f &line)
    {
        append((fpoint3)line.p0);
        append((fpoint3)line.p1);
    }
    void append (tTriangle3f &tri)
    {
        append((fpoint3)tri.p0, (fpoint3)tri.p1, (fpoint3)tri.p2);
    }
    void append (tQuads3f &quad)
    {
        append(quad.t0);
        append(quad.t1);
    }

    void append (tPrimitive3f &pri)
    {
        append((fpoint3)pri.vertex);
        append((vec2)pri.coord);
        append((vec3)pri.normal);
    }
    void append (tPrimitiveTriangle3f &pri)
    {
        append(pri.p0);
        append(pri.p1);
        append(pri.p2);
    }
    void append (tPrimitiveQuads3f &pri)
    {
        append(pri.t0);
        append(pri.t1);
    }

    void clear()
    {vertex.clear();normal.clear();coord.clear();indices.clear();}

    void append (const pimesh &m)
    {
        if (m->has_vertex())
            vertex.append(m->vertex);
        else
            return ;

        if (m->has_normal())
            normal.append(m->normal);
        if (m->has_coord())
            coord.append(m->coord);
        if (m->has_index())
            indices.append(m->indices);
        update();
    }

    int count_vertex() const{return vertex.count();}
    int count_normal() const{return normal.count();}
    int count_coord() const{return coord.count();}
    int count_indices() const{return indices.count();}

    bool has_vertex() const{return count_vertex() > 0;}
    bool has_normal() const{return count_normal() > 0;}
    bool has_coord() const{return count_coord() > 0;}
    bool has_index() const{return count_indices() > 0;}

    //!
    //! \brief autoindices 自动索引计算
    //! 根据绘制模式 [默认:Draw_TriangleStrip]
    //!   a. Draw_Lines:
    //!   b. Draw_LineStrip:
    //!   c. Draw_Triangles:
    //!   d. Draw_TriangleStrip:
    //!
    virtual void autoindices() = 0;

    //!
    //! \brief draw
    //! \param dm
    //!
    virtual void draw (eDrawModes dm) {mode = dm;}
};

#endif // IMESH_H
