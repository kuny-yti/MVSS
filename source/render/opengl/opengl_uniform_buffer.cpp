#include "opengl_uniform_buffer.h"
#include "opengl_mx.h"

#include "tools/bs_map.h"
#include "tools/bs_algorlthm.h"

static bs_map<int, int> _type_size;
static bs_vector<int> _binding_point;

static bs_map<int, int> matrixRowSize;
static bs_map<int,int> matrixRowCount;
static bs_map<int, std::string> spGLSLType;

static void init_type_size()
{
    _type_size.clear();
    _type_size[GL_FLOAT] = sizeof(float);
    _type_size[GL_FLOAT_VEC2] = sizeof(float)*2;
    _type_size[GL_FLOAT_VEC3] = sizeof(float)*3;
    _type_size[GL_FLOAT_VEC4] = sizeof(float)*4;

    _type_size[GL_DOUBLE] = sizeof(double);
    _type_size[GL_DOUBLE_VEC2] = sizeof(double)*2;
    _type_size[GL_DOUBLE_VEC3] = sizeof(double)*3;
    _type_size[GL_DOUBLE_VEC4] = sizeof(double)*4;

    _type_size[GL_SAMPLER_1D] = sizeof(int);
    _type_size[GL_SAMPLER_2D] = sizeof(int);
    _type_size[GL_SAMPLER_3D] = sizeof(int);
    _type_size[GL_SAMPLER_CUBE] = sizeof(int);
    _type_size[GL_SAMPLER_1D_SHADOW] = sizeof(int);
    _type_size[GL_SAMPLER_2D_SHADOW] = sizeof(int);
    _type_size[GL_SAMPLER_1D_ARRAY] = sizeof(int);
    _type_size[GL_SAMPLER_2D_ARRAY] = sizeof(int);
    _type_size[GL_SAMPLER_1D_ARRAY_SHADOW] = sizeof(int);
    _type_size[GL_SAMPLER_2D_ARRAY_SHADOW] = sizeof(int);
    _type_size[GL_SAMPLER_2D_MULTISAMPLE] = sizeof(int);
    _type_size[GL_SAMPLER_2D_MULTISAMPLE_ARRAY] = sizeof(int);
    _type_size[GL_SAMPLER_CUBE_SHADOW] = sizeof(int);
    _type_size[GL_SAMPLER_BUFFER] = sizeof(int);
    _type_size[GL_SAMPLER_2D_RECT] = sizeof(int);
    _type_size[GL_SAMPLER_2D_RECT_SHADOW] = sizeof(int);
    _type_size[GL_INT_SAMPLER_1D] = sizeof(int);
    _type_size[GL_INT_SAMPLER_2D] = sizeof(int);
    _type_size[GL_INT_SAMPLER_3D] = sizeof(int);
    _type_size[GL_INT_SAMPLER_CUBE] = sizeof(int);
    _type_size[GL_INT_SAMPLER_1D_ARRAY] = sizeof(int);
    _type_size[GL_INT_SAMPLER_2D_ARRAY] = sizeof(int);
    _type_size[GL_INT_SAMPLER_2D_MULTISAMPLE] = sizeof(int);
    _type_size[GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY] = sizeof(int);
    _type_size[GL_INT_SAMPLER_BUFFER] = sizeof(int);
    _type_size[GL_INT_SAMPLER_2D_RECT] = sizeof(int);
    _type_size[GL_UNSIGNED_INT_SAMPLER_1D] = sizeof(int);
    _type_size[GL_UNSIGNED_INT_SAMPLER_2D] = sizeof(int);
    _type_size[GL_UNSIGNED_INT_SAMPLER_3D] = sizeof(int);
    _type_size[GL_UNSIGNED_INT_SAMPLER_CUBE] = sizeof(int);
    _type_size[GL_UNSIGNED_INT_SAMPLER_1D_ARRAY] = sizeof(int);
    _type_size[GL_UNSIGNED_INT_SAMPLER_2D_ARRAY] = sizeof(int);
    _type_size[GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE] = sizeof(int);
    _type_size[GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY] = sizeof(int);
    _type_size[GL_UNSIGNED_INT_SAMPLER_BUFFER] = sizeof(int);
    _type_size[GL_UNSIGNED_INT_SAMPLER_2D_RECT] = sizeof(int);
    _type_size[GL_BOOL] = sizeof(int);
    _type_size[GL_INT] = sizeof(int);
    _type_size[GL_BOOL_VEC2] = sizeof(int)*2;
    _type_size[GL_INT_VEC2] = sizeof(int)*2;
    _type_size[GL_BOOL_VEC3] = sizeof(int)*3;
    _type_size[GL_INT_VEC3] = sizeof(int)*3;
    _type_size[GL_BOOL_VEC4] = sizeof(int)*4;
    _type_size[GL_INT_VEC4] = sizeof(int)*4;

    _type_size[GL_UNSIGNED_INT] = sizeof(int);
    _type_size[GL_UNSIGNED_INT_VEC2] = sizeof(int)*2;
    _type_size[GL_UNSIGNED_INT_VEC3] = sizeof(int)*2;
    _type_size[GL_UNSIGNED_INT_VEC4] = sizeof(int)*2;

    _type_size[GL_FLOAT_MAT2] = sizeof(float)*4;
    _type_size[GL_FLOAT_MAT3] = sizeof(float)*9;
    _type_size[GL_FLOAT_MAT4] = sizeof(float)*16;
    _type_size[GL_FLOAT_MAT2x3] = sizeof(float)*6;
    _type_size[GL_FLOAT_MAT2x4] = sizeof(float)*8;
    _type_size[GL_FLOAT_MAT3x2] = sizeof(float)*6;
    _type_size[GL_FLOAT_MAT3x4] = sizeof(float)*12;
    _type_size[GL_FLOAT_MAT4x2] = sizeof(float)*8;
    _type_size[GL_FLOAT_MAT4x3] = sizeof(float)*12;
    _type_size[GL_DOUBLE_MAT2] = sizeof(double)*4;
    _type_size[GL_DOUBLE_MAT3] = sizeof(double)*9;
    _type_size[GL_DOUBLE_MAT4] = sizeof(double)*16;


    spGLSLType[GL_FLOAT] = "GL_FLOAT";
    spGLSLType[GL_FLOAT_VEC2] = "GL_FLOAT_VEC2";
    spGLSLType[GL_FLOAT_VEC3] = "GL_FLOAT_VEC3";
    spGLSLType[GL_FLOAT_VEC4] = "GL_FLOAT_VEC4";
    spGLSLType[GL_DOUBLE] = "GL_DOUBLE";
    spGLSLType[GL_DOUBLE_VEC2] = "GL_DOUBLE_VEC2";
    spGLSLType[GL_DOUBLE_VEC3] = "GL_DOUBLE_VEC3";
    spGLSLType[GL_DOUBLE_VEC4] = "GL_DOUBLE_VEC4";
    spGLSLType[GL_SAMPLER_1D] = "GL_SAMPLER_1D";
    spGLSLType[GL_SAMPLER_2D] = "GL_SAMPLER_2D";
    spGLSLType[GL_SAMPLER_3D] = "GL_SAMPLER_3D";
    spGLSLType[GL_SAMPLER_CUBE] = "GL_SAMPLER_CUBE";
    spGLSLType[GL_SAMPLER_1D_SHADOW] = "GL_SAMPLER_1D_SHADOW";
    spGLSLType[GL_SAMPLER_2D_SHADOW] = "GL_SAMPLER_2D_SHADOW";
    spGLSLType[GL_SAMPLER_1D_ARRAY] = "GL_SAMPLER_1D_ARRAY";
    spGLSLType[GL_SAMPLER_2D_ARRAY] = "GL_SAMPLER_2D_ARRAY";
    spGLSLType[GL_SAMPLER_1D_ARRAY_SHADOW] = "GL_SAMPLER_1D_ARRAY_SHADOW";
    spGLSLType[GL_SAMPLER_2D_ARRAY_SHADOW] = "GL_SAMPLER_2D_ARRAY_SHADOW";
    spGLSLType[GL_SAMPLER_2D_MULTISAMPLE] = "GL_SAMPLER_2D_MULTISAMPLE";
    spGLSLType[GL_SAMPLER_2D_MULTISAMPLE_ARRAY] = "GL_SAMPLER_2D_MULTISAMPLE_ARRAY";
    spGLSLType[GL_SAMPLER_CUBE_SHADOW] = "GL_SAMPLER_CUBE_SHADOW";
    spGLSLType[GL_SAMPLER_BUFFER] = "GL_SAMPLER_BUFFER";
    spGLSLType[GL_SAMPLER_2D_RECT] = "GL_SAMPLER_2D_RECT";
    spGLSLType[GL_SAMPLER_2D_RECT_SHADOW] = "GL_SAMPLER_2D_RECT_SHADOW";
    spGLSLType[GL_INT_SAMPLER_1D] = "GL_INT_SAMPLER_1D";
    spGLSLType[GL_INT_SAMPLER_2D] = "GL_INT_SAMPLER_2D";
    spGLSLType[GL_INT_SAMPLER_3D] = "GL_INT_SAMPLER_3D";
    spGLSLType[GL_INT_SAMPLER_CUBE] = "GL_INT_SAMPLER_CUBE";
    spGLSLType[GL_INT_SAMPLER_1D_ARRAY] = "GL_INT_SAMPLER_1D_ARRAY";
    spGLSLType[GL_INT_SAMPLER_2D_ARRAY] = "GL_INT_SAMPLER_2D_ARRAY";
    spGLSLType[GL_INT_SAMPLER_2D_MULTISAMPLE] = "GL_INT_SAMPLER_2D_MULTISAMPLE";
    spGLSLType[GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY] = "GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY";
    spGLSLType[GL_INT_SAMPLER_BUFFER] = "GL_INT_SAMPLER_BUFFER";
    spGLSLType[GL_INT_SAMPLER_2D_RECT] = "GL_INT_SAMPLER_2D_RECT";
    spGLSLType[GL_UNSIGNED_INT_SAMPLER_1D] = "GL_UNSIGNED_INT_SAMPLER_1D";
    spGLSLType[GL_UNSIGNED_INT_SAMPLER_2D] = "GL_UNSIGNED_INT_SAMPLER_2D";
    spGLSLType[GL_UNSIGNED_INT_SAMPLER_3D] = "GL_UNSIGNED_INT_SAMPLER_3D";
    spGLSLType[GL_UNSIGNED_INT_SAMPLER_CUBE] = "GL_UNSIGNED_INT_SAMPLER_CUBE";
    spGLSLType[GL_UNSIGNED_INT_SAMPLER_1D_ARRAY] = "GL_UNSIGNED_INT_SAMPLER_1D_ARRAY";
    spGLSLType[GL_UNSIGNED_INT_SAMPLER_2D_ARRAY] = "GL_UNSIGNED_INT_SAMPLER_2D_ARRAY";
    spGLSLType[GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE] = "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE";
    spGLSLType[GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY] = "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY";
    spGLSLType[GL_UNSIGNED_INT_SAMPLER_BUFFER] = "GL_UNSIGNED_INT_SAMPLER_BUFFER";
    spGLSLType[GL_UNSIGNED_INT_SAMPLER_2D_RECT] = "GL_UNSIGNED_INT_SAMPLER_2D_RECT";
    spGLSLType[GL_BOOL] = "GL_BOOL";
    spGLSLType[GL_INT] = "GL_INT";
    spGLSLType[GL_BOOL_VEC2] = "GL_BOOL_VEC2";
    spGLSLType[GL_INT_VEC2] = "GL_INT_VEC2";
    spGLSLType[GL_BOOL_VEC3] = "GL_BOOL_VEC3";
    spGLSLType[GL_INT_VEC3] = "GL_INT_VEC3";
    spGLSLType[GL_BOOL_VEC4] = "GL_BOOL_VEC4";
    spGLSLType[GL_INT_VEC4] = "GL_INT_VEC4";
    spGLSLType[GL_UNSIGNED_INT] = "GL_UNSIGNED_INT";
    spGLSLType[GL_UNSIGNED_INT_VEC2] = "GL_UNSIGNED_INT_VEC2";
    spGLSLType[GL_UNSIGNED_INT_VEC3] = "GL_UNSIGNED_INT_VEC3";
    spGLSLType[GL_UNSIGNED_INT_VEC4] = "GL_UNSIGNED_INT_VEC4";
    spGLSLType[GL_FLOAT_MAT2] = "GL_FLOAT_MAT2";
    spGLSLType[GL_FLOAT_MAT3] = "GL_FLOAT_MAT3";
    spGLSLType[GL_FLOAT_MAT4] = "GL_FLOAT_MAT4";
    spGLSLType[GL_FLOAT_MAT2x3] = "GL_FLOAT_MAT2x3";
    spGLSLType[GL_FLOAT_MAT2x4] = "GL_FLOAT_MAT2x4";
    spGLSLType[GL_FLOAT_MAT3x2] = "GL_FLOAT_MAT3x2";
    spGLSLType[GL_FLOAT_MAT3x4] = "GL_FLOAT_MAT3x4";
    spGLSLType[GL_FLOAT_MAT4x2] = "GL_FLOAT_MAT4x2";
    spGLSLType[GL_FLOAT_MAT4x3] = "GL_FLOAT_MAT4x3";
    spGLSLType[GL_DOUBLE_MAT2] = "GL_DOUBLE_MAT2";
    spGLSLType[GL_DOUBLE_MAT3] = "GL_DOUBLE_MAT3";
    spGLSLType[GL_DOUBLE_MAT4] = "GL_DOUBLE_MAT4";

    matrixRowSize[GL_FLOAT_MAT2] = sizeof(float) * 2;
    matrixRowSize[GL_FLOAT_MAT3] = sizeof(float) * 3;
    matrixRowSize[GL_FLOAT_MAT4] = sizeof(float) * 4;
    matrixRowSize[GL_FLOAT_MAT2x3] = sizeof(float) * 2;
    matrixRowSize[GL_FLOAT_MAT2x4] = sizeof(float) * 2;
    matrixRowSize[GL_FLOAT_MAT3x2] = sizeof(float) * 3;
    matrixRowSize[GL_FLOAT_MAT3x4] = sizeof(float) * 3;
    matrixRowSize[GL_FLOAT_MAT4x2] = sizeof(float) * 4;
    matrixRowSize[GL_FLOAT_MAT4x3] = sizeof(float) * 4;
    matrixRowSize[GL_DOUBLE_MAT2] = sizeof(double) * 2;
    matrixRowSize[GL_DOUBLE_MAT3] = sizeof(double) * 3;
    matrixRowSize[GL_DOUBLE_MAT4] = sizeof(double) * 4;

    matrixRowCount[GL_FLOAT_MAT2] =  2;
    matrixRowCount[GL_FLOAT_MAT3] =  3;
    matrixRowCount[GL_FLOAT_MAT4] =  4;
    matrixRowCount[GL_FLOAT_MAT2x3] =  2;
    matrixRowCount[GL_FLOAT_MAT2x4] =  2;
    matrixRowCount[GL_FLOAT_MAT3x2] =  3;
    matrixRowCount[GL_FLOAT_MAT3x4] =  3;
    matrixRowCount[GL_FLOAT_MAT4x2] =  4;
    matrixRowCount[GL_FLOAT_MAT4x3] =  4;
    matrixRowCount[GL_DOUBLE_MAT2] =  2;
    matrixRowCount[GL_DOUBLE_MAT3] =  3;
    matrixRowCount[GL_DOUBLE_MAT4] =  4;
}

opengl_uniform_buffer::opengl_uniform_buffer():
    opengl_buffer_object()
{
    init_type_size();
    block.binding = 0;
    block.data.clear();
    block.index = 0;
    block.name = "";
    block.size = 0;
    id_bak = 0;
}
opengl_uniform_buffer::~opengl_uniform_buffer()
{
    _binding_point[block.binding] = false;
}
void opengl_uniform_buffer::create_ubo (int size)
{
    if (!opengl_buffer_object::id )
    {
        opengl_buffer_object::target = Target_Uniform;
        opengl_buffer_object::usage = Usage_DynamicDraw;
        opengl_buffer_object::create(size);
    }
}

int opengl_uniform_buffer::binding_point()const
{
    for (size_t i = 0; i < _binding_point.size(); i++)
    {
        if (!_binding_point[i])
        {
            _binding_point[i] = true;
            return i;
        }
    }
    _binding_point.append(true);
    return _binding_point.size()-1 ;
}
int opengl_uniform_buffer::uniform(const piprogram &pro, const bs_string &block_name)
{
    if (!pro.get() && block_name.is_empty ())
        return -1;

    int bufferSize = 0;
    int activeUnif = 0;
    int _member_len = 0;
    int _member_type = 0;
    int _member_offset = 0;
    int _member_size = 0;
    int _member_array_stride = 0;
    int _member_mat_stride = 0;
    char _member_name[256]= {0};

    block.data.clear();
    // 获取着色器中的block索引
    uint blockIndex = glGetUniformBlockIndex(pro->id, (char*)block_name.data());
    if (blockIndex == GL_INVALID_INDEX)
        return -1;

    // 获取block数据大小
    glGetActiveUniformBlockiv(pro->id, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &bufferSize);
    // 获取block成员数
    glGetActiveUniformBlockiv(pro->id, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &activeUnif);

    // 获取block成员索引
    uint *_indices = new uint[activeUnif];
    glGetActiveUniformBlockiv(pro->id, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, (int *)_indices);


    // 处理block成员信息
    for (int j = 0; j < activeUnif; j++)
    {
        // 获取block成员名称
        bs_memory::set_c(_member_name, 0, sizeof(_member_name));
        glGetActiveUniformName(pro->id, _indices[j], 256, &_member_len, _member_name);
        // 获取block成员数据类型
        glGetActiveUniformsiv(pro->id, 1, &_indices[j], GL_UNIFORM_TYPE, &_member_type);
        // 获取block成员偏移量
        glGetActiveUniformsiv(pro->id, 1, &_indices[j], GL_UNIFORM_OFFSET, &_member_offset);

        // 获取block成员大小
        glGetActiveUniformsiv(pro->id, 1, &_indices[j], GL_UNIFORM_SIZE, &_member_size);
        // 获取block成员数组步数
        glGetActiveUniformsiv(pro->id, 1, &_indices[j], GL_UNIFORM_ARRAY_STRIDE, &_member_array_stride);
        // 获取block成员矩阵步数
        glGetActiveUniformsiv(pro->id, 1, &_indices[j], GL_UNIFORM_MATRIX_STRIDE, &_member_mat_stride);

        int auxSize;
        if (_member_array_stride > 0)
            auxSize = _member_array_stride * _member_size;
        else if (_member_mat_stride > 0)
        {
            switch(_member_type)
            {
                case GL_FLOAT_MAT2:
                case GL_FLOAT_MAT2x3:
                case GL_FLOAT_MAT2x4:
                case GL_DOUBLE_MAT2:
                    auxSize = 2 * _member_mat_stride;
                    break;
                case GL_FLOAT_MAT3:
                case GL_FLOAT_MAT3x2:
                case GL_FLOAT_MAT3x4:
                case GL_DOUBLE_MAT3:
                    auxSize = 3 * _member_mat_stride;
                    break;
                case GL_FLOAT_MAT4:
                case GL_FLOAT_MAT4x2:
                case GL_FLOAT_MAT4x3:
                case GL_DOUBLE_MAT4:
                    auxSize = 4 * _member_mat_stride;
                    break;
            }
        }
        else
        {
            auxSize = _type_size[_member_type];
        }

        tUniformDatas info;
        info.name = bs_string(_member_name);
        info.type = _member_type;
        info.offest = _member_offset;
        info.size = auxSize;
        info.array_stride = _member_array_stride;
        info.matrix_stride = _member_mat_stride;
        block.data.append( info);
    }

    block.size = bufferSize;

    // 新建一个ubo
    this->create_ubo(bufferSize);
    // 关联ubo和block位置
    block.binding = binding_point();
    glBindBufferBase(GL_UNIFORM_BUFFER, block.binding, opengl_buffer_object::id);

    block.index = blockIndex;
    block.name = block_name;

    //_block.sort();
    //std::sort(_block.data.begin(), _block.data.end());
    bs_qsort<bs_vector <tUniformDatas>::iterator>(block.data.begin(), block.data.end());
    delete []_indices;

    return blockIndex;
}
void opengl_uniform_buffer::bind(const piprogram &pro)
{
    glGetIntegerv(GL_UNIFORM_BLOCK_BINDING, (int*)&id_bak);
    // 绑定着色器位置
    glUniformBlockBinding(pro->id, block.index, block.binding);
}
void opengl_uniform_buffer::bind(const piprogram &pro, int binding)
{
    glGetIntegerv(GL_UNIFORM_BLOCK_BINDING, (int*)&id_bak);
    // 绑定着色器位置
    glUniformBlockBinding(pro->id, block.index, binding);
}
void opengl_uniform_buffer::wirte (void *data, const bs_string &member)
{
    char *dataPtr = (char *)data;
    if (member.is_empty ())//块写
    {
        // start with a zero buffer equal to the size of the layout
        std::vector<char> buffer(block.size, '0');

        int dataOffest = 0;
        // loop through each unifom of bufferLayout, and add them to buffer
        for (size_t i = 0; i < block.data.size(); i++)
        {
            const tUniformDatas &uniform = block.data[i];
            // There are 4 basics types to handle: matrix, matrix array, regular type, and regular type array.
            if (uniform.array_stride > 0 && uniform.matrix_stride > 0)
            { // is it a matrix array
                int matrixOffset = uniform.offest;
                int arrayCount = uniform.size / uniform.array_stride;
                for (int j = 0; j < arrayCount; j++)
                {
                    int rowOffset = matrixOffset;
                    // This loop uploads one matrix,one row at a time
                    for (int k = 0; k < matrixRowCount[uniform.type]; k++)
                    {
                        bs_memory::copy_sse(buffer.data() + rowOffset, dataPtr + dataOffest, matrixRowSize[uniform.type]);
                        dataOffest += matrixRowSize[uniform.type];
                        rowOffset += uniform.matrix_stride;
                    }
                    matrixOffset += uniform.array_stride;
                }
            }
            else if (uniform.array_stride == 0 && uniform.matrix_stride > 0 )
            { // is it a matrix
                int rowOffset = uniform.offest;
                for (int k = 0; k < matrixRowCount[uniform.type]; k++)
                {
                    bs_memory::copy_sse(buffer.data() + rowOffset, dataPtr + dataOffest, matrixRowSize[uniform.type]);
                    dataOffest += matrixRowSize[uniform.type];
                    rowOffset += uniform.matrix_stride;
                }
            }
            else if (uniform.array_stride >0 && uniform.matrix_stride == 0)
            {// is it a regular array
                int offset = uniform.offest;
                int arrayCount = uniform.size / uniform.array_stride;
                for (int j = 0; j < arrayCount; j++)
                {
                    bs_memory::copy_sse(buffer.data() + offset, dataPtr + dataOffest, _type_size[uniform.type]);
                    dataOffest += _type_size[uniform.type];
                    offset += uniform.array_stride;
                }
            }
            else if (uniform.array_stride == 0 && uniform.matrix_stride == 0)
            {// is it a regular type
                int offset = uniform.offest;
                bs_memory::copy_sse(buffer.data() + offset, dataPtr + dataOffest, _type_size[uniform.type]);
                dataOffest += _type_size[uniform.type];
            }
        }
        opengl_buffer_object::writesub(buffer.data(), block.size, 0);
    }
    else//成员写
    {
        for (size_t i = 0; i < block.data.size(); i++)
        {
            if (member == block.data[i].name)
            {
                opengl_buffer_object::writesub(data, block.data[i].size, block.data[i].offest);
                return;
            }
        }
    }
}

