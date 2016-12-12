#ifndef ITEXTURE_H
#define ITEXTURE_H
#include "rdefine.h"
#include "bs_ptr.h"
#include "maths/bs_vec4.h"
#include "tools/bs_string.h"
#include "tools/bs_image.h"

typedef struct _TextureFormat_
{
    eInternalFormats internal;
    eDataFormats format;
    eDataDepths depth;
    isize size;
    eTextureParams wrap_s;
    eTextureParams wrap_t;
    eTextureParams filter_min;
    eTextureParams filter_mag;
    eTextureCombines combine;
    int level_base;
    int level_max;
    int pixel_byte;
    bool is_compressed;
    _TextureFormat_():
        wrap_s(Wrap_Repeat),
        wrap_t(Wrap_Repeat),
        filter_min(Filter_Linear),
        filter_mag(Filter_Linear),
        combine(Combine_Replace),
        format(Data_RGBA),
        depth(Data_UByte),
        internal(Internal_RGBA),
        level_base(0),
        level_max(0),
        size(),
        pixel_byte(4),
        is_compressed(false)
    {}
    bool operator != (const _TextureFormat_ &rhs){return !operator ==(rhs);}
    bool operator == (const _TextureFormat_ &rhs)
    {
        return internal == rhs.internal &&
                format == rhs.format &&
                depth == rhs.depth &&
                size == rhs.size &&
                wrap_s == rhs.wrap_s &&
                wrap_t == rhs.wrap_t &&
                filter_mag == rhs.filter_mag &&
                filter_min == rhs.filter_min &&
                combine == rhs.combine &&
                level_base == rhs.level_base &&
                level_max == rhs.level_max &&
                is_compressed == rhs.is_compressed &&
                pixel_byte == rhs.pixel_byte;
    }
}tTextureFormats;

#define iTexture bs_ptr(bsPackage itexture)
typedef iTexture pitexture;
bsPackage itexture : public tTextureFormats
{
    uint id;
    bs_string file;

    virtual ~itexture(){}

    virtual int units_max() = 0;
    virtual int size_max() = 0;
    virtual int size_gpu(int level) = 0;

    virtual bool load(const bs_string &f) {file = f;return true;}
    virtual bool save(const bs_string &, ImageWirteFormat ) {return true;}

    virtual void genmipmap() = 0;

    virtual void update(const isize &size, void *data) = 0;
    virtual void upload(const isize &size, void *data) = 0;
    virtual void *upload(const isize &size) = 0; // begin
    virtual void upload() = 0; // end

    virtual void bind(uint mid = 0) = 0;
    virtual void unbind(uint mid = 0) = 0;

    virtual bs_image read(const pitexture &tex, int level) = 0;
    virtual void copy(const ivec4 &viewport, int level = 0,
                        const ipoint &off = ipoint(0,0)) = 0;
};


#endif // ITEXTURE_H
