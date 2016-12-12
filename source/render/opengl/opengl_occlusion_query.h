#ifndef OCCLUSION_QUERY_IMPL_H
#define OCCLUSION_QUERY_IMPL_H

#include "iocclusion_query.h"
#include "opengl_render.h"


class opengl_occlusion_query : public iocclusion_query
{
    bsMemManage()
private:
    uint handle;
public:
    opengl_occlusion_query();
    ~opengl_occlusion_query();

    void begin();
    void end();

    bool is_result() ;

    uint result_uint();
    int result_int() ;
    int64 result_int64();
    uint64 result_uint64();

    void create();

};

#endif // OCCLUSION_QUERY_IMPL_H
