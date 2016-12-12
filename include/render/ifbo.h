#ifndef IFRAME_BUFFER_H
#define IFRAME_BUFFER_H

#include "rdefine.h"
#include "bs_ptr.h"
#include "maths/bs_vec4.h"
#include "itexture.h"

bsPackage itexture;
bsPackage ifbo;
#define iFbo bs_ptr(ifbo)
typedef iFbo pifbo;

bsPackage ifbo
{
    uint id;
    int samples;
    int samples_max;
    bool mipmap;
    isize size;
    eFrameBufferAttachs attachment;
    bs_vector <pitexture> texture;

    ifbo():
        id(0),
        samples(0),
        samples_max(0),
        mipmap(false),
        size(),
        attachment(Attach_Color0),
        texture()
    {}

    virtual ~ifbo(){destroy();}

    virtual bool has_fbo(){return true;}
    virtual bool has_fbo_blit(){return false;}

    virtual uint create(const isize &s) {size = s; return id;}
    virtual void destroy()
    {
        id = (0);
        samples = (0);
        samples_max = 0;
        mipmap = (false);
        size = isize();
        attachment = Attach_Color0;
        texture.clear();
    }

    virtual void blit(const pifbo &source, const pifbo &target,
                      const ivec4 &targetRect, const ivec4 &sourceRect,
                      eBufferTypes buffers = Buffer_Color,
                      eTextureParams filter = Filter_Nearest)=0;

    virtual const bs_image read(){return bs_image();}

    int target_count(void) const{return texture.size();}
    void clean_target() {texture.clear();}

    virtual int targetadd(const pitexture & = pitexture()) {return texture.size();}
    virtual void target(int usedTarget=0) = 0;

    virtual void begin(int usedTarget=0)= 0;
    virtual void end(int usedTarget=0)= 0;
    virtual void bind(void)= 0;
    virtual void unbind(void)= 0;
};

#endif // IFRAME_BUFFER_H
