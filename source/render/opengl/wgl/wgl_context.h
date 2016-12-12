#ifndef WGL_CONTEXT_H
#define WGL_CONTEXT_H

#include "ircontext.h"
#include "iwcontext.h"

namespace impl {
struct context;
}
class wgl_context : public ircontext
{
    friend struct impl::context;
public:
    wgl_context();
    virtual ~wgl_context();

    virtual bool share(const pircontext &s);

    virtual bs_image image() const;

    virtual bool create(iwcontext *ctx, const tRenderFormats &rf);

    virtual void close();

    virtual void swap();
    virtual void make();
    virtual void done();
private:
    impl::context *impl;
};


#endif // WGL_CONTEXT_H
