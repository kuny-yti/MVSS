#ifndef ILIGHT_H
#define ILIGHT_H
#include "render_define.h"
#include "bs_vec4.h"
#include "bs_color.h"
#include "bs_size.h"
#include "bs_rect.h"
#include "bs_ptr.h"

const int SHADER_LIGHT_MAX = 32;
const int OPENGL_LIGHT_MAX = 8;
using namespace bs_maths;
typedef struct tLights
{
    bool state;
    int type;// LIGHT_AREA, LIGHT_DIRECTION, LIGHT_POINT, LIGHT_SPOT

    vec3 position;
    bs_color ambient;
    bs_color diffuse;
    bs_color specular;

    vec3 direction;
    float cutoff;
    float exponent;

    float constant;
    float linear;
    float quadratic;

    bs_sizef area;
    vec3 right;
} tLights;

bsPackage ilight
{
    virtual ~ilight(){}
    virtual uint &id() = 0;
    virtual tLights &parame(eLightTypes t = Light_OpenGL) = 0;

    // apply the light
    virtual void bind(eLightTypes t = Light_OpenGL) = 0;
    virtual void unbind(eLightTypes t = Light_OpenGL) = 0;
};
#define iLight bs_ptr(ilight)
typedef iLight pilight;

#endif // ILIGHT_H
