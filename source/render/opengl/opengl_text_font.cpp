#include "text_font_impl.h"

void text_font_impl::id(int i)
{
    font.id(i);
}
const int &text_font_impl::id()const
{
    return font.id();
}
void text_font_impl::family(const wString &name, const wString &path)
{
    font.family(name, path);
}
const wString &text_font_impl::family()const
{
    return font.family();
}
const wString &text_font_impl::file(const wString &name)const
{
    return font.file(name);
}
Size &text_font_impl::size_image()
{
    return font.size_image();
}
const Size &text_font_impl::size_image()const
{
    return font.size_image();
}
void text_font_impl::size_image(const Size &s)
{
    font.size_image(s);
}
float &text_font_impl::size()
{
    return font.size();
}
const float &text_font_impl::size()const
{
    return font.size();
}
void text_font_impl::size(const float &n)
{
    font.size(n);
}

Color &text_font_impl::color()
{
    return font.color();
}
const Color &text_font_impl::color()const
{
    return font.color();
}
void text_font_impl::color(const Color &n)
{
    font.color(n);
}

//! FontTextAlign
uint &text_font_impl::align()
{
    return font.align();
}
const uint &text_font_impl::align()const
{
    return font.align();
}
void text_font_impl::align(const uint &n)
{
    font.align(n);
}
float &text_font_impl::spacing()
{
    return font.spacing();
}
float &text_font_impl::blur()
{
    return font.blur();
}
const float &text_font_impl::spacing()const
{
    return font.spacing();
}
const float &text_font_impl::blur()const
{
    return font.blur();
}
void text_font_impl::spacing(const float &v)
{
    font.spacing(v);
}
void text_font_impl::blur(const float &v)
{
    font.blur(v);
}
Sizef text_font_impl::width(const Point2f& pos, const wString &text_start,
                            const wString &text_end)
{
    font.width(pos, text_start, text_end);
}
void text_font_impl::begin(FontDrawType type )
{
    font.begin(type);
}
void text_font_impl::draw(const Point2f &pos, const wString &text_start,
                          const wString &text_end )
{
    font.draw(pos, text_start, text_end);
}
const smart_ptr<itexture> &text_font_impl::end()const
{
    return font.end();
}
const smart_ptr<itexture> &text_font_impl::end()
{
    return font.end();
}
