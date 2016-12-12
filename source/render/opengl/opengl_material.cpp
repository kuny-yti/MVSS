#include "material_impl.h"
#include "opengl_api.h"

class material_impl_hw
{
public:
    float front_save_diffuse[4];
    float front_save_specular[4];
    float front_save_ambient[4];
    float front_save_emissive[4];
    float front_save_shininess;

    float back_save_diffuse[4];
    float back_save_specular[4];
    float back_save_ambient[4];
    float back_save_emissive[4];
    float back_save_shininess;

    material_impl_hw()
    {

    }

    void push_current()
    {
        glGetMaterialfv(GL_FRONT, GL_DIFFUSE, &front_save_diffuse[0]);
        glGetMaterialfv(GL_FRONT, GL_SPECULAR, &front_save_specular[0]);
        glGetMaterialfv(GL_FRONT, GL_AMBIENT, &front_save_ambient[0]);
        glGetMaterialfv(GL_FRONT, GL_EMISSION, &front_save_emissive[0]);
        glGetMaterialfv(GL_FRONT, GL_SHININESS, &front_save_shininess);

        glGetMaterialfv(GL_BACK, GL_DIFFUSE, &back_save_diffuse[0]);
        glGetMaterialfv(GL_BACK, GL_SPECULAR, &back_save_specular[0]);
        glGetMaterialfv(GL_BACK, GL_AMBIENT, &back_save_ambient[0]);
        glGetMaterialfv(GL_BACK, GL_EMISSION, &back_save_emissive[0]);
        glGetMaterialfv(GL_BACK, GL_SHININESS, &back_save_shininess);
    }
    void pop_current()
    {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, &front_save_diffuse[0]);
        glMaterialfv(GL_FRONT, GL_SPECULAR, &front_save_specular[0]);
        glMaterialfv(GL_FRONT, GL_AMBIENT, &front_save_ambient[0]);
        glMaterialfv(GL_FRONT, GL_EMISSION, &front_save_emissive[0]);
        glMaterialfv(GL_FRONT, GL_SHININESS, &front_save_shininess);

        glMaterialfv(GL_BACK, GL_DIFFUSE, &back_save_diffuse[0]);
        glMaterialfv(GL_BACK, GL_SPECULAR, &back_save_specular[0]);
        glMaterialfv(GL_BACK, GL_AMBIENT, &back_save_ambient[0]);
        glMaterialfv(GL_BACK, GL_EMISSION, &back_save_emissive[0]);
        glMaterialfv(GL_BACK, GL_SHININESS, &back_save_shininess);
    }
    void apply(const Material &parame)
    {
        push_current();
        float diffuse[4] = {parame.diffuse.r, parame.diffuse.g,
                            parame.diffuse.b, parame.diffuse.a};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

        float specular[4] = {parame.specular.r, parame.specular.g,
                            parame.specular.b, parame.specular.a};
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

        float ambient[4] = {parame.ambient.r, parame.ambient.g,
                            parame.ambient.b, parame.ambient.a};
        glMaterialfv(GL_FRONT_AND_BACK,  GL_AMBIENT, ambient);

        float emission[4] = {parame.emission.r, parame.emission.g,
                            parame.emission.b, parame.emission.a};
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &parame.shininess);
        pop_current();
    }

};

material_impl::material_impl()
{
    material_opacity = 1.0;
    memset (material_parame, 0, sizeof(Material) *2);
}
material_impl::~material_impl()
{
}

Material &material_impl::parame(eMaterialTypes t)
{
    if (t == Material_GLSL)
        return material_parame[1];
    return material_parame[0];
}

// apply the material
void material_impl::begin(eMaterialTypes t)
{
    if (has_texture())
    {
        material_texture->bind();
    }
    if (t == Material_OpenGL)
    {
        if (!material_hw)
            material_hw = bsMalloc(material_impl_hw);

        material_parame[0].ambient.a = material_opacity;
        material_parame[0].diffuse.a = material_opacity;
        material_parame[0].specular.a = material_opacity;
        material_parame[0].emission.a = material_opacity;

        material_hw->apply (material_parame[0]);
    }
    else if (t == Material_GLSL)
    {

    }
}
void material_impl::end(eMaterialTypes t)
{
    if (has_texture())
    {
        material_texture->unbind();
    }
}
bool material_impl::has_texture() const
{
    return (bool)material_texture;
}
itexture *material_impl::texture() const
{
    return material_texture;
}
void material_impl::texture(itexture *t)
{
    material_texture = t;
}

bool material_impl::is_transparent() const
{
    return material_opacity < 1.0;
}
real material_impl::opacity() const
{
    return material_opacity;
}
