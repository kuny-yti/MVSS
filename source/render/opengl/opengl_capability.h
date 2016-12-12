#ifndef OPENGL_NEWS_H
#define OPENGL_NEWS_H

#include "bs_define.h"
#include "tools/bs_string.h"
#include "render/icapability.h"
#include "bs_ptr.h"

namespace impl {
class opengl_capab;
}
class opengl_capability :public icapability
{
public:
    opengl_capability();
    ~opengl_capability();

    void initialize();

    eOpenGLVendor vendors()const;
    eOpenGLPackages package() const;
    eGPUArchs arch()const ;
    bool is_support(const bs_string &ext)const;
    bool is_support(eCapabilityExtensions ext)const;
private:
    impl::opengl_capab *impl;
};

#endif // OPENGL_NEWS_H
