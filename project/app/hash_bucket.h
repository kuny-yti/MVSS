#ifndef HASH_BUCKET_H
#define HASH_BUCKET_H

#include "bs_define.h"
#include "tools/bs_algorlthm.h"
#include "tools/bs_typeinfo.h"
#include "tools/bs_memory.h"
#include "tools/bs_vector.h"


struct bucket_node
{
    bucket_node *next;
    uint32 hash; //节点的哈希值
};
struct hash_bucket
{
    bucket_node * np;
    bucket_node **buckets; // 桶句柄
    bs_ref         ref; // 引用计数
    bs_atomic<uint> bucket_capacity; // 桶储备
    bs_atomic<uint> bucket_count; // 元素数
    bs_vector<uint32>::iterator bucket_pos;

    ~hash_bucket(){}

    static bool is_null (intptr p);
    static intptr null();
    static size_t getprime(size_t size);

    void addref(){ref.addref();}
    bool lessref(){return ref.lessref();}

    static bucket_node *next(bucket_node *n);
    static bucket_node *prev(bucket_node *n);
    bucket_node* first();

    static const hash_bucket shared_null;
};


template<typename KeyT, typename ValueT>
struct hash_node
{
    hash_node<KeyT, ValueT> *next;
    uint32 hash;

    KeyT key;
    ValueT value;

    hash_node():next(NULL){hash = 0;}
    hash_node(const KeyT &k, const ValueT &v, uint32 h = 0):next(NULL), hash(h), key(k), value(v){}
};

typedef bs_alloc<void> bs_hmap_alloc;
template<typename KeyT, typename ValueT, typename Alloc = bs_hmap_alloc>
class bs_maphash :public Alloc
{
public:
    typedef hash_node<KeyT, ValueT> hnode;

    /// STL style
public:
    bs_maphash():
        impl((hash_bucket*)hash_bucket::null ())
    {}
    explicit bs_maphash(size_t capacity):
        impl((hash_bucket*)hash_bucket::null ())
    {impl = recreate (capacity);}
    bs_maphash(const bs_maphash<KeyT, ValueT, Alloc> &rhs):
        impl(rhs.impl)
    {
        if (!is_null())
            impl->addref ();
    }
    ~bs_maphash(){destroy();}

    bs_maphash& operator = (const bs_maphash<KeyT, ValueT, Alloc> &rhs)
    {
        impl = rhs.impl;
        if (!is_null())
            impl->addref ();
        return *this;
    }

    ValueT &operator [](const KeyT &k)
    {
        return append(k);
    }
    // base
public:
    //!
    //! \brief append 附加元素
    //! \param k
    //! \return
    //!
    ValueT &append(const KeyT &k)
    {
        detach();
        hnode *hn = newnode(k, ValueT(), bs_hash(k));
        return hn->value;
    }
    void append(const KeyT& k, const ValueT &v)
    {
        detach();
        newnode(k, v, bs_hash(k));
    }

    //!
    //! \brief remove 删除元素
    //! \param k
    //!
    void remove(const KeyT &k)
    {
        detach();
    }

    ValueT find(const KeyT &k)
    {
        const uint32 khash = bs_hash (k);
        const idx = khash % capacity ();

        hnode *tnd = (hnode *)impl->buckets[idx];
        if (tnd->hash != khash && tnd->key != k)
        {
            hnode *tnd = tnd->next;
            if (tnd && tnd->key == k)
                return tnd->value;
            else if (tnd == NULL)
                return ValueT();
        }
        return tnd->value;
    }

    size_t count()const{return impl->bucket_count;}
    bool is_empty(){return count() <= 0;}
    bool is_null(){return hash_bucket::is_null ((intptr)impl);}

protected:
    size_t capacity() const{return impl->bucket_capacity;}
    //!
    //! \brief detach 更改时需要分离共享
    //!
    void detach()
    {
        if (!is_null() && impl->ref.has_detach() && impl->ref.is_shared())
            impl = recreate(count());
    }
    //!
    //! \brief newnode 新建一个节点并放入桶中
    //! \param k
    //! \param v
    //! \param hash
    //! \return
    //!
    hnode *newnode(const KeyT &k, const ValueT &v, uint32 hash)
    {
        if (is_null() || count()+ 1 >= capacity ())
            impl = recreate (count()+ 1);

        hnode * n = (hnode *)this->alloc (sizeof(hnode));

        n->hash = hash;
        n->next = NULL;
        if (!bs_is_type(ValueT))
            new (&n->value) ValueT(v);
        else
            n->value = v;
        if (!bs_is_type(KeyT))
            new (&n->key) KeyT(k);
        else
            n->key = k;

        // 计算出当前需要的桶位置
        const size_t idx = n->hash % impl->bucket_capacity.value ();

        // 桶未被使用则直接使用
        if (impl->buckets[idx] == NULL)
        {
            impl->buckets[idx] = (bucket_node*)n;
            ++impl->bucket_count;
            htab.append (hash);
        }
        // 出现哈希冲突
        else
        {
            // 查找是否有相同的key
            bucket_node *tmpc = impl->buckets[idx];
            while (tmpc && tmpc->hash != hash)
            {
                hnode* tmpn = (hnode*)tmpc;
                if (tmpn->key != k)
                    tmpc = impl->buckets[idx]->next;
                else
                    break;
            }
            // 有相同的key则直接赋值
            if (tmpc)
            {
                hnode* tmpn = (hnode*)tmpc;
                tmpn->value = v;
                tmpn->key = k;
                freenode(n);
            }
            // 没想相同的key,在头部进行添加
            else
            {
                hnode *ntemp = (hnode*)impl->buckets[idx]->next ;
                impl->buckets[idx]->next = (bucket_node*)n;
                n->next = ntemp;
                ++impl->bucket_count;
                htab.append (hash);
            }
        }
        return n;
    }
    //!
    //! \brief freenode 释放一个节点 (链表指针不处理)
    //! \param n
    //!
    void freenode(hnode *n)
    {
        if (!bs_is_type(KeyT))
            n->key.~KeyT();
        if (!bs_is_type(ValueT))
            n->value.~ValueT();
        Alloc::destroy ((void*)n);
    }

    //!
    //! \brief create 创建一个空桶
    //! \return
    //!
    hash_bucket *create()
    {
        hash_bucket *hb = (hash_bucket*)this->alloc(sizeof(hash_bucket));

        hb->ref = bs_ref(bs_ref::refShareableDetach);
        hb->h = 0;
        hb->buckets = NULL;
        hb->bucket_capacity = 0;
        hb->bucket_count = 0;

        return hb;
    }
    //!
    //! \brief recreate 根据储备空间重新创建桶
    //! \param s
    //! \return
    //!
    hash_bucket* recreate(size_t s)
    {
        hash_bucket *rhb = NULL;
        {
            rhb = create();
            const size_t pc = hash_bucket::getprime(s);
            rhb->buckets = (bucket_node **)Alloc::alloc(pc * sizeof(bucket_node *));
            bs_memory::set_c (rhb->buckets, 0, pc * sizeof(hnode *));
            rhb->bucket_capacity = pc;
            for (int i = 0; i < pc; ++i)
                rhb->buckets[i] = (bucket_node*)rhb;
        }
        if (is_null())
            return rhb;

        for (size_t i = 0; i < capacity (); ++i)
        {
            bucket_node *fnode = impl->buckets[i];
            while (fnode != (bucket_node*)impl)
            {
                uint32 hash = fnode->hash;
                bucket_node *lnode = fnode;
                while (lnode->next != (bucket_node*)impl && lnode->next->hash == hash)
                    lnode = lnode->next;

                bucket_node *alnode = lnode->next;
                bucket_node **bfode = &rhb->buckets[hash % rhb->bucket_capacity.value ()];
                while (*bfode != (bucket_node*)impl)
                    bfode = &(*bfode)->next;
                lnode->next = *bfode;
                *bfode = fnode;
                fnode = alnode;
            }
        }
        destroy ();
        return rhb;
    }

    //!
    //! \brief destroy 释放桶
    //!
    void destroy()
    {
        bucket_node *self = (bucket_node *)impl;
        bucket_node **bucket = impl->buckets;

        int n = capacity ();
        while (n--)
        {
            bucket_node *cur = *bucket++;
            while (cur != self)
            {
                bucket_node *next = cur->next;
                freenode(cur);
                cur = next;
            }
        }

        Alloc::destroy ((void*)impl->buckets);
        Alloc::destroy ((void*)impl);

        impl = (hash_bucket*)hash_bucket::null ();
    }

private:
    // node 单向链表, impl 哈希桶
    union {hnode *node;hash_bucket *impl;};
};

#include "bs_maphash.inl"
#endif // HASH_BUCKET_H
