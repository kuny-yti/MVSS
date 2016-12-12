#include "opengl_texture.h"
#include "opengl_mx.h"
#include "opengl_buffer_object.h"
#include "tools/bs_string.h"

static int next_multiple_of_4(int x)
{
    return (x / 4 + (x % 4 == 0 ? 0 : 1)) * 4;
}
namespace impl {
static GLuint returnDataFormat(eDataFormats format)
{
    switch ((int)format)
    {
    case Data_RGB :
        return GL_RGB;
    case Data_RGBA :
        return GL_RGBA;
    case Data_Luminance:
        return GL_LUMINANCE;
    case Data_LuminanceAlpha:
        return GL_LUMINANCE_ALPHA;
    case Data_BGR:
        return GL_BGR;
    case Data_BGRA:
        return GL_BGRA;
    case Data_Alpha:
        return GL_ALPHA;
    default:
        break;
    }
    return GL_RGBA;
}
static GLuint returnDataDepth(eDataDepths depth)
{
    switch ((int)depth)
    {
    case Data_Byte :
        return GL_BYTE;
    case Data_UByte:
        return GL_UNSIGNED_BYTE;
    case Data_Short:
        return GL_SHORT;
    case Data_UShort:
        return GL_UNSIGNED_SHORT;
    case Data_Int:
        return GL_INT;
    case Data_UInt:
        return GL_UNSIGNED_INT;
    case Data_Float:
        return GL_FLOAT;
    case Data_2Bytes :
        return GL_2_BYTES;
    case Data_3Bytes :
        return GL_3_BYTES;
    case Data_4Bytes:
        return GL_4_BYTES;
    case Data_Double :
        return GL_DOUBLE;
    case Data_Uint8888Rev:
        return GL_UNSIGNED_INT_8_8_8_8_REV;
    default:
        break;
    }
    return GL_UNSIGNED_BYTE;
}
static GLuint returnInternalFormat(eInternalFormats internal)
{
    switch ((int)internal)
    {
    case Internal_Red:
        return GL_RED;
    case Internal_Green:
        return GL_GREEN;
    case Internal_Blue:
        return GL_BLUE;
    case Internal_Alpha:
        return GL_ALPHA;
    case Internal_RGB:
        return GL_RGB;
    case Internal_RGBA:
        return GL_RGBA;
    case Internal_Luminance:
        return GL_LUMINANCE;
    case Internal_Luminance8:
        return GL_LUMINANCE8;
    case Internal_LuminanceAlpha:
        return GL_LUMINANCE_ALPHA;
    case Internal_RGB8:
        return GL_RGB8;
    case Internal_SRGB8:
        return GL_SRGB8;
    case Internal_RGB16:
        return GL_RGB16;
    case Internal_RGBA8:
        return GL_RGBA8;
    case Internal_Luminance16:
        return GL_LUMINANCE16;
    default:
        break;
    }
    return GL_RGBA;
}
static GLenum returnTexFilterMode(eTextureParams mode)
{
    switch((int)mode)
    {
    default:
    case Filter_Nearest:
        return GL_NEAREST;

    case Filter_NearestMipmapNearest:
        return GL_NEAREST_MIPMAP_NEAREST;

    case Filter_NearestMipmapLinear:
        return GL_NEAREST_MIPMAP_LINEAR;

    case Filter_Linear:
        return GL_LINEAR;

    case Filter_LinearMipmapNearest:
        return GL_LINEAR_MIPMAP_NEAREST;

    case Filter_LinearMipmapLinear:
        return GL_LINEAR_MIPMAP_LINEAR;
    }
}
static int returnFormatByte(eInternalFormats internal)
{
    switch ((int)internal)
    {
    case Internal_Red:
        return 1;
    case Internal_Green:
        return 1;
    case Internal_Blue:
        return 1;
    case Internal_Alpha:
        return 1;
    case Internal_RGB:
        return 3;
    case Internal_RGBA:
        return 4;
    case Internal_Luminance:
        return 4;
    case Internal_LuminanceAlpha:
        return 4;
    default:
        break;
    }
    return 4;
}

class opengl_tex
{
public :
    uint id_bak;
    uint active_texun_bak;
    pibobject pbo;

    bs_image read_data;

    opengl_tex():
        id_bak(0),
        active_texun_bak(0),
        pbo(),
        read_data()
    {
    }
};
}

int opengl_texture::units_max()
{
    int max;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max);
    return max;
}

int opengl_texture::size_max()
{
    int s;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &s);
    return s;
}
int opengl_texture::size_gpu(int level)
{
    GLint s;

    bind();

    if(is_compressed)
        glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &s);
    else
    {
        GLint rs, gs, bs, as, ls, is, ds, _w, _h, bytesPerPixel;
        glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_WIDTH,         &_w );
        glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_HEIGHT,        &_h );
        glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_RED_SIZE,      &rs );
        glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_GREEN_SIZE,	 &gs );
        glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_BLUE_SIZE,	 &bs );
        glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_ALPHA_SIZE,	 &as );
        glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_LUMINANCE_SIZE,&ls );
        glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_INTENSITY_SIZE,&is );
        glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_DEPTH_SIZE,	 &ds );

        bytesPerPixel = (rs+gs+bs+as+ls+is+ds)/8;
        s = _w*_h*bytesPerPixel;
    }
    unbind();
    return s;
}

opengl_texture::opengl_texture()
{
    impl = bsNew (impl::opengl_tex());
}
opengl_texture::~opengl_texture()
{
    if (id)
        glDeleteTextures(1, (uint*)&id);
    bsDelete (impl);
}


void opengl_texture::create()
{
    if ((size.width < 1) && (size.height < 1))
    {
        throw bs_string("texture::create (size empty).");
    }
    // pixel pack/unpack
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    glGenTextures(1, (uint*)&id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, impl::returnTexFilterMode(filter_min));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, impl::returnTexFilterMode(filter_mag));

    int glWrap = GL_REPEAT;
    if(wrap_s == Wrap_Clamp)
        glWrap = GL_CLAMP;
    else if (wrap_s == Wrap_ClampBorder)
        glWrap = GL_CLAMP_TO_BORDER;
    else if (wrap_t == Wrap_ClampEdge)
         glWrap = GL_CLAMP_TO_EDGE;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrap);

    glWrap = GL_REPEAT;
    if(wrap_t == Wrap_Clamp)
        glWrap = GL_CLAMP;
    else if (wrap_t == Wrap_ClampBorder)
        glWrap = GL_CLAMP_TO_BORDER;
    else if (wrap_t == Wrap_ClampEdge)
         glWrap = GL_CLAMP_TO_EDGE;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, level_base);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, level_max);

    if(level_max > 0 )
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    if (is_compressed)
    {
        glCompressedTexImage2D(GL_TEXTURE_2D, 0, impl::returnInternalFormat(internal),
                               size.width, size.height, 0,
                               size.width *size.height * impl::returnFormatByte(internal), 0);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, impl::returnInternalFormat(internal),
                     size.width, size.height, 0, impl::returnDataFormat(format),
                     impl::returnDataDepth(depth), 0);
    }
    if (level_max > 0)
        genmipmap();

    if (!id )
        throw bs_string("texture::create (id empty).");
}
void opengl_texture::update(const isize &_size, void *data)
{
    if ((_size != size) || (!id))
    {
        size = _size;
        if (id)
            glDeleteTextures(1, (uint*)&id);

        create();
    }
    if (data)
    {
        if (is_compressed)
        {
            glBindTexture(GL_TEXTURE_2D, id);
            glCompressedTexSubImage2D(GL_TEXTURE_2D, 0, 0,0,
                                      size.width, size.height, impl::returnDataFormat(format),
                                      size.width * size.height* impl::returnFormatByte(internal), data);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, id);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.width, size.height,
                            impl::returnDataFormat(format), impl::returnDataDepth(depth), data);
        }
        if (level_max > 0)
            genmipmap();
    }
}
void *opengl_texture::upload(const isize &_size)
{
    if ((_size != size) || (!id))
    {
        size = _size;
        if (id )
            glDeleteTextures(1, (uint*)&id);

        create();
    }

    int row_size = next_multiple_of_4(size.width * pixel_byte);
    int siz = row_size * size.height;

    if (!impl->pbo ||(impl->pbo->size != siz))
    {
        impl->pbo =  bs_malloc (opengl_buffer_object);
        impl->pbo->target = Target_PixelUnPack;
        impl->pbo->usage = Usage_DynamicDraw;
        impl->pbo->create(siz);
    }

    void *buf = impl->pbo->map(Access_WriteOnly, Target_PixelUnPack);
    bsASSERT(((uintptr_t)(buf)) % 4 == 0, "texture (pbo map ptr) error!");

    return buf;
}
void opengl_texture::upload()
{
    if (!impl->pbo)
        return;
    uint row_size = next_multiple_of_4(size.width * pixel_byte);

    impl->pbo->unmap(Target_PixelUnPack);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, row_size / pixel_byte);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.width, size.height,
                    impl::returnDataFormat(format), impl::returnDataDepth(depth), 0);
    unbind();
    impl->pbo->unbind(Target_PixelUnPack);

    if (level_max > 0)
        genmipmap();
}
void opengl_texture::upload(const isize &_size, void *data)
{
    if ((_size != size) || (!id))
    {
        size = _size;
        if (id)
            glDeleteTextures(1, (uint*)&id);

        create();
    }

    if (is_compressed)
    {
        glBindTexture(GL_TEXTURE_2D, id);
        glCompressedTexSubImage2D(GL_TEXTURE_2D, 0, 0,0,
                                  size.width, size.height, impl::returnDataFormat(format),
                                  size.width * size.height* impl::returnFormatByte(internal), data);
    }
    else
    {
        int row_size = next_multiple_of_4(size.width * pixel_byte);
        int siz = row_size * size.height;

        if (!impl->pbo ||(impl->pbo->size !=siz))
        {
            impl->pbo =  bs_malloc (opengl_buffer_object);
            impl->pbo->target = Target_PixelUnPack;
            impl->pbo->usage = Usage_DynamicDraw;
            impl->pbo->create(siz);
        }

        impl->pbo->bind (Target_PixelUnPack);
        void *buf = impl->pbo->map(Access_WriteOnly, Target_PixelUnPack);
        bsASSERT(((uintptr_t)(buf)) % 4 != 0, "texture (pbo map ptr) error!");

        bs_memory::copy_sse(buf, data, siz);

        impl->pbo->unmap(Target_PixelUnPack);

        glPixelStorei(GL_UNPACK_ROW_LENGTH, row_size / pixel_byte);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        bind();
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.width, size.height,
                        impl::returnDataFormat(format), impl::returnDataDepth(depth), 0);
        unbind();
        impl->pbo->unbind(Target_PixelUnPack);
    }

    if (level_max > 0)
        genmipmap();
}
void opengl_texture::bind(uint mid)
{
    glGetIntegerv(GL_TEXTURE_BINDING_2D, (int*)&impl->id_bak);
    glGetIntegerv(GL_ACTIVE_TEXTURE, (int*)&impl->active_texun_bak);
    glActiveTexture(GL_TEXTURE0 + mid);
    glClientActiveTexture(GL_TEXTURE0 + mid);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, id);
}
void opengl_texture::unbind(uint mid)
{
    glActiveTexture(GL_TEXTURE0 + mid);
    glClientActiveTexture(GL_TEXTURE0 + mid);
    glDisable(GL_TEXTURE_2D);
    glActiveTexture(impl->active_texun_bak);
    glBindTexture(GL_TEXTURE_2D, impl->id_bak);
}

bs_image opengl_texture::read(const pitexture &tex, int level)
{
    int width, height;
    int r, g, b, a, depth, format;

    tex->bind();
    glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_HEIGHT, &height);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_RED_SIZE, &r);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_GREEN_SIZE, &g);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_BLUE_SIZE, &b);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_ALPHA_SIZE, &a);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_DEPTH_SIZE, &depth);

    uint dpp = 0;
    if(b > 0)
    {
        if(a > 0)
        {
            format = GL_RGBA;
            dpp = 4;
        }
        else
        {
            format = GL_RGB;
            dpp = 3;
        }
    }
    else if(depth > 0)
    {
        format = GL_DEPTH_COMPONENT;
        dpp = 1;
    }

    if(dpp > 0)
    {
        uchar *_dat = bsNew(uchar[width *height *dpp]);

        glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, _dat);
        impl->read_data = bs_image(width ,height, _dat,
                              format == GL_RGBA ? ImageData_RGBA :
                              format == GL_RGB ? ImageData_RGB : ImageData_Grey);
        bsDelete([]_dat) ;
    }
    tex->unbind();
    return impl->read_data;
}
void opengl_texture::copy(const ivec4 &viewport, int level, const ipoint &off )
{
    glCopyTexSubImage2D(GL_TEXTURE_2D, level, off.x, off.y,
                        viewport.x, viewport.y, viewport.width, viewport.height);
}

void opengl_texture::genmipmap()
{
    glGenerateMipmap(GL_TEXTURE_2D);
}

bool opengl_texture::load(const bs_string &f)
{
    file = f;
    bs_image img(file);
    if (img.is_null())
        return false;
    this->update (img.size(), img.bits());
    return true;
}

bool opengl_texture::save(const bs_string &file, ImageWirteFormat form)
{
    int width, height;
    int r, g, b, a, depth, format;

    this->bind();
    glGetTexLevelParameteriv(GL_TEXTURE_2D, this->level_max, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, this->level_max, GL_TEXTURE_HEIGHT, &height);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, this->level_max, GL_TEXTURE_RED_SIZE, &r);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, this->level_max, GL_TEXTURE_GREEN_SIZE, &g);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, this->level_max, GL_TEXTURE_BLUE_SIZE, &b);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, this->level_max, GL_TEXTURE_ALPHA_SIZE, &a);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, this->level_max, GL_TEXTURE_DEPTH_SIZE, &depth);

    uint dpp = 0;
    if(b > 0)
    {
        if(a > 0)
        {
            format = GL_RGBA;
            dpp = 4;
        }
        else
        {
            format = GL_RGB;
            dpp = 3;
        }
    }
    else if(depth > 0)
    {
        format = GL_DEPTH_COMPONENT;
        dpp = 1;
    }

    if(dpp > 0)
    {
        uchar *_dat = bsNew(uchar[width *height *dpp]);

        glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, _dat);
        impl->read_data = bs_image(width ,height, _dat,
                              format == GL_RGBA ? ImageData_RGBA :
                              format == GL_RGB ? ImageData_RGB : ImageData_Grey);
        bsDelete([]_dat) ;
    }
    this->unbind();

    return impl->read_data.save(file, form);
}

