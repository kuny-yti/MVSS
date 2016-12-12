#ifndef BS_MAPHASH_H
#define BS_MAPHASH_H
#include "bs_define.h"
#include "tools/bs_typeinfo.h"
#include "tools/bs_memory.h"
#include "tools/bs_algorlthm.h"
#include "tools/bs_list.h"

struct _bs_maphash_
{
    bs_ref ref;                  ///< 引用计数
    bs_atomic<size_t> bcapacity; ///< 桶储备空间
    bs_atomic<size_t> ecount;    ///< 桶内元素数
    bs_atomic<size_t> ecountmax; ///< 桶能承受的最大元素数

    static float sMaxUsageRate;  ///< 桶的最大使用率
    static float sGrowRate;      ///< 桶的增长率

    static bool is_null(intptr p);
    static intptr null();

    void addref(){ref.addref();}
    bool lessref(){return ref.lessref();}

    static const _bs_maphash_ shared_null;
};

template <typename K, typename V, typename Alloc = bs_alloc<void> >
class bs_maphash : public Alloc
{
public:
    struct hash_entry
    {
    public:
        hash_entry(const K& k):_key(k){}
        hash_entry(const K& k,const V& v):_key(k), _value(v){}

        const K& key() const {return _key;}
        const V& value() const {return _value;}
        V& value()  {return _value;}
        hash_entry& operator =(const V& nv){_value = nv; return *this;}

    private:
        K _key;
        V _value;
    };

private:
    struct bucket_item : hash_entry
    {
        bucket_item* next;

        bucket_item(const hash_entry& entry)
            :hash_entry(entry), next(NULL){}

        bucket_item& entry(const hash_entry& e){hash_entry::operator = (e); return *this;}
    };
    struct hash_bucket{bucket_item* first;};

    struct _data_
    {
        _bs_maphash_ *header;
        hash_bucket bh;
    };

    hash_bucket* buckets;

    _bs_maphash_ *header()
    {
        _bs_maphash_ *h = (_bs_maphash_ *)buckets;
        if (h) --h;
        return h;
    }
    const _bs_maphash_ *header()const
    {
        _bs_maphash_ *h = (_bs_maphash_ *)buckets;
        if (h) --h;
        return h;
    }
protected:
    void grow(size_t newsize);

    void detach();

    hash_bucket *create(size_t mins = 4);
    void destroy();

    bucket_item *newitem(const K& k, const V &v)
    {
        bucket_item* nitem = (bucket_item*)Alloc::alloc (sizeof(bucket_item));
        new (nitem) bucket_item(hash_entry(k, v));
        return nitem;
    }
    void freeitem(bucket_item *item)
    {
        item->~bucket_item();
        Alloc::destroy (item);
    }

public:
    class opehmap_node;
    class const_opehmap_node;
    typedef opehmap_node iterator;
    typedef const_opehmap_node const_iterator;

    /// STL style
public:
    bs_maphash():
        buckets((hash_bucket*)_bs_maphash_::null ()){}
    bs_maphash(const bs_maphash & rhs):
        buckets((hash_bucket*)_bs_maphash_::null ())
    {
        buckets = rhs.buckets;
        if (!is_null ())
            header()->addref ();
    }
    ~bs_maphash()
    {
        bs_maphash::destroy();
    }

    iterator begin();
    const_iterator begin() const;
    iterator end(void);
    const_iterator end(void) const;
#if bsLanguage < bsLanguage11
    //!
    //! \brief erase 擦除指定pos的元素
    //! \param pos
    //!
    void erase(iterator pos)

    //!
    //! \brief erase 擦除从first开始到last之间的元素
    //! \param first
    //! \param last
    //!
    void erase(iterator first, iterator last);

    //!
    //! \brief insert 插入key，val
    //! \param key
    //! \param val
    //!
    void insert(const K& key, const V& val);
#endif
    //!
    //! \brief find 查找key并返回一个迭代器
    //! \param key
    //! \return
    //!
    const_iterator find(const K &key) const;
    //!
    //! \brief operator = 默认赋值
    //! \param rhs
    //! \return
    //!
    bs_maphash<K, V, Alloc> &operator = (const bs_maphash<K, V, Alloc> &rhs);
    //!
    //! \brief at 返回key的值
    //! \param key
    //! \return
    //!
    V &at(const K &key);
    const V &at(const K &key)const;
    //!
    //! \brief operator []
    //! \param key
    //! \return 返回指定key的值
    //!
    const V &operator [](const K &key)const;
    V &operator [](const K &key);
    //!
    //! \brief insert 插入key并返回迭代器
    //! \param key
    //! \return
    //!
    iterator insert(const K& key);
    //!
    //! \brief insert 插入key，val并返回迭代器
    //! \param key
    //! \param val
    //! \return
    //!
    iterator insert(const K& key, const V& val);

    //!
    //! \brief swap 交换两个map
    //! \param rhs
    //!
    void swap(bs_maphash<K, V, Alloc> &rhs);
    friend void bs_swap (bs_maphash<K, V, Alloc> &a, bs_maphash<K, V, Alloc> &b){a.swap (b);}
    //!
    //! \brief clear 清空map
    //!
    void clear();
    //!
    //! \brief empty 是否为空map
    //! \return
    //!
    bool empty() const{return is_empty ();}
    //!
    //! \brief size map的元素数
    //! \return
    //!
    size_t size() const{return count();}
    //!
    //! \brief find 查找第一个key出现的迭代器
    //! \param key
    //! \return
    //!
    iterator find(const K &key);

    void remove(const iterator& it);
    void remove(const const_iterator& it);

    /// C++ 11
    #if bsLanguage >= bsLanguage11
public:
    bs_maphash(bs_maphash<K, V, Alloc> &&m)
    {
        if (buckets == m.buckets)
            return;

        buckets = m.buckets;
        m.buckets = (hash_bucket*)_bs_maphash_::null ();
    }
    //bs_maphash<K, V, Alloc>& operator = (bs_maphash<K, V, Alloc> &&m) ;
    const_iterator cbegin() const{return begin();}
    const_iterator cend() const{return end();}

    V &operator []( K &&key);
    iterator erase(const_iterator pos);
    iterator erase(iterator pos);
    size_t erase(const K& key);
    iterator erase(const_iterator first, const_iterator last);

    #endif

    // base
public:
    explicit bs_maphash(size_t capacity):
        buckets((hash_bucket*)_bs_maphash_::null ())
    {
        buckets = create(capacity);
    }

    void append(const K& k, const V& v);
    void append(const hash_entry& entry);
    bool contains(const K& k) const;
    bool contains(const hash_entry& entry) const;
    const hash_entry& search(const K& k) const;
    hash_entry& search(const K& k);

    //!
    //! \brief value
    //! \param key
    //! \return 返回指定key的值
    //!
    V value(const K& key);
    //!
    //! \brief remove 删除指定key ok
    //! \param key
    //!
    void remove(const K& key);
    //!
    //! \brief value
    //! \param key
    //! \return 返回指定key的值
    //!
    const V value(const K &key) const;

    //!
    //! \brief reserve 分配储备空间 ok
    //! \param size
    //!
    void reserve(size_t size);
    //!
    //! \brief values
    //! \return 返回map的全部或指定key的值
    //!
    bs_list<V> values(const K &key) const;

    bool is_empty() const {return header()->ecount;}
    bool is_null()const {return _bs_maphash_::is_null ((intptr)buckets);}

    size_t capacity()const{return header()->bcapacity;}
    size_t count()const{return header()->ecount;}
public:
    class opehmap_node
    {
        friend class bs_maphash;
    private:
        bs_maphash* table;
        size_t bindex;
        bucket_item* bitem;

    public:
        opehmap_node():table(0), bindex(0), bitem(0){}

    private:
        opehmap_node(bs_maphash* tab)
            :table(tab), bindex(0), bitem(table->buckets[bindex].first)
        {
            while(bitem == NULL)
            {
                ++bindex;
                if(bindex == table->capacity ())
                    break;
                bitem = table->buckets[bindex].first;
            }
        }
        opehmap_node(bs_maphash* tab, size_t index, bucket_item* item)
            :table(tab), bindex(index), bitem(item){}

    public:
        bool is_finished(void) const
        {
            return bindex >= table->capacity ();
        }
        friend bool operator == (const opehmap_node& it1,const opehmap_node& it2)
        {
            return it1.bitem == it2.bitem;
        }
        friend bool operator!=(const opehmap_node& it1,const opehmap_node& it2)
        {
            return it1.bitem != it2.bitem;
        }
        hash_entry& operator*(void) const
        {
            return *bitem;
        }
        hash_entry* operator->(void) const
        {
            return bitem;
        }
        opehmap_node& operator++(void)
        {
            bitem = bitem->next;
            while (bitem == 0)
            {
                ++bindex;
                if(bindex == table->capacity ())
                    break;
                bitem = table->buckets[bindex].first;
            }
            return *this;
        }
    };
    class const_opehmap_node
    {
        friend class bs_maphash;
    private:
        const bs_maphash* table;
        size_t bindex;
        bucket_item* bitem;

    public:
        const_opehmap_node(void) :table(0), bindex(0), bitem(0){}
    private:
        const_opehmap_node(const bs_maphash* tab)
            :table(tab), bindex(0), bitem(table->buckets[bindex].first)
        {
            while (bitem==0)
            {
                ++bindex;
                if(bindex == table->capacity())
                    break;
                bitem = table->buckets[bindex].first;
            }
        }
        const_opehmap_node(const bs_maphash* tab,size_t index,const bucket_item* item) // Elementwise constructor
            :table(tab), bindex(index), bitem(item){}

    public:
        bool is_finished(void) const
        {
            return bindex >= table->capacity();
        }
        friend bool operator==(const const_opehmap_node& it1,const const_opehmap_node& it2)
        {
            return it1.bitem == it2.bitem;
        }
        friend bool operator!=(const const_opehmap_node& it1,const const_opehmap_node& it2)
        {
            return it1.bitem != it2.bitem;
        }
        const hash_entry& operator*(void) const
        {
            return *bitem;
        }
        const hash_entry* operator->(void) const
        {
            return bitem;
        }
        const_opehmap_node& operator++(void)
        {
            bitem = bitem->next;

            while(bitem == NULL)
            {
                ++bindex;
                if(bindex == table->capacity())
                    break;
                bitem = table->buckets[bindex].first;
            }
            return *this;
        }
    };

    friend class opehmap_node;
    friend class const_opehmap_node;
};
#include "bs_maphash.inl"
#endif // BS_MAPHASH_H
