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
 * @file     bs_list.h
 * @brief    模板链表行数据空间容器类(支持C++11)
 *       bs_list 类主要完成一个链表型模板类，内部使用bs_alloc来完成内存分配，
 * 有bs_ref完成对类的引用管理，具备共享后修改着会进行单独复制使用。
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.0.2.2
 * @date     2010/05/08
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2010/06/01 | 1.0.0.1   | kunyang  | 创建文件
 * 2011/02/16 | 1.0.1.0   | kunyang  | 加入bs_alloc模板分配类
 * 2014/08/10 | 1.0.2.0   | kunyang  | 加入对象引用计数，同时加入C++11支持
 * 2015/03/06 | 1.0.2.2   | kunyang  | 修改将头信息和链表地址共用
 */

#ifndef BS_LIST
#define BS_LIST
#include "bs_define.h"
#include "bs_algorlthm.h"
#include "bs_memory.h"
#include <list>

struct _bs_list_
{
    _bs_list_  *prev;
    _bs_list_  *next;
    bs_ref ref;
    bs_atomic<size_t> count;

    static const _bs_list_ shared_null;
    static bool is_null (intptr p);
    static intptr null();

    void addref(){ref.addref();}
    bool lessref(){return ref.lessref();}
};

template <typename T, typename Alloc = bs_alloc<T> >
class bs_list : public Alloc
{
public:
    class opelist_node;
    typedef opelist_node  iterator;
    typedef const iterator    const_iterator;

    // STL
public:
    bs_list();
    bs_list(const bs_list<T, Alloc> &rhs);
    ~bs_list();

    //!
    //! \brief operator = 链表赋值
    //! \param rhs
    //! \return
    //!
    bs_list& operator = (const bs_list<T, Alloc> &rhs);

    bool empty() const {return is_empty();}
    size_t size() const {return count();}
    //!
    //! \brief swap 交换链表
    //! \param rhs
    //!
    void swap(bs_list<T, Alloc> &rhs);
    friend void bs_swap(bs_list<T, Alloc>  &a, bs_list<T, Alloc>  &b){a.swap(b);}
    //!
    //! \brief sort 排序链表-未实现
    //!
    void sort();
    //!
    //! \brief reverse 逆向链表-未实现
    //!
    void reverse();

    //!
    //! \brief clear 清空链表
    //!
    void clear();

    //!
    //! \brief begin 开始节点
    //! \return
    //!
    iterator begin();
    const_iterator begin() const;
    inline const_iterator cbegin() const{return begin();}

    //!
    //! \brief end 结束节点
    //! \return
    //!
    iterator end();
    const_iterator end() const;
    inline const_iterator cend() const{return end();}

    //!
    //! \brief front 头部元素
    //! \return
    //!
    T& front();
    const T& front() const;

    //!
    //! \brief back 尾部元素
    //! \return
    //!
    T& back();
    const T& back() const;

    //!
    //! \brief push_front 在头和尾添加一个元素
    //! \param x
    //!
    void push_front( const T &x ){prepend(x);}
    void push_back( const T &x ){append(x);}

    //!
    //! \brief pop_front 在头和尾删掉一个元素
    //!
    void pop_front();
    void pop_back();

#if bsLanguage < bsLanguage11
    void insert(iterator itr, size_t n, const T &x );
    void splice(iterator pos, bs_list& x);
    void splice(iterator pos, bs_list& x, iterator i);
    void splice(iterator pos, bs_list& x, iterator first, iterator last);
    void merge(bs_list& x);
    //!
    //! \brief insert 插入操作
    //! \param itr
    //! \param x
    //! \return
    //!
    iterator insert(iterator itr, const bs_list<T, Alloc> &x );
    iterator insert(iterator itr, const T &x );
    iterator insert(iterator itr, const iterator &x );

    //!
    //! \brief erase 擦除操作
    //! \param itr
    //! \return
    //!
    iterator erase(iterator itr );
    iterator erase(iterator start, iterator end );
#endif
    //!
    //! \brief assign 赋值操作
    //! \param itr
    //! \param x
    //!
    void assign(iterator itr, const T &x );
    void assign(iterator itr, const iterator &x );
    void assign(size_t, const T &x );

    //void remove(const T& val)
    //void unique()

    // C++11
#if bsLanguage >= bsLanguage11
public:
    explicit bs_list(const bs_list<T, Alloc> &&rhs);
    bs_list<T, Alloc>& operator = (bs_list<T, Alloc> &&rhs);
    void push_front( T &&x ){prepend(x);}
    void push_back( T &&x ){append(x);}

    void splice(const_iterator pos, bs_list<T, Alloc>&& x);
    void splice(const_iterator pos, bs_list<T, Alloc>& x);
    void splice(const_iterator pos, bs_list<T, Alloc>&& x, const_iterator i);
    void splice(const_iterator pos, bs_list<T, Alloc>& x, const_iterator i);
    void splice(const_iterator pos, bs_list<T, Alloc>&& x, const_iterator first, const_iterator last);
    void splice(const_iterator pos, bs_list<T, Alloc>& x, const_iterator first, const_iterator last);
    void merge(bs_list<T, Alloc>&& x);
    //!
    //! \brief insert 插入操作
    //! \param itr
    //! \param x
    //! \return
    //!
    iterator insert(const_iterator itr, const bs_list<T, Alloc> &x );
    iterator insert(const_iterator itr, const T &x );
    iterator insert(const_iterator itr, const iterator &x );
    iterator insert(const_iterator itr, T &&x );
    iterator insert(const_iterator itr, size_t n, const T &x );

    //!
    //! \brief erase 擦除操作
    //! \param itr
    //! \return
    //!
    iterator erase(const_iterator itr );
    iterator erase(const_iterator start, const_iterator end );
#endif

    // base
public:
    explicit bs_list(const std::list<T> &rhs);
    //!
    //! \brief to_std
    //! \return 返回stl型链表
    //!
    std::list<T> to_std();
    //!
    //! \brief form stl型链表输入
    //! \param rhs
    //!
    void form(const std::list<T> &rhs);
    bs_list<T, Alloc>& operator = (const std::list<T> &rhs);

    //!
    //! \brief operator += 链表追加
    //! \param rhs
    //! \return
    //!
    bs_list<T, Alloc> &operator +=(const bs_list<T, Alloc> &rhs);
    bs_list<T, Alloc> &operator +=(const T &val);

    //!
    //! \brief operator + 链表相加
    //! \param rhs
    //! \return
    //!
    bs_list<T, Alloc> operator +(const bs_list<T, Alloc> &rhs) const;

    //!
    //! \brief operator << 链表添加
    //! \param val
    //! \return
    //!
    bs_list<T, Alloc> &operator << (const T &val);
    bs_list<T, Alloc> &operator << (const bs_list<T, Alloc> &rhs);

    //!
    //! \brief operator 链表比对
    //! \param rhs
    //! \return
    //!
    bool operator ==(const bs_list<T, Alloc> &rhs) const;
    bool operator !=(const bs_list<T, Alloc> &rhs) const { return !(*this == rhs); }

    //!
    //! \brief operator [] 重载下标符号访问数据
    //! \param inx
    //! \return
    //!
    T &operator [](size_t inx) {return *(begin() + inx);}
    const T &operator [](size_t inx)const {return *(begin() + inx);}

    //!
    //! \brief is_empty 是否空链表
    //! \return
    //!
    bool is_empty() const;
    bool is_null() const;
    //!
    //! \brief count 链表长度
    //! \return
    //!
    size_t count() const;
    //!
    //! \brief prepend 前置添加元素
    //! \param val
    //!
    void prepend(const T& val);
    void prepend(const bs_list<T, Alloc>& val) ;
    //!
    //! \brief append 后置添加元素
    //! \param val
    //!
    void append(const T& val) ;
    void append(const bs_list& val);
    //!
    //! \brief insert 根据位置插入元素
    //! \param pos
    //! \param val
    //!
    iterator insert(size_t pos, const T& val) ;
    void insert(size_t pos, const bs_list<T, Alloc>& val);

    //!
    //! \brief replace 替换操作
    //! \param i
    //! \param t
    //!
    void replace(size_t i, const T &t);
    //!
    //! \brief move 移动操作
    //! \param inx
    //! \param to
    //!
    void move(size_t inx, size_t to);

    //!
    //! \brief remove 根据位置删除元素
    //! \param pos
    //!
    void remove(size_t pos) ;
    void remove() {pop_back();}
    //!
    //! \brief swap 交换操作
    //! \param i
    //! \param j
    //!
    void swap(size_t i, size_t j);
    void swap(iterator x, iterator y);
    //!
    //! \brief contains 查找是否包含指定元素
    //! \param t
    //! \return
    //!
    bool contains(const T &t) const;
    int find(const T &t) const;

protected:
    struct list_node
    {
        list_node  *prev;
        list_node  *next;

        T        data;
    };

public:
    class opelist_node
    {
        friend class bs_list;
    public:

        opelist_node():ope(NULL){}
        opelist_node(list_node *p): ope(p){}
        opelist_node(const opelist_node&ope_n): ope(ope_n.ope){}
        ~opelist_node(){}

        T* operator ->(){return &ope->data;}
        T& operator *() {return ope->data;}
        const T& operator *() const{return ope->data;}

        bool operator ==(const opelist_node &rhs) const { return ope == rhs.ope; }
        bool operator !=(const opelist_node &rhs) const { return ope != rhs.ope; }
        bool operator <(const opelist_node& rhs) const { return ope < rhs.ope; }
        bool operator <=(const opelist_node& rhs) const { return ope <= rhs.ope; }
        bool operator >(const opelist_node& rhs) const { return ope > rhs.ope; }
        bool operator >=(const opelist_node& rhs) const { return ope >= rhs.ope; }

        opelist_node operator +(int c);
        const opelist_node operator +(int c)const;
        inline opelist_node operator -(int c){return operator+(-c);}
        const opelist_node operator -(int c)const{return operator+(-c);}

        int operator -(const opelist_node &rhs)const;

        const opelist_node& operator++()const;
        const opelist_node operator++(int)const;

        const opelist_node& operator--()const;
        const opelist_node operator--(int)const;

        opelist_node& operator++() ;
        opelist_node operator++(int);

        opelist_node& operator--();
        opelist_node operator--(int);

        opelist_node &operator +=(int c){return *this = *this + c; }
        const opelist_node &operator +=(int c)const{return *this = *this + c; }
        opelist_node &operator -=(int c){return *this = *this - c; }
        const opelist_node &operator -=(int c)const{return *this = *this - c; }

    private:
        list_node *ope;
    };

private:
    union {_bs_list_ *impl; list_node *list;};

private:
    list_node *newnode(const T &);
    void freenode(list_node *n);
    void free(struct _bs_list_ *n);
    void copy();
    void detach();

    _bs_list_ *header() {return impl;}
    const _bs_list_ *header()const {return impl;}
};

#include "bs_list.inl"

#endif // BS_LIST

