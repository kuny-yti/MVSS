#ifndef FRAME_BUFFER_IMPL_H
#define FRAME_BUFFER_IMPL_H
#include "render/ifbo.h"

namespace impl {
class opengl_fbo;
}
class opengl_frame_buffer :public ifbo
{
public:
    opengl_frame_buffer();
    ~opengl_frame_buffer();

    uint create(const isize &size);
    void destroy();

    bool has_fbo();
    bool has_fbo_blit();

    void blit(const pifbo &source, const pifbo &target,
              const ivec4 &targetRect, const ivec4 &sourceRect,
              eBufferTypes buffers = Buffer_Color,
              eTextureParams filter = Filter_Nearest);

    const bs_image read() ;

    int targetadd(const pitexture &tex = pitexture());
    void target(int usedTarget=0);

    void begin(int usedTarget=0);
    void end(int usedTarget=0);
    void bind(void);
    void unbind(void);

protected:
    void create_texture(const pitexture  &tex = pitexture());
    void create_rbo_t(int samples, const isize &size);
private:
    impl::opengl_fbo *impl;
};

#endif // FRAME_BUFFER_IMPL_H
