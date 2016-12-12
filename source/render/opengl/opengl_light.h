#ifndef LIGHT_IMPL_H
#define LIGHT_IMPL_H
#include "render/ilight.h"


class opengl_light :public ilight
{
private:
    bs_ptr(class opengl_light_hw) light_hw;
    bs_ptr(class opengl_light_sl) light_sl;
    bs_ptr(class opengl_light_per) light_per;

    tLights light_parame[3];
    bool light_action[3];
    uint light_id;

public:
    opengl_light();
    ~opengl_light();

    virtual uint &id() ;
    virtual tLights &parame(eLightTypes t = Light_OpenGL);

    // apply the light
    virtual void bind(eLightTypes t = Light_OpenGL) ;
    virtual void unbind(eLightTypes t = Light_OpenGL) ;
};

#endif // LIGHT_IMPL_H
