#ifndef IMATERIAL_H
#define IMATERIAL_H
#include "render_define.h"
#include "itexture.h"
#include "bs_color.h"
#include "bs_string.h"

typedef struct Material
{
    bs_color emission; //发光颜色
    bs_color ambient; //环境颜色
    bs_color diffuse; //漫反射颜色
    bs_color specular; //反射光颜色
    float shininess;
}Material;

bsPackage imaterial
{
    virtual ~imaterial(){}
    virtual Material &parame(eMaterialTypes t = Material_OpenGL) = 0;

    virtual bool has_texture() const= 0;
    virtual bs_ptr(itexture) texture() const=0;
    virtual void texture(const bs_ptr(itexture) &) =0;

    virtual bool is_transparent() const= 0;//! 是否透明
    virtual real opacity() const = 0;//! 不透明度

    // apply the material
    virtual void begin(eMaterialTypes t = Material_OpenGL) = 0;
    virtual void end(eMaterialTypes t = Material_OpenGL) = 0;
};
#define iMaterial bs_ptr(imaterial)
typedef iMaterial pimaterial;

#endif // IMATERIAL_H
