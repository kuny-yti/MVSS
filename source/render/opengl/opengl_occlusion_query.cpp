#include "opengl_occlusion_query.h"
#include "opengl_render.h"

opengl_occlusion_query::opengl_occlusion_query()
{
    handle = 0;
}
opengl_occlusion_query::~opengl_occlusion_query()
{
    if (handle)
        glDeleteQueries(1, &handle);
}

void opengl_occlusion_query::begin()
{
    glBeginQuery(GL_SAMPLES_PASSED, handle);
}
void opengl_occlusion_query::end()
{
    glEndQuery(GL_SAMPLES_PASSED);
}
bool opengl_occlusion_query::is_result()
{
    int available;
    glGetQueryObjectiv(handle, GL_QUERY_RESULT_AVAILABLE, &available);
    return (available != 0);
}
uint opengl_occlusion_query::result_uint ()
{
    GLuint result_;
    glGetQueryObjectuiv(handle, GL_QUERY_RESULT, &result_);
    return result_;
}
int opengl_occlusion_query::result_int ()
{
    GLint result_;
    glGetQueryObjectiv(handle, GL_QUERY_RESULT, &result_);
    return result_;
}
int64 opengl_occlusion_query::result_int64 ()
{
    GLint64 result_;
    glGetQueryObjecti64v(handle, GL_QUERY_RESULT, &result_);
    return result_;
}
uint64 opengl_occlusion_query::result_uint64 ()
{
    GLuint64 result_;
    glGetQueryObjectui64v(handle, GL_QUERY_RESULT, &result_);
    return result_;
}
void opengl_occlusion_query::create()
{
    if (handle)
    {
        glDeleteQueries (1, &handle);
        handle = 0;
    }
    glGenQueries(1, &handle);
}
