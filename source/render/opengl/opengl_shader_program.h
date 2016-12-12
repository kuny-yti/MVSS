#ifndef SHADER_PROGRAM_IMPL_H
#define SHADER_PROGRAM_IMPL_H
#include "render/iglsl.h"

class opengl_shader : public ishader
{
private:
    bs_string _source;

    friend class opengl_program;
public:
    virtual void destroy();
    virtual int create(eShaderTypes type = Shader_None) ;
    virtual bs_string compile(const bs_string &source) ;

    virtual void detach(const uint &) ;
    virtual void attach(const uint &) ;
};

class opengl_program : public iprogram
{
private:
    uint _id_bak;

public:
    opengl_program();

    virtual int create() ;
    virtual void destroy();
    virtual bs_string link(void) ;

    virtual bool uniform(const char* name, int value);
    virtual bool uniform(const char* name, float value);
    virtual bool uniform(const char* name, const vec2& values);
    virtual bool uniform(const char* name, const vec3 &values);
    virtual bool uniform(const char* name, const vec4& values);
    virtual bool uniform(const char* name, const bs_color& values);
    virtual bool uniform(const char* name,const mat4 &matrix,
                         const bool transpose = false);
    virtual bool get(const char* name, mat4 &matrix);
    virtual void bind(const char* name, int AttribLocation);

    virtual uint location(const char* name);

    virtual void bind() ;
    virtual void unbind() ;
};
#endif // SHADER_PROGRAM_IMPL_H
