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
 * @file     bs_array.h
 * @brief    模板数组类(支持C++11)
 *       bs_array 类主要完成一个连续性地址的模板数组管理，内部使用bs_alloc来完成内存分配，
 * 有bs_ref完成对类的引用管理，具备共享后修改时会进行单独复制使用。
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.0.2.3
 * @date     2010/05/02
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2010/05/02 | 1.0.0.1   | kunyang  | 创建文件
 * 2011/02/12 | 1.0.1.0   | kunyang  | 加入bs_alloc模板分配类
 * 2014/08/10 | 1.0.2.0   | kunyang  | 加入对象引用计数，同时加入C++11支持
 * 2015/03/06 | 1.0.2.2   | kunyang  | 修改将头信息和实际类数据分开
 * 2016/06/29 | 1.0.2.3   | kunyang  | 修改引用计数的可复制对象
 */

#ifndef BS_ARRAY_H
#define BS_ARRAY_H

#include "bs_define.h"
#include "bs_memory.h"

/* bs_array 的默认储备空间 */
const size_t arrayCapacityDefaults = 32;


struct _bs_array_ : bs_ref
{
    bs_atomic<size_t> cnum;
    bs_atomic<size_t> num;

    static const _bs_array_ shared_null;

    static bool is_null (intptr p);
    static intptr null();
};

/*!
 * @brief The bs_array class
 * @class bs_array
 */
template<typename T, typename Alloc = bs_alloc<T> >
class bs_array :public Alloc
{
public:
    typedef T array_t ;
    enum {sizeOf = sizeof(array_t)};

    // C++11
#if bsLanguage >= bsLanguage11
public:
    bs_array(bs_array&& rhs);
    bs_array& operator =(bs_array&& rhs);
#endif

public:
    bs_array();
    //!
    //! \brief bs_array 根据默认储备大小构造函数
    //! \param size 储备空间
    //!
    explicit bs_array(size_t size );
    //!
    //! \brief bs_array 默认引用构造函数
    //! \param ref
    //!
     bs_array(const bs_array &ref);
    //!
    //! \brief bs_array 根据数据构造函数
    //! \param buf
    //! \param len
    //!
    bs_array(const T *buf, size_t len);
    virtual ~bs_array();

    //!
    //! \brief capacity 返回当前储备空间大小
    //! \return
    //!
    size_t capacity() const;
    //!
    //! \brief reserve 分配储备空间
    //! \param size
    //!
    void reserve(size_t size);
    //!
    //! \brief size 返回当前数据的大小
    //! \return
    //!
    size_t size()const;
    //!
    //! \brief bytecount 返回当前字节数
    //! \return
    //!
    size_t bytecount()const;
    //!
    //! \brief resize 申请数据空间
    //! \param s
    //!
    void resize(size_t s);
    void resize(size_t s, const T &v);

    //!
    //! \brief is_empty 返回当前数组是否是空数据
    //! \return
    //!
    bool is_empty() const;
    bool is_null()const;

    operator  T *() const;
    operator  void *() const;

    //!
    //! \brief data 返回当前数据指针
    //! \return
    //!
    T *data();
    T *data() const;
    //!
    //! \brief clear 清除储备空间
    //!
    void clear();
    //!
    //! \brief fill 填充储备空间为c
    //! \param c
    //! \param len
    //!
    void fill(const T &c, size_t len, size_t pos = 0);
    //!
    //! \brief at 返回i的位置数组元素
    //! \param i
    //! \return
    //!
    T at(size_t i);
    T &operator[](size_t i) ;
    T at(size_t i)const;
    const T &operator[](size_t i) const;

    //!
    //! \brief prepend 在最前面添加元素
    //! \param c
    //! \return
    //!
    bs_array &prepend(const T &c);
    bs_array &prepend(const T *s, int len);
    bs_array &prepend(const bs_array &a);
    //!
    //! \brief append 在尾部附加元素
    //! \param c
    //! \return
    //!
    bs_array &append(const T &c);
    bs_array &append(const T *s, size_t len);
    bs_array &append(const bs_array &a);
    //!
    //! \brief insert 在指定i位置插入元素
    //! \param i
    //! \param c
    //! \return
    //!
    bs_array &insert(size_t i, const T &c);
    bs_array &insert(size_t i, const T *s, size_t len);
    bs_array &insert(size_t i, const bs_array &a);
    //!
    //! \brief remove 在指定位置删除len长度的元素
    //! \param index
    //! \param len
    //! \return
    //!
    bs_array &remove(size_t index, size_t len = 1);

    //!
    //! \brief find 查找 c 第一个出现的位置
    //! \param c
    //! \return
    //!
    int find(const T &c)const
    {
        for (size_t i = 0; i < size(); ++i)
            if (operator [](i) == c)
                return i;
        return -1;
    }

    //!
    //! \brief replace 将指定位置的len长度元素替换为alen长度的s数据
    //! \param index
    //! \param len
    //! \param s
    //! \param alen
    //! \return
    //!
    bs_array &replace(size_t index, const T&c);
    bs_array &replace(size_t index, size_t len, const T *s, size_t alen);
    bs_array &replace(size_t index, size_t len, const bs_array &s);

    //!
    //! \brief operator = 默认赋值函数
    //! \return
    //!
    bs_array &operator =(const bs_array &);

    //!
    //! \brief extract  (count 负数时根据pos位置向前取，此时顺序不为逆)
    //! \param pos
    //! \param count
    //! \return
    //!
    bs_array extract(int pos, int count)const;
    bs_array mid(int pos, int count)const{return extract(pos, count);}
    //!
    //! \brief extract 根据pos位置将后面的全部提取
    //! \param pos
    //! \return
    //!
    bs_array extract(int pos)const;
    //!
    //! \brief start  从开头提取count个字符
    //! \param count
    //! \return
    //!
    bs_array start(int count)const;
    //!
    //! \brief end  从结尾提取count个字符，此时不为逆
    //! \param count
    //! \return
    //!
    bs_array end(int count)const;

    //!
    //! \brief shrink 收缩储备空间到合适位置,返回收缩的空间
    //! \return
    //!
    size_t shrink();

    //!
    //! \brief swap  交换两个对象的数据
    //! \param rhs
    //!
    void swap(bs_array &rhs);
    friend void bs_swap(bs_array &a, bs_array &b) {a.swap(b);}

protected:
    void detach();

private:
    struct _data_
    {
        _bs_array_ header;
        array_t d;
    } ;
    array_t *impl;///< 连续地址数据

    void copy();
    _bs_array_ *header()
    {
        _bs_array_ *h = (_bs_array_ *)impl;
        if (h) --h;
        return h;
    }
    const _bs_array_ *header()const
    {
        _bs_array_ *h = (_bs_array_ *)impl;
        if (h) --h;
        return h;
    }
};

#include "bs_array.inl"

typedef bs_array<uint8> bs_byte;
static bs_byte base64enc(const bs_byte &s);
static bs_byte base64dec(const bs_byte &d);
#endif // BS_ARRAY_H
