#ifndef TEXTURE_IMPL_H
#define TEXTURE_IMPL_H
#include "render/itexture.h"

namespace impl {
class opengl_tex;
}

class opengl_texture : public itexture
{
    void create();
public:
    opengl_texture();
    ~opengl_texture();

    int units_max();
    int size_gpu(int level);
    int size_max();

    virtual bool load(const bs_string &file) ;
    virtual bool save(const bs_string &file, ImageWirteFormat form);

    void genmipmap();

    void update(const isize &size, void *data);
    void upload(const isize &size, void *data);

    void *upload(const isize &size);
    void upload();

    void bind(uint mid = 0);
    void unbind(uint mid = 0);

    bs_image read(const pitexture &tex, int level);
    void copy(const ivec4 &viewport, int level = 0, const ipoint &off = ipoint(0,0));
private:
    impl::opengl_tex *impl;
};

#endif // TEXTURE_IMPL_H
