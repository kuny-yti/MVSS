#ifndef IOPENGL_CONTEXT
#define IOPENGL_CONTEXT
#include "bs_define.h"
#include "bs_ptr.h"
#include "tools/bs_string.h"
#include "tools/bs_image.h"

/*!
 * @brief The eRenderClients enum 渲染的客户端
 * @enum eRenderClients
 */
typedef enum {
    Render_Default    = 0x0,
    Render_OpenGL     = 0x1,
    Render_OpenGLES   = 0x2,
    Render_OpenVG     = 0x4
}eRenderClients;

/*!
 * @brief The eRenderTargets enum 渲染的目标
 * @enum eRenderTargets
 */
typedef enum
{
    Render_Image,     // 渲染到图像中     (离屏渲染的一种)
    Render_Window,    // 渲染到系统窗口中
    Render_PBuffer,   // 渲染到PBufffer (离屏渲染的一种)
    Render_Affinity,  // 渲染组合 (Nvidia的一种多GPU模式)
    Render_OffScreen, // FBO实现 (离屏渲染的一种)
}eRenderTargets;

/*!
 * @brief The eSwapMethods enum 交换方法
 * @enum eSwapMethods
 */
typedef enum
{
    Swap_DefaultBuffer,
    Swap_SingleBuffer,
    Swap_DoubleBuffer,
    Swap_TripleBuffer
}eSwapMethods;

/*!
 * @brief The eChannelBits enum 通道位数
 * @enum eChannelBits
 */
typedef enum
{
    Channel_BitNot = 0,
    Channel_Bit8 = 8,
    Channel_Bit16 = 16,
    Channel_Bit24 = 24,
    Channel_Bit32 = 32,

    Channel_RedMask = 0xFF,
    Channel_GreenMask = 0xFF << 8,
    Channel_BlueMask = 0xFF << 16,
    Channel_AlphaMask = 0xFF << 24,

    Channel_Red8 = Channel_Bit8,
    Channel_Green8 = Channel_Bit8 << 8,
    Channel_Blue8 = Channel_Bit8 << 16,
    Channel_Alpha8 = Channel_Bit8 << 24,
    Channel_RGB8 = Channel_Red8 | Channel_Green8 | Channel_Blue8,
    Channel_RGBA8 = Channel_Red8 | Channel_Green8 | Channel_Blue8| Channel_Alpha8,

    Channel_Red16 = Channel_Bit16,
    Channel_Green16 = Channel_Bit16 << 8,
    Channel_Blue16 = Channel_Bit16 << 16,
    Channel_Alpha16 = Channel_Bit16 << 24,
    Channel_RGB16 = Channel_Red16 | Channel_Green16 | Channel_Blue16,
    Channel_RGBA16 = Channel_Red16 | Channel_Green16 | Channel_Blue16| Channel_Alpha16,

    Channel_Red32 = Channel_Bit32,
    Channel_Green32 = Channel_Bit32 << 8,
    Channel_Blue32 = Channel_Bit32 << 16,
    Channel_Alpha32 = Channel_Bit32 << 24,
    Channel_RGB32 = Channel_Red32 | Channel_Green32 | Channel_Blue32,
    Channel_RGBA32 = Channel_Red32 | Channel_Green32 | Channel_Blue32| Channel_Alpha32,
}eChannelBits;
#define RedChannelBit(rf) (rf & 0xFF)
#define GreenChannelBit(rf) (rf >> 8 & 0xFF)
#define BlueChannelBit(rf) (rf >> 16& 0xFF)
#define AlphaChannelBit(rf) (rf >> 24& 0xFF)

/*!
 * @brief The tRenderFormats struct 渲染格式
 * @struct tRenderFormats
 */
typedef struct _RenderForame_
{
    eSwapMethods   swap;         /// 支持双缓冲
    eRenderClients client;
    eRenderTargets target;       /// 渲染目标
    ivec4          viewport;     ///
    eChannelBits   color_bits;
    eChannelBits   depth_bits;   /// 深度缓存位数
    eChannelBits   stencil_bits; /// 模板缓存位数
    eChannelBits   accum_bits;   /// 累积缓存位数

    uint samples:8;              /// 采样位数
    uint has_stereo:1;           /// 是否支持立体
    uint enable_stereo:1;        /// 开启立体支持
    uint has_alpha:1;            /// 支持alpha通道
    uint swap_sync:1;
    uint has_debug:1;

    _RenderForame_():
        swap(Swap_DoubleBuffer),
        client(Render_Default),
        target(Render_Window),
        viewport(0,0,0,0),
        color_bits(Channel_RGBA8),
        depth_bits(Channel_Bit24),
        stencil_bits(Channel_Bit8),
        accum_bits(Channel_BitNot),
        samples(16),
        has_stereo(0),
        enable_stereo(0),
        has_alpha(1),
        swap_sync(0),
        has_debug(1)
    {

    }

    bool is_valid()const
    {
        return viewport.width > 0 && viewport.height > 0;
    }
    bool operator !=(const _RenderForame_ &rhs)const
    {
        return !((*this) == rhs);
    }
    bool operator ==(const _RenderForame_ &rhs)const
    {
        return (rhs.client == client) &&
                (rhs.target == target) &&
                (rhs.depth_bits == depth_bits)&&
                (rhs.stencil_bits ==  stencil_bits )&&
                (rhs.color_bits == color_bits )&&
                (rhs.accum_bits ==  accum_bits)&&
                (rhs.samples ==  samples)&&
                (rhs.has_stereo ==  has_stereo)&&
                (rhs.enable_stereo ==  enable_stereo)&&
                (rhs.has_alpha ==  has_alpha)&&
                (rhs.swap ==  swap)&&
                (rhs.viewport == viewport) &&
                (rhs.has_debug == has_debug);
    }

    bs_list<_RenderForame_> sys_rf;
} tRenderFormats;

bsPackage iwcontext;
bsPackage ircontext;
#define iContext bs_ptr(ircontext)
typedef iContext pircontext;
#undef iContext

bsPackage ircontext
{
    iwcontext*     wcontext; ///< 当前绑定的窗口id
    pircontext     sharectx; ///< 当前共享的上下文
    ctxid          ctx;      ///< 当前的上下文
    cfgid          cfg;      ///< 当前上下文内部配置

    tRenderFormats format;   ///< 当前上下文的格式

    ircontext(const pircontext & ctxs = pircontext())
    {
        wcontext = NULL;
        ctx = NULL;
        cfg = NULL;
        sharectx = ctxs;
    }
    virtual ~ircontext(){}

    template <typename T>
    T context() {return (T)ctx;}
    template <typename T>
    T config() {return (T)cfg;}

    //!
    //! \brief share 返回当前共享的上下文
    //! \return
    //!
    pircontext share(){return sharectx;}
    //!
    //! \brief is_valid 上下文是否有效
    //! \return
    //!
    virtual bool is_valid()const {return ctx && wcontext;}

    //!
    //! \brief share 共享一个上下文
    //! \return
    //!
    virtual bool share(const pircontext &s){sharectx = s;return true;}

    //!
    //! \brief capture 从当前上下文截图
    //! \return
    //!
    virtual bs_image capture() const= 0;

    //!
    //! \brief create 根据窗口id创建一个上下文
    //! \param id
    //! \return
    //!
    virtual bool create(iwcontext *wctx, const tRenderFormats &rf)
    {wcontext = wctx; format = rf;return true;}
    //!
    //! \brief close 关闭上下文
    //! \param id
    //!
    virtual void close(){ctx = NULL;cfg = NULL;}

    typedef void ( *fDebugCall)(uint source, uint type, uint id, uint severity, int length, const char* message, const void*);
    virtual void debugcall(fDebugCall func) = 0;

    virtual void swap()= 0;
    virtual void make()= 0;
    virtual void done()= 0;
};


#endif // IOPENGL_CONTEXT

