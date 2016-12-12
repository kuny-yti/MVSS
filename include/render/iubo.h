#ifndef IUNIFORM_BUFFER_H
#define IUNIFORM_BUFFER_H

#include "ibobject.h"
#include "iglsl.h"

//! glsl:
//! layout(std140) uniform Block
//! {
//!     vec2 t1;
//!     vec3 t2;
//! }Block;
//!
//! C++:
//! struct Block
//! {
//!     float t1[2];//Vec2::Vec2
//!     float t2[3];//Vec3::Vec3
//! };
//!
//!

typedef struct _UniformData_
{
    bs_string name;
    int type;
    int offest;
    int size;
    int array_stride;
    int matrix_stride;

    bool operator < ( const _UniformData_ &v) const
    {
        return offest < v.offest;
    }
    friend void bs_swap(_UniformData_ &a, _UniformData_ &b)
    {
        std::swap(a, b);
    }
}tUniformDatas;

typedef struct
{
    bs_string name;
    int size;
    uint index;//block索引
    uint binding;//block绑定位置
    bs_vector <tUniformDatas> data;

}tUniformBlocks;

bsPackage iubo : ibobject
{
    tUniformBlocks block;

    virtual ~iubo(){}

    virtual int uniform(const piprogram &pro, const bs_string &block_name)= 0 ;
    virtual void bind(const piprogram &pro, int binding)= 0 ;
    virtual void bind(const piprogram &pro)= 0 ;
    virtual void wirte (void *data, const bs_string &member = bs_string())= 0 ;

};

#define iUbo bs_ptr(iubo)
typedef iUbo piubo;
#undef iUbo

#endif // IUNIFORM_BUFFER_H
