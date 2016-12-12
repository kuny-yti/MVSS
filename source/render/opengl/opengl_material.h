#ifndef MATERIAL_IMPL_H
#define MATERIAL_IMPL_H
#include "imaterial.h"

using namespace module;
using namespace base;
class material_impl :public imaterial
{
private:
    Material material_parame[2];

    class material_impl_hw* material_hw;
    itexture* material_texture;
    real material_opacity;
public:
    material_impl();
    ~material_impl();

    virtual Material &parame(eMaterialTypes t = Material_OpenGL);

    virtual bool has_texture() const;
    virtual itexture *texture() const;
    virtual void texture(itexture *);

    virtual bool is_transparent() const;//! 是否透明
    virtual real opacity() const;//! 不透明度

    // apply the material
    virtual void begin(eMaterialTypes t = Material_OpenGL);
    virtual void end(eMaterialTypes t = Material_OpenGL);
};

#endif // MATERIAL_IMPL_H
