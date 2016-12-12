#include "opengl_light.h"
#include "opengl_mx.h"

class opengl_light_hw
{
public:

    opengl_light_hw()
    {
    }
    ~opengl_light_hw()
    {

    }

    void position(uint id, const vec3 & position)
    {
        glLightfv(GL_LIGHT0 + id, GL_POSITION, position.elem);
    }
    void diffuse(uint id, const bs_color & diffuse)
    {
        float dat[4] = {diffuse.r, diffuse.g, diffuse.b, diffuse.a};
        glLightfv(GL_LIGHT0 + id, GL_DIFFUSE, dat);
    }
    void specular(uint id, const bs_color & specular)
    {
        float dat[4] = {specular.r, specular.g, specular.b, specular.a};
        glLightfv(GL_LIGHT0 + id, GL_SPECULAR, dat);
    }
    void ambient(uint id, const bs_color & ambient)
    {
        float dat[4] = {ambient.r, ambient.g, ambient.b, ambient.a};
        glLightfv(GL_LIGHT0 + id, GL_AMBIENT, dat);
    }
    void attenuation(uint id, const vec3 &val)
    {
        glLightf(GL_LIGHT0 + id, GL_CONSTANT_ATTENUATION, val.x);
        glLightf(GL_LIGHT0 + id, GL_LINEAR_ATTENUATION, val.y);
        glLightf(GL_LIGHT0 + id, GL_QUADRATIC_ATTENUATION, val.z);
    }
    void spot_direction(uint id, const vec3 & direction)
    {
        glLightfv(GL_LIGHT0 + id, GL_SPOT_DIRECTION, direction.elem);
    }
    void spot_angle(uint id, float angle)
    {
        glLightf(GL_LIGHT0 + id, GL_SPOT_CUTOFF, angle);
    }
    void spot_exponent(uint id, float exponent)
    {
        float  val =exponent * 128.0f;
        glLightf(GL_LIGHT0 + id, GL_SPOT_EXPONENT, val);
    }
    void apple(uint id, const tLights &parame)
    {
        position(id, parame.position);
        diffuse(id, parame.diffuse);
        specular(id, parame.specular);
        ambient(id, parame.ambient);
        attenuation(id, vec3(parame.constant, parame.linear,
                                  parame.quadratic));
        spot_direction(id, parame.direction);
        spot_angle(id, parame.cutoff);
        spot_exponent(id, parame.exponent);
    }
};

class opengl_light_sl
{
public:

    opengl_light_sl()
    {
    }
    ~opengl_light_sl()
    {

    }

    void position(uint id, const vec4 & position)
    {
    }
    void diffuse(uint id, const bs_color & diffuse)
    {
    }
    void specular(uint id, const bs_color & specular)
    {
    }
    void ambient(uint id, const bs_color & ambient)
    {
    }
    void attenuation(uint id, const vec3 &val)
    {
    }
    void spot_direction(uint id, const vec3 & direction)
    {
    }
    void spot_angle(uint id, float angle)
    {
    }
    void spot_exponent(uint id, float exponent)
    {
    }
    void apple(uint id, const tLights &parame)
    {
    }
};

class opengl_light_per
{
public:

    opengl_light_per()
    {
    }
    ~opengl_light_per()
    {

    }

    void position(uint id, const vec4 & position)
    {
    }
    void diffuse(uint id, const bs_color & diffuse)
    {
    }
    void specular(uint id, const bs_color & specular)
    {
    }
    void ambient(uint id, const bs_color & ambient)
    {
    }
    void attenuation(uint id, const vec3 &val)
    {
    }
    void spot_direction(uint id, const vec3 & direction)
    {
    }
    void spot_angle(uint id, float angle)
    {
    }
    void spot_exponent(uint id, float exponent)
    {
    }
    void apple(uint id, const tLights &parame)
    {
    }
};

opengl_light::opengl_light()
{
    light_id = 0;
    memset (light_parame, 0, sizeof(tLights)*3);
    light_action[0] = false;
    light_action[1] = false;
    light_action[2] = false;
}
opengl_light::~opengl_light()
{
    light_hw.reset ();
    light_sl.reset ();
    light_per.reset ();
}
uint &opengl_light::id()
{
    return light_id;
}
tLights &opengl_light::parame(eLightTypes t )
{
    if (t & Light_GLSL)
    {
        return light_parame[1];
    }
    else if (t & Light_OpenGL)
    {
        return light_parame[0];
    }
    else if (t & Light_PerPixelGLSL)
    {
        return light_parame[2];
    }
    return light_parame[0];
}


// apply the light
void opengl_light::bind(eLightTypes t)
{
    if (t & Light_GLSL)
    {
        if (!light_sl)
            light_sl = bs_malloc(opengl_light_sl);
        light_sl->apple (light_id, light_parame[1]);
        light_action[1] = true;
    }
    else if (t & Light_OpenGL)
    {
        if (!light_hw)
            light_hw = bs_malloc(opengl_light_hw) ;
        glEnable (GL_LIGHTING);
        if (light_id < 8)
        {
            glEnable (GL_LIGHT0 +light_id );
            light_hw->apple(light_id, light_parame[0]);
            light_action[0] = true;
        }
    }
    else if (t & Light_PerPixelGLSL)
    {
        if (!light_per)
            light_per = bs_malloc(opengl_light_per);
        light_per->apple (light_id, light_parame[2]);
        light_action[2] = true;
    }

}
void opengl_light::unbind(eLightTypes t)
{
    if (t & Light_GLSL)
    {
        if (!light_action[1]) return;

        light_action[1] = false;
    }
    else if (t & Light_OpenGL)
    {
        if (!light_action[0]) return;
        if (light_id < 8)
            glDisable(GL_LIGHT0 +light_id );
         glDisable(GL_LIGHTING);
         light_action[0] = false;
    }
    else if (t & Light_PerPixelGLSL)
    {
        if (!light_action[2]) return;

        light_action[2] = false;
    }

}
