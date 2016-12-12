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
 * @file     bs_vector.h
 * @brief    模板连续数据空间容器类(支持C++11)
 *       bs_vector 类主要从bs_array 继承而来，主要加入STL接口和迭代接口
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.0.1.5
 * @date     2010/05/08
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2010/05/08 | 1.0.0.1   | kunyang  | 创建文件
 * 2010/08/10 | 1.0.1.0   | kunyang  | 将原有继承std::vector完成类修改为bs_array
 * 2012/04/02 | 1.0.1.2   | kunyang  | 将迭代接口进行重新编写
 * 2014/03/09 | 1.0.1.5   | kunyang  | 加入C++11支持
 */

#ifndef VECTOR_H
#define VECTOR_H

#include "bs_define.h"
#include <vector>
#include "bs_array.h"
#include "bs_algorlthm.h"

/*!
 * \brief The bs_vector class
 */
template <typename T >
class bs_vector : public bs_array<T>
{
    typedef bs_array<T> VecBase;
public:
    class opevec_node;
    typedef opevec_node  iterator;
    typedef const iterator  const_iterator;

    // STL
public:
    bs_vector();
    explicit bs_vector(size_t count);
    bs_vector(const bs_vector& rhs);
    ~bs_vector(){}

    bs_vector& operator =(const bs_vector& rhs);

    iterator begin();
    iterator end();
    const_iterator begin()const;
    const_iterator end()const;
    inline const_iterator cbegin()const{return begin();}
    inline const_iterator cend()const{return end();}

    bool empty() const;

    T &front();
    const T &front()const;

    T &back();
    const T &back()const;

    void push_back(const T& v);
    inline void push_front(const T& v){prepend(v);}
    void pop_back();
    inline void pop_front(){remove(0);}

#if bsLanguage < bsLanguage11
    iterator insert(iterator pos, const T& v);
    void insert(iterator pos, size_t count, const T& v);

    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
#endif

    // C++11
#if bsLanguage >= bsLanguage11
public:
    bs_vector(bs_vector&& rhs);
    void push_back(T&& v);

    iterator insert(const_iterator pos, const T& v);
    iterator insert(const_iterator pos, T&& v);
    iterator insert(const_iterator pos, size_t count, const T& v);

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
#endif

    // base
public:
    bs_vector(const std::vector<T> &rhs);

    std::vector<T> to_std();
    void form(const std::vector<T> &rhs);
    bs_vector& operator =(const std::vector<T>& rhs);

    inline bool operator!=(const bs_vector<T> &v) const { return !(*this == v); }
    bool operator==(const bs_vector<T> &v) const;
    bool contains(const T &t) const;

    inline bs_vector<T> &operator +=(const bs_vector<T> &l) {return append(l.data(), l.size());}
    inline bs_vector<T> operator +(const bs_vector<T> &l) const { bs_vector n = *this; n += l; return n; }
    inline bs_vector<T> &operator +=(const T &t) { append(t); return *this; }
    inline bs_vector<T> &operator << (const T &t) { append(t); return *this; }
    inline bs_vector<T> &operator <<(const bs_vector<T> &l) { *this += l; return *this; }

    size_t count()const;

    bs_vector<T> &prepend(T c);
    bs_vector<T> &prepend(const T *s, int len);
    bs_vector<T> &prepend(const bs_vector<T> &a);

    bs_vector<T> &append(T c);
    bs_vector<T> &append(const T *s, size_t len);
    bs_vector<T> &append(const bs_vector<T> &a);

    bs_vector<T> &insert(size_t i, T c);
    bs_vector<T> &insert(size_t i, const T *s, size_t len);
    bs_vector<T> &insert(size_t i, const bs_vector<T> &a);

    bs_vector<T> &remove(size_t index, size_t len = 1);

    bs_vector<T> &replace(size_t index, size_t len, const T *s, size_t alen);
    bs_vector<T> &replace(size_t index, size_t len, const bs_vector<T> &s);

    inline bs_vector<T> mid(size_t pos, int len) const {return extract(pos, len);}
    bs_vector<T> extract( int pos, int count);
    bs_vector<T> extract( int pos );
    bs_vector<T> start( int count);
    bs_vector<T> end( int count);

    void sort(){bs_qsort<iterator>(begin(), end());}
    //!
    //! \brief swap 覆盖bs_array后重写
    //! \param rhs
    //!
    void swap(bs_vector<T> &rhs);
    friend void bs_swap(bs_vector<T> &a, bs_vector<T> &b) {a.swap(b);}

public:
    class opevec_node
    {
        friend class bs_vector;
    public:

        opevec_node():ope(NULL){}
        opevec_node(T *p): ope(p){}
        opevec_node(const opevec_node &ope_v): ope(ope_v.ope){}
        ~opevec_node(){}

        inline T* operator->(){return &ope;}
        inline T& operator*() {return *ope;}
        inline const T& operator*() const{return *ope;}

        inline bool operator ==(const opevec_node &rhs) const { return ope == rhs.ope; }
        inline bool operator !=(const opevec_node &rhs) const { return ope != rhs.ope; }
        inline bool operator <(const opevec_node& rhs) const { return ope < rhs.ope; }
        inline bool operator <=(const opevec_node& rhs) const { return ope <= rhs.ope; }
        inline bool operator >(const opevec_node& rhs) const { return ope > rhs.ope; }
        inline bool operator >=(const opevec_node& rhs) const { return ope >= rhs.ope; }

        opevec_node operator +(int c);
        const opevec_node operator +(int c)const;
        inline opevec_node operator -(int c){return operator+(-c);}
        inline const opevec_node operator -(int c)const{return operator+(-c);}

        int operator - (const opevec_node &rhs);

        const opevec_node& operator++()const;
        const opevec_node operator++(int)const;

        const opevec_node& operator--()const;
        const opevec_node operator--(int)const;

        opevec_node& operator++();
        opevec_node operator++(int);

        opevec_node& operator--();
        opevec_node operator--(int);

        inline opevec_node &operator +=(int c){return *this = *this + c; }
        inline const opevec_node &operator +=(int c)const{return *this = *this + c; }
        inline opevec_node &operator -=(int c){return *this = *this - c; }
        inline const opevec_node &operator -=(int c)const{return *this = *this - c; }

    private:
        T *ope;
    };
};

#include "bs_vector.inl"
#endif // VECTOR_H
