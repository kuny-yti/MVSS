#include "opengl_capability.h"
#include "tools/bs_string.h"
#include "maths/bs_vec2.h"
#include "opengl_mx.h"

const static char *GL_VENDOR_STR[] =
{
    "GL_",
    "GL_ARB_",
    "GL_EXT_",
    "GL_ATI_",
    "GL_AMD_",
    "GL_AMDX_",
    "GL_NV_",
    "GL_NVX_",
    "GL_IBM_",
    "GL_INTEL_",
    "GL_SGI_",
    "GL_SGIS_",
    "GL_SGIX_",
    "GL_SUN_",
    "GL_SUNX_"
};
const static char *GL_CAPABIL_STR[] =
{
    "not",
    "debug_output",             //(OpenGL 3.0)
    "vertex_array",             //(Requires OpenGL 1.1)
    "vertex_array_object",      //(OpenGL 2.1)
    "vertex_buffer_object",     //(OpenGL 1.5)
    "framebuffer_object",       //(OpenGL 3.0)
    "pixel_buffer_object",      //(OpenGL 2.1)
    "shader_objects",           //(OpenGL 2.0)
    "texture_buffer_object",    //(OpenGL 3.1)
    "uniform_buffer_object",    //(OpenGL 3.1)
    "framebuffer_sRGB",         //(OpenGL 3.0)
    "framebuffer_blit",         //(OpenGL 3.0)
    "framebuffer_multisample",  //(OpenGL 3.0)

    "timer_query",              //(OpenGL 3.3)
    "occlusion_query",          //(OpenGL 1.5)
    "occlusion_query2",         //(OpenGL 3.3)
    "multisample",              //(OpenGL 1.3)
    "multitexture",             //(OpenGL 1.3)

    "fragment_program",         //(OpenGL 1.3)
    "vertex_program",           //(Requires OpenGL 1.3)
    "fragment_shader",          //(OpenGL 2.0)
    "vertex_shader",            //(OpenGL 2.0)
    "geometry_shader4",         //(OpenGL 3.2)
    "compute_shader",
    "tessellation_shader",

    "texture_compression",      //(OpenGL 1.3)
    "texture_rectangle",        //(OpenGL 3.1)
    "texture_float",            //(OpenGL 3.0)
    "texture_multisample",      //(OpenGL 3.2)
    "texture_sRGB", //

    "copy_buffer",               //(OpenGL 3.1)
    "copy_image",                //(OpenGL 4.3)
    "depth_texture",             //(OpenGL 1.4)
    "draw_buffers",              //(OpenGL 2.0)
    "window_pos",                //(OpenGL 1.4)
    "abgr",
    "bgra",                      //(OpenGL 1.2)
};


#define DefineValue(GL_T) \
{\
    int val_##GL_T;\
    glGetIntegerv(GL_T, &val_##GL_T);\
    caps_value_list.append(val_##GL_T);\
    caps_name_list.append(#GL_T);\
    caps_var_list.append(GL_T);\
}

/*
class opengl_max
{

    int ARBTextureUnits;
    int VertexShaderTextureUnits;
    int PixelShaderTextureUnits;
    int GeometryShaderTextureUnits;
    bs_size MaxTextureSize;
    float MaxAnisotropicFilteringValue;
    float MaxPointSpriteSize;
    int MaxDynamicLights;
    bs_size MaxViewportSize;
    int MaxVertexUniformComponents;
    int MaxFragmentUniformComponents;
    int MaxGeometryUniformComponents;
    int MaxVaryingFloat;
    int MaxVertexBindableUniforms;
    int MaxFragmentBindableUniforms;
    int MaxGeometryBindableUniforms;
    int MultipleRenderTargets_MaxDrawBuffers;
    bs_vector<int> MSAA;
public:
    bs_vector<bs_string> caps_name_list;
    bs_vector<ulong> caps_value_list;
    bs_vector<ulong> caps_var_list;

    int caps_initialize()
    {
        //OpenGL core capabilities) //179 caps listed
        // 1.1 sdk
        DefineValue(GL_MAX_LIGHTS) //灯光最大值，值最小为8
        DefineValue(GL_MAX_CLIP_PLANES) //裁剪平面的最大数
        DefineValue(GL_MAX_TEXTURE_SIZE) //可处理最大纹理粗略估计，最小64，在3.0以上最小2048
        DefineValue(GL_MAX_MODELVIEW_STACK_DEPTH) //模型视图矩阵栈的最大深度，最小为16
        DefineValue(GL_MAX_PROJECTION_STACK_DEPTH) //投影矩阵栈的最大深度，最小为2
        DefineValue(GL_MAX_TEXTURE_STACK_DEPTH) //纹理矩阵栈的最大深度，最小为2
        DefineValue(GL_MAX_TEXTURE_UNITS) //指示纹理单元的最大数，最小为1
        DefineValue(GL_MAX_VIEWPORT_DIMS) //最大支持的视口宽度和高度


        // 3.0sdk
        DefineValue(GL_MAX_3D_TEXTURE_SIZE) //可处理最大3D纹理粗略估计，最小256
                //glTexImage3D
        DefineValue(GL_MAX_ARRAY_TEXTURE_LAYERS) //数组纹理数组中最大层数，最小256
                //glTexImage2D
        DefineValue(GL_MAX_COLOR_ATTACHMENTS) //帧缓存中最大附加颜色点最大值，最小4.
                //glFramebufferRenderbuffer and glFramebufferTexture2D.
        DefineValue(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS) //在所有的Uniform中Block的片段着色的统一变量的数目，至少是 GL_MAX_FRAGMENT_UNIFORM_COMPONENTS + GL_MAX_UNIFORM_BLOCK_SIZE * GL_MAX_FRAGMENT_UNIFORM_BLOCKS / 4
                // glUniform
                //
        DefineValue(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS) //最大支持的纹理图像单元，最小32
                //glActiveTexture
        DefineValue(GL_MAX_COMBINED_UNIFORM_BLOCKS) //每个程序的统一Block最大数，最小24
                //glUniformBlockBinding
        DefineValue(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS) //所有的Uniform Block 中顶点着色统一变量的数目，至少是  GL_MAX_VERTEX_UNIFORM_COMPONENTS + GL_MAX_UNIFORM_BLOCK_SIZE * GL_MAX_VERTEX_UNIFORM_BLOCKS / 4
                // glUniform
        DefineValue(GL_MAX_CUBE_MAP_TEXTURE_SIZE) //可处理最大Cube纹理粗略估计，最小2048
                // glTexImage2D
        DefineValue(GL_MAX_DRAW_BUFFERS) //可写入片段着色的同时输出最大数，最少4
                // glDrawBuffers.
        DefineValue(GL_MAX_ELEMENT_INDEX) //支持Element的最大索引，最少 2的24次方−1
                // .
        DefineValue(GL_MAX_ELEMENTS_INDICES) //顶点数组索引的最大个数
                // glDrawRangeElements
        DefineValue(GL_MAX_ELEMENTS_VERTICES) //顶点数组顶点的推荐最大数
                //  glDrawRangeElements
        DefineValue(GL_MAX_FRAGMENT_INPUT_COMPONENTS) //由片段着色所读取的输入组件的最大数量，必须至少为60。
        DefineValue(GL_MAX_FRAGMENT_UNIFORM_BLOCKS) //每一个片段着色的Uniform Block的最大数目。该值必须至少为12
                // glUniformBlockBinding
        DefineValue(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS) //可以在一个片段着色的统一变量存储中保存的单个浮点、整型或布尔值的最大数目。该值必须至少为896。
                // glUniform.
        DefineValue(GL_MAX_FRAGMENT_UNIFORM_VECTORS) //可以在一个片段着色的统一变量存储中保存的单个向量浮点、整型或布尔值的最大数目。该值必须至少为224。

        DefineValue(GL_MAX_PROGRAM_TEXEL_OFFSET) //纹理查找允许中最大像素偏移，最小7

        DefineValue(GL_MAX_RENDERBUFFER_SIZE) //支持的最大渲染缓存尺寸，至少2048
                //glFramebufferRenderbuffer.
        DefineValue(GL_MAX_SAMPLES) //多重采样的最大值，至少4
                // glGetInternalformativ
        DefineValue(GL_MAX_SERVER_WAIT_TIMEOUT) // glWaitSync最大超时时间
        DefineValue(GL_MAX_TEXTURE_IMAGE_UNITS) //纹理图像单元数最大值，最小16
                // glActiveTexture
        DefineValue(GL_MAX_TEXTURE_LOD_BIAS) //纹理的细节偏差最大绝对值，最小2

        DefineValue(GL_MAX_UNIFORM_BLOCK_SIZE) //65536
                //glUniformBlockBinding.



        DefineValue(GL_MAX_LIST_NESTING) //获取显示列表的嵌套层次限制
        DefineValue(GL_MAX_EVAL_ORDER) //40
        DefineValue(GL_MAX_PIXEL_MAP_TABLE) //256
        DefineValue(GL_MAX_ATTRIB_STACK_DEPTH) //16

        DefineValue(GL_MAX_NAME_STACK_DEPTH) //64

        DefineValue(GL_MAX_CLIENT_ATTRIB_STACK_DEPTH) //16

        DefineValue(GL_MAX_CLIP_DISTANCES) //8

        DefineValue(GL_MAX_VARYING_COMPONENTS) //128

        DefineValue(GL_MIN_PROGRAM_TEXEL_OFFSET) //-8

        DefineValue(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS) //4
        DefineValue(GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS)
        DefineValue(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS) //4

        DefineValue(GL_MAX_TEXTURE_BUFFER_SIZE) //268435456

        DefineValue(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS) //32

        DefineValue(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS) //16384

        DefineValue(GL_MAX_GEOMETRY_OUTPUT_VERTICES) //1024

        DefineValue(GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS) //16384

        DefineValue(GL_MAX_VERTEX_OUTPUT_COMPONENTS) //128

        DefineValue(GL_MAX_GEOMETRY_INPUT_COMPONENTS) //128

        DefineValue(GL_MAX_GEOMETRY_OUTPUT_COMPONENTS) //128


        DefineValue(GL_MAX_COMPUTE_VARIABLE_GROUP_INVOCATIONS_ARB) //0

        DefineValue(GL_MAX_COMPUTE_FIXED_GROUP_INVOCATIONS_ARB) //1024

        DefineValue(GL_MAX_COMPUTE_VARIABLE_GROUP_SIZE_ARB) //0

        DefineValue(GL_MAX_COMPUTE_FIXED_GROUP_SIZE_ARB) //0

        DefineValue(GL_MAX_SPARSE_TEXTURE_SIZE_ARB) //16384

        DefineValue(GL_MAX_SPARSE_3D_TEXTURE_SIZE_ARB) //2048

        DefineValue(GL_MAX_SPARSE_ARRAY_TEXTURE_LAYERS_ARB) //2048

        DefineValue(GL_SPARSE_TEXTURE_FULL_ARRAY_CUBE_MIPMAPS_ARB) //1
#ifdef GL_MAX_CULL_DISTANCES
        DefineValue(GL_MAX_CULL_DISTANCES) //0
#endif
#ifdef GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES
        DefineValue(GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES) //0
#endif
        //OpenGL extension capabilities) //163 caps listed

        DefineValue(GL_RGBA_FLOAT_MODE_ARB) //0 (GL_ARB_color_buffer_float)

        DefineValue(GL_MAX_COLOR_ATTACHMENTS_EXT) //8 (GL_EXT_framebuffer_object)

        DefineValue(GL_MAX_RENDERBUFFER_SIZE_EXT) //16384 (GL_EXT_framebuffer_object)

        DefineValue(GL_MAX_SAMPLES_EXT) //8 (GL_EXT_framebuffer_multisample)

        DefineValue(GL_RGBA_INTEGER_MODE_EXT) //0 (GL_EXT_texture_integer)

        DefineValue(GL_MAX_ARRAY_TEXTURE_LAYERS_EXT) //2048 (GL_EXT_texture_array)

        DefineValue(GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS_EXT) //64 (GL_EXT_transform_feedback)

        DefineValue(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS_EXT) //4 (GL_EXT_transform_feedback)

        DefineValue(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS_EXT) //4 (GL_EXT_transform_feedback)

        DefineValue(GL_FRAMEBUFFER_SRGB_CAPABLE_EXT) //1 (GL_EXT_framebuffer_sRGB)

        DefineValue(GL_MAX_TEXTURE_BUFFER_SIZE_ARB) //268435456 (GL_ARB_texture_buffer_object)

        DefineValue(GL_MAX_VERTEX_UNIFORM_BLOCKS) //15 (GL_ARB_uniform_buffer_object)

        DefineValue(GL_MAX_GEOMETRY_UNIFORM_BLOCKS) //15 (GL_ARB_uniform_buffer_object)

        DefineValue(GL_MAX_FRAGMENT_UNIFORM_BLOCKS) //15 (GL_ARB_uniform_buffer_object)

        DefineValue(GL_MAX_COMBINED_UNIFORM_BLOCKS) //75 (GL_ARB_uniform_buffer_object)

        DefineValue(GL_MAX_UNIFORM_BUFFER_BINDINGS) //75 (GL_ARB_uniform_buffer_object)

        DefineValue(GL_MAX_UNIFORM_BLOCK_SIZE) //65536 (GL_ARB_uniform_buffer_object)

        DefineValue(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS) //246784 (GL_ARB_uniform_buffer_object)

        DefineValue(GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS) //262144 (GL_ARB_uniform_buffer_object)

        DefineValue(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS) //246784 (GL_ARB_uniform_buffer_object)

        DefineValue(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT) //256 (GL_ARB_uniform_buffer_object)

        DefineValue(GL_MAX_RECTANGLE_TEXTURE_SIZE) //16384 (GL_ARB_texture_rectangle)

        DefineValue(GL_PROVOKING_VERTEX) //36430 (GL_ARB_provoking_vertex)

        DefineValue(GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION) //1 (GL_ARB_provoking_vertex)

        DefineValue(GL_MAX_SAMPLE_MASK_WORDS) //1 (GL_ARB_texture_multisample)

        DefineValue(GL_MAX_COLOR_TEXTURE_SAMPLES) //8 (GL_ARB_texture_multisample)

        DefineValue(GL_MAX_DEPTH_TEXTURE_SAMPLES) //8 (GL_ARB_texture_multisample)

        DefineValue(GL_MAX_INTEGER_SAMPLES) //8 (GL_ARB_texture_multisample)

        DefineValue(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_ARB) //32 (GL_ARB_geometry_shader4)

        DefineValue(GL_MAX_GEOMETRY_VARYING_COMPONENTS_ARB) //128 (GL_ARB_geometry_shader4)

        DefineValue(GL_MAX_VERTEX_VARYING_COMPONENTS_ARB) //128 (GL_ARB_geometry_shader4)

        DefineValue(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_ARB) //16384 (GL_ARB_geometry_shader4)

        DefineValue(GL_MAX_GEOMETRY_OUTPUT_VERTICES_ARB) //1024 (GL_ARB_geometry_shader4)

        DefineValue(GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_ARB) //16384 (GL_ARB_geometry_shader4)

        DefineValue(GL_MAX_SERVER_WAIT_TIMEOUT) //9223372036854775807 (GL_ARB_sync)

        DefineValue(GL_MAX_DUAL_SOURCE_DRAW_BUFFERS) //1 (GL_ARB_blend_func_extended)

        DefineValue(GL_MAX_GEOMETRY_SHADER_INVOCATIONS) //127 (GL_ARB_gpu_shader5)

        DefineValue(GL_MIN_FRAGMENT_INTERPOLATION_OFFSET) //-1 (GL_ARB_gpu_shader5)

        DefineValue(GL_MAX_FRAGMENT_INTERPOLATION_OFFSET) //1 (GL_ARB_gpu_shader5)

        DefineValue(GL_FRAGMENT_INTERPOLATION_OFFSET_BITS) //4 (GL_ARB_gpu_shader5)

        DefineValue(GL_MAX_VERTEX_STREAMS) //4 (GL_ARB_gpu_shader5)

        DefineValue(GL_MIN_SAMPLE_SHADING_VALUE) //0 (GL_ARB_sample_shading)

        DefineValue(GL_MAX_SUBROUTINES) //4096 (GL_ARB_shader_subroutine)

        DefineValue(GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS) //4096 (GL_ARB_shader_subroutine)

        DefineValue(GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS) //32 (GL_ARB_tessellation_shader)

        DefineValue(GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS) //32 (GL_ARB_tessellation_shader)

        DefineValue(GL_MAX_TESS_GEN_LEVEL) //64 (GL_ARB_tessellation_shader)

        DefineValue(GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS) //16384 (GL_ARB_tessellation_shader)

        DefineValue(GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS) //16384 (GL_ARB_tessellation_shader)

        DefineValue(GL_MAX_TESS_CONTROL_INPUT_COMPONENTS) //128 (GL_ARB_tessellation_shader)

        DefineValue(GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS) //128 (GL_ARB_tessellation_shader)

        DefineValue(GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS) //246784 (GL_ARB_tessellation_shader)

        DefineValue(GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS) //246784 (GL_ARB_tessellation_shader)

        DefineValue(GL_MAX_PATCH_VERTICES) //32 (GL_ARB_tessellation_shader)

        DefineValue(GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS) //128 (GL_ARB_tessellation_shader)

        DefineValue(GL_MAX_TESS_PATCH_COMPONENTS) //120 (GL_ARB_tessellation_shader)

        DefineValue(GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS) //4096 (GL_ARB_tessellation_shader)

        DefineValue(GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS) //128 (GL_ARB_tessellation_shader)

        DefineValue(GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS) //15 (GL_ARB_tessellation_shader)

        DefineValue(GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS) //15 (GL_ARB_tessellation_shader)

        DefineValue(GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET) //-32 (GL_ARB_texture_gather)

        DefineValue(GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET) //31 (GL_ARB_texture_gather)
#ifdef GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS
        DefineValue(GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS) //4 (GL_ARB_texture_gather)
#endif
        DefineValue(GL_MAX_TRANSFORM_FEEDBACK_BUFFERS) //4 (GL_ARB_transform_feedback3)

        DefineValue(GL_NUM_PROGRAM_BINARY_FORMATS) //1 (GL_ARB_get_program_binary)

        DefineValue(GL_MAX_VIEWPORTS) //16 (GL_ARB_viewport_array)

        DefineValue(GL_UNPACK_COMPRESSED_BLOCK_WIDTH) //0 (GL_ARB_compressed_texture_pixel_storage)

        DefineValue(GL_UNPACK_COMPRESSED_BLOCK_HEIGHT) //0 (GL_ARB_compressed_texture_pixel_storage)

        DefineValue(GL_UNPACK_COMPRESSED_BLOCK_DEPTH) //0 (GL_ARB_compressed_texture_pixel_storage)

        DefineValue(GL_UNPACK_COMPRESSED_BLOCK_SIZE) //0 (GL_ARB_compressed_texture_pixel_storage)

        DefineValue(GL_PACK_COMPRESSED_BLOCK_WIDTH) //0 (GL_ARB_compressed_texture_pixel_storage)

        DefineValue(GL_PACK_COMPRESSED_BLOCK_HEIGHT) //0 (GL_ARB_compressed_texture_pixel_storage)

        DefineValue(GL_PACK_COMPRESSED_BLOCK_DEPTH) //0 (GL_ARB_compressed_texture_pixel_storage)

        DefineValue(GL_PACK_COMPRESSED_BLOCK_SIZE) //0 (GL_ARB_compressed_texture_pixel_storage)

        DefineValue(GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS) //8 (GL_ARB_shader_atomic_counters)

        DefineValue(GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS) //8 (GL_ARB_shader_atomic_counters)

        DefineValue(GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS) //8 (GL_ARB_shader_atomic_counters)

        DefineValue(GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS) //8 (GL_ARB_shader_atomic_counters)

        DefineValue(GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS) //8 (GL_ARB_shader_atomic_counters)

        DefineValue(GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS) //8 (GL_ARB_shader_atomic_counters)

        DefineValue(GL_MAX_VERTEX_ATOMIC_COUNTERS) //8 (GL_ARB_shader_atomic_counters)

        DefineValue(GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS) //8 (GL_ARB_shader_atomic_counters)

        DefineValue(GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS) //8 (GL_ARB_shader_atomic_counters)

        DefineValue(GL_MAX_GEOMETRY_ATOMIC_COUNTERS) //8 (GL_ARB_shader_atomic_counters)

        DefineValue(GL_MAX_FRAGMENT_ATOMIC_COUNTERS) //8 (GL_ARB_shader_atomic_counters)

        DefineValue(GL_MAX_COMBINED_ATOMIC_COUNTERS) //8 (GL_ARB_shader_atomic_counters)

        DefineValue(GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE) //32 (GL_ARB_shader_atomic_counters)

        DefineValue(GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS) //8 (GL_ARB_shader_atomic_counters)

        DefineValue(GL_MAX_IMAGE_UNITS) //32 (GL_ARB_shader_image_load_store)

        DefineValue(GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS) //40 (GL_ARB_shader_image_load_store)

        DefineValue(GL_MAX_IMAGE_SAMPLES) //8 (GL_ARB_shader_image_load_store)

        DefineValue(GL_MAX_VERTEX_IMAGE_UNIFORMS) //0 (GL_ARB_shader_image_load_store)

        DefineValue(GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS) //0 (GL_ARB_shader_image_load_store)

        DefineValue(GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS) //0 (GL_ARB_shader_image_load_store)

        DefineValue(GL_MAX_GEOMETRY_IMAGE_UNIFORMS) //0 (GL_ARB_shader_image_load_store)

        DefineValue(GL_MAX_FRAGMENT_IMAGE_UNIFORMS) //32 (GL_ARB_shader_image_load_store)

        DefineValue(GL_MAX_COMBINED_IMAGE_UNIFORMS) //32 (GL_ARB_shader_image_load_store)

        DefineValue(GL_MIN_MAP_BUFFER_ALIGNMENT) //256 (GL_ARB_map_buffer_alignment)

        DefineValue(GL_MAX_COMPUTE_UNIFORM_BLOCKS) //16 (GL_ARB_compute_shader)

        DefineValue(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS) //32 (GL_ARB_compute_shader)

        DefineValue(GL_MAX_COMPUTE_IMAGE_UNIFORMS) //32 (GL_ARB_compute_shader)

        DefineValue(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE) //32768 (GL_ARB_compute_shader)

        DefineValue(GL_MAX_COMPUTE_UNIFORM_COMPONENTS) //1024 (GL_ARB_compute_shader)

        DefineValue(GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS) //8 (GL_ARB_compute_shader)

        DefineValue(GL_MAX_COMPUTE_ATOMIC_COUNTERS) //8 (GL_ARB_compute_shader)

        DefineValue(GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS) //1024 (GL_ARB_compute_shader)
#ifdef GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS
        DefineValue(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS) //1024 (GL_ARB_compute_shader)
#endif
        DefineValue(GL_MAX_COMPUTE_WORK_GROUP_COUNT) //65535/65535/65535 (GL_ARB_compute_shader)

        DefineValue(GL_MAX_COMPUTE_WORK_GROUP_SIZE) //1024/1024/1024 (GL_ARB_compute_shader)

        DefineValue(GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET) //2047 (GL_ARB_vertex_attrib_binding)

        DefineValue(GL_MAX_VERTEX_ATTRIB_BINDINGS) //2047 (GL_ARB_vertex_attrib_binding)

        DefineValue(GL_MAX_UNIFORM_LOCATIONS) //4096 (GL_ARB_explicit_uniform_location)

        DefineValue(GL_MAX_FRAMEBUFFER_WIDTH) //16384 (GL_ARB_framebuffer_no_attachments)

        DefineValue(GL_MAX_FRAMEBUFFER_HEIGHT) //16384 (GL_ARB_framebuffer_no_attachments)

        DefineValue(GL_MAX_FRAMEBUFFER_LAYERS) //8192 (GL_ARB_framebuffer_no_attachments)

        DefineValue(GL_MAX_FRAMEBUFFER_SAMPLES) //16 (GL_ARB_framebuffer_no_attachments)

        DefineValue(GL_MIN_PROGRAM_TEXEL_OFFSET_EXT) //-8 (GL_EXT_gpu_shader4)

        DefineValue(GL_MAX_PROGRAM_TEXEL_OFFSET_EXT) //7 (GL_EXT_gpu_shader4)

        DefineValue(GL_MAX_TEXTURE_UNITS_ARB) //8 (GL_ARB_multitexture)

        DefineValue(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB) //32 (GL_ARB_multitexture)

        DefineValue(GL_MAX_TEXTURE_IMAGE_UNITS_ARB) //32 (GL_ARB_multitexture)

        DefineValue(GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB) //16384 (GL_ARB_texture_cube_map)

        DefineValue(GL_NUM_COMPRESSED_TEXTURE_FORMATS) //18 (GL_ARB_texture_compression)

        DefineValue(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT) //16.000000 (GL_EXT_texture_filter_anisotropic)

        DefineValue(GL_MAX_VERTEX_UNIFORM_COMPONENTS) //1024 (GL_ARB_vertex_shader)

        DefineValue(GL_MAX_VARYING_FLOATS) //128 (GL_ARB_vertex_shader)

        DefineValue(GL_MAX_VERTEX_ATTRIBS) //29 (GL_ARB_vertex_shader)

        DefineValue(GL_MAX_TEXTURE_IMAGE_UNITS) //32 (GL_ARB_vertex_shader)

        DefineValue(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS) //32 (GL_ARB_vertex_shader)

        DefineValue(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS) //192 (GL_ARB_vertex_shader)

        DefineValue(GL_MAX_TEXTURE_COORDS) //16 (GL_ARB_vertex_shader)

        DefineValue(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB) //1024 (GL_ARB_fragment_shader)

        DefineValue(GL_MAX_VERTEX_ATTRIBS_ARB) //29 (GL_ARB_vertex_program)

        DefineValue(GL_MAX_PROGRAM_MATRICES_ARB) //32 (GL_ARB_vertex_program)

        DefineValue(GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB) //32 (GL_ARB_vertex_program)

        DefineValue(GL_MAX_TEXTURE_COORDS_ARB) //16 (GL_ARB_fragment_program)

        DefineValue(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS) //1024 (GL_ARB_shading_language_100)

        DefineValue(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_EXT) //16384 (GL_ARB_shading_language_100)

        DefineValue(GL_MAX_DRAW_BUFFERS_ARB) //8 (GL_ARB_draw_buffers)

        DefineValue(GL_MAX_COLOR_ATTACHMENTS) //8 (GL_ARB_framebuffer_object)

        DefineValue(GL_MAX_RENDERBUFFER_SIZE) //16384 (GL_ARB_framebuffer_object)

        DefineValue(GL_MAX_SAMPLES) //8 (GL_ARB_framebuffer_object)

        DefineValue(GL_MAX_CONVOLUTION_WIDTH) //0 (GL_ARB_imaging)

        DefineValue(GL_MAX_CONVOLUTION_HEIGHT) //0 (GL_ARB_imaging)

        DefineValue(GL_MAX_COLOR_MATRIX_STACK_DEPTH) //10 (GL_ARB_imaging)

        DefineValue(GL_POINT_SIZE_MIN_ARB) //0.000000 (GL_ARB_point_parameters)

        DefineValue(GL_POINT_SIZE_MAX_ARB) //8192.000000 (GL_ARB_point_parameters)

        DefineValue(GL_MAX_VERTEX_UNIFORM_VECTORS) //256 (GL_ARB_ES2_compatibility)

        DefineValue(GL_MAX_VARYING_VECTORS) //32 (GL_ARB_ES2_compatibility)

        DefineValue(GL_MAX_FRAGMENT_UNIFORM_VECTORS) //256 (GL_ARB_ES2_compatibility)

        DefineValue(GL_MAX_DEBUG_MESSAGE_LENGTH) //1024 (GL_ARB_debug_output)

        DefineValue(GL_MAX_DEBUG_LOGGED_MESSAGES_ARB) //256 (GL_ARB_debug_output)

        DefineValue(GL_MAX_VERTEX_BINDABLE_UNIFORMS_EXT) //15 (GL_EXT_bindable_uniform)

        DefineValue(GL_MAX_FRAGMENT_BINDABLE_UNIFORMS_EXT) //15 (GL_EXT_bindable_uniform)

        DefineValue(GL_MAX_GEOMETRY_BINDABLE_UNIFORMS_EXT) //15 (GL_EXT_bindable_uniform)

        DefineValue(GL_MAX_BINDABLE_UNIFORM_SIZE_EXT) //65536 (GL_EXT_bindable_uniform)

        DefineValue(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_EXT) //32 (GL_EXT_geometry_shader4)

        DefineValue(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT) //1024 (GL_EXT_geometry_shader4)

        DefineValue(GL_MAX_TEXTURE_BUFFER_SIZE_EXT) //268435456 (GL_EXT_texture_buffer_object)

        return caps_name_list.count();
    }
};
*/
namespace impl {
class opengl_capab
{
public:
    bs_string_list gl_ext;
    bs_string package_name;
    eOpenGLPackages gl_package;
    //opengl_max caps_list;
    bool is_init;

    bool is_sRGB8;

    opengl_capab()
    {
        gl_ext.clear();
        is_init = false;
    }
    ~opengl_capab()
    {
    }
    bool test_srgb8()
    {
        bool retval = true;
        GLuint fbo;
        GLuint tex;

        glGenFramebuffers(1, &fbo);
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, 2, 2, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, NULL);
        GLint framebuffer_bak;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebuffer_bak);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
        GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (e != GL_FRAMEBUFFER_COMPLETE) {
            retval = false;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_bak);
        glDeleteFramebuffers(1, &fbo);
        glDeleteTextures(1, &tex);
        return retval;
    }
};
}

opengl_capability::opengl_capability()
{
    impl = bsNew (impl::opengl_capab());
    impl->is_init = false;
}

opengl_capability::~opengl_capability()
{
    bsDelete (impl);
}

bool opengl_capability::is_support(const bs_string &ext)const
{
    for (size_t i = 0; i < impl->gl_ext.count(); ++i)
    {
        if (ext == impl->gl_ext[i])
            return true;
    }
    return false;
}
eOpenGLVendor opengl_capability::vendors()const
{
    bs_string ved = vendor;
    ved.upper();
    if (ved.find("ATI") >=0 )
        return Vendor_Ati;
    if (ved.find("AMD") >=0 )
        return Vendor_Amd;
    if (ved.find("NVIDIA") >=0 )
        return Vendor_Nvidia;
    if (ved.find("INTEL(R)") >=0 )// "Intel(R)"
        return Vendor_Intel;
    if (ved.find("ARM") >=0 )
        return Vendor_Arm;
    if (ved.find("QIALCOMM") >=0 )//"Qualcomm"
        return Vendor_Qualcomm;

    if (ved.find("VMWARW, INC.") >=0 )//"VMware, Inc."
        return Vendor_Mesa;
    if (ved.find("MESA PROJECT") >=0 )//"Mesa Project"
        return Vendor_Mesa;
    return Vendor_Not;
}
eOpenGLPackages opengl_capability::package()const
{
    //获取软件包
    if (version.find("Mesa") >= 0)
        return Package_Mesa;

    return Package_Unknown;
}
eGPUArchs opengl_capability::arch()const
{
    if (renderer.find("Sandybridge") >= 0)
        return Arch_Sandybridge;
    if (renderer.find("llvmpipe") >= 0)
        return Arch_Llvmpipe;
    if (renderer.find("softpipe") >= 0)
        return Arch_Softpipe;
    if (renderer.find("software rasterizer") >= 0)
        return Arch_SWRast;
    if (renderer.find("Software Rasterizer") >= 0)
        return Arch_SWRast;
    if (renderer.find("PowerVR SGX") >= 0)
        return Arch_Sgx;
    if (renderer.find("Mali-") >= 0)
        return Arch_Mali;
    return Arch_Unknown;
}

static bs_string ext_conv(eCapabilityExtensions ext)
{
    switch (ext) {
    case Extension_DebugOutput:
    case Extension_VertexArray:
    case Extension_VertexArrayObject:
    case Extension_VertexBufferObject:
    case Extension_FrameBufferObject:
    case Extension_PixelBufferObject:
    case Extension_ShaderObject :
    case Extension_TextureBufferObject :
    case Extension_UniformBufferObject :
    case Extension_FrameBuffersRGB :
    case Extension_FrameBufferBlit:
    case Extension_FrameBufferMultiSample :
    case Extension_TimerQuery :
    case Extension_OcclusionQuery :
    case Extension_MultiSample :
    case Extension_MultiTexture :
    case Extension_FragmentProgram :
    case Extension_VertexProgram :
    case Extension_FragmentShader :
    case Extension_VertexShader :
    case Extension_GeometryShader4 :
    case Extension_ComputeShader:
    case Extension_TessellationShader:
    case Extension_TextureCompression:
    case Extension_TextureRectangle:
    case Extension_TextureFloating:
    case Extension_TextureMultiSample :
    case Extension_TexturesRGB :
    case Extension_CopyBuffer:
    case Extension_CopyImage:
    case Extension_DepthTexture :
    case Extension_DrawBuffer :
    case Extension_WindowPos :
    case Extension_ABGR :
    case Extension_BGRA :
        return GL_CAPABIL_STR[ext];
    case Extension_sRGB8:
    case Extension_Count:
    default:
        break;
    }
    return "";
}
bool opengl_capability::is_support(eCapabilityExtensions ext)const
{
    bs_string caps = ext_conv(ext);
    if (caps.is_empty())
        return false;
    if (ext ==  Extension_sRGB8)
        return impl->is_sRGB8;

    if (is_support(bs_string(GL_VENDOR_STR[0])+caps))//GL
        return true;
    if (is_support(bs_string(GL_VENDOR_STR[1])+caps))//GL_ARB
        return true;
    if (is_support(bs_string(GL_VENDOR_STR[2])+caps))//GL_EXT
        return true;

    eOpenGLVendor ved = vendors();
    if (ved == Vendor_Amd)
    {
        if (is_support(bs_string(GL_VENDOR_STR[3])+caps))//ATI
            return true;
        if (is_support(bs_string(GL_VENDOR_STR[4])+caps))//AMD
            return true;
        if (is_support(bs_string(GL_VENDOR_STR[5])+caps))//AMDX
            return true;
    }
    else if (ved == Vendor_Nvidia)
    {
        if (is_support(bs_string(GL_VENDOR_STR[6])+caps))//NV
            return true;
        if (is_support(bs_string(GL_VENDOR_STR[7])+caps))//NVX
            return true;
    }
    else if (ved == Vendor_Intel)
    {
        if (is_support(bs_string(GL_VENDOR_STR[9])+caps))//NV
            return true;
    }
    return false;
}

void opengl_capability::initialize()
{
    if (!impl->is_init)
    {
        //获取OpenGL版本字符串
        version = (char*)glGetString(GL_VERSION);
        //获取OpenGL显卡厂家信息字符串
        vendor = (char*)glGetString(GL_VENDOR);
        //获取OpenGL显卡型号信息字符串
        renderer = (char*)glGetString(GL_RENDERER);
        //获取GLSL 版本
        version_glsl = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
        //获取OpenGL扩展函数名称字符串
        bs_string ext = (char*)glGetString(GL_EXTENSIONS);
        impl->gl_ext = ext.split(" ");

        impl->is_sRGB8 = impl->test_srgb8();
        impl->is_init = true;
    }
}
