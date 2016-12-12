#ifndef IMAGE
#define IMAGE
#include "bs_define.h"
#include "bs_string.h"
#include "maths/bs_color.h"
#include "maths/bs_vec4.h"

enum ImageProcessorDevice
{
    ImageProcessor_OpenCL,
    ImageProcessor_CPU,
    ImageProcessor_GPU,
    ImageProcessor_OpenGL
};
enum ImageReadFormat
{
    ImageRead_NOT,
    ImageRead_TGA,
    ImageRead_PNG,
    ImageRead_BMP,
    ImageRead_JPEG,
    ImageRead_PSD,
    ImageRead_HDR,
    ImageRead_PIC,
    ImageRead_GIF,
    ImageRead_PNM
};
enum ImageWirteFormat
{
    ImageWirte_NOT,
    ImageWirte_TGA,
    ImageWirte_PNG,
    ImageWirte_BMP,
    ImageWirte_HDR
};
enum ImageDataFormat
{
    ImageData_ARGB,
    ImageData_ABGR,
    ImageData_RGBA,
    ImageData_BGRA,
    ImageData_BGR,
    ImageData_RGB,
    ImageData_Grey,
    ImageData_HSL,
    ImageData_HSV,
    ImageData_CMYK,
    ImageData_YUV420,
    ImageData_YUV422,
    ImageData_YUV411,
    ImageData_YUV444
};

namespace impl {
class imageio;
}
class bs_image
{
public:
    bs_image();
    bs_image(const bs_image &img);
    explicit bs_image(const bs_string &file);
    //!
    //! \brief ui_image 将指定的尺寸图像赋值
    //! \param width
    //! \param height
    //! \param data
    //! \param forms 指定data的图像格式
    //!
    bs_image(int width, int height, uchar *data,
             ImageDataFormat forms );

    bs_image(uchar *data, size_t dlen, ImageReadFormat rforms );
    //!
    //! \brief form 将指定的尺寸图像赋值
    //! \param s
    //! \param data
    //! \param forms 指定data的图像格式
    //!
     bs_image(const isize &s, uchar *data,
                      ImageDataFormat forms ) ;

    ~bs_image();

    //!
    //! \brief format 返回图像数据的格式
    //! \return
    //!
     ImageDataFormat format() const ;
    //!
    //! \brief is_null 返回图像是否为空图像
    //! \return
    //!
     bool is_null() const ;
     bool is_empty() const;
    //!
    //! \brief size 返回图像的尺寸
    //! \return
    //!
     isize size() const ;
     uint byte() const;
    //!
    //! \brief depth 返回图像像素的字节数
    //! \return
    //!
     int depth() const;
    //!
    //! \brief bits 返回图像数据指针
    //! \return
    //!
     uchar* bits() ;
     const uchar* bits() const;
    //!
    //! \brief pixel 返回图像指定像素点颜色
    //! \param pos
    //! \return
    //!
     bs_color pixel(const ipoint &pos) const ;
     bs_color color(const ipoint &pos) const ;
    //!
    //! \brief pixel 修改图像指定像素点值
    //! \param pos 像素位置
    //! \param col 颜色值
    //!
     void pixel(const ipoint &pos, const bs_color &col) ;
     void color(const ipoint &pos, const bs_color &col) ;
    //!
    //! \brief has_alpha 图像是否支持透明通道
    //! \return
    //!
     bool has_alpha() const ;

    //!
    //! \brief resize 重新申请内部数据的空间
    //! \param width
    //! \param height
    //! \param depth 像素字节数
    //!
     void resize(int width, int height, int depth = 4)  ;
     void resize(const isize &s, int depth = 4)  ;


    //!
    //! \brief form 将指定的尺寸图像生成iimage格式并返回
    //! \param width
    //! \param height
    //! \param data
    //! \param src_forms 指定data的图像格式
    //! \return
    //!
     static bs_image form(int width, int height, uchar *data,
                                   ImageDataFormat src_forms) ;
    //!
    //! \brief form 将指定的尺寸图像生成iimage格式并返回
    //! \param s
    //! \param data
    //! \param src_forms 指定data的图像格式
    //! \return
    //!
     static bs_image form(const isize &s, uchar *data,
                                   ImageDataFormat src_forms) ;
/*
    //!
    //! \brief convert 转换image到form指定的格式并返回
    //! \param image
    //! \param form 转换后的格式
    //! \param dev 转换时使用的设备
    //! \return
    //!
     iimage convert(const iimage &image, ImageDataFormat form,
                           ImageProcessorDevice dev= ImageProcessor_CPU) const ;
    //!
    //! \brief convert 转换对象到form指定的格式并返回
    //! \param form
    //! \param dev 转换时使用的设备
    //! \return
    //!
     iimage convert(ImageDataFormat form,
                           ImageProcessorDevice dev= ImageProcessor_CPU) const ;
    //!
    //! \brief convert 将对象的图像转换到form指定的格式
    //! \param form
    //! \param dev 转换时使用的设备
    //!
     void convert(ImageDataFormat form,
                         ImageProcessorDevice dev= ImageProcessor_CPU) ;

    //!
    //! \brief scaled 将image图像按照宽高进行缩放并返回
    //! \param image
    //! \param width
    //! \param height
    //! \param dev 转换时使用的设备
    //! \return
    //!
     iimage scaled(const iimage &image, int width, int height,
                          ImageProcessorDevice dev= ImageProcessor_CPU) const;
    //!
    //! \brief scaled 将image图像按照宽高进行缩放并返回
    //! \param image
    //! \param s
    //! \param dev 转换时使用的设备
    //! \return
    //!
     iimage scaled(const iimage &image, const Size &s,
                          ImageProcessorDevice dev= ImageProcessor_CPU) const;
    //!
    //! \brief scaled 将对象图像按照宽高进行缩放
    //! \param width
    //! \param height
    //! \param dev 转换时使用的设备
    //!
     void scaled(int width, int height,
                        ImageProcessorDevice dev= ImageProcessor_CPU) ;
     void scaled(const Size &s,
                        ImageProcessorDevice dev= ImageProcessor_CPU) ;

    //!
    //! \brief rotate  将image图像根据angle角度进行旋转并返回
    //! \param image
    //! \param angle
    //! \param dev 转换时使用的设备
    //! \return
    //!
     iimage rotate(const iimage &image, int angle,
                          ImageProcessorDevice dev= ImageProcessor_CPU) const;
    //!
    //! \brief rotate 将对象图像根据angle角度进行旋转
    //! \param angle
    //! \param dev 转换时使用的设备
    //!
     void rotate(int angle,
                        ImageProcessorDevice dev= ImageProcessor_CPU) ;

    //!
    //! \brief cutting  将对象图像按照指定范围进行裁剪
    //! \param x
    //! \param y
    //! \param width
    //! \param height
    //! \param dev 转换时使用的设备
    //!
     void cutting(int x, int y, int width, int height,
                         ImageProcessorDevice dev= ImageProcessor_CPU) ;
    //!
    //! \brief cutting 将对象图像按照ret指定范围进行裁剪
    //! \param ret
    //! \param dev 转换时使用的设备
    //!
     void cutting(const Rect &ret,
                         ImageProcessorDevice dev= ImageProcessor_CPU) ;
    //!
    //! \brief cutting 将image图像按照ret指定范围进行裁剪并返回
    //! \param image
    //! \param ret
    //! \param dev 转换时使用的设备
    //! \return
    //!
     iimage cutting(const iimage &image, const Rect &ret,
                           ImageProcessorDevice dev= ImageProcessor_CPU) const;
     iimage cutting(const iimage &image,
                           int x, int y, int width, int height,
                           ImageProcessorDevice dev= ImageProcessor_CPU) const;

    //!
    //! \brief blend  将file指定的文件和对象图像进行混合
    //! \param file
    //! \param dev 转换时使用的设备
    //!
     void blend(const bs_string &file,
                       ImageProcessorDevice dev= ImageProcessor_CPU) ;
    //!
    //! \brief blend 将image图像和对象图像进行混合
    //! \param image
    //! \param dev 转换时使用的设备
    //!
     void blend(const iimage &image,
                       ImageProcessorDevice dev= ImageProcessor_CPU) ;
    //!
    //! \brief blend 将src_file1文件src_file2图像进行混合后保存到dst_file中
    //! \param src_file1
    //! \param src_file2
    //! \param dst_file
    //! \param dev 转换时使用的设备
    //! \return true 成功，否则失败
    //!
     bool blend(const bs_string &src_file1, const bs_string &src_file2,
                       const bs_string &dst_file,
                       ImageProcessorDevice dev= ImageProcessor_CPU) ;
    //!
    //! \brief blend 将image_1图像和image_2进行混合并返回
    //! \param image_1
    //! \param image_2
    //! \param dev 转换时使用的设备
    //! \return
    //!
     iimage blend(const iimage &image_1, const iimage &image_2,
                         ImageProcessorDevice dev= ImageProcessor_CPU) const;

    //! 将图像进行调色
*/
    //!
    //! \brief load 加载图像文件
    //! \param name
    //! \param form 指定图像的格式
    //! \return true 加载成功，否则失败
    //!
     bool load(const bs_string &name,
               ImageReadFormat form = ImageRead_NOT) ;
     bool load(uchar *data, size_t dlen, ImageReadFormat form  = ImageRead_PNG) ;
    //!
    //! \brief save 将当前数据按照form格式保存到name文件中，
    //! \param name
    //! \param form
    //!
     bool save(const bs_string &name,
                      ImageWirteFormat form = ImageWirte_NOT) ;
     bool save(uchar **data, size_t *dlen, ImageWirteFormat form ) ;

    //!
    //! \brief fill 将数据按照pixel指定像素进行填充
    //! \param pixel
    //!
     void fill(const bs_color &pixel) ;
    //!
    //! \brief fill 将数据按照val指定值进行填充
    //! \param val
    //!
     void fill(const char val) ;

     bool operator ==(const bs_image &rhs);
     bool operator !=(const bs_image &rhs);

     bs_image &operator =(const bs_image &rhs);

     int ref()const;
private:
    int return_depth(ImageDataFormat format);

    void lessref();
    void addref();

    impl::imageio *impl;
};

#endif // IMAGE

