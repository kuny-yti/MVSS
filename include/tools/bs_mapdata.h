#ifndef BS_MAPDATA
#define BS_MAPDATA

#include "bs_define.h"
#include "bs_typeinfo.h"
#include "bs_memory.h"

#define rb_parent(r)   ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_is_red(r)   ((r)->color == bs_treebase::rbRed)
#define rb_is_black(r)  ((r)->color == bs_treebase::rbBlack)
#define rb_set_black(r)  do { (r)->color = bs_treebase::rbBlack; } while (0)
#define rb_set_red(r)  do { (r)->color = bs_treebase::rbRed; } while (0)
#define rb_set_parent(r,p)  do { (r)->parent = (p); } while (0)
#define rb_set_color(r,c)  do { (r)->color = (c); } while (0)

typedef bs_alloc<void> bs_mapalloc;

struct bs_treebase
{
    typedef enum  {rbBlack = 0, rbRed = 1, rbMask = 3}rbColor;
    rbColor color;
    bs_treebase *lchild;
    bs_treebase *rchild;
    bs_treebase *parent;

    //! 下一个或上一个节点
    bs_treebase *next();
    bs_treebase *prev();
};

template<typename KeyT, typename ValT>
struct bs_nodemap : bs_treebase
{
    KeyT key;
    ValT value;

    //! 销毁key和val类型，若为对象则执行析构函数
    void destroy();

    //! 返回当前的左节点或又节点
    inline bs_nodemap *left() const { return (bs_nodemap *)lchild; }
    inline bs_nodemap *right() const { return (bs_nodemap *)rchild; }

    inline bs_nodemap *next(){return (bs_nodemap *)bs_treebase::next();}
    inline bs_nodemap *prev(){return (bs_nodemap *)bs_treebase::prev();}
};

struct bs_treedata : bs_ref
{
    bs_atomic<size_t> count;
    bs_treebase header;
    bs_treebase *most_left;

    static const bs_treedata shared_null;
    static bool is_null (intptr p);
    static intptr null();

    //! 对红黑树的节点(x)进行左旋转
    void left_rotate(bs_treebase* x);
    //! 对红黑树的节点(y)进行右旋转
    void right_rotate(bs_treebase* y);
    //! 对红黑树进行修正
    void rebalance(bs_treebase *x);
    //! 销毁红黑树节点并进行修正
    template<typename Alloc = bs_mapalloc>
    inline void destroy_rebalance(bs_treebase *z, Alloc &al = bs_mapalloc());

    //! 创建一个红黑树节点
    template<typename Alloc = bs_mapalloc>
    inline bs_treebase *create(int alloc, bs_treebase *parent, bool left, Alloc &al);
    //! 销毁红黑树节点(以及子节点)
    template<typename Alloc = bs_mapalloc>
    inline void destroy(bs_treebase *x, Alloc &al = bs_mapalloc());

    //! 创建和销毁数据
    template<typename Alloc = bs_mapalloc>
    static bs_treedata *create_data(Alloc &al = bs_mapalloc())
    {
        bs_treedata *dat = (bs_treedata *)al.alloc(sizeof(bs_treedata));
        dat->count = 0;
        dat->set(bs_ref::refShareableDetach);

        rb_set_black(&dat->header);
        dat->header.lchild = NULL;
        dat->header.rchild = NULL;
        dat->most_left = &(dat->header);
        return dat;
    }

 };

template <typename KeyT, typename ValT, typename Alloc = bs_mapalloc >
struct bs_mapdata : public bs_treedata
{
    typedef bs_nodemap<KeyT, ValT> node;
    typedef const node const_node;

    inline node *root() const { return (node *)header.lchild;}

    inline const_node *end() const { return (const_node *)&header;}
    inline node *end() { return (node *)&header; }
    inline const_node *begin() const { if (root()) return (const_node*)most_left; return end();}
    inline node *begin() { if (root()) return (node *)most_left; return end();}

    node *find(const KeyT &key) const
    {
        node* tn = root();
        // 非递归查找
        forever ((tn != NULL) && (tn->key != key))
        {
            if (key < tn->key)
                tn = tn->left();
            else
                tn = tn->right();
        }
        return (!tn || (tn == root() && tn->key != key)) ? NULL : tn;
    }

    node *newnode(const KeyT &k, const ValT &v, node *parent = 0, bool left = false)
    {
        node *n = (node *)bs_treedata::create<Alloc>(sizeof(node), parent, left, staticAlloc);
        if (!bs_is_type(KeyT))
            new (&n->key) KeyT(k);
        else
            n->key = k;
        if (!bs_is_type(ValT))
            new (&n->value) ValT(v);
        else
            n->value = v;
        return n;
    }
    void delnode(node *z)
    {
        if (!bs_is_type(KeyT))
            z->key.~KeyT();
        if (!bs_is_type(ValT))
            z->value.~ValT();
        destroy_rebalance<Alloc>(z, staticAlloc);
    }

    static bs_mapdata *create()
    {
        return (bs_mapdata *)bs_treedata::create_data<Alloc>(staticAlloc);
    }
    inline void destroy()
    {
        if (root())
        {
            root()->destroy();
            bs_treedata::destroy<Alloc>(header.lchild, staticAlloc);
        }
        staticAlloc.destroy (this);
    }

    static Alloc staticAlloc;
};
template <typename KeyT, typename ValT, typename Alloc >
Alloc bs_mapdata<KeyT, ValT, Alloc>::staticAlloc;

// hash map

#endif // BS_MAPDATA

