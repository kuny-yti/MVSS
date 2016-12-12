#ifndef IPROPERTY_H
#define IPROPERTY_H

#include "bs_define.h"
#include "geomdefine.h"
#include "maths/bs_vec2.h"
#include "bs_ptr.h"

bsPackage iproperty
{
    bool  obj_up;
    float obj_radius;
    ivec2 obj_segment;
    vec3  obj_whd;
    int   obj_angle;
    vec2  obj_tex_min;
    vec2  obj_tex_max;
    uint  obj_proper:32;

    virtual ~iproperty(){}

    iproperty()
    {
        obj_up = true;
        obj_radius = 0.5f;
        obj_segment = ivec2(20, 20);
        obj_whd.x = 0.5f;
        obj_whd.y = 0.5f;
        obj_whd.z = 0.5f;
        obj_angle = 360;
        obj_tex_min = vec2(0,0);
        obj_tex_max = vec2(1,1);
        obj_proper = 0x03;
    }
    void other(uint v) {obj_proper = v;}
    uint other()const {return obj_proper;}
    void update(bool s = true) {obj_up = s;}
    bool is_update()const {return obj_up;}

    void radius(float rad) {obj_radius = rad;update(true);}
    float radius()const {return obj_radius;}
    void height(float h) {obj_whd.y = h;update(true);}
    float height()const {return obj_whd.y;}
    void width(float h) {obj_whd.x = h;update(true);}
    float width()const {return obj_whd.x;}
    void depth(float h) {obj_whd.z = h;update(true);}
    float depth()const {return obj_whd.z;}
    void angle(int a) {obj_angle = a;update(true);}
    int angle()const {return obj_angle;}

    void segment(int x, int y) {obj_segment.x = x; obj_segment.y = y;update(true);}
    void segment(int x) {obj_segment.x = x; obj_segment.y = x;update(true);}
    void segment(const ivec2 &seg) {obj_segment = seg;update(true);}
    ivec2 segment()const{return obj_segment;}

    void coord(const vec2 &min, const vec2 &max) {obj_tex_min = min;obj_tex_max = max;update(true);}
    vec2 coord_min()const{return obj_tex_min;}
    vec2 coord_max()const{return obj_tex_max;}
};

#define IPRO bs_ptr(iproperty)
typedef IPRO piproperty;
#undef IPRO

#endif // IPROPERTY_H
