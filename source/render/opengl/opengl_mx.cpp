#include "opengl_mx.h"

namespace render {
#ifdef GLEW_MX
    static GLEWContext _glewctx;

    #if bsPlatform == bsPlatform_Win32
        static WGLEWContext _wglewctx ;
    #elif bsPlatform != bsPlatform_Apple
        static GLXEWContext _glxewctx ;
    #endif
#endif


#ifdef GLEW_MX
    GLEWContext *glew_ctx()
    {
        return &_glewctx;
    }
    #if bsPlatform == bsPlatform_Win32
    WGLEWContext *glew_wctx()
    {
        return &_wglewctx;
    }
    #elif bsPlatform != bsPlatform_Apple
    GLXEWContext *glew_xctx()
    {
        return &_glxewctx;
    }
    #endif
#endif
}
