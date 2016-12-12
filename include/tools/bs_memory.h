/**
 * Basic tool library
 * Copyright (C) 2014 kunyang  kunyang.@gmail.com.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * You should have received a copy of the GNU General Public License.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file     bs_memory.h
 * @brief    内存的操作定义
 *       1.bs_alloc 简单的内存分配器
 *       2.bs_memory 内存的拷贝及清理的快速实现，可以评估那种加速方式更高效
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.0.1.0
 * @date     2014/02/20
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2013/05/01 | 1.0.0.1   | kunyang  | 创建文件
 * 2014/01/10 | 1.0.0.2   | kunyang  | 加入快速评估加速指令的选择
 * 2014/02/20 | 1.0.1.0   | kunyang  | 建立bs_alloc
 */

#ifndef BS_MEMCPY
#define BS_MEMCPY

#include "bs_define.h"

typedef enum
{
    CPU_SSE = 1<<0,
    CPU_SSE2 = 1<<1,
    CPU_SSE3 = 1<<2,
    CPU_MMX = 1<<3,
    CPU_MMXEXT = 1<<4,
    CPU_SSSE3 = 1<<5,
    CPU_3DNOW = 1<<6,
    CPU_3DNOWEXT = 1<<7,
    CPU_Unknown
}eCPUCapables;

typedef enum
{
    Memcpy_Default,
    Memcpy_SSE,
    Memcpy_MMX,
    Memcpy_MMX2,
    Memcpy_Auto
}eMemcpyModes;

template<typename T>
struct bs_alloc
{
    virtual T* alloc(size_t size)
    {
        return (T*)bsMalloc (size);
    }
    virtual T* realloc(T* mem, size_t size)
    {
        return (T*)::bsRealloc((void*)mem, size);
    }
    virtual void destroy(T *mem)
    {
        bsFree (mem);
    }
};


class bs_memory
{
public:
    static void *copy_sse(void *dst, const void* src, int64 len);
    static void *copy_mmx(void *dst, const void* src, int64 len);
    static void *copy_mmx2(void *dst, const void* src, int64 len);
    static void *move_c(void *dst, const void* src, int64 len);
    static void *set_c(void *dat, int val, int64 len);
public:
    int cpu_count()const;
    int capable()const;
    bool have_capable(eCPUCapables cap)const;
    eMemcpyModes probe(int64 size = 1024*1024, uint64 *time = NULL)const;
    void *copy(void *dst, const void* src, int64 len);
    void *move(void *dst, const void* src, int64 len);
    void *set(void *dst, int val, int64 len);

    bs_memory(eMemcpyModes = Memcpy_Auto);
private:
    bool have_cpuid()const;

private:
    eMemcpyModes mode ; //auto
};

#endif // BS_MEMCPY

