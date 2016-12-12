#include "opengl_shader_program.h"
#include "bs_filesystem.h"
#include "opengl_mx.h"

static GLenum retureType(eShaderTypes t)
{
    switch ((int)t)
    {
    case Shader_Vertex:
        return GL_VERTEX_SHADER;
    case Shader_Fragment:
        return GL_FRAGMENT_SHADER;
    case Shader_Geometry:
        return GL_GEOMETRY_SHADER;
    case Shader_TessControl:
        return GL_TESS_CONTROL_SHADER;
    case Shader_TessEvaluation:
        return GL_TESS_EVALUATION_SHADER;
    case Shader_Compute:
        return GL_COMPUTE_SHADER;
    }
    return GL_VERTEX_SHADER;
}

void opengl_shader::destroy()
{
    if (id)
        glDeleteShader(id);
    ishader::destroy();
}
int opengl_shader::create(eShaderTypes t)
{
    if ((t == type) && id)
        return id;
    else if ((type != t) || (!id))
    {
        if (id)
        {
            glDeleteShader(id);
            is_compile = false;
            id = 0;
        }
        id = glCreateShader(retureType(t));
    }
    return ishader::create(t);
}

bs_string opengl_shader::compile(const bs_string &source)
{
    if ((source != _source) || (source.is_empty() && (!_source.is_empty())))
    {
        if (!source.is_empty())
            _source = source;
        bs_utf8 utf8 = _source.to_utf8();
        char *data = (char*)utf8.data();
        glShaderSource(id, 1, (const GLchar**)&(data), NULL);
        glCompileShader(id);

        // check the compilation
        GLint compile_status = GL_TRUE;
        glGetShaderiv(id, GL_COMPILE_STATUS,  &compile_status);

        if(compile_status != GL_TRUE)
        {
            GLint logSize;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logSize);

            char *log = bsNew (char[logSize+1]); // +1 <=> '/0'
            memset(log, 0, logSize+1);

            glGetShaderInfoLog(id, logSize, &logSize, log);
            log[logSize] = 0;

            bs_string str = bs_string(log);
            bsDelete([] log);
            return str;
        }
    }
    is_compile = true;
    return bs_string();
}
void opengl_shader::detach(const uint &pro_id)
{
    glDetachShader(pro_id, id);
}
void opengl_shader::attach(const uint &pro_id)
{
    glAttachShader(pro_id, id);
}
opengl_program::opengl_program():
    iprogram(),
    _id_bak(0)
{

}


int opengl_program::create()
{
    id = glCreateProgram();
    return iprogram::create();
}
void opengl_program::destroy()
{
    for (size_t i = 0; i < shader.size(); i++)
    {
        glDetachShader(id, shader[i]->id);
        shader[i]->is_attach = false;
    }

    if (id)
        glDeleteProgram(id);
}
bs_string opengl_program::link(void)
{
    glLinkProgram(id);

    // Look for some error during the linking
    GLint link_status = GL_TRUE;
    glGetProgramiv(id, GL_LINK_STATUS, &link_status);
    if(link_status != GL_TRUE)
    {
        // get the log
        GLint logSize;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logSize);

        char *log = bsNew (char[logSize+1]); // +1 <=> '/0'
        memset(log, 0, logSize+1);

        glGetProgramInfoLog(id, logSize, &logSize, log);
        log[logSize] = 0;
        bs_string str = bs_string(log);
        bsDelete ([] log);
        return str;
    }
    is_link = true;
    return bs_string();
}

void opengl_program::bind()
{
    if (!is_link)
        link();

    glGetIntegerv(GL_CURRENT_PROGRAM, (int*)&_id_bak);
    glUseProgram(id);
}
void opengl_program::unbind()
{
    glUseProgram(_id_bak);
}

bool opengl_program::uniform(const char*name, int values)
{
    GLint uValue = glGetUniformLocation(id, name);
    if(uValue != -1)
        glUniform1i(uValue, values);
    else return false;
    return true;
}

bool opengl_program::uniform(const char*name, float values)
{
    GLint uValue = glGetUniformLocation(id, name);
    if(uValue != -1)
        glUniform1f(uValue, values);
    else return false;
    return true;
}

bool opengl_program::uniform(const char*name, const bs_vector2f &values)
{
    GLint uValue = glGetUniformLocation(id, name);
    if(uValue != -1)
        glUniform2fv(uValue, 1, values.elem);
    else return false;
    return true;
}

bool opengl_program::uniform(const char*name, const bs_vector3f &values)
{
    GLint uValue = glGetUniformLocation(id, name);
    if(uValue != -1)
        glUniform3fv(uValue, 1, values.elem);
    else return false;
    return true;
}

bool opengl_program::uniform(const char*name, const vec4 &values)
{
    GLint uValue = glGetUniformLocation(id, name);
    if(uValue != -1)
        glUniform4fv(uValue, 1, values.elem);
    else return false;
    return true;
}
bool opengl_program::uniform(const char* name, const bs_color& values)
{
    GLint uValue = glGetUniformLocation(id, name);
    if(uValue != -1)
        glUniform4fv(uValue, 1, values.elem);
    else return false;
    return true;
}
bool opengl_program::uniform(const char*name, const mat4 &matrix, const bool transpose)
{
    GLint uValue = glGetUniformLocation(id, name);
    if(uValue != -1)
        glUniformMatrix4fv(uValue, 1, transpose, matrix.elem);
    else return false;
    return true;
}
bool opengl_program::get(const char* name, mat4 &matrix)
{
    GLfloat buf[4*4] = {0.0f};
    GLint uValue = glGetUniformLocation(id, name);
    if(uValue != -1)
        glGetUniformfv(id, uValue, buf);
    else
        return false;

    matrix = mat4(buf);
    return true;
}
void opengl_program::bind(const char* name, int AttribLocation)
{
    glBindAttribLocation(id, AttribLocation, name);
}
uint opengl_program::location(const char *name)
{
    return glGetAttribLocation(id, name);
}

