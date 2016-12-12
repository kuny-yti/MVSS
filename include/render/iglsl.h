#ifndef ISHADER_H
#define ISHADER_H
#include "rdefine.h"
#include "tools/bs_string.h"
#include "maths/bs_mat4X4.h"
#include "maths/bs_color.h"
#include "maths/bs_vec4.h"
#include "tools/bs_vector.h"
#include "bs_ptr.h"

#define iProgram bs_ptr(bsPackage iprogram)
typedef iProgram piprogram;
#undef iProgram

bsPackage ishader
{
    uint id;
    bs_string name;
    eShaderTypes type;
    bs_string _source;
    bool is_compile;
    bool is_attach;
    ishader():
        id(0),
        name(),
        type(Shader_None),
        is_compile(false),
        is_attach(false)
    {}
    virtual ~ishader(){destroy();}

    virtual int create(eShaderTypes t = Shader_None) {type = t;return id;}
    virtual void destroy()
    {
        id = 0;
        name = bs_string();
        type = Shader_None;
        is_compile = false;
        is_attach = false;
    }
    virtual void detach(const uint &) {is_attach = false;}
    virtual void attach(const uint &) {is_attach = true;}
    virtual bs_string compile(const bs_string &source) = 0;
};

#define iShader bs_ptr(ishader)
typedef iShader pishader;
#undef iShader
typedef bs_list <pishader> pishader_t;

bsPackage iprogram
{
    uint id;
    bool is_link;
    pishader_t shader;
    iprogram():
        id(0),
        is_link(false),
        shader()
    {
    }
    virtual ~iprogram(){destroy();}

    void append(const pishader &sr)
    {
        for (size_t i = 0; i < shader.count(); i++)
        {
            if (sr->name == shader[i]->name && sr->id == shader[i]->id)
            {
                shader[i]->detach(id);
                shader.remove(i);
                break;
            }
        }

        shader.append ( sr);
        for (size_t i =0 ; i < shader.size(); i++)
        {
            if (!shader[i]->is_attach)
                shader[i]->attach(id);
        }
    }
    void remove(const pishader &sr)
    {
        for (size_t i = 0; i < shader.size(); i++)
        {
            if (shader[i] == sr)
            {
                shader[i]->detach(id);
                shader.remove(i);
                return ;
            }
        }
    }

    virtual int create() {return id;}
    virtual void destroy()
    {
        id = 0;
        is_link = false;
        shader.clear();
    }
    virtual bs_string link(void) = 0;

    virtual bool uniform(const char* name, int value) = 0;
    virtual bool uniform(const char* name, float value) = 0;
    virtual bool uniform(const char* name, const vec2& values)= 0;
    virtual bool uniform(const char* name, const vec3& values)= 0;
    virtual bool uniform(const char* name, const vec4& values)= 0;
    virtual bool uniform(const char* name, const bs_color& values)= 0;
    virtual bool uniform(const char* name, const mat4 &matrix,
                         const bool transpose = false)= 0;
    virtual bool get(const char* name, mat4 &matrix) = 0;
    virtual void bind(const char* name, int AttribLocation)= 0;

    virtual void bind() = 0;
    virtual void unbind() = 0;
};


#endif // ISHADER_H
