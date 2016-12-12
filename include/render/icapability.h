#ifndef ICAPABILITY
#define ICAPABILITY

#include "bs_define.h"
#include "bs_ptr.h"
#include "tools/bs_string.h"

typedef enum
{
    Extension_Not,
    Extension_DebugOutput ,            /// 支持调试日志

    Extension_VertexArray ,            /// 支持顶点数组
    Extension_VertexArrayObject,       /// 支持顶点数组对象
    Extension_VertexBufferObject,      /// 支持顶点缓存对象
    Extension_FrameBufferObject,       /// 支持帧缓存对象
    Extension_PixelBufferObject,       /// 支持像素缓存对象
    Extension_ShaderObject ,           /// 支持着色器对象
    Extension_TextureBufferObject ,    /// 支持纹理缓存对象
    Extension_UniformBufferObject ,    /// 支持glsl块缓存对象
    Extension_FrameBuffersRGB ,        /// 帧缓存支持sRGB色彩空间
    Extension_FrameBufferBlit,         /// 帧缓存支持帧拷贝
    Extension_FrameBufferMultiSample , /// 帧缓存支持多重采样

    Extension_TimerQuery ,             /// 支持精准定时器查询
    Extension_OcclusionQuery ,         /// 支持遮挡查询
    Extension_MultiSample ,            /// 支持多重采样
    Extension_MultiTexture ,           /// 支持多重纹理

    Extension_FragmentProgram ,
    Extension_VertexProgram ,
    Extension_FragmentShader ,
    Extension_VertexShader ,
    Extension_GeometryShader4 ,
    Extension_ComputeShader,
    Extension_TessellationShader,

    Extension_TextureCompression,
    Extension_TextureRectangle,
    Extension_TextureFloating,
    Extension_TextureMultiSample ,
    Extension_TexturesRGB ,

    Extension_CopyBuffer,
    Extension_CopyImage,

    Extension_DepthTexture ,
    Extension_DrawBuffer ,

    Extension_WindowPos ,
    Extension_ABGR ,
    Extension_BGRA ,
    Extension_sRGB8,

    Extension_Count ,
}eCapabilityExtensions;

typedef enum
{
    Vendor_Not,
    Vendor_Amd,
    Vendor_Ati = Vendor_Amd,
    Vendor_Nvidia,
    Vendor_Intel,
    Vendor_Qualcomm,
    Vendor_Arm,
    Vendor_Mesa,
    Vendor_Count
}eOpenGLVendor;

typedef enum
{
  Package_Unknown,
  Package_Mesa
} eOpenGLPackages;

typedef enum
{
  Arch_Unknown,
  Arch_Sandybridge,
  Arch_Sgx,
  Arch_Mali,
  Arch_Llvmpipe,
  Arch_Softpipe,
  Arch_SWRast
} eGPUArchs;

bsPackage icapability
{
    virtual ~icapability(){}

    //! OpenGL 版本
    bs_string version;
    //! 供应商
    bs_string vendor;
    //! 渲染设备
    bs_string renderer;
    //! GlSL版本
    bs_string version_glsl;

    //! 供应商
    virtual eOpenGLVendor vendors()const = 0;
    //! 软件包
    virtual eOpenGLPackages package() const = 0;
    //! 体系架构
    virtual eGPUArchs arch()const = 0;
    virtual bool is_support(const bs_string &ext)const = 0;
    virtual bool is_support(eCapabilityExtensions ext)const = 0;
};

#define iCapability bs_ptr(icapability)
typedef iCapability picapability;

#endif // ICAPABILITY

