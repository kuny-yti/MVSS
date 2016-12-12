#include "opengl_font3.h"

#include "opengl_mx.h"


opengl_font3::opengl_font3():
    ifonts()
{
    fmode = Font_Mode2D;
}

opengl_font3::~opengl_font3()
{

}

void opengl_font3::type(eFontTypes t)
{
    ifonts::type(t);
    file(ifonts::file());
}
void opengl_font3::encode(eFontEncodes e)
{
    if (font_)
    {
        if (e == Font_EncodeUnicode)
            font_->CharMap(FT_ENCODING_UNICODE);
        else if (e == Font_EncodeMsSymbol)
            font_->CharMap(FT_ENCODING_MS_SYMBOL);
        else if (e == Font_EncodeGB2312)
            font_->CharMap(FT_ENCODING_GB2312);
    }
    ifonts::encode(e);
}
bool opengl_font3::file(const bs_string &f)
{
    font_.reset ();
    simpleLayout_.reset ();

    bs_string fstr = f;
#if bsPlatform == bsPlatform_WIN32
    fstr.replace("/", "\\");
#else
    fstr.replace("\\", "/");
#endif
    bs_string_utf8 fp = fstr.to_utf8();

    switch((int)ifonts::type()) {
        case Font_Bitmap:
            font_ = bs_malloc(FTBitmapFont, (char*)fp.data ()); break;
        case Font_Buffer:
            font_ = bs_malloc(FTBufferFont, (char*)fp.data ()); break;
        case Font_Extrude:
            font_ = bs_malloc(FTExtrudeFont, (char*)fp.data ()); break;
        case Font_Outline:
            font_ = bs_malloc(FTOutlineFont, (char*)fp.data ()); break;
        case Font_Pixmap:
            font_ = bs_malloc(FTPixmapFont, (char*)fp.data ()); break;
        case Font_Polygon:
            font_ = bs_malloc(FTPolygonFont, (char*)fp.data ()); break;
        case Font_Texture:
            font_ = bs_malloc(FTTextureFont, (char*)fp.data ()); break;
        default:
            font_ = bs_malloc(FTBufferFont, (char*)fp.data ()); break;
    }
    if (!font_->Error())
    {
        font_->FaceSize(ifonts::size());
        font_->Depth(ifonts::depth());
        //font_->Outset(-.5, 1.5);
        if (ifonts::encode() == Font_EncodeUnicode)
            font_->CharMap(FT_ENCODING_UNICODE);
        else if (ifonts::encode() == Font_EncodeMsSymbol)
            font_->CharMap(FT_ENCODING_MS_SYMBOL);
        else if (ifonts::encode() == Font_EncodeGB2312)
            font_->CharMap(FT_ENCODING_GB2312);

        simpleLayout_ = bs_malloc(FTSimpleLayout);
        simpleLayout_->SetFont(font_);
        simpleLayout_->SetLineLength(ifonts::length());

        FTGL::TextAlignment hAlign = FTGL::ALIGN_LEFT;
        if (ifonts::align() & Font_AlignLeft)
            hAlign = FTGL::ALIGN_LEFT;
        else if (ifonts::align() & Font_AlignCenter)
            hAlign = FTGL::ALIGN_CENTER;
        else if (ifonts::align() & Font_AlignRight)
            hAlign = FTGL::ALIGN_RIGHT;

        simpleLayout_->SetAlignment(hAlign);
    }
    else
    {
        font_.reset();
        return false;
    }
    return true;
}

void opengl_font3::align(int a)
{
    if (simpleLayout_)
    {
        FTGL::TextAlignment hAlign = FTGL::ALIGN_LEFT;
        if (a & Font_AlignLeft)
            hAlign = FTGL::ALIGN_LEFT;
        else if (a & Font_AlignCenter)
            hAlign = FTGL::ALIGN_CENTER;
        else if (a & Font_AlignRight)
            hAlign = FTGL::ALIGN_RIGHT;

        simpleLayout_->SetAlignment(hAlign);
    }
    ifonts::align(a);
}
void opengl_font3::size(float s)
{
    if (font_)
        font_->FaceSize(s);
    ifonts::size(s);
}
void opengl_font3::length(float l)
{
    if (simpleLayout_)
        simpleLayout_->SetLineLength(l);
    ifonts::length(l);
}
void opengl_font3::depth(float d)
{
    if (font_)
        font_->Depth(d);
    ifonts::depth(d);
}

void opengl_font3::draw(const bs_point3f &pos, const bs_string &text)
{
    if (!simpleLayout_.get() || !font_.get())
        file (ifonts::file());
    if (simpleLayout_)
    {
        float delta = 0;

        FTPoint point(static_cast<double>(pos.x),
                      static_cast<double>(pos.y),
                      static_cast<double>(pos.z));
        FTBBox box = font_->BBox(text.to_wchar (), -1, point);
        delta -= box.Upper().Yf() - box.Lower().Yf(); // height of first line

        bs_bounds _bounds = bounds(pos, text);
        float height = _bounds.urb().y - _bounds.llf().y;
        if (ifonts::align() & Font_AlignTop)
            delta += height;
        if (ifonts::align() & Font_AlignMiddle)
            delta += height * 0.5f;
        if (ifonts::align() & Font_AlignBottom)
            delta = delta;

        bs_point3f dpos = bs_point3f(pos.x, pos.y + delta, pos.z);
        glPushMatrix ();

        //glRasterPos3f(dpos.x, dpos.y, dpos.z);
        glScalef (scale().x, scale().y, scale().z);
        glTranslatef (dpos.x, dpos.y, dpos.z);
        glColor4fv(color().elem);
        simpleLayout_->Render(text.to_wchar (), -1, FTPoint(dpos.x, dpos.y, dpos.z));

        glPopMatrix ();
    }
}

bs_bounds opengl_font3::bounds(const bs_point3f& pos, const bs_string& text) const
{
    if (!font_.get())
        return bs_bounds();

    FTPoint point(static_cast<real>(pos.x),
                  static_cast<real>(pos.y),
                  static_cast<real>(pos.z));

    float delta = 0;

    std::string line;
    std::stringstream ss((char*)text.to_utf8().data ());
    std::getline(ss, line);
    FTBBox box_tmp = font_->BBox(line.c_str(), -1, point);
    delta -= box_tmp.Upper().Yf() - box_tmp.Lower().Yf(); // height of first line

    FTBBox box = simpleLayout_->BBox((char*)text.to_utf8().data (), -1, point);
    FTPoint upper = box.Upper();
    FTPoint lower = box.Lower();
    float height = upper.Yf() - lower.Yf();

    if (ifonts::align() & Font_AlignTop)
        delta += height;
    if (ifonts::align() & Font_AlignMiddle)
        delta += height * 0.5f;
    if (ifonts::align() & Font_AlignBottom)
        delta = delta;

    vec3 upperTGT = vec3(upper.Xf(), upper.Yf() + delta, upper.Zf());
    vec3 lowerTGT = vec3(lower.Xf(), lower.Yf() + delta, lower.Zf());

    return bs_bounds(lowerTGT, upperTGT);
}
