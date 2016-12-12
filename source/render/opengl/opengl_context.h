#ifndef OPENGL_CONTEXT_H
#define OPENGL_CONTEXT_H

#include "bs_define.h"
#include "interface/ircontext.h"

#if bsPlatform == bsPlatform_Win32
#include "wgl/wgl_context.h"
class opengl_context : public wgl_context{};
#elif bsPlatform == bsPlatform_Unix || bsPlatform == bsPlatform_Linux
#include "glx/glx_context.h"
class opengl_context : public glx_context{};
#endif

#endif // OPENGL_CONTEXT_H
