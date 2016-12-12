#ifndef HARDWARE_DEFINE_H
#define HARDWARE_DEFINE_H
#include "bs_define.h"

typedef enum
{
    Attrib_Current = 1 << 1,        // 0x00000001
    Attrib_Point = 1 << 2,          // 0x00000002
    Attrib_Line = 1 << 3,           // 0x00000004
    Attrib_Polygon = 1 << 4,        // 0x00000008
    Attrib_PolygonStipple = 1 << 5,// 0x00000010
    Attrib_PixelMode = 1 << 6,     // 0x00000020
    Attrib_Lighting = 1 << 7,       // 0x00000040
    Attrib_Fog = 1 << 8,            // 0x00000080
    Attrib_DepthBuffer = 1 << 9,   // 0x00000100
    Attrib_AccumBuffer = 1 << 10,   // 0x00000200
    Attrib_StencilBuffer = 1 << 11, // 0x00000400
    Attrib_Viewport = 1 << 12,       // 0x00000800
    Attrib_Transform = 1 << 13,      // 0x00001000
    Attrib_Enable = 1 << 14,         // 0x00002000
    Attrib_ColorBuffer = 1 << 15,   // 0x00004000
    Attrib_Hint = 1 << 16,           // 0x00008000
    Attrib_Eval = 1 << 17,           // 0x00010000
    Attrib_List = 1 << 18,           // 0x00020000
    Attrib_Texture = 1 << 19,        // 0x00040000
    Attrib_Scissor = 1 << 20,        // 0x00080000
    Attrib_All = 1 << 21,            // 0x000fffff
}eAttribMasks;

// matrix modes
typedef enum
{
    Matrix_ModelView = 0,
    Matrix_Projection,
    Matrix_Texture,
    Matrix_Not
}eMatrixModes;

typedef enum
{
    Orthogonal_Viewport,
    Orthogonal_ViewportUpward,
    Orthogonal_One,
    Orthogonal_OneUpward,
}eOrthogonalModes;

typedef enum
{
    Face_Front =1 <<1,
    Face_Back =1 <<2,
    Face_Left =1 <<3,
    Face_Right =1 <<4
}ePolygonFaces;

typedef enum
{
    Polygon_Point,
    Polygon_Wireframe,
    Polygon_Fill
}ePolygonModes;

typedef enum
{
    Enable_Scissor,
    Enable_Alpha,
    Enable_Blend,
    Enable_Depth,
    Enable_Stencil,
    Enable_Cull,
    Enable_Fog,
    Enable_Antialiasing,
    Enable_Debug
}eEnableModes;

// depth or alpha modes
typedef enum
{
    Alpha_Never =0,
    Alpha_Less,
    Alpha_Equal ,
    Alpha_LEqual ,
    Alpha_Greater ,
    Alpha_NotEqual ,
    Alpha_GEqual ,
    Alpha_Always
}eAlphaFuncs;

typedef enum
{
    Blend_Zero =0,
    Blend_One =1,
    Blend_SrcColor ,
    Blend_OneMinusSrcColor ,
    Blend_SrcAlpha ,
    Blend_OneMinusSrcAlpha ,
    Blend_DstAlpha ,
    Blend_OneMinusDstAlpha
}eBlendFuncs;

// blending modes
typedef enum
{
    Blending_None = 0,
    Blending_Alpha,
    Blending_Add,
    Blending_Sub,
    Blending_Light,
    Blending_Product
}eBlendingModes;

// depth modes
typedef enum
{
    Depth_None = 0,
    Depth_Always,
    Depth_Less,
    Depth_Greater,
    Depth_Equal,
    Depth_LEqual,
    Depth_GEqual,
    Depth_NotEqual
}eDepthModes;

typedef enum
{
    Face_None = 0,
    Face_CCW = 1,
    Face_CW = 2
}eFaceModes;
// cull modes
typedef enum
{
    Cull_None = 0,
    Cull_Front,
    Cull_Back,
    Cull_FrontBack
}eCullModes;

// stencil funcs
typedef enum
{
    Stencil_Always = 0,
    Stencil_Never,
    Stencil_Equal,
    Stencil_NotEqual,
    Stencil_Less,
    Stencil_LEqual,
    Stencil_Greater,
    Stencil_GEqual
}eStencilFuncs;

// stencil ops
typedef enum
{
    Stencil_Keep = 0,
    Stencil_Invert,
    Stencil_Decr,
    Stencil_Incr,
    Stencil_Replace
}eStencilOps;

typedef enum
{
    Array_Vertex = 1<<0,
    Array_Color = 1<<1,
    Array_Index = 1<<2,
    Array_Normal=1<<3,
    Array_TextureCoord=1<<4,
    Array_Attrib=1<<5
}eArrayTypes;

typedef enum
{
    Attrib_Position = 0,
    Attrib_Color = 1,
    Attrib_Normal = 2,
    Attrib_TextureCoord = 3
}eAttribIndexs;

// primitives types
typedef enum
{
    Draw_None = 0,
    Draw_Points,
    Draw_Lines,
    Draw_LineLoop,
    Draw_LineStrip,
    Draw_Triangles,
    Draw_TriangleStrip,
    Draw_TriangleFan,
}eDrawModes;

typedef uint tIndicess;

// texture filtering modes
// wrap modes
typedef enum
{
    Filter_Nearest = 0,
    Filter_Linear,
    Filter_NearestMipmapNearest ,
    Filter_LinearMipmapNearest,
    Filter_NearestMipmapLinear,
    Filter_LinearMipmapLinear,
    Wrap_Clamp ,
    Wrap_Repeat,
    Wrap_ClampEdge,
    Wrap_ClampBorder
}eTextureParams;

// types
typedef enum
{
    Type_Bool = 0,
    Type_Byte,
    Type_UByte,
    Type_Short,
    Type_UShort,
    Type_Int,
    Type_UInt,
    Type_Float,
    Type_Double
}eDataTypes;

typedef enum
{
    Data_RGB = 0,
    Data_RGBA,
    Data_Luminance,
    Data_LuminanceAlpha,
    Data_BGR,
    Data_BGRA,
    Data_Alpha
}eDataFormats;

typedef enum
{
    Data_Byte = 0,
    Data_UByte,
    Data_Short,
    Data_UShort,
    Data_Int,
    Data_UInt,
    Data_Float,
    Data_2Bytes ,
    Data_3Bytes ,
    Data_4Bytes,
    Data_Double,
    Data_Uint8888Rev
}eDataDepths;

typedef enum
{
    Internal_Red = 0,
    Internal_Green,
    Internal_Blue,
    Internal_Alpha,
    Internal_RGB,
    Internal_RGBA,
    Internal_Luminance,
    Internal_Luminance8,
    Internal_LuminanceAlpha,
    Internal_RGB8,
    Internal_SRGB8,
    Internal_RGB16,
    Internal_RGBA8,
    Internal_Luminance16
}eInternalFormats;
// texture gen modes
typedef enum
{
    Genera_None = 0,
    Genera_SphereMap,
    Genera_CubeMap
}eTextureGeneras;

// texture combine modes
typedef enum
{
    Combine_Replace = 0,
    Combine_Modulate,
    Combine_Alpha,
    Combine_Add,
    Combine_Sub,
    Combine_Dot
}eTextureCombines;


typedef enum
{
    Shader_None,
    Shader_Vertex,
    Shader_Fragment,
    Shader_Geometry,
    Shader_TessControl,
    Shader_TessEvaluation,
    Shader_Compute
}eShaderTypes;

typedef enum
{
    Material_GLSL,
    Material_OpenGL
}eMaterialTypes;

typedef enum
{
    Light_NoTexture,
    Light_Texture
}eLightShaderTypes;

typedef enum
{
    Light_Area =1 << 0,
    Light_Direction = 1<<1,
    Light_Point = 1 << 2,
    Light_Spot = 1 << 3,
    Light_GLSL = 1 <<4,
    Light_OpenGL = 1 <<5,
    Light_PerPixelGLSL = 1 <<6,
}eLightTypes;

typedef enum
{
    Attach_Color0 = 0,
    Attach_Color1,
    Attach_Color2,
    Attach_Color3,
    Attach_Color4,
    Attach_Color5,
    Attach_Color6,
    Attach_Color7,
    Attach_Color8,
    Attach_Color9,
    Attach_Color10,
    Attach_Color11,
    Attach_Color12,
    Attach_Color13,
    Attach_Color14,
    Attach_Color15,
    Attach_Depth,
    Attach_Stencil,
}eFrameBufferAttachs;
// buffer types
typedef enum
{
    Buffer_Color = 2,
    Buffer_Depth = 4,
    Buffer_Stencil = 8
}eBufferTypes;

typedef enum
{
    Target_None = -1,
    Target_Array = 0,
    Target_ElementArray,
    Target_PixelUnPack,
    Target_PixelPack,
    Target_Uniform,
    Target_Count
}eTargetModes;

typedef enum
{
    Usage_StaticDraw,
    Usage_StaticRead,
    Usage_StaticCopy,
    Usage_DynamicDraw,
    Usage_DynamicRead,
    Usage_DynamicCopy,
    Usage_StreamDraw,
    Usage_StreamRead,
    Usage_StreamCopy
}eUsageModes;

typedef enum
{
    Access_None,
    Access_ReadOnly,
    Access_WriteOnly,
    Access_ReadWrite
}eAccessModes;


#endif // CONTEXT_DEFINE_H

