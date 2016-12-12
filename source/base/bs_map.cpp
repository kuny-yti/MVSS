
#include "bs_map.h"

const bs_treedata bs_treedata::shared_null = {};

bool bs_treedata::is_null (intptr p)
{
    return p == (intptr)&shared_null;
}
intptr bs_treedata::null()
{
    return (intptr)&shared_null;
}

bs_treebase *bs_treebase::next()
{
    bs_treebase *take = this;

    if (take->rchild != NULL)
    {
        take = take->rchild;
        forever (take->lchild)
            take = take->lchild;
    }
    else
    {
        bs_treebase *tmp = take->parent;
        forever (tmp != NULL && take == tmp->rchild)
        {
            take = tmp;
            tmp = take->parent;
        }
        take = tmp;
    }
    return take;
}
bs_treebase *bs_treebase::prev()
{
    bs_treebase *take = this;
    if (take->lchild != NULL)
    {
        take = take->lchild;
        forever (take->rchild)
            take = take->rchild;
    }
    else
    {
        bs_treebase *tmp = take->parent;
        forever (tmp != NULL && take == tmp->lchild)
        {
            take = tmp;
            tmp = take->parent;
        }
        take = tmp;
    }
    return take;
}


void bs_treedata::left_rotate(bs_treebase* x)
{
    bs_treebase *&rot = header.lchild;
    bs_treebase *y = x->rchild;

    x->rchild = y->lchild;
    if (y->lchild != NULL)
        y->lchild->parent = x;

    y->parent = x->parent;

    if (x == rot)
        rot = y;
    else if (x == x->parent->lchild)
        x->parent->lchild = y;
    else
        x->parent->rchild = y;

    y->lchild = x;
    x->parent = y;
}
void bs_treedata::right_rotate(bs_treebase* y)
{
    bs_treebase *&rot = header.lchild;
    bs_treebase *x = y->lchild;

    y->lchild = x->rchild;
    if (x->rchild != NULL)
        x->rchild->parent = y;

    x->parent = y->parent;
    if (y == rot)
        rot = x;
    else if (y == y->parent->rchild)
        y->parent->rchild = x;
    else
        y->parent->lchild = x;

    x->rchild = y;
    y->parent = x;
}
void bs_treedata::rebalance(bs_treebase *x)
{
    bs_treebase *&rot = header.lchild;
    rb_set_red(x);
    while (x != rot && rb_is_red(x->parent))
    {
        if (x->parent == x->parent->parent->lchild)
        {
            bs_treebase *y = x->parent->parent->rchild;
            if (y && rb_is_red(y))
            {
                rb_set_black(x->parent);
                rb_set_black(y);
                rb_set_red(x->parent->parent);
                x = x->parent->parent;
            }
            else
            {
                if (x == x->parent->rchild)
                {
                    x = x->parent;
                    left_rotate(x);
                }
                rb_set_black(x->parent);
                rb_set_red(x->parent->parent);
                right_rotate(x->parent->parent);
            }
        }
        else
        {
            bs_treebase *y = x->parent->parent->lchild;
            if (y && rb_is_red(y))
            {
                rb_set_black(x->parent);
                rb_set_black(y);
                rb_set_red(x->parent->parent);
                x = x->parent->parent;
            }
            else
            {
                if (x == x->parent->lchild)
                {
                    x = x->parent;
                    right_rotate(x);
                }
                rb_set_black(x->parent);
                rb_set_red(x->parent->parent);
                left_rotate(x->parent->parent);
            }
        }
    }
    rb_set_black(rot);
}
