#ifndef GLX_CONTEXT_H
#define GLX_CONTEXT_H

#include "interface/ircontext.h"

namespace impl {
struct context;
}
class glx_context : public ircontext
{
    friend struct impl::context;
public:
    virtual bool share(const pircontext &s);
    virtual bs_image capture() const;

    virtual bool create(iwcontext *ctx, const tRenderFormats &rf);
    virtual void close();
    void debugcall(fDebugCall func);

    virtual void swap();
    virtual void make();
    virtual void done();
private:
    impl::context *impl;
};

#endif // GLX_CONTEXT_H
