#ifndef BS_COLOR_H
#define BS_COLOR_H
#include "bs_maths.h"
#include "tools/bs_stream.h"

typedef float color_type_t ;
typedef uint bs_color32; // R8-G8-B8-A8

const bs_color32 COLOR32_BLACK     = 0x000000ff;   // Black
const bs_color32 COLOR32_WHITE     = 0xffffffff;   // White
const bs_color32 COLOR32_ZERO      = 0x00000000;   // Zero
const bs_color32 COLOR32_RED       = 0xff0000ff;   // Red
const bs_color32 COLOR32_GREEN     = 0x00ff00ff;   // Green
const bs_color32 COLOR32_BLUE      = 0x0000ffff;   // Blue
const color_type_t  HSV_UNDEFINED = -1.0;
const color_type_t  HSL_UNDEFINED = HSV_UNDEFINED;

/*!
 * @brief The eRGBCompositeModes enum RGB颜色合成模式
 * @enum eRGBCompositeModes
 */
typedef enum
{
    RGB_CompositeNone = 0,
    RGB_CompositeNormal,
    RGB_CompositeBehind
} eRGBCompositeModes;

/*!
 * @brief The bs_color class 简单的颜色处理类
 * @class bs_color
 * 1.涵盖了多种颜色格式：
 *   a.RGBA
 *   b.YUV
 *   c.HSB
 *   d.HSV
 *   e.HSL
 *   f.HWB
 *   g.CMYK
 * 2.涵盖有各种颜色的转换方法
 * 3.涵盖的功能有：
 *   a.伽马校正
 *   b.亮度值计算
 *   c.强度值计算
 *   d.颜色合成、混合
 *   e.颜色运算 (相加、相减、相乘、差值)
 *   f.颜色的插值 (线性插值、双线性插值)
 *
 * 备注：内部存储为RGBA格式
 */
struct bs_color
{
    enum {size = 4};

    union {
        struct {
            //![0.0..1.0] or [0.0..255.0]
            //!@r: red @g: green @b: blue @a: alpha
            color_type_t r; color_type_t g; color_type_t b;  color_type_t a;
        };
        struct {
            //![0.0..255.0]
            color_type_t yuv_y; color_type_t u; color_type_t v; color_type_t _yuv_a;
        };
        struct {
            color_type_t h; color_type_t s; color_type_t _hsb_b; color_type_t _hsb_a;
        };
        struct {
            //![0.0..1.0] or H [0, 359], S [0, 255], V [0, 255]
            //!@h: hue @s: saturation @v: value @a: alpha
            color_type_t _hsv_h; color_type_t _hsv_s; color_type_t _hsv_v; color_type_t _hsv_a;
        };
        struct {
            //![0.0..1.0]  H [0, 360] L [0, 255] S [0, 255]
            //!@h: hue @s: saturation @l: lightness @a: alpha
            color_type_t _hsl_h; color_type_t _hsl_s; color_type_t l; color_type_t _hsl_a;
        };
        struct {
            //! @h :hue (色调0-6) @w :whiteness (白度0-1) @b :blackness (黑度0-1)
            color_type_t _hwb_h; color_type_t w; color_type_t _hwb_b; color_type_t _hwb_a;
        };
        struct {
            //![0.0..1.0]
            //!@c: cyan @m: magenta @y: yellow @k: black @a: alpha
            color_type_t c; color_type_t m; color_type_t y; color_type_t k;
        };

        color_type_t elem[size];
    };

public:
    bs_color()
    {
        for (int i = 0; i < size; ++i)
            elem[i] = (color_type_t)0;
    }

    explicit bs_color(bs_color32 hex)
    {
        *this = IntColor::hex_to_rgba(hex);
    }

    explicit bs_color(real init)
    {
        for (size_t i = 0; i < size; ++i)
            elem[i] = (color_type_t)bs_clamp(init, 0., 1.);
    }
    explicit bs_color(real t1, real t2, real t3, real t4 = 1.0)
    {
        elem[0] = (color_type_t)bs_clamp(t1, 0., 1.);
        elem[1] = (color_type_t)bs_clamp(t2, 0., 1.);
        elem[2] = (color_type_t)bs_clamp(t3, 0., 1.);
        elem[3] = (color_type_t)bs_clamp(t4, 0., 1.);
    }
    explicit bs_color(const real* init)
    {
        for (size_t i = 0; i < size; ++i)
            elem[i] = bs_clamp(init[i], 0., 1.);
    }

    explicit bs_color(color_type_t init)
    {
        for (size_t i = 0; i < size; ++i)
            elem[i] = bs_clamp(init, (color_type_t)0., (color_type_t)1.);
    }
    explicit bs_color(const color_type_t* init)
    {
        for (size_t i = 0; i < size; ++i)
            elem[i] = bs_clamp(init[i], (color_type_t)0., (color_type_t)1.);
    }
    explicit bs_color(color_type_t t1, color_type_t t2, color_type_t t3, color_type_t t4 = 1.0)
    {
        elem[0] = bs_clamp(t1, (color_type_t)0., (color_type_t)1.);
        elem[1] = bs_clamp(t2, (color_type_t)0., (color_type_t)1.);
        elem[2] = bs_clamp(t3, (color_type_t)0., (color_type_t)1.);
        elem[3] = bs_clamp(t4, (color_type_t)0., (color_type_t)1.);
    }

    explicit bs_color(uchar t1, uchar t2, uchar t3, uchar t4 = 255)
    {
        elem[0] = (color_type_t)t1 / 255.;
        elem[1] = (color_type_t)t2 / 255.;
        elem[2] = (color_type_t)t3 / 255.;
        elem[3] = (color_type_t)t4 / 255.;
    }
    explicit bs_color(const uchar* init)
    {
        for (size_t i = 0; i < size; ++i)
            elem[i] = (color_type_t)init[i] / 255.;
    }
    explicit bs_color(uchar init)
    {
        for (size_t i = 0; i < size; ++i)
            elem[i] = (color_type_t)init / 255.;
    }

    explicit bs_color(int t1, int t2, int t3, int t4 = 255)
    {
        elem[0] = (color_type_t)(bs_clamp(t1, 0, 255)) / 255.;
        elem[1] = (color_type_t)(bs_clamp(t2, 0, 255)) / 255.;
        elem[2] = (color_type_t)(bs_clamp(t3, 0, 255)) / 255.;
        elem[3] = (color_type_t)(bs_clamp(t4, 0, 255)) / 255.;
    }
    explicit bs_color(const int* init)
    {
        for (size_t i = 0; i < size; ++i)
            elem[i] = (color_type_t)(bs_clamp(init[i], 0, 255)) / 255.;
    }

    const color_type_t& operator [] (size_t index) const
    {
        return elem[index];
    }
    color_type_t & operator [] (size_t index)
    {
        return elem[index];
    }

    bs_color32 to_hex()const
    {
        return FloatColor::rgba_to_hex(r,g,b,a);
    }

    void form_hex(bs_color32 hex)
    {
        *this = IntColor::hex_to_rgba(hex);
    }
    void form_hsl (const bs_color &hsl)
    {
        *this = FloatColor::hsl_to_rgb(hsl);
    }
    void form_hsv (const bs_color &hsv)
    {
        *this = FloatColor::hsv_to_rgb(hsv);
    }
    void form_hwb (const bs_color &hwb)
    {
        *this = FloatColor::hwb_to_rgb(hwb);
    }
    void form_cmyk (const bs_color &cmyk)
    {
        *this = FloatColor::cmyk_to_rgb(cmyk);
    }
    void form_yuv (const bs_color &yuv)
    {
        *this = IntColor::yuv_to_rgb(yuv.y, yuv.u, yuv.v);
    }

    bs_color to_hsl()const
    {
        return FloatColor::rgb_to_hsl(*this);
    }
    bs_color to_hsv()const
    {
        return FloatColor::rgb_to_hsv(*this);
    }
    bs_color to_hwb()const
    {
        return FloatColor::rgb_to_hwb(*this);
    }
    bs_color to_cmyk()const
    {
        return FloatColor::rgb_to_cmyk(*this, 0);
    }
    bs_color to_yuv()const
    {
        uchar R = this->r * 255.;
        uchar G = this->g * 255.;
        uchar B = this->b * 255.;
        uchar A = this->a * 255.;
        return IntColor::rgb_to_yuv(R, G, B, A);
    }
public:
    void gamma(color_type_t  gamma)
    {
        *this = rgb_gamma(*this, gamma);
    }
    color_type_t luminance()const
    {
        return rgb_luminance(*this);
    }
    color_type_t intensity()const
    {
        return rgb_intensity(*this);
    }
    void composite(const bs_color &col, eRGBCompositeModes  mode)
    {
        *this = rgb_composite(*this, col, mode);
    }
    color_type_t distance(const bs_color &col)
    {
        return rgba_distance(*this, col);
    }

    static bs_color interpolate (const bs_color &initial,
                                 const bs_color &final,
                                 color_type_t    pos)
    {
        bs_color out;
        out.r = initial.r + (final.r - initial.r) * pos;
        out.g = initial.g + (final.g - initial.g) * pos;
        out.b = initial.b + (final.b - initial.b) * pos;
        out.a = initial.a + (final.a - initial.a) * pos;
        return out;
    }
public:
    bs_color operator +(const bs_color &_src)
    {
        return rgba_add(*this, _src);
    }
    bs_color operator -(const bs_color &_src)
    {
        return rgba_subtract(*this, _src);
    }
    bs_color operator *(color_type_t  factor)
    {
        return rgba_multiply(*this, factor);
    }
    void operator +=(const bs_color &_src)
    {
        *this = rgba_add(*this, _src);
    }
    void operator -=(const bs_color &_src)
    {
        *this = rgba_subtract(*this, _src);
    }
    void operator *=(color_type_t  factor)
    {
        *this = rgba_multiply(*this, factor);
    }

    bool operator == (const bs_color &rhs)
    {
        return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
    }
    bool operator != (const bs_color &rhs)
    {
        return !((*this) == rhs);
    }
private:
    static bs_color blend(const bs_color &_src ,const bs_color &_dst, color_type_t _S)
    {
        bs_color out;
        color_type_t t = 1.0f - _S;
        out.a = (t*_src.a +_S*_dst.a );
        out.r = (t*_src.r +_S*_dst.r );
        out.g = (t*_src.g +_S*_dst.g );
        out.b = (t*_src.b +_S*_dst.b );
        return out;
    }

    class FloatColor
    {
    public:
        //! HSV格式
        static bs_color rgb_to_hsv (const bs_color &rgb)
        {
            bs_color hsv;
            real max, min, delta;

            max = rgb_max (rgb.r, rgb.g, rgb.b);
            min = rgb_min (rgb.r, rgb.g, rgb.b);

            hsv.v = max;
            delta = max - min;

            if (delta > 0.0001)
            {
                hsv.s = delta / max;

                if (rgb.r == max)
                {
                    hsv.h = (rgb.g - rgb.b) / delta;
                    if (hsv.h < 0.0)
                        hsv.h += 6.0;
                }
                else if (rgb.g == max)
                {
                    hsv.h = 2.0 + (rgb.b - rgb.r) / delta;
                }
                else
                {
                    hsv.h = 4.0 + (rgb.r - rgb.g) / delta;
                }

                hsv.h /= 6.0;
            }
            else
            {
                hsv.s = 0.0;
                hsv.h = 0.0;
            }

            hsv.a = rgb.a;
            return hsv;
        }
        static bs_color hsv_to_rgb (const bs_color &hsv)
        {
            bs_color rgb;
            int    i;
            real f, w, q, t;
            real hue;

            if (hsv.s == 0.0)
            {
                rgb.r = hsv.v;
                rgb.g = hsv.v;
                rgb.b = hsv.v;
            }
            else
            {
                hue = hsv.h;

                if (hue == 1.0)
                    hue = 0.0;

                hue *= 6.0;

                i = (int) hue;
                f = hue - i;
                w = hsv.v * (1.0 - hsv.s);
                q = hsv.v * (1.0 - (hsv.s * f));
                t = hsv.v * (1.0 - (hsv.s * (1.0 - f)));

                switch (i)
                {
                case 0:
                    rgb.r = hsv.v;
                    rgb.g = t;
                    rgb.b = w;
                    break;
                case 1:
                    rgb.r = q;
                    rgb.g = hsv.v;
                    rgb.b = w;
                    break;
                case 2:
                    rgb.r = w;
                    rgb.g = hsv.v;
                    rgb.b = t;
                    break;
                case 3:
                    rgb.r = w;
                    rgb.g = q;
                    rgb.b = hsv.v;
                    break;
                case 4:
                    rgb.r = t;
                    rgb.g = w;
                    rgb.b = hsv.v;
                    break;
                case 5:
                    rgb.r = hsv.v;
                    rgb.g = w;
                    rgb.b = q;
                    break;
                }
            }

            rgb.a = hsv.a;
            return rgb;
        }

        //! HSL格式
        static bs_color rgb_to_hsl (const bs_color &rgb)
        {
            bs_color hsl;
            color_type_t max, min, delta;

            max = rgb_max (rgb.r, rgb.g, rgb.b);
            min = rgb_min (rgb.r, rgb.g, rgb.b);

            hsl.l = (max + min) / 2.0;

            if (max == min)
            {
                hsl.s = 0.0;
                hsl.h = HSL_UNDEFINED;
            }
            else
            {
                if (hsl.l <= 0.5)
                    hsl.s = (max - min) / (max + min);
                else
                    hsl.s = (max - min) / (2.0 - max - min);

                delta = max - min;

                if (delta == 0.0)
                    delta = 1.0;

                if (rgb.r == max)
                    hsl.h = (rgb.g - rgb.b) / delta;
                else if (rgb.g == max)
                    hsl.h = 2.0 + (rgb.b - rgb.r) / delta;
                else
                    hsl.h = 4.0 + (rgb.r - rgb.g) / delta;

                hsl.h /= 6.0;

                if (hsl.h < 0.0)
                    hsl.h += 1.0;
            }

            hsl.a = rgb.a;
            return hsl;
        }
        static bs_color hsl_to_rgb (const bs_color &hsl)
        {
            bs_color rgb;
            struct CLocal
            {
                static inline color_type_t hsl_value (color_type_t n1, color_type_t n2, color_type_t hue)
                {
                    color_type_t val;

                    if (hue > 6.0)
                        hue -= 6.0;
                    else if (hue < 0.0)
                        hue += 6.0;

                    if (hue < 1.0)
                        val = n1 + (n2 - n1) * hue;
                    else if (hue < 3.0)
                        val = n2;
                    else if (hue < 4.0)
                        val = n1 + (n2 - n1) * (4.0 - hue);
                    else
                        val = n1;

                    return val;
                }
            };
            if (hsl.s == 0)
            {
                rgb.r = hsl.l;
                rgb.g = hsl.l;
                rgb.b = hsl.l;
            }
            else
            {
                color_type_t m1, m2;

                if (hsl.l <= 0.5)
                    m2 = hsl.l * (1.0 + hsl.s);
                else
                    m2 = hsl.l + hsl.s - hsl.l * hsl.s;

                m1 = 2.0 * hsl.l - m2;

                rgb.r = CLocal::hsl_value (m1, m2, hsl.h * 6.0 + 2.0);
                rgb.g = CLocal::hsl_value (m1, m2, hsl.h * 6.0);
                rgb.b = CLocal::hsl_value (m1, m2, hsl.h * 6.0 - 2.0);
            }

            rgb.a = hsl.a;
            return rgb;
        }

        //! CMYK格式
        //! \brief rgb_to_cmyk
        //! \param rgb
        //! \param pullout (0-1) 0使rgb转换到CMY，1使rgb黑色转换到最大量
        //! \return
        //!
        static bs_color rgb_to_cmyk (const bs_color &rgb, color_type_t pullout)
        {
            bs_color cmyk;
            color_type_t c, m, y, k;

            c = 1.0 - rgb.r;
            m = 1.0 - rgb.g;
            y = 1.0 - rgb.b;

            k = 1.0;
            if (c < k)  k = c;
            if (m < k)  k = m;
            if (y < k)  k = y;

            k *= pullout;

            if (k < 1.0)
            {
                cmyk.c = (c - k) / (1.0 - k);
                cmyk.m = (m - k) / (1.0 - k);
                cmyk.y = (y - k) / (1.0 - k);
            }
            else
            {
                cmyk.c = 0.0;
                cmyk.m = 0.0;
                cmyk.y = 0.0;
            }

            cmyk.k = k;
            //cmyk.a = rgb.a;
            return cmyk;
        }
        static bs_color cmyk_to_rgb (const bs_color &cmyk)
        {
            bs_color rgb;
            color_type_t c, m, y, k;
            k = cmyk.k;

            if (k < 1.0)
            {
                c = cmyk.c * (1.0 - k) + k;
                m = cmyk.m * (1.0 - k) + k;
                y = cmyk.y * (1.0 - k) + k;
            }
            else
            {
                c = 1.0;
                m = 1.0;
                y = 1.0;
            }

            rgb.r = 1.0 - c;
            rgb.g = 1.0 - m;
            rgb.b = 1.0 - y;
            //rgb.a = cmyk.a;
            return rgb;
        }

        //! HWB格式
        //! \brief rgb_to_hwb
        //! 当w == 1 - b，h是不确定的。
        //! \param rgb
        //! \return
        //!
        static bs_color rgb_to_hwb (const bs_color &rgb)
        {
            bs_color hwb;
            color_type_t R = rgb.r, G = rgb.g, B = rgb.b, w, v, b, f;
            int i;

            w = rgb_min (rgb.r, rgb.g, rgb.b);
            v = rgb_max (rgb.r, rgb.g, rgb.b);
            b = 1.0 - v;

            if (v == w)
            {
                hwb.h = HSV_UNDEFINED;
                hwb.w = w;
                hwb.b = b;
            }
            else
            {
                f = (R == w) ? G - B : ((G == w) ? B - R : R - G);
                i = (R == w) ? 3.0 : ((G == w) ? 5.0 : 1.0);

                hwb.h = (360.0 / 6.0) * (i - f / (v - w));
                hwb.w = w;
                hwb.b = b;
            }
            return hwb;
        }
        static bs_color hwb_to_rgb (const bs_color &hwb)
        {
#define RETURN_RGB(x, y, z) { rgb.r = x; rgb.g = y; rgb.b = z; }
            bs_color rgb;
            color_type_t h = hwb.h, w = hwb.w, b = hwb.b, v, n, f;
            int    i;

            h = 6.0 * h/ 360.0;

            v = 1.0 - b;
            if (h == HSV_UNDEFINED)
            {
                rgb.r = v;
                rgb.g = v;
                rgb.b = v;
            }
            else
            {
                i = bs_ifloor (h);
                f = h - i;

                if (i & 1)
                    f = 1.0 - f;  /* if i is odd */

                n = w + f * (v - w);     /* linear interpolation between w and v */

                switch (i)
                {
                case 6:
                case 0: RETURN_RGB (v, n, w);
                    break;
                case 1: RETURN_RGB (n, v, w);
                    break;
                case 2: RETURN_RGB (w, v, n);
                    break;
                case 3: RETURN_RGB (w, n, v);
                    break;
                case 4: RETURN_RGB (n, w, v);
                    break;
                case 5: RETURN_RGB (v, w, n);
                    break;
                }
            }
#undef RETURN_RGB
            return rgb;
        }

        //! HEX格式
        static bs_color hex_to_rgba(bs_color32 hex)
        {
            bs_color rgba;
            rgba.r = (1.0 / 255.0)*((hex >> 24) & 0xff);
            rgba.g = (1.0 / 255.0)*((hex >> 16) & 0xff);
            rgba.b = (1.0 / 255.0)*((hex >> 8) & 0xff);
            rgba.a = (1.0 / 255.0)* (hex & 0xff);
            return rgba;
        }
        static bs_color32 rgba_to_hex(color_type_t R, color_type_t G, color_type_t B, color_type_t A)
        {
            return (((bs_color32)bs_clamp(R*255.0, 0.0, 255.0))<<24) |
                    (((bs_color32)bs_clamp(G*255.0, 0.0, 255.0))<<16) |
                    (((bs_color32)bs_clamp(B*255.0, 0.0, 255.0))<<8) |
                    ((bs_color32)bs_clamp(A*255.0, 0.0, 255.0));
        }
    };
    class IntColor
    {
    public:
        //! HSV格式
        //! \brief rgb_to_hsv
        //! \param red
        //! \param green
        //! \param blue
        //! \return
        //!
        static bs_color rgb_to_hsv (int red, int green, int blue)
        {
#define ROUND(x) ((int) ((x) + 0.5))
            bs_color hsv;
            color_type_t  r, g, b;
            color_type_t  h, s, v;
            int     mins;
            color_type_t  delta;

            r = red; g = green; b = blue;

            if (r > g)
            {
                v = std::max (r, b);
                mins = std::min (g, b);
            }
            else
            {
                v = std::max (g, b);
                mins = std::min (r, b);
            }

            delta = v - mins;

            if (v == 0.0)
                s = 0.0;
            else
                s = delta / v;

            if (s == 0.0)
            {
                h = 0.0;
            }
            else
            {
                if (r == v)
                    h = 60.0 * (g - b) / delta;
                else if (g == v)
                    h = 120 + 60.0 * (b - r) / delta;
                else
                    h = 240 + 60.0 * (r - g) / delta;

                if (h < 0.0)
                    h += 360.0;

                if (h > 360.0)
                    h -= 360.0;
            }

            hsv.h   = ROUND (h);
            hsv.s = ROUND (s * 255.0);
            hsv.v  = ROUND (v);

            if (hsv.h == 360)
                hsv.h = 0;
            #undef ROUND
            return hsv;
        }
        static bs_color hsv_to_rgb (int hue, int saturation, int value)
        {
            bs_color rgb;
#define ROUND(x) ((int) ((x) + 0.5))
            color_type_t h, s, v, h_temp;
            color_type_t f, p, q, t;
            int i;

            if (saturation == 0)
            {
                hue        = value;
                saturation = value;
                value      = value;
            }
            else
            {
                h = hue;
                s = saturation / 255.0;
                v = value      / 255.0;

                if (h == 360)
                    h_temp = 0;
                else
                    h_temp = h;

                h_temp = h_temp / 60.0;
                i = bs_ifloor (h_temp);
                f = h_temp - i;
                p = v * (1.0 - s);
                q = v * (1.0 - (s * f));
                t = v * (1.0 - (s * (1.0 - f)));

                switch (i)
                {
                case 0:
                    rgb.r = ROUND (v * 255.0);
                    rgb.g = ROUND (t * 255.0);
                    rgb.b = ROUND (p * 255.0);
                    break;

                case 1:
                    rgb.r = ROUND (q * 255.0);
                    rgb.g = ROUND (v * 255.0);
                    rgb.b = ROUND (p * 255.0);
                    break;

                case 2:
                    rgb.r = ROUND (p * 255.0);
                    rgb.g = ROUND (v * 255.0);
                    rgb.b = ROUND (t * 255.0);
                    break;

                case 3:
                    rgb.r = ROUND (p * 255.0);
                    rgb.g = ROUND (q * 255.0);
                    rgb.b = ROUND (v * 255.0);
                    break;

                case 4:
                    rgb.r = ROUND (t * 255.0);
                    rgb.g = ROUND (p * 255.0);
                    rgb.b = ROUND (v * 255.0);
                    break;

                case 5:
                    rgb.r = ROUND (v * 255.0);
                    rgb.g = ROUND (p * 255.0);
                    rgb.b = ROUND (q * 255.0);
                    break;
                }
            }
#undef ROUND
            return rgb;
        }

        //! HSL格式
        static bs_color rgb_to_hsl (int red, int green, int blue)
        {
            bs_color hsl;
#define ROUND(x) ((int) ((x) + 0.5))
            int    r, g, b;
            color_type_t h, s, l;
            int    mins, maxs;
            int    delta;

            r = red; g = green; b = blue;

            if (r > g)
            {
                maxs = std::max (r, b);
                mins = std::min (g, b);
            }
            else
            {
                maxs = std::max (g, b);
                mins = std::min (r, b);
            }

            l = (maxs + mins) / 2.0;

            if (maxs == mins)
            {
                s = 0.0;
                h = 0.0;
            }
            else
            {
                delta = (maxs - mins);

                if (l < 128)
                    s = 255 * (color_type_t) delta / (color_type_t) (maxs + mins);
                else
                    s = 255 * (color_type_t) delta / (color_type_t) (511 - maxs - mins);

                if (r == maxs)
                    h = (g - b) / (color_type_t) delta;
                else if (g == maxs)
                    h = 2 + (b - r) / (color_type_t) delta;
                else
                    h = 4 + (r - g) / (color_type_t) delta;

                h = h * 42.5;

                if (h < 0)
                    h += 255;
                else if (h > 255)
                    h -= 255;
            }

            hsl.h   = ROUND (h);
            hsl.s = ROUND (s);
            hsl.l  = ROUND (l);
#undef ROUND
            return hsl;
        }
        static bs_color hsl_to_rgb (int hue, int saturation, int lightness)
        {
            struct CLocal
            {
                static int hsl_value (color_type_t n1, color_type_t n2, color_type_t hue)
                {
#define ROUND(x) ((int) ((x) + 0.5))
                  color_type_t value;

                  if (hue > 255)
                    hue -= 255;
                  else if (hue < 0)
                    hue += 255;

                  if (hue < 42.5)
                    value = n1 + (n2 - n1) * (hue / 42.5);
                  else if (hue < 127.5)
                    value = n2;
                  else if (hue < 170)
                    value = n1 + (n2 - n1) * ((170 - hue) / 42.5);
                  else
                    value = n1;

                  return ROUND (value * 255.0);
#undef ROUND
                }
            };
            bs_color rgb;
            color_type_t h, s, l;

            h = hue; s = saturation; l = lightness;

            if (s == 0)
            {
                rgb.r = l;
                rgb.g = l;
                rgb.b = l;
            }
            else
            {
                color_type_t m1, m2;

                if (l < 128)
                    m2 = (l * (255 + s)) / 65025.0;
                else
                    m2 = (l + s - (l * s) / 255.0) / 255.0;

                m1 = (l / 127.5) - m2;

                rgb.r = CLocal::hsl_value (m1, m2, h + 85);
                rgb.g = CLocal::hsl_value (m1, m2, h);
                rgb.b = CLocal::hsl_value (m1, m2, h - 85);
            }
            return rgb;
        }

        //! CMYK格式
        //! \brief rgb_to_cmyk
        //! \param red
        //! \param green
        //! \param blue
        //! \param pullout (0-100)
        //! \return
        //!
        static bs_color rgb_to_cmyk (int red, int green, int blue, int pullout)
        {
            bs_color cmyk;
            int c, m, y;

            c = 255 - red; m = 255 - green; y = 255 - blue;

            if (pullout == 0)
            {
                red   = c;
                green = m;
                blue  = y;
            }
            else
            {
                int k = 255;

                if (c < k)  k = c;
                if (m < k)  k = m;
                if (y < k)  k = y;

                k = (k * bs_clamp(pullout, 0, 100)) / 100;

                cmyk.c   = ((c - k) << 8) / (256 - k);
                cmyk.m = ((m - k) << 8) / (256 - k);
                cmyk.y  = ((y - k) << 8) / (256 - k);
                cmyk.k = k;
            }
            return cmyk;
        }
        static bs_color cmyk_to_rgb (int cyan, int magenta, int yellow, int black)
        {
            bs_color rgb;
            int c, m, y, k;

            c = cyan; m = magenta; y = yellow; k = black;

            if (k)
            {
                c = ((c * (256 - k)) >> 8) + k;
                m = ((m * (256 - k)) >> 8) + k;
                y = ((y * (256 - k)) >> 8) + k;
            }

            rgb.r = 255 - c;
            rgb.g = 255 - m;
            rgb.b = 255 - y;
            return rgb;
        }

        //! YUV格式
        static bs_color yuv_to_rgb(int Y, int U, int V)
        {
            bs_color rgb;

            int C = Y - 16;
            int D = U - 128;
            int E = V - 128;
            int R = (( 298 * C           + 409 * E + 128) >> 8);
            int G = (( 298 * C - 100 * D - 208 * E + 128) >> 8);
            int B = (( 298 * C + 516 * D           + 128) >> 8);

            //rgb.r = (int)((Y & 0xff) + 1.4075 * ((V & 0xff) -128));
            //rgb.g = (int)((Y & 0xff) - 0.3455 * ((U & 0xff) -128)
            //              - 0.7169 * ((V & 0xff) - 128));
            //rgb.b = (int)((Y & 0xff) + 1.779 * ((U & 0xff) -128));
            //rgb.r =(rgb.r < 0? 0: rgb.r >255? 255 : rgb.r);
            //rgb.g =(rgb.g < 0? 0: rgb.g >255? 255 : rgb.g);
            //rgb.b =(rgb.b < 0? 0: rgb.b >255? 255 : rgb.b);

            rgb.r =(R < 0? 0: R >255? 255 : R);
            rgb.g =(G < 0? 0: G >255? 255 : G);
            rgb.b =(B < 0? 0: B >255? 255 : B);
            return rgb;
        }
        static bs_color rgb_to_yuv(uchar R, uchar G, uchar B, uchar A)
        {
            bs_color out;
            out.y = ( (  66 * R + 129 * G +  25 * B + 128) >> 8) +  16;
            out.u = ( ( -38 * R -  74 * G + 112 * B + 128) >> 8) + 128;
            out.v = ( ( 112 * R -  94 * G -  18 * B + 128) >> 8) + 128;
            out.a = A;
            return out;
        }

        //! HEX格式
        static bs_color hex_to_rgba(bs_color32 hex)
        {
            bs_color rgba;
            rgba.r = ((hex >> 24) & 0xff);
            rgba.g = ((hex >> 16) & 0xff) ;
            rgba.b = ((hex >> 8) & 0xff);
            rgba.a = (hex & 0xff) ;
            return rgba;
        }
        static bs_color32 rgba_to_hex(uchar R, uchar G, uchar B, uchar A)
        {
            return (((bs_color32)bs_clamp(R, 0, 255))<<24) |
                    (((bs_color32)bs_clamp(G, 0, 255))<<16) |
                    (((bs_color32)bs_clamp(B, 0, 255))<<8) |
                    ((bs_color32)bs_clamp(A, 0, 255));
        }
    };
    //!双线性插值。
    class BilinearColor
    {
    public:
        color_type_t bilinear (color_type_t  x, color_type_t  y, color_type_t *values)
        {
          color_type_t m0, m1;

          x = std::fmod (x, 1.0);
          y = std::fmod (y, 1.0);

          if (x < 0.0)
            x += 1.0;
          if (y < 0.0)
            y += 1.0;

          m0 = (1.0 - x) * values[0] + x * values[1];
          m1 = (1.0 - x) * values[2] + x * values[3];

          return (1.0 - y) * m0 + y * m1;
        }
        uchar bilinear_8 (color_type_t x, color_type_t y, uchar *values)
        {
          color_type_t m0, m1;

          x = std::fmod (x, 1.0);
          y = std::fmod (y, 1.0);

          if (x < 0.0)
            x += 1.0;
          if (y < 0.0)
            y += 1.0;

          m0 = (1.0 - x) * values[0] + x * values[1];
          m1 = (1.0 - x) * values[2] + x * values[3];

          return (uchar) ((1.0 - y) * m0 + y * m1);
        }
        uint16 bilinear_16 (color_type_t  x, color_type_t  y, uint16 *values)
        {
          real m0, m1;

          x = std::fmod (x, 1.0);
          y = std::fmod (y, 1.0);

          if (x < 0.0)
            x += 1.0;
          if (y < 0.0)
            y += 1.0;

          m0 = (1.0 - x) * values[0] + x * values[1];
          m1 = (1.0 - x) * values[2] + x * values[3];

          return (uint16) ((1.0 - y) * m0 + y * m1);
        }
        uint32 bilinear_32 (color_type_t  x, color_type_t  y, uint32 *values)
        {
          real m0, m1;

          x = std::fmod (x, 1.0);
          y = std::fmod (y, 1.0);

          if (x < 0.0)
            x += 1.0;
          if (y < 0.0)
            y += 1.0;

          m0 = (1.0 - x) * values[0] + x * values[1];
          m1 = (1.0 - x) * values[2] + x * values[3];

          return (uint32) ((1.0 - y) * m0 + y * m1);
        }
        bs_color bilinear_rgb (color_type_t  x, color_type_t  y, bs_color *values)
        {
          color_type_t m0, m1;
          color_type_t ix, iy;
          bs_color v(0.0f);

          x = std::fmod(x, 1.0);
          y = std::fmod(y, 1.0);

          if (x < 0)
            x += 1.0;
          if (y < 0)
            y += 1.0;

          ix = 1.0 - x;
          iy = 1.0 - y;

          /* Red */

          m0 = ix * values[0].r + x * values[1].r;
          m1 = ix * values[2].r + x * values[3].r;

          v.r = iy * m0 + y * m1;

          /* Green */

          m0 = ix * values[0].g + x * values[1].g;
          m1 = ix * values[2].g + x * values[3].g;

          v.g = iy * m0 + y * m1;

          /* Blue */

          m0 = ix * values[0].b + x * values[1].b;
          m1 = ix * values[2].b + x * values[3].b;

          v.b = iy * m0 + y * m1;

          return v;
        }
        bs_color bilinear_rgba (color_type_t  x, color_type_t  y, bs_color *values)
        {
          color_type_t m0, m1;
          color_type_t ix, iy;
          color_type_t a0, a1, a2, a3, alpha;
          bs_color v(0.0f);

          x = std::fmod (x, 1.0);
          y = std::fmod (y, 1.0);

          if (x < 0)
            x += 1.0;
          if (y < 0)
            y += 1.0;

          ix = 1.0 - x;
          iy = 1.0 - y;

          a0 = values[0].a;
          a1 = values[1].a;
          a2 = values[2].a;
          a3 = values[3].a;

          /* Alpha */

          m0 = ix * a0 + x * a1;
          m1 = ix * a2 + x * a3;

          alpha = v.a = iy * m0 + y * m1;

          if (alpha > 0)
            {
              /* Red */

              m0 = ix * a0 * values[0].r + x * a1 * values[1].r;
              m1 = ix * a2 * values[2].r + x * a3 * values[3].r;

              v.r = (iy * m0 + y * m1)/alpha;

              /* Green */

              m0 = ix * a0 * values[0].g + x * a1 * values[1].g;
              m1 = ix * a2 * values[2].g + x * a3 * values[3].g;

              v.g = (iy * m0 + y * m1)/alpha;

              /* Blue */

              m0 = ix * a0 * values[0].b + x * a1 * values[1].b;
              m1 = ix * a2 * values[2].b + x * a3 * values[3].b;

              v.b = (iy * m0 + y * m1)/alpha;
            }

          return v;
        }

        /**
         * bilinear_pixels_8:
         * @dest: Pixel, where interpolation result is to be stored.
         * @x: x-coordinate (0.0 to 1.0).
         * @y: y-coordinate (0.0 to 1.0).
         * @bpp: Bytes per pixel.  @dest and each @values item is an array of
         *    @bpp bytes.
         * @has_alpha: %TRUE if the last channel is an alpha channel.
         * @values: Array of four pointers to pixels.
         *
         * Computes bilinear interpolation of four pixels.
         *
         * When @has_alpha is %FALSE, it's identical to bilinear_8() on
         * each channel separately.  When @has_alpha is %TRUE, it handles
         * alpha channel correctly.
         *
         * The pixels in @values correspond to corner x, y coordinates in the
         * following order: [0,0], [1,0], [0,1], [1,1].
         **/
        void bilinear_pixels_8 (uchar    *dest,
                                color_type_t    x,
                                color_type_t    y,
                                uint      bpp,
                                bool      has_alpha,
                                uchar    **values)
        {
            uint i;

            x = std::fmod (x, 1.0);
            y = std::fmod (y, 1.0);

            if (x < 0.0)
                x += 1.0;
            if (y < 0.0)
                y += 1.0;

            if (has_alpha)
            {
                uint   ai     = bpp - 1;
                color_type_t alpha0 = values[0][ai];
                color_type_t alpha1 = values[1][ai];
                color_type_t alpha2 = values[2][ai];
                color_type_t alpha3 = values[3][ai];
                color_type_t alpha  = ((1.0 - y) * ((1.0 - x) * alpha0 + x * alpha1)
                                 + y * ((1.0 - x) * alpha2 + x * alpha3));

                dest[ai] = (uchar) alpha;
                if (dest[ai])
                {
                    for (i = 0; i < ai; i++)
                    {
                        color_type_t m0 = ((1.0 - x) * values[0][i] * alpha0
                                + x * values[1][i] * alpha1);
                        color_type_t m1 = ((1.0 - x) * values[2][i] * alpha2
                                + x * values[3][i] * alpha3);

                        dest[i] = (uchar) (((1.0 - y) * m0 + y * m1) / alpha);
                    }
                }
            }
            else
            {
                for (i = 0; i < bpp; i++)
                {
                    color_type_t m0 = (1.0 - x) * values[0][i] + x * values[1][i];
                    color_type_t m1 = (1.0 - x) * values[2][i] + x * values[3][i];

                    dest[i] = (uchar) ((1.0 - y) * m0 + y * m1);
                }
            }
        }

    };

    //!
    //! \brief rgb_gamma 伽马校正
    //! \param rgb
    //! \param gamma
    //! \return
    //!
    static bs_color rgb_gamma (bs_color &rgb, color_type_t  gamma)
    {
      color_type_t ig;

      if (gamma != 0.0)
        ig = 1.0 / gamma;
      else
        ig = 0.0;

      rgb.r = std::pow (rgb.r, ig);
      rgb.g = std::pow (rgb.g, ig);
      rgb.b = std::pow (rgb.b, ig);
      return rgb;
    }
    //!
    //! \brief rgb_luminance 亮度值
    //! \param rgb
    //! \return
    //!
    static color_type_t rgb_luminance (const bs_color &rgb)
    {
#define RGB_LUMINANCE_RED    (0.2126)
#define RGB_LUMINANCE_GREEN  (0.7152)
#define RGB_LUMINANCE_BLUE   (0.0722)

#define RGB_LUMINANCE(r,g,b)((r) * RGB_LUMINANCE_RED   + \
                            (g) * RGB_LUMINANCE_GREEN + \
                            (b) * RGB_LUMINANCE_BLUE)

      color_type_t luminance = RGB_LUMINANCE (rgb.r, rgb.g, rgb.b);
#undef RGB_LUMINANCE_RED
#undef RGB_LUMINANCE_GREEN
#undef RGB_LUMINANCE_BLUE
#undef RGB_LUMINANCE
      return bs_clamp(luminance, (color_type_t)0.0, (color_type_t)1.0);
    }
    //!
    //! \brief rgb_intensity 强度值
    //! \param rgb
    //! \return
    //!
    static color_type_t rgb_intensity (const bs_color &rgb)
    {
#define RGB_INTENSITY_RED    (0.30)
#define RGB_INTENSITY_GREEN  (0.59)
#define RGB_INTENSITY_BLUE   (0.11)

#define RGB_INTENSITY(r,g,b) ((r) * RGB_INTENSITY_RED   + \
                                   (g) * RGB_INTENSITY_GREEN + \
                                   (b) * RGB_INTENSITY_BLUE)
      real intensity = RGB_INTENSITY (rgb.r, rgb.g, rgb.b);
#undef RGB_INTENSITY_RED
#undef RGB_INTENSITY_GREEN
#undef RGB_INTENSITY_BLUE
#undef RGB_INTENSITY
      return bs_clamp (intensity, 0.0, 1.0);
    }

    static bs_color rgb_composite (bs_color &color1,
                        const bs_color &color2,
                        eRGBCompositeModes  mode)
    {
        switch (mode)
        {
        case RGB_CompositeNone:
            break;

        case RGB_CompositeNormal:
            /*  put color2 on top of color1  */
            if (color2.a == 1.0)
            {
                color1 = color2;
            }
            else
            {
                color_type_t factor = color1.a * (1.0 - color2.a);

                color1.r = color1.r * factor + color2.r * color2.a;
                color1.g = color1.g * factor + color2.g * color2.a;
                color1.b = color1.b * factor + color2.b * color2.a;
                color1.a = factor + color2.a;
            }
            break;

        case RGB_CompositeBehind:
            /*  put color2 below color1  */
            if (color1.a < 1.0)
            {
                color_type_t factor = color2.a * (1.0 - color1.a);

                color1.r = color2.r * factor + color1.r * color1.a;
                color1.g = color2.g * factor + color1.g * color1.a;
                color1.b = color2.b * factor + color1.b * color1.a;
                color1.a = factor + color1.a;
            }
            break;
        }
        return color1;
    }
    static bs_color rgba_add (bs_color &rgba1, const bs_color &rgba2)
    {
        rgba1.r += rgba2.r;
        rgba1.g += rgba2.g;
        rgba1.b += rgba2.b;
        rgba1.a += rgba2.a;
        return rgba1;
    }
    static bs_color rgba_subtract (bs_color &rgba1, const bs_color &rgba2)
    {
        rgba1.r -= rgba2.r;
        rgba1.g -= rgba2.g;
        rgba1.b -= rgba2.b;
        rgba1.a -= rgba2.a;
        return rgba1;
    }
    static bs_color rgba_multiply (bs_color &rgba ,color_type_t  factor)
    {
      rgba.r *= factor;
      rgba.g *= factor;
      rgba.b *= factor;
      rgba.a *= factor;
      return rgba;
    }
    static color_type_t rgba_distance (const bs_color &rgba1, const bs_color &rgba2)
    {
      return (std::fabs (rgba1.r - rgba2.r) +
              std::fabs (rgba1.g - rgba2.g) +
              std::fabs (rgba1.b - rgba2.b) +
              std::fabs (rgba1.a - rgba2.a));
    }

private:
    static color_type_t rgb_max (color_type_t R, color_type_t G, color_type_t B)
    {
      if (R > G)
        return (R > B) ? R : B;
      else
        return (G > B) ? G : B;
    }
    static color_type_t rgb_min (color_type_t R, color_type_t G, color_type_t B)
    {
      if (R < G)
        return (R < B) ? R : B;
      else
        return (G < B) ? G : B;
    }
};

typedef bs_color color;

inline bs_bstream &operator << (bs_bstream &in, const bs_color &v)
{
    for (int i = 0;i < v.size; ++i)
        in << v.elem[i];
    return in;
}
inline bs_bstream &operator >> (bs_bstream &out, bs_color &v)
{
    for (int i = 0;i < v.size; ++i)
        out >> v.elem[i];
    return out;
}
inline bs_tstream &operator << (bs_tstream &in, const bs_color &v)
{
    for (int i = 0;i < v.size; ++i)
        in << v.elem[i];
    return in;
}
inline bs_tstream &operator >> (bs_tstream &out, bs_color &v)
{
    for (int i = 0;i < v.size; ++i)
        out >> v.elem[i];
    return out;
}

#endif // COLOR_H
