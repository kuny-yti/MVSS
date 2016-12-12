#ifndef OPENGL_MX_H
#define OPENGL_MX_H
#include "bs_define.h"
#include "interface/ircontext.h"
#include "glew/GL/glew.h"

#if bsPlatform == bsPlatform_Win32
#include "glew/GL/wglew.h"
#elif bsPlatform != bsPlatform_Apple
#include "glew/GL/glxew.h"
#endif

#ifdef GLEW_MX
    #define glewGetContext() (render::glew_ctx())

    #if bsPlatform == bsPlatform_Win32
        #define wglewGetContext() (render::glew_wctx())
    #elif bsPlatform != bsPlatform_Apple
        #define glxewGetContext() (render::glew_xctx())
    #endif
#endif

namespace render {
#ifdef GLEW_MX
     GLEWContext *glew_ctx();

    #if bsPlatform == bsPlatform_Win32
     WGLEWContext *glew_wctx();
    #elif bsPlatform != bsPlatform_Apple
     GLXEWContext *glew_xctx();
    #endif
#endif
}
#endif // OPENGL_MX_H

