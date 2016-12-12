#include "wgl_context.h"

namespace impl {

static tRenderFormats toRF(const PIXELFORMATDESCRIPTOR &pfd)
{
    tRenderFormats rf;
    rf.accum_bits = pfd.cAccumBits;
    rf.has_alpha = (pfd.cAlphaBits) ? 1 : 0;
    rf.alpha_bits = pfd.cAlphaBits;
    rf.blue_bits = pfd.cBlueBits;
    rf.color_bits = pfd.cColorBits;
    rf.depth_bits = pfd.cDepthBits;
    rf.green_bits = pfd.cGreenBits;
    rf.has_double = (pfd.dwFlags & PFD_DOUBLEBUFFER) ? 1 : 0;
    rf.has_stereo = (pfd.dwFlags & PFD_STEREO) ? 1 : 0;
    rf.red_bits = pfd.cRedBits;
    rf.stencil_bits = pfd.cStencilBits;
    return rf;
}
static int findPixelFormat(HDC hdc, tRenderFormats &rf)
{
    int currMode, bestScore;                         // pixel format mode ID
    int bestMode = 0, best_count = 0, last_count = 0;//  best pixel format
    PIXELFORMATDESCRIPTOR pfd;

    // search the available formats for the best mode
    bestMode = 0; bestScore = 0;
    for(currMode = 1; ::DescribePixelFormat(hdc, currMode, sizeof(pfd), &pfd) > 0; ++currMode)
    {
        if (best_count > last_count)
        {
            last_count = best_count;
            bestScore = bestMode;
        }
        rf.sys_rf.append(toRF(pfd));

        if(!(pfd.dwFlags & PFD_SUPPORT_OPENGL))
            continue;
        if((pfd.iPixelType != PFD_TYPE_RGBA) || (pfd.dwFlags & PFD_NEED_PALETTE))
            continue;

        int rflag = PFD_DRAW_TO_WINDOW;
        if (rf.target == Render_Window)
            rflag = PFD_DRAW_TO_WINDOW;
        else if (rf.target == Render_Image)
            rflag = PFD_DRAW_TO_BITMAP;
        if(!(pfd.dwFlags & rflag))
            continue;

        if (rf.has_double && !(pfd.dwFlags & PFD_DOUBLEBUFFER))
            continue;
        if (rf.has_stereo && !(pfd.dwFlags & PFD_STEREO))
            continue;

        // dwFlags－>cColorBits－>cAlphaBits－>cAccumBits－>cDepthBits－>cStencilBits－>cAuxBuffers－>iLayerType
        if (pfd.cColorBits >= rf.color_bits)
        {
            bestMode = currMode;
            best_count++;
        }
        else continue;
        if (rf.has_alpha)
        {
            if (pfd.cAlphaBits >= rf.alpha_bits)
            {
                bestMode = currMode;
                best_count++;
            }
            else continue;
        }
        if (pfd.cAccumBits >= rf.accum_bits)
        {
            bestMode = currMode;
            best_count++;
        }
        else continue;

        if (pfd.cDepthBits >= rf.depth_bits)
        {
            bestMode = currMode;
            best_count++;
        }
        else continue;
        if (pfd.cStencilBits >= rf.stencil_bits)
        {
            bestMode = currMode;
            best_count++;
        }
        else continue;

        if (pfd.cRedBits >= rf.red_bits)
        {
            bestMode = currMode;
            best_count++;
        }
        else continue;
        if (pfd.cGreenBits >= rf.green_bits)
        {
            bestMode = currMode;
            best_count++;
        }
        else continue;
        if (pfd.cBlueBits >= rf.blue_bits)
        {
            bestMode = currMode;
            best_count++;
        }
        else continue;
    }

    return bestScore;
}

static bool setPixelFormat(HDC hdc, tRenderFormats &rf)
{
    int flags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
    if (rf.has_double)
        flags |= PFD_DOUBLEBUFFER;

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof (PIXELFORMATDESCRIPTOR),
        1,
        flags,                             //标志
        PFD_TYPE_RGBA,                     //颜色模式
        rf.color_bits,                     //颜色位数
        rf.red_bits,   0,
        rf.green_bits, 0,
        rf.blue_bits,  0,
        rf.alpha_bits, 0,
        rf.accum_bits,
        0, 0, 0, 0,
        rf.depth_bits,                    //深度位数
        rf.stencil_bits,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);//findPixelFormat(hdc, rf);
    if(pixelFormat == 0)
        return false;

    ::DescribePixelFormat(hdc, pixelFormat, sizeof(pfd), &pfd);

    rf.accum_bits = pfd.cAccumBits;
    rf.has_alpha = (pfd.cAlphaBits >= rf.alpha_bits && rf.alpha_bits > 0) ? 1 : 0;
    rf.alpha_bits = pfd.cAlphaBits;
    rf.blue_bits = pfd.cBlueBits;
    rf.color_bits = pfd.cColorBits;
    rf.depth_bits = pfd.cDepthBits;
    rf.green_bits = pfd.cGreenBits;
    rf.has_double = (pfd.dwFlags & PFD_DOUBLEBUFFER) ? 1 : 0;
    rf.has_stereo = (pfd.dwFlags & PFD_STEREO) ? 1 : 0;
    rf.red_bits = pfd.cRedBits;
    rf.stencil_bits = pfd.cStencilBits;

    // set the fixel format
    if(!(SetPixelFormat(hdc, pixelFormat, &pfd)))
        return false;

    return true;
}

static bs_array_byte getBits(HDC hdc, HBITMAP bitmap, int width, int height, bool topToBottom = true)
{
    BITMAPINFO bmi;
    memset(&bmi.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = width;
    bmi.bmiHeader.biHeight      = topToBottom ? -height : height;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage   = width * height * 4;
    memset(bmi.bmiColors, 0, sizeof(RGBQUAD));

    bs_array_byte result(bmi.bmiHeader.biSizeImage);
    if (!GetDIBits(hdc, bitmap, 0, height, result.data(), &bmi, DIB_RGB_COLORS))
    {
        result.clear();
        return bs_array_byte();
    }
    return result;
}

static bs_image from_image(HBITMAP hbmp)
{
    BITMAP bmp_info;
    memset(&bmp_info, 0, sizeof(BITMAP));

    const int ret = GetObject(hbmp, sizeof(BITMAP), &bmp_info);
    if (!ret)
        return bs_image();

    HDC dc = GetDC(NULL);

    bs_array_byte byte_bit = getBits(dc, hbmp, bmp_info.bmWidth, bmp_info.bmHeight);
    if (byte_bit.is_empty())
    {
        ReleaseDC(0, dc);
        return bs_image();
    }

    bs_image target(bmp_info.bmWidth, bmp_info.bmHeight, NULL, ImageData_RGBA);
    if (target.is_null())
        return bs_image();
    const uint mask = 0;
    const bs_size dim = target.size();
    const int bytesPerLine = dim.width() * int(sizeof(uint));

    for (int y = 0; y < dim.height(); ++y)
    {
        uint *dest = reinterpret_cast<uint *>(target.bits() + dim.width() * y);
        const uint *src = reinterpret_cast<const uint *>(byte_bit.data() + y * bytesPerLine);

        for (int x = 0; x < dim.width(); ++x)
        {
            const uint pixel = src[x];
            if ((pixel & 0xff000000) == 0 && (pixel & 0x00ffffff) != 0)
                dest[x] = pixel | 0xff000000;
            else
                dest[x] = pixel | mask;
        }
    }

    ReleaseDC(0, dc);
    return target;
}

static bs_image from_image(HICON icon)
{
    bool foundAlpha = false;
    HDC dc = GetDC(0);
    HDC hdc = CreateCompatibleDC(dc);
    ReleaseDC(0, dc);

    ICONINFO iconinfo;
    if (!GetIconInfo(icon, &iconinfo))
        return bs_image();

    const int width = iconinfo.xHotspot * 2;
    const int height = iconinfo.yHotspot * 2;

    BITMAPINFOHEADER bmpinfo;
    memset(&bmpinfo, 0, sizeof(BITMAPINFOHEADER));
    bmpinfo.biSize        = sizeof(BITMAPINFOHEADER);
    bmpinfo.biWidth       = width;
    bmpinfo.biHeight      = height;
    bmpinfo.biPlanes      = 1;
    bmpinfo.biBitCount    = 32;
    bmpinfo.biCompression = BI_RGB;
    bmpinfo.biSizeImage   = width * height * 4;

    DWORD* bits = NULL;

    HBITMAP bmp = CreateDIBSection(hdc, (BITMAPINFO*)&bmpinfo, DIB_RGB_COLORS, (VOID**)&bits, NULL, 0);
    HGDIOBJ oldhdc = (HBITMAP)SelectObject(hdc, bmp);
    DrawIconEx(hdc, 0, 0, icon, iconinfo.xHotspot * 2, iconinfo.yHotspot * 2, 0, 0, DI_NORMAL);

    bs_image image(width, height, NULL, ImageData_RGBA);
    bs_array_byte byte_bit = getBits(hdc, bmp, width, height);

    if (byte_bit.is_empty() || image.is_null())
        return bs_image();

    const int bytesPerLine = width * image.depth();
    for (int y = 0; y < height; ++y)
    {
        void *dest = image.bits() + y * bytesPerLine;
        void *src = byte_bit.data() + y * bytesPerLine;
        bs_memory::copy_sse(dest, src, bytesPerLine);
    }

    for (int y = 0 ; y < height && !foundAlpha ; y++)
    {
        uint *lined= (uint*)(image.bits() + y*bytesPerLine);
        for (int x = 0; x < width ; x++)
        {
            if ((lined[x]>> 24) != 0)
            {
                foundAlpha = true;
                break;
            }
        }
    }
    if (!foundAlpha)
    {
        DrawIconEx( hdc, 0, 0, icon, width, height, 0, 0, DI_MASK);
        bs_image mask(width, height, NULL, ImageData_RGBA);
        bs_array_byte byte_bit = getBits(hdc, bmp, width, height);

        if (byte_bit.is_empty() || mask.is_null())
            return bs_image();

        const int bytesPerLine = width * mask.depth();
        for (int y = 0; y < height; ++y)
        {
            void *dest = mask.bits() + y * bytesPerLine;
            const void *src = byte_bit.data() + y * bytesPerLine;
            bs_memory::copy_sse(dest, src, bytesPerLine);
        }

        for (int y = 0 ; y < height ; y++)
        {
            uint *linimg = (uint*)(image.bits() + y*bytesPerLine);
            uint *linemk = mask.is_null() ? 0 :
                        (uint*)(mask.bits() + y*width * mask.depth());
            for (int x = 0; x < width ; x++)
            {
                if (linemk && (linemk[x] >> 16) != 0)
                    linimg[x] = 0; //mask out this pixel
                else
                    linimg[x] |= 0xff000000; // set the alpha channel to 255
            }
        }
    }
    //dispose resources created by iconinfo call
    DeleteObject(iconinfo.hbmMask);
    DeleteObject(iconinfo.hbmColor);

    SelectObject(hdc, oldhdc); //restore state
    DeleteObject(bmp);
    DeleteDC(hdc);
    return image;
}

class context
{
public:
    context(const tRenderFormats &rf):format(rf),
        share_ctx(),
        hdc(NULL),
        hglrc(NULL),
        oldhdc(NULL),
        oldglrc(NULL),
        id(NULL)
    {}
    virtual ~context(){}

    virtual bool create(HDC = NULL){return false;}
    virtual bool is_valid()const{return hdc && hglrc;}
    virtual void close(){}
    virtual void swap(bool vsync = true){
        if (vsync)
            SwapBuffers(hdc);
        else
           glFinish(); // 发送命令后等待GPU处理完成后返回
           //glFlush(); // 发送命令后立即返回,不等待GPU是否处理完成
    }
    virtual void make(){
        oldhdc  = wglGetCurrentDC();
        oldglrc = wglGetCurrentContext();
        wglMakeCurrent(hdc, hglrc);
    }
    virtual void done(){
        wglMakeCurrent(oldhdc, oldglrc);
    }

    virtual bs_image image()const{return bs_image();}

    bool is_nv(HDC dc)
    {
        const char* ext = wglGetExtensionsStringARB(dc);
        std::string extension(ext);
        if(extension.find("WGL_NV_gpu_affinity") != std::string::npos)
            return true;
        return false;
    }
    bool is_amd(HDC dc)
    {
        const char* ext = wglGetExtensionsStringARB(dc);
        std::string extension(ext);
        if(extension.find("WGL_AMD_gpu_association") != std::string::npos)
            return true;
        return false;
    }

    tRenderFormats format;
    prd_context share_ctx;

    HDC     hdc;        // handle to device context
    HGLRC   hglrc;      // handle to OpenGL rendering context
    HDC     oldhdc ;
    HGLRC   oldglrc;

    winid   id;

protected:
    HBITMAP ReadPixels(int x, int y, int width, int height)const
    {
        void *pdata = NULL;

        /* Note that this values only makes sense if we know a target
         * output size like if we put the image to paper. */
        const int physical_resolution = 2835; /* 72 DPI */

        BITMAPINFOHEADER bmih = {
            /* .biSize          = */ sizeof(bmih),
            /* .biWidth         = */ width,
            /* .bi.Height       = */ height,
            /* .biPlanes        = */ 1,                   /* mandatory */
            /* .biBitCount      = */ 24,                  /* 8 bits per pixel */
            /* .biCompression   = */ BI_RGB,              /* uncompressed */
            /* .biSizeImage     = */ 0,                   /* implicit */
            /* .biXPelsPerMeter = */ physical_resolution, /* ignored */
            /* .biYPelsPerMeter = */ physical_resolution, /* ignored */
            /* .biClrUsed       = */ 0,                   /* no palette */
            /* .biClrImportant  = */ 0
        };

        HBITMAP hbm = CreateDIBSection(
            hdc, /* may be different than the DC used for OpenGL */
            (PBITMAPINFO)&bmih, /* can do this cast, because no palette is used */
            DIB_RGB_COLORS,
            &pdata,
            NULL,
            0
        );

        if( !hbm )
            return NULL;

        glPixelStorei(GL_PACK_SWAP_BYTES,   GL_FALSE);
        glPixelStorei(GL_PACK_LSB_FIRST,    GL_TRUE);
        glPixelStorei(GL_PACK_ROW_LENGTH,   0);
        glPixelStorei(GL_PACK_IMAGE_HEIGHT, 0);
        glPixelStorei(GL_PACK_SKIP_PIXELS,  0);
        glPixelStorei(GL_PACK_SKIP_ROWS,    0);
        glPixelStorei(GL_PACK_ALIGNMENT,    1);

        if( glGetError() != GL_NO_ERROR ) {
            DeleteObject(hbm);
            return NULL;
        }

        glReadPixels(x, y, width, height, GL_BGR, GL_UNSIGNED_BYTE, pdata);

        if( glGetError() != GL_NO_ERROR ) {
            DeleteObject(hbm);
            return NULL;
        }

        return hbm;
    }

};

class render_image :public context
{
public:
    DWORD *pbits; // pointer to bitmap bits
    HBITMAP hbmp;
    HGDIOBJ gdiobj;

public:
    render_image(const tRenderFormats &rf):
        context(rf)
    {
        pbits = NULL;
        hbmp = NULL;
        gdiobj = NULL;
        hdc = NULL;
        hglrc = NULL;
        oldhdc = NULL;
        oldglrc = NULL;
    }
    ~render_image()
    {
        close();
    }

    bool create(HDC dc = NULL)
    {
        // Create a memory DC compatible with the screen
        hdc = CreateCompatibleDC(dc);
        if (!hdc)
            return false;

        BITMAPINFO bmi =
        {
            { sizeof(BITMAPINFOHEADER),
              format.size.width(),
              format.size.height(),
              1,
              format.color_bits,
              BI_RGB, 0, 0, 0, 0, 0 },
            { 0 }
        };

        hbmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void **) &pbits, 0, 0);

        // Select the bitmap into the DC
        gdiobj = SelectObject(hdc, hbmp);
        if (!gdiobj)
            return false;

        if (!setPixelFormat(hdc, format))
            return false;

        hglrc = wglCreateContext(hdc);
        if (hglrc == 0)
            return false;
        return true;
    }
    void close()
    {
        if (!is_valid())
            return ;

        // delete DC and RC
        done();
        if (hglrc)
            wglDeleteContext(hglrc);
        if (gdiobj)
            SelectObject(hdc, gdiobj);
        if (hbmp)
            DeleteObject(hbmp);
        if (hdc)
            DeleteDC(hdc);

        hdc = 0;
        hglrc = 0;
        gdiobj = 0;
        hbmp = 0;
    }

    void make()
    {
        context::make();
        GdiFlush();
    }

    bs_image image()const
    {
        return from_image(hbmp);
    }
};

class render_window :public context
{
public:
    render_window(const tRenderFormats &rf):
        context(rf)
    {
        hdc = NULL;
        hglrc = NULL;
        oldhdc = NULL;
        oldglrc = NULL;
    }
    ~render_window()
    {
        close();
    }

    bool create(HDC dc = NULL)
    {
        if (is_valid())
            return true;

        // retrieve a handle to a display device context
        hdc = GetDC((HWND)dc);
        id = (winid)dc;
        if (!id)
            return false;

        // set pixel format
        if(!impl::setPixelFormat(hdc, format))
        {
            log_error("Cannot set a suitable pixel format.");
            ::ReleaseDC((HWND)id, hdc);  // remove device context
            hdc = 0;
            return false;
        }

        // create a new OpenGL rendering context
        hglrc = wglCreateContext(hdc);

        oldhdc  = wglGetCurrentDC();
        oldglrc = wglGetCurrentContext();

        if (::wglMakeCurrent(hdc, hglrc))
            ::wglMakeCurrent(oldhdc, oldglrc);
        else
            log_error("OpenGL rendering context make error.");

        ::ReleaseDC((HWND)id, hdc);
        return true;
    }

    void close()
    {
        if (is_valid())
        {
            // delete DC and RC
            wglDeleteContext(hglrc);
            ::ReleaseDC((HWND)id, hdc);

            hdc = 0;
            hglrc = 0;
        }
    }

    bs_image image()const
    {
        HBITMAP hbmp = ReadPixels(0, 0, format.size.width(), format.size.height());
        return from_image(hbmp);
    }

};

class render_pbuffer:public context
{
public:
    HPBUFFERARB pbuffer;
public:
    render_pbuffer(const tRenderFormats &rf):
        context(rf)
    {
        hdc = NULL;
        hglrc = NULL;
        pbuffer = NULL;
    }
    ~render_pbuffer()
    {

    }
    bool create(HDC dc = NULL)
    {
        if(!WGLEW_ARB_pbuffer)
            return false;

        int intAttrs[32] ={WGL_RED_BITS_ARB,       format.red_bits,
                           WGL_GREEN_BITS_ARB,     format.green_bits,
                           WGL_BLUE_BITS_ARB,      format.blue_bits,
                           WGL_ALPHA_BITS_ARB,     format.alpha_bits,
                           WGL_DEPTH_BITS_ARB,     format.depth_bits,
                           WGL_STENCIL_BITS_ARB,   format.stencil_bits,
                           WGL_SAMPLE_BUFFERS_ARB, format.samples ? GL_TRUE : GL_FALSE,
                           WGL_SAMPLES_ARB,        format.samples,

                           WGL_DRAW_TO_PBUFFER_ARB, GL_TRUE,
                           WGL_BIND_TO_TEXTURE_RGBA_ARB, GL_TRUE,
                           WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                           WGL_PBUFFER_LARGEST_ARB, GL_TRUE,

                           WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
                           WGL_DOUBLE_BUFFER_ARB, format.has_double ? GL_TRUE : GL_FALSE,
                           0}; // 0 terminate the list

        int pfid  = 0;
        uint numFormats = 0;
        if (wglChoosePixelFormatARB(dc, intAttrs, NULL, 1, &pfid, &numFormats) == FALSE)
            return false;
        if (!numFormats)
            return false;

        const int attributes[]= {WGL_TEXTURE_FORMAT_ARB, WGL_TEXTURE_RGBA_ARB,
                                 WGL_TEXTURE_TARGET_ARB, WGL_TEXTURE_2D_ARB,
                                 WGL_MIPMAP_TEXTURE_ARB, TRUE,
                                 WGL_PBUFFER_LARGEST_ARB,FALSE}; // Of texture target will be GL_TEXTURE_2D

        if(dc && pfid)
            pbuffer = wglCreatePbufferARB(dc, pfid, format.size.width(), format.size.height(), attributes);
        else
            return false;

        if(!pbuffer)
            return false;

        // Create pbuffer context
        hdc = wglGetPbufferDCARB(pbuffer);
        hglrc = wglCreateContext(hdc);

        if (!hdc && !hglrc)
        {
            wglDestroyPbufferARB(pbuffer);
            return false;
        }

        // query pixel viewport of PBuffer
        int w,h;
        wglQueryPbufferARB(pbuffer, WGL_PBUFFER_WIDTH_ARB, &w);
        wglQueryPbufferARB(pbuffer, WGL_PBUFFER_HEIGHT_ARB, &h);

        if (bs_size(w, h) != format.size)
        {
            close();
            return false;
        }

        return true;
    }

    void close()
    {
        if (is_valid())
        {
            // Clean up all resource
            wglDeleteContext(hglrc);
            wglReleasePbufferDCARB(pbuffer, hdc);
            wglDestroyPbufferARB(pbuffer);
            hglrc = NULL;
            hdc = NULL;
            pbuffer = NULL;
        }
    }

    void make()
    {
        wglReleaseTexImageARB(pbuffer, WGL_FRONT_LEFT_ARB);
        context::make();
    }
    void done()
    {
        context::done();
        wglBindTexImageARB(pbuffer, WGL_FRONT_LEFT_ARB);
    }

    bs_image image()const
    {
        //HBITMAP hbmp = ReadPixels(0, 0, size.width(), size.height());
        //return from_image(hbmp);
        return bs_image();
    }

};

class render_offscreen :public context
{
public:
    render_offscreen(const tRenderFormats &rf):
        context(rf)
    {
    }
    ~render_offscreen(){}
};
class render_affinity :public context
{
public:
    bool is_primary;// 是否为主显示
    bool is_attached;// 是否连接到桌面

public:
    render_affinity(const tRenderFormats &rf):
        context(rf)
    {
    }
    ~render_affinity(){}
    //! dc转换为int则为设备id号，也为端口号
    //!
    virtual bool create(HDC dc = NULL){
        HGPUNV GpuMask[2] = {0};
        int *tmport = (int *)dc;
        if(!wglEnumGpusNV(*tmport, &GpuMask[0]))
            return false;

        uint ddi = 0;
        is_primary = false; // 是否为主显示
        is_attached = false; // 是否连接到桌面
        GPU_DEVICE gpu_dev;
        if(wglEnumGpuDevicesNV(GpuMask[0], ddi, &gpu_dev))
        {
            is_primary |= (gpu_dev.Flags & DISPLAY_DEVICE_PRIMARY_DEVICE) != 0;
            if(gpu_dev.Flags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)
                is_attached = true;
        }
        hdc = wglCreateAffinityDCNV(GpuMask);
        if(!hdc)
            return false;

        setPixelFormat(hdc, format);
        hglrc = wglCreateContext(hdc);
        if(hglrc == 0)
            return false;

        return true;
    }

    virtual void close(){
        if (is_valid())
        {
            wglDeleteContext(hglrc);
            wglDeleteDCNV(hdc);
            hglrc = NULL;
            hdc = NULL;
        }
    }

    virtual bs_image image()const{
        return bs_image();
    }

};

}

wgl_context::wgl_context()
{
    impl = NULL;
}

wgl_context::~wgl_context()
{
    if (is_valid())
    {
         close();
        bsDelete (impl);
    }
}

void wgl_context::close()
{
    if (is_valid())
        impl->close();
}

bool wgl_context::create(iwcontext *ctx, const tRenderFormats &rf)
{
    if (rf.is_valid() && rf.target != Render_Window)
        return false;
    if (rf.target == Render_Window)
    {
        impl = bsNew (impl::render_window(rf));
        impl->id = handle;
        return impl->create((HDC)handle);
    }
    else if (rf.target == Render_Image)
    {
        impl = bsNew (impl::render_image(rf));
        impl->id = handle;
        HDC dc = GetDC((HWND)handle);
        return impl->create(dc);
    }
    else if (rf.target == Render_PBuffer)
    {
        impl = bsNew (impl::render_pbuffer(rf));
        impl->id = handle;
        HDC dc = GetDC((HWND)handle);
        return impl->create(dc);
    }
    else if (rf.target == Render_OffScreen)
    {
        impl = bsNew (impl::render_offscreen(rf));
        impl->id = handle;
        HDC dc = GetDC((HWND)handle);
        return impl->create(dc);
    }
    else if (rf.target == Render_Affinity)
    {
        impl = bsNew (impl::render_affinity(rf));
        impl->id = handle;
        return impl->create((HDC)handle);
    }
    return false;
}
bool wgl_context::is_valid()const
{
    return impl && impl->is_valid();
}

tRenderFormats wgl_context::format()const
{
    return impl->format;
}

bs_image wgl_context::image()const
{
    return impl->image();
}

bool wgl_context::share(const pircontext &s)
{
    if (!s->is_valid())
        return false;

    wglShareLists(((wgl_context*)s.get())->impl->hglrc, this->impl->hglrc);
    impl->share_ctx = (s);
    return true;
}

void wgl_context::swap()
{
    if (is_valid())
        impl->swap(impl->format.target != Render_Image? vsync : false);
}

void wgl_context::make()
{
    if (is_valid())
        impl->make();
}

void wgl_context::done()
{
    if (is_valid())
        impl->done();
}
