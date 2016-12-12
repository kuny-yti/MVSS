#ifndef TEXT_IMPL_H
#define TEXT_IMPL_H

#include "itext_font.h"
#include "texture_font.h"
using namespace context;
class text_font_impl :public itext_font
{
public:
    virtual void id(int);
    virtual const int &id()const;
    virtual void family(const wString &name, const wString &path);
    virtual const wString &family()const;
    virtual const wString &file(const wString &name)const;

    virtual Size &size_image();
    virtual const Size &size_image()const;
    virtual void size_image(const Size &);

    virtual float &size();
    virtual const float &size()const;
    virtual void size(const float &n);

    virtual Color &color();
    virtual const Color &color()const;
    virtual void color(const Color &n);

    //! FontTextAlign
    virtual uint &align() ;
    virtual const uint &align()const;
    virtual void align(const uint &n) ;

    virtual float &spacing() ;
    virtual float &blur();
    virtual const float &spacing()const ;
    virtual const float &blur()const ;
    virtual void spacing(const float &) ;
    virtual void blur(const float &) ;
public:
    virtual Sizef width(const Point2f& pos, const wString &text_start,
                        const wString &text_end = wString());
    virtual void begin(FontDrawType type = FontDraw_Screen) ;
    virtual void draw(const Point2f &pos, const wString &text_start,
              const wString &text_end = wString()) ;
    virtual const smart_ptr<itexture> &end()const ;
    virtual const smart_ptr<itexture> &end() ;

private:
    texture_font font;
};
#endif // TEXT_IMPL_H
