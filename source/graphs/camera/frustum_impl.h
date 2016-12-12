#ifndef FRUSTUM_H
#define FRUSTUM_H
#include "graphs/ifrustum.h"

class frustum_impl :public ifrustum
{
private:
    enum normalsEnum { LEFT, RIGHT, TOP, BOTTOM, NEARs, FARs, NORMALS_NUM };
    enum vertexsEnum { RTN, LTN, LBN, RBN, RTF,LTF, LBF, RBF, VERTEXS_NUM };

    vec3 normals_[NORMALS_NUM];
    vec3 vertex_[VERTEXS_NUM];

    vec3 nearp_;
    vec3 farp_;
    vec3 campos_;

    void calc_vertex(const bs_ptr(bsPackage icamera ) &c);
public:
    frustum_impl();
    frustum_impl(float fovy, float ratio, float _near, float _far);
    frustum_impl(float left, float right, float bottom, float top, float _near, float _far);

    bool is_culled_xz(const faabb &bound) const;
    bool is_culled(const faabb& b) const;
    bool is_culled(const vec3& v) const;

    void update(const bs_ptr(bsPackage icamera ) & c);

    float fovy() const;
    float ratio() const;

    void fovy(float fovy);
    void ratio(float ratio);

    const vec3& leftn() const;
    const vec3& rightn() const;
    const vec3& bottomn() const;
    const vec3& topn() const;
    const vec3& nearn() const;
    const vec3& farn() const;

    const vec3& nearp() const;
    const vec3& farp() const;
    const vec3& campos() const;

    const vec3& normal(int num) const;
    const vec3& vertex(int num)const ;
    bool operator == (const pifrustum &rhs) ;
    void operator = (const pifrustum &rhs);
};

#endif // FRUSTUM_H
