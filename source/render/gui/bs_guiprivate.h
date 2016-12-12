#ifndef BS_GUIPRIVATE_H
#define BS_GUIPRIVATE_H

#include "opengl/opengl_mx.h"
#include "gui/panel/nanovg.h"
#include "gui/panel/nanovg_gl.h"

#define uiRGBA(col)  nvgRGBAf(col.r, col.g, col.b, col.a)
#define uiRGB(col,a)  nvgRGBAf(col.r, col.g, col.b, a)
#define uiContext() (NVGcontext*)bs_gui::instance ()->context

#endif // BS_GUIPRIVATE_H
