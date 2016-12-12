
#include "bs_image.h"
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

namespace impl {
class imageio
{
public:
    uchar* _data;
    isize _size;
    int _depth;
    ImageDataFormat _format;
    bool _is_null;
    bool _is_empty;
    bool _is_alpha;
    bs_atomic<int> *refcount;
    imageio():
        _data(NULL),
        _size(),
        _depth(4),
        _format(ImageData_RGBA),
        _is_null(true),
        _is_empty(true),
        _is_alpha(true),
        refcount(NULL)
    {
        refcount = bsNew (bs_atomic<int>(1));
    }

};
}
void bs_image::lessref()
{
    if (impl && impl->refcount && (--(*impl->refcount) <= 0))
    {
        if (impl->_data)
            stbi_image_free(impl->_data);
        impl->_data = NULL;
        bsDelete (impl->refcount);
        impl->refcount = NULL;
        bsDelete (impl);
        impl = NULL;
    }
}
int bs_image::ref()const
{
    return *impl->refcount;
}
void bs_image::addref()
{
    if (impl && impl->refcount)
        ++(*impl->refcount);
}
bs_image::bs_image()
{
    impl = bsNew (impl::imageio());
}
int bs_image::return_depth(ImageDataFormat format)
{
    switch ((int)format)
    {
    case ImageData_ARGB:
    case ImageData_ABGR:
    case ImageData_RGBA:
    case ImageData_BGRA:
    case ImageData_CMYK:
        impl->_is_alpha = true;
        return 4;
    case ImageData_BGR:
    case ImageData_RGB:
    case ImageData_HSL:
    case ImageData_HSV:
        impl->_is_alpha = false;
        return 3;
    case ImageData_YUV420:
    case ImageData_YUV422:
    case ImageData_YUV411:
    case ImageData_YUV444:
        impl->_is_alpha = true;
        return 2;
    case ImageData_Grey:
        impl->_is_alpha = false;
        return 1;
    }
    return 4;
}

bs_image::bs_image(const bs_image &img)
{
    impl = img.impl ;
    addref();
}
bs_image::bs_image(int width, int height, uchar *data, ImageDataFormat forms )
{
    impl = bsNew (impl::imageio());
    impl->_format = forms;
    impl->_depth = return_depth(forms);
    impl->_size = isize(width, height);
    resize(width, height, impl->_depth);
    impl->_is_null = false;
    if (data != NULL)
    {
        bs_memory::copy_mmx(impl->_data, data, width * height *impl->_depth);
        impl->_is_empty = false;
    }
}
bs_image::bs_image(uchar *data, size_t dlen, ImageReadFormat rforms )
{
    impl = bsNew (impl::imageio());
    load (data, dlen, rforms);
}

bs_image::bs_image(const isize &s, uchar *data, ImageDataFormat forms )
{
    impl = bsNew (impl::imageio());
    impl->_format = forms;
    impl->_size = s;
    impl->_depth = return_depth(forms);
    resize(s.width, s.height, impl->_depth);
    impl->_is_null = false;
    if (data != NULL)
    {
        bs_memory::copy_mmx(impl->_data, data, s.width * s.height *impl->_depth);
        impl->_is_empty = false;
    }
}
bs_image::bs_image(const bs_string &file)
{
    impl = bsNew (impl::imageio());
    load(file);
}
bs_image::~bs_image()
{
    lessref();
}
ImageDataFormat bs_image::format() const
{
    return impl->_format;
}
bool bs_image::is_null() const
{
    return impl->_is_null;
}
bool bs_image::is_empty() const
{
    return impl->_is_empty;
}
isize bs_image::size() const
{
    return impl->_size;
}
uint bs_image::byte() const
{
    return impl->_depth * size().width *size().height;
}
int bs_image::depth() const
{
    return impl->_depth;
}

uchar* bs_image::bits()
{
    return impl->_data;
}
const uchar* bs_image::bits() const
{
    return impl->_data;
}

bs_color bs_image::pixel(const ipoint &pos) const
{
    if (!impl->_is_null)
        return bs_color();
    uchar *temp = impl->_data +((pos.y*impl->_size.width*impl->_depth) + ((pos.x)*impl->_depth));

    return bs_color(temp[0],
                impl->_depth >= 2?temp[1]:uchar(0),
                impl->_depth >= 3?temp[2]:uchar(0),
                impl->_depth >= 4?temp[3]:uchar(0));
}
bs_color bs_image::color(const ipoint &pos) const
{
    return pixel(pos);
}

void bs_image::pixel(const ipoint &pos, const bs_color &col)
{
    if (!impl->_is_null)
        return ;
    uchar *temp = impl->_data +((pos.y*impl->_size.width*impl->_depth) + ((pos.x)*impl->_depth));
    temp[0] = col.r;
    if (impl->_depth >= 2)
        temp[1] = col.g;
    if (impl->_depth >= 3)
        temp[2] = col.b;
    if (impl->_depth >= 4)
        temp[3] = col.a;
}
void bs_image::color(const ipoint &pos, const bs_color &col)
{
    pixel(pos, col);
}
bool bs_image::has_alpha() const
{
    return impl->_is_alpha;
}

void bs_image::resize(int width, int height, int depth )
{
    lessref();

    impl = bsNew (impl::imageio());
    impl->_size = isize(width, height);
    impl->_data = (uchar *)bsMalloc(width * height *depth);
    memset(impl->_data, 0 , width * height *depth);
    impl->_depth = depth;
    impl->_is_null = false;
    impl->_is_alpha = false;
    impl->_is_empty = true;
}
void bs_image::resize(const isize &s, int depth)
{
    resize(s.width, s.height, depth);
}
bs_image bs_image::form(int width, int height, uchar *data,
                               ImageDataFormat src_forms)
{
    return bs_image(width, height, data, src_forms);
}
bs_image bs_image::form(const isize &s, uchar *data,
                               ImageDataFormat src_forms)
{
    return bs_image(s, data, src_forms);
}
bool bs_image::load(uchar *data, size_t dlen, ImageReadFormat form )
{
    int width = 0, height = 0;
    lessref();

    impl = bsNew (impl::imageio());

    if (((int)form > ImageRead_NOT) && ((int)form <= ImageRead_PNM ))
    {
        impl->_data = stbi_load_from_memory (data, dlen,&width, &height, &impl->_depth, 4);
    }
    else
        return false;

    impl->_size = isize(width, height);
    impl->_is_alpha = true;
    if (impl->_data)
    {
        impl->_is_null = false;
        impl->_is_empty = false;
    }

    return impl->_data;
}
bool bs_image::load(const bs_string &name, ImageReadFormat form)
{
    int width = 0, height = 0;
    lessref();
    impl = bsNew (impl::imageio());

    stbi_set_unpremultiply_on_load(1);
    stbi_convert_iphone_png_to_rgb(1);

    if (form == ImageRead_NOT)
    {
        impl->_data = stbi_load((char*)name.to_utf8().data(), &width, &height, &impl->_depth, 4);
    }
    else if (((int)form > ImageRead_NOT) && ((int)form <= ImageRead_PNM ))
    {
        impl->_data = stbi_load((char*)name.to_utf8().data(), &width, &height, &impl->_depth, 4);
    }
    else
    {
        return false;
    }

    impl->_size = isize(width, height);
    impl->_is_alpha = true;
    if (impl->_data)
    {
        impl->_is_null = false;
        impl->_is_empty = false;
    }

    return impl->_data;
}

bool bs_image::save(uchar **data, size_t *dlen, ImageWirteFormat form )
{
    if (is_null() && !data && !dlen)
        return false;
    switch ((int)form)
    {
    case ImageWirte_TGA :
        break;
    case ImageWirte_PNG :
        *data = stbi_write_png_to_mem(impl->_data, size().width*impl->_depth,
                                      size().width, size().height,impl-> _depth, (int*)dlen);
        return true;
    case ImageWirte_BMP :
        break;
    case ImageWirte_HDR :
        break;
    default:
        break;
    }
    return false;
}
bool bs_image::save(const bs_string &name, ImageWirteFormat form )
{
    int ret = 0;
    if (is_null())
        return false;
    switch ((int)form)
    {
    case ImageWirte_TGA :
        ret = stbi_write_tga((char*)name.to_utf8().data(), size().width, size().height,
                             impl->_depth, impl->_data);
        break;
    case ImageWirte_PNG :
         ret = stbi_write_png((char*)name.to_utf8().data(), size().width, size().height,
                              impl->_depth, impl->_data, size().width*impl->_depth);
        break;
    case ImageWirte_BMP :
         ret = stbi_write_bmp((char*)name.to_utf8().data(), size().width, size().height,
                              impl->_depth, impl->_data);
        break;
    case ImageWirte_HDR :
    {
        float * dat = stbi__ldr_to_hdr(impl->_data, size().width, size().height, impl->_depth);
        ret = stbi_write_hdr((char*)name.to_utf8().data(), size().width, size().height,
                             impl->_depth, dat);
        if (dat)
            stbi_image_free(dat);
    }
    break;
    default:
        break;
    }
    return ret;
}

void bs_image::fill(const bs_color &pixel)
{
    if (is_null())
        return ;
    memset(impl->_data, 0 , impl->_size.width * impl->_size.height *impl->_depth);
    for (int i = 0; i < impl->_size.height; i++)
    {
        for (int j = 0; j < impl->_size.width*impl->_depth; j+=impl->_depth)
        {
            impl->_data[(i*impl->_size.width*impl->_depth)+j] = pixel.r;
            if (impl->_depth >= 2)
                impl->_data[(i*impl->_size.width*impl->_depth)+j+1] = pixel.g;
            if (impl->_depth >= 3)
                impl->_data[(i*impl->_size.width*impl->_depth)+j+2] = pixel.b;
            if (impl->_depth >= 4)
                impl->_data[(i*impl->_size.width*impl->_depth)+j+3] = pixel.a;
        }
    }
}
void bs_image::fill(const char val)
{
    if (is_null())
        return ;
    memset(impl->_data, val, impl->_size.width * impl->_size.height *impl->_depth);
}
bool bs_image::operator ==(const bs_image &rhs)
{
    return rhs.impl == impl;
}
bool bs_image::operator !=(const bs_image &rhs)
{
    return !((*this) == rhs);
}

bs_image &bs_image::operator =(const bs_image &rhs)
{
    lessref();

    impl = rhs.impl;
    addref();
    return *this;
}
class yuv2rgb
{
    //I420是yuv420格式，是3个plane，排列方式为(Y)(U)(V)
    static int* I420ToRGB(uchar* src, int width, int height)
    {
        int numOfPixel = width * height;
        int positionOfV = numOfPixel;
        int positionOfU = numOfPixel/4 + numOfPixel;
        int *rgb = new int[numOfPixel*3];

        for(int i = 0; i < height; i++)
        {
            int startY = i*width;
            int step = (i/2)*(width/2);
            int startU = positionOfV + step;
            int startV = positionOfU + step;
            for(int j = 0; j < width; j++)
            {
                int Y = startY + j;
                int U = startU + j/2;
                int V = startV + j/2;
                int index = Y*3;
                RGB tmp = yuv_to_rgb(src[Y], src[U], src[V]);
                rgb[index+R] = tmp.r;
                rgb[index+G] = tmp.g;
                rgb[index+B] = tmp.b;
            }
        }

        return rgb;
    }

    //YV16是yuv422格式，是三个plane，(Y)(U)(V)
    static int* YV16ToRGB(uchar* src, int width, int height)
    {
        int numOfPixel = width * height;
        int positionOfU = numOfPixel;
        int positionOfV = numOfPixel/2 + numOfPixel;
        int *rgb = new int[numOfPixel*3];

        for(int i = 0; i < height; i++)
        {
            int startY = i*width;
            int step = i*width/2;
            int startU = positionOfU + step;
            int startV = positionOfV + step;
            for(int j = 0; j < width; j++)
            {
                int Y = startY + j;
                int U = startU + j/2;
                int V = startV + j/2;
                int index = Y*3;
                //rgb[index+R] = (int)((src[Y]&0xff) + 1.4075 * ((src[V]&0xff)-128));
                //rgb[index+G] = (int)((src[Y]&0xff) - 0.3455 * ((src[U]&0xff)-128) - 0.7169*((src[V]&0xff)-128));
                //rgb[index+B] = (int)((src[Y]&0xff) + 1.779 * ((src[U]&0xff)-128));
                RGB tmp = yuv_to_rgb(src[Y], src[U], src[V]);
                rgb[index+R] = tmp.r;
                rgb[index+G] = tmp.g;
                rgb[index+B] = tmp.b;
            }
        }
        return rgb;
    }

    //YV12是yuv420格式，是3个plane，排列方式为(Y)(V)(U)
    static int* YV12ToRGB(uchar* src, int width, int height)
    {
        int numOfPixel = width * height;
        int positionOfV = numOfPixel;
        int positionOfU = numOfPixel/4 + numOfPixel;
        int *rgb = new int[numOfPixel*3];

        for(int i = 0; i < height; i++)
        {
            int startY = i*width;
            int step = (i/2)*(width/2);
            int startV = positionOfV + step;
            int startU = positionOfU + step;
            for(int j = 0; j < width; j++)
            {
                int Y = startY + j;
                int V = startV + j/2;
                int U = startU + j/2;
                int index = Y*3;

                //rgb[index+R] = (int)((src[Y]&0xff) + 1.4075 * ((src[V]&0xff)-128));
                //rgb[index+G] = (int)((src[Y]&0xff) - 0.3455 * ((src[U]&0xff)-128) - 0.7169*((src[V]&0xff)-128));
                //rgb[index+B] = (int)((src[Y]&0xff) + 1.779 * ((src[U]&0xff)-128));
                RGB tmp = yuv_to_rgb(src[Y], src[U], src[V]);
                rgb[index+R] = tmp.r;
                rgb[index+G] = tmp.g;
                rgb[index+B] = tmp.b;
            }
        }
        return rgb;
    }

    //YUY2是YUV422格式，排列是(YUYV)，是1 plane
    static int* YUY2ToRGB(uchar* src, int width, int height)
    {
        int numOfPixel = width * height;
        int *rgb = new int[numOfPixel*3];
        int lineWidth = 2*width;

        for(int i = 0; i < height; i++)
        {
            int startY = i*lineWidth;
            for(int j = 0; j < lineWidth; j+=4)
            {
                int Y1 = j + startY;
                int Y2 = Y1+2;
                int U = Y1+1;
                int V = Y1+3;
                int index = (Y1>>1)*3;
                RGB tmp = yuv_to_rgb(src[Y1], src[U], src[V]);
                rgb[index+R] = tmp.r;
                rgb[index+G] = tmp.g;
                rgb[index+B] = tmp.b;
                index += 3;
                tmp = yuv_to_rgb(src[Y2], src[U], src[V]);
                rgb[index+R] = tmp.r;
                rgb[index+G] = tmp.g;
                rgb[index+B] = tmp.b;
            }
        }
        return rgb;
    }

    //UYVY是YUV422格式，排列是(UYVY)，是1 plane
    static int* UYVYToRGB(uchar* src, int width, int height)
    {
        int numOfPixel = width * height;
        int *rgb = new int[numOfPixel*3];
        int lineWidth = 2*width;

        for(int i = 0; i < height; i++)
        {
            int startU = i*lineWidth;
            for(int j = 0; j < lineWidth; j += 4)
            {
                int U = j + startU;
                int Y1 = U+1;
                int Y2 = U+3;
                int V = U+2;
                int index = (U>>1)*3;
                RGB tmp = yuv_to_rgb(src[Y1], src[U], src[V]);
                rgb[index+R] = tmp.r;
                rgb[index+G] = tmp.g;
                rgb[index+B] = tmp.b;
                index += 3;
                tmp = yuv_to_rgb(src[Y2], src[U], src[V]);
                rgb[index+R] = tmp.r;
                rgb[index+G] = tmp.g;
                rgb[index+B] = tmp.b;
            }
        }
        return rgb;
    }

    //NV21是YUV420格式，排列是(Y), (VU)，是2 plane
    static int* NV21ToRGB(uchar* src, int width, int height)
    {
        int numOfPixel = width * height;
        int positionOfV = numOfPixel;
        int *rgb = new int[numOfPixel*3];

        for(int i = 0; i < height; i++)
        {
            int startY = i*width;
            int step = i/2*width;
            int startV = positionOfV + step;
            for(int j = 0; j < width; j++)
            {
                int Y = startY + j;
                int V = startV + j/2;
                int U = V + 1;
                int index = Y*3;
                RGB tmp = yuv_to_rgb(src[Y], src[U], src[V]);
                rgb[index+R] = tmp.r;
                rgb[index+G] = tmp.g;
                rgb[index+B] = tmp.b;
            }
        }
        return rgb;
    }

    //NV12是YUV420格式，排列是(Y), (UV)，是2 plane
    static int* NV12ToRGB(uchar* src, int width, int height)
    {
        int numOfPixel = width * height;
        int positionOfU = numOfPixel;
        int *rgb = new int[numOfPixel*3];

        for(int i=0; i<height; i++)
        {
            int startY = i*width;
            int step = i/2*width;
            int startU = positionOfU + step;
            for(int j = 0; j < width; j++)
            {
                int Y = startY + j;
                int U = startU + j/2;
                int V = U + 1;
                int index = Y*3;
                RGB tmp = yuv_to_rgb(src[Y], src[U], src[V]);
                rgb[index+R] = tmp.r;
                rgb[index+G] = tmp.g;
                rgb[index+B] = tmp.b;
            }
        }
        return rgb;
    }

    //NV16是YUV422格式，排列是(Y), (UV)，是2 plane
    static int *NV16ToRGB(uchar* src, int width, int height)
    {
        int numOfPixel = width * height;
        int positionOfU = numOfPixel;
        int *rgb = new int[numOfPixel*3];

        for(int i=0; i<height; i++)
        {
            int startY = i*width;
            int step = i*width;
            int startU = positionOfU + step;
            for(int j = 0; j < width; j++)
            {
                int Y = startY + j;
                int U = startU + j/2;
                int V = U + 1;
                int index = Y*3;
                RGB tmp = yuv_to_rgb(src[Y], src[U], src[V]);
                rgb[index+R] = tmp.r;
                rgb[index+G] = tmp.g;
                rgb[index+B] = tmp.b;
            }
        }
        return rgb;
    }

    //NV61是YUV422格式，排列是(Y), (VU)，是2 plane
    static int *NV61ToRGB(uchar* src, int width, int height){
        int numOfPixel = width * height;
        int positionOfV = numOfPixel;
        int *rgb = new int[numOfPixel*3];

        for(int i =0; i < height; i++)
        {
            int startY = i*width;
            int step = i*width;
            int startV = positionOfV + step;
            for(int j = 0; j < width; j++)
            {
                int Y = startY + j;
                int V = startV + j/2;
                int U = V + 1;
                int index = Y*3;
                RGB tmp = yuv_to_rgb(src[Y], src[U], src[V]);
                rgb[index+R] = tmp.r;
                rgb[index+G] = tmp.g;
                rgb[index+B] = tmp.b;
            }
        }
        return rgb;
    }

    //YVYU是YUV422格式，排列是(YVYU)，是1 plane
    static int *YVYUToRGB(uchar* src, int width, int height)
    {
        int numOfPixel = width * height;
        int *rgb = new int[numOfPixel*3];
        int lineWidth = 2*width;
        for(int i = 0; i < height; i++)
        {
            int startY = i*lineWidth;
            for(int j = 0; j < lineWidth; j+=4)
            {
                int Y1 = j + startY;
                int Y2 = Y1+2;
                int V = Y1+1;
                int U = Y1+3;
                int index = (Y1>>1)*3;
                RGB tmp = yuv_to_rgb(src[Y1], src[U], src[V]);
                rgb[index+R] = tmp.r;
                rgb[index+G] = tmp.g;
                rgb[index+B] = tmp.b;
                index += 3;
                tmp = yuv_to_rgb(src[Y2], src[U], src[V]);
                rgb[index+R] = tmp.r;
                rgb[index+G] = tmp.g;
                rgb[index+B] = tmp.b;
            }
        }
        return rgb;
    }

    //VYUY是YUV422格式，排列是(VYUY)，是1 plane
    static int *VYUYToRGB(uchar* src, int width, int height)
    {
        int numOfPixel = width * height;
        int *rgb = new int[numOfPixel*3];
        int lineWidth = 2*width;
        for(int i =0; i < height; i++)
        {
            int startV = i*lineWidth;
            for(int j = 0; j < lineWidth; j+=4)
            {
                int V = j + startV;
                int Y1 = V+1;
                int Y2 = V+3;
                int U = V+2;
                int index = (U>>1)*3;
                RGB tmp = yuv_to_rgb(src[Y1], src[U], src[V]);
                rgb[index+R] = tmp.r;
                rgb[index+G] = tmp.g;
                rgb[index+B] = tmp.b;
                index += 3;
                tmp = yuv_to_rgb(src[Y2], src[U], src[V]);
                rgb[index+R] = tmp.r;
                rgb[index+G] = tmp.g;
                rgb[index+B] = tmp.b;
            }
        }
        return rgb;
    }

private:
    enum
    {
        R = 0,
        G = 1,
        B = 2
    };
    struct RGB
    {
        int r, g, b;
    };
    static RGB yuv_to_rgb(uchar Y, uchar U, uchar V)
    {
        RGB rgb;
        rgb.r = (int)((Y&0xff) + 1.4075 * ((V&0xff)-128));
        rgb.g = (int)((Y&0xff) - 0.3455 * ((U&0xff)-128) - 0.7169*((V&0xff)-128));
        rgb.b = (int)((Y&0xff) + 1.779 * ((U&0xff)-128));
        rgb.r =(rgb.r<0? 0: rgb.r>255? 255 : rgb.r);
        rgb.g =(rgb.g<0? 0: rgb.g>255? 255 : rgb.g);
        rgb.b =(rgb.b<0? 0: rgb.b>255? 255 : rgb.b);
        return rgb;
    }

};
