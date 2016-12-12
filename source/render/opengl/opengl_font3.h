#ifndef OPENGL_FONT3_H
#define OPENGL_FONT3_H

#include "bs_define.h"
#include "irender.h"
#include "ifonts.h"
#include <FTGL/ftgl.h>

class opengl_font3 :public ifonts
{
public:
    opengl_font3();
    ~opengl_font3();

    virtual void type(eFontTypes t);
    virtual void encode(eFontEncodes e);
    virtual bool file(const bs_string &f);
    virtual void align(int a);
    virtual void size(float s);
    virtual void length(float l);
    virtual void depth(float d);

    virtual void draw(const bs_point3f &pos, const bs_string &text);

    virtual bs_bounds bounds(const bs_point3f& pos, const bs_string& text) const;

private:
    bs_ptr(FTFont) font_;
    bs_ptr(FTSimpleLayout) simpleLayout_;

};

#endif // OPENGL_FONT_H
