#ifndef BS_LIST_INL
#define BS_LIST_INL


#include "bs_typeinfo.h"

template< typename T , typename Alloc>
bool bs_list<T, Alloc>::is_null() const
{
    return _bs_list_::is_null((intptr)impl);
}

template< typename T , typename Alloc>
void bs_list<T, Alloc>::free(_bs_list_ *x)
{
    list_node *end = (list_node*)x;
    list_node *current = end->next;
    while (current != end)
    {
        list_node *node = current;
        current= current->next;
        freenode(node);
    }
    if (end != current)
        this->destroy((T*)x);
}
template< typename T , typename Alloc>
typename bs_list<T, Alloc>::list_node *bs_list<T, Alloc>::newnode(const T &d)
{
    list_node *n = (list_node*)this->alloc(sizeof(list_node));
    n->next = NULL;
    n->prev = NULL;
    if (!bs_is_type(T))
        new (&n->data) T(d);
    else
        n->data = d;
    return n;
}
template< typename T , typename Alloc>
void bs_list<T, Alloc>::freenode(list_node *n)
{
    if (!bs_is_type(T))
        n->data.~T();
    this->destroy((T*)n);
}
// C++11
#if bsLanguage >= bsLanguage11
template< typename T , typename Alloc>
bs_list<T, Alloc>::bs_list(const bs_list<T, Alloc> &&rhs):
    impl((_bs_list_*)_bs_list_::null())
{
    *this = rhs;
}
template< typename T , typename Alloc>
bs_list<T, Alloc>& bs_list<T, Alloc>::operator = (bs_list<T, Alloc> &&rhs)
{
    if(this == &rhs)
        return *this;

    if (!is_null() && header()->lessref())
        free(impl);
    impl = rhs.impl;
    rhs.impl = (_bs_list_*)_bs_list_::null();

    return *this;
}
#endif


// core
template< typename T , typename Alloc>
void bs_list<T, Alloc>::copy()
{
    if (size())
    {
        union { _bs_list_ *d; list_node *e; } tmp;
        tmp.d = (_bs_list_ *)this->alloc(sizeof(_bs_list_));
        tmp.d->ref.set(bs_ref::refShareableDetach);
        tmp.d->count = this->count();

        list_node *current = list->next;
        list_node *ope = tmp.e;
        list_node *end = list;

        while (current != end)
        {
            ope->next = newnode(current->data);
            ope->next->prev = ope;
            current = current->next;
            ope = ope->next;
        }
        ope->next = tmp.e;
        tmp.e->prev = ope;
        if (header()->lessref())
            free(impl);
        impl = tmp.d;
    }
}
template< typename T , typename Alloc>
void bs_list<T, Alloc>::detach()
{
    if (is_null())
    {
        impl = (_bs_list_ *)this->alloc(sizeof(_bs_list_));
        impl->ref.set(bs_ref::refShareableDetach);
        impl->count = 0;
        list->next = list->prev = list;
    }
    else if (header()->ref.has_detach() && header()->ref.is_shared())
        copy();
}

// STL
template< typename T , typename Alloc>
bs_list<T, Alloc>::bs_list():
    impl((_bs_list_*)_bs_list_::null())
{
}

template< typename T , typename Alloc>
bs_list<T, Alloc>::bs_list( const bs_list<T, Alloc> &rhs ):
    impl((_bs_list_*)_bs_list_::null())
{
    if (impl == rhs.impl)
        return ;
    impl = rhs.impl;
    if (header()->ref.has_shareable())
        header()->addref();
    else
        detach();
}

template< typename T , typename Alloc>
bs_list<T, Alloc>::~bs_list()
{
    if (!is_null() && header()->lessref())
        free(impl);
    impl = NULL;
}
template< typename T , typename Alloc>
bs_list<T, Alloc> &bs_list<T, Alloc>::operator =(const bs_list<T, Alloc> &rhs )
{
    if( impl == rhs.impl)
        return *this;

    if (!is_null() && header()->lessref())
        free(impl);
    impl = rhs.impl;
    if (header()->ref.has_shareable())
        header()->addref();
    else
        detach();

    return *this;
}
template< typename T , typename Alloc>
void bs_list<T, Alloc>::swap(bs_list<T, Alloc> &rhs)
{
    _bs_list_ *tmp = impl;
    impl = rhs.impl;
    rhs.impl = tmp;
}
template< typename T , typename Alloc>
void bs_list<T, Alloc>::sort()
{
    bs_qsort<iterator>(begin(), end());
}
template< typename T , typename Alloc>
void bs_list<T, Alloc>::reverse()
{
    iterator first = begin();
    iterator last = end();
    while ((first != last) && (first != --last))
        bs_swap (*first++, *last);
}

template< typename T , typename Alloc>
typename bs_list<T, Alloc>::iterator bs_list<T, Alloc>::insert(size_t pos, const T& val)
{
    detach();
    list_node *nnode = newnode(val);

    if (pos > size())
        pos = size()-1;

    list_node *p = (begin() + pos).ope;

    nnode->prev = p->prev;
    nnode->next = p;

    p->prev->next = nnode;
    p->prev = nnode;
    ++impl->count;
    return iterator(p->prev);
}
template< typename T , typename Alloc>
void bs_list<T, Alloc>::insert(size_t pos, const bs_list<T, Alloc>& val)
{
    //for(iterator temp = val.begin(); temp != val.end(); ++temp)
    foreach (const T &var, val)
        insert(pos++, var);
}

template< typename T , typename Alloc>
void bs_list<T, Alloc>::clear()
{
    *this = bs_list<T, Alloc>();
}
template< typename T , typename Alloc>
typename bs_list<T, Alloc>::iterator bs_list<T, Alloc>::begin()
{
    detach();
    return iterator(list->next);
}
template< typename T , typename Alloc>
typename bs_list<T, Alloc>::const_iterator bs_list<T, Alloc>::begin() const
{
    return iterator(list->next);
}
template< typename T , typename Alloc>
typename bs_list<T, Alloc>::iterator bs_list<T, Alloc>::end()
{
    detach();
    return iterator(list);
}
template< typename T , typename Alloc>
typename bs_list<T, Alloc>::const_iterator bs_list<T, Alloc>::end() const
{
    return iterator(list);
}
template< typename T , typename Alloc>
T& bs_list<T, Alloc>::front()
{
    return *begin();
}
template< typename T , typename Alloc>
const T& bs_list<T, Alloc>::front() const
{
    return *begin();
}
template< typename T , typename Alloc>
T& bs_list<T, Alloc>::back()
{
    return *(--end());
}
template< typename T , typename Alloc>
const T& bs_list<T, Alloc>::back() const
{
    return *(--end());
}
template< typename T , typename Alloc>
void bs_list<T, Alloc>::pop_front()
{
    erase(begin());
}
template< typename T , typename Alloc>
void bs_list<T, Alloc>::pop_back()
{
    erase(--end());
}
template< typename T , typename Alloc>
#if bsLanguage < bsLanguage11
void bs_list<T, Alloc>::insert(iterator itr, size_t n, const T &x )
{
    iterator i = itr;
    forever (n--)
        i = insert(i, x);
}
#else
typename bs_list<T, Alloc>::iterator bs_list<T, Alloc>::insert(const_iterator itr, size_t n, const T &x )
{
    iterator i = itr;
    forever (n--)
        i = insert(i, x);
    return i;
}

#endif
template< typename T , typename Alloc>
#if bsLanguage < bsLanguage11
void bs_list<T, Alloc>::splice(iterator pos, bs_list& x)
#else
void bs_list<T, Alloc>::splice(const_iterator pos, bs_list<T, Alloc>& x)
#endif
{
    insert(pos, x);
    x.clear();
}
template< typename T , typename Alloc>
#if bsLanguage < bsLanguage11
void bs_list<T, Alloc>::splice(iterator pos, bs_list<T, Alloc>& x, iterator i)
#else
void bs_list<T, Alloc>::splice(const_iterator pos, bs_list<T, Alloc>& x, const_iterator i)
#endif
{
    insert(pos, i, x.end());
    x.erase(i, x.end());
}
template< typename T , typename Alloc>
#if bsLanguage < bsLanguage11
void bs_list<T, Alloc>::splice(iterator pos, bs_list& x, iterator first, iterator last)
#else
void bs_list<T, Alloc>::splice(const_iterator pos, bs_list& x, const_iterator first, const_iterator last)
#endif
{
    insert(pos, first, last);
    x.erase(first, last);
}

#if bsLanguage < bsLanguage11
template< typename T , typename Alloc>
void bs_list<T, Alloc>::merge(bs_list& x)
{

}
#endif

#if bsLanguage >= bsLanguage11
template< typename T , typename Alloc>
typename bs_list<T, Alloc>::iterator bs_list<T, Alloc>::insert(const_iterator itr, T &&x )
{
    size_t pos = itr - begin();
    return insert(pos, x);
}
template< typename T , typename Alloc>
void bs_list<T, Alloc>::splice(const_iterator pos, bs_list&& x)
{

}
template< typename T , typename Alloc>
void bs_list<T, Alloc>::splice(const_iterator pos, bs_list&& x, const_iterator i)
{

}
template< typename T , typename Alloc>
void bs_list<T, Alloc>::splice(const_iterator pos, bs_list&& x, const_iterator first, const_iterator last)
{

}
template< typename T , typename Alloc>
void bs_list<T, Alloc>::merge(bs_list&& x)
{

}
#endif

template< typename T , typename Alloc>
#if bsLanguage < bsLanguage11
typename bs_list<T, Alloc>::iterator bs_list<T, Alloc>::insert(iterator itr, const T &x)
#else
typename bs_list<T, Alloc>::iterator bs_list<T, Alloc>::insert(const_iterator itr, const T &x)
#endif
{
    int inx = itr -begin();
    detach();

    list_node *i = (begin()+inx).ope;
    list_node *m = newnode(x);
    m->next = i;
    m->prev = i->prev;
    m->prev->next = m;
    i->prev = m;
    ++header()->count;
    return m;
}
template< typename T , typename Alloc>
#if bsLanguage < bsLanguage11
typename bs_list<T, Alloc>::iterator bs_list<T, Alloc>::insert(iterator itr, const iterator &x )
#else
typename bs_list<T, Alloc>::iterator bs_list<T, Alloc>::insert(const_iterator itr, const iterator &x )
#endif
{
    return insert(itr, *x);
}

template< typename T , typename Alloc>
#if bsLanguage < bsLanguage11
typename bs_list<T, Alloc>::iterator bs_list<T, Alloc>::insert(iterator itr, const bs_list<T, Alloc> &x)
#else
typename bs_list<T, Alloc>::iterator bs_list<T, Alloc>::insert(const_iterator itr, const bs_list<T, Alloc> &x)
#endif
{
    iterator n = itr;
    foreach (const T &val, x)
        n = insert(n, val);
    /*
    for(iterator temp = x.begin(); temp != x.end(); ++temp)
        itr = insert(itr, *temp);*/
    return ++n;
}
template< typename T , typename Alloc>
#if bsLanguage < bsLanguage11
typename bs_list<T, Alloc>::iterator bs_list<T, Alloc>::erase(iterator itr)
#else
typename bs_list<T, Alloc>::iterator bs_list<T, Alloc>::erase(const_iterator itr)
#endif
{
    int inx =  itr - cbegin();
    remove(inx);
    return begin()+inx;
}
template< typename T , typename Alloc>
#if bsLanguage < bsLanguage11
typename bs_list<T, Alloc>::iterator bs_list<T, Alloc>::erase(iterator start, iterator end)
#else
typename bs_list<T, Alloc>::iterator bs_list<T, Alloc>::erase(const_iterator start, const_iterator end)
#endif
{
    int c = end - start;
    int inx = start - begin();
    while (c--)
        remove(inx);

    return begin()+inx;
}

template< typename T , typename Alloc>
void bs_list<T, Alloc>::assign(iterator itr, const T &x )
{
    int inx = itr - begin();
    replace(inx, x);
}
template< typename T , typename Alloc>
void bs_list<T, Alloc>::assign(iterator itr, const iterator &x )
{
    int inx = itr - begin();
    replace(inx, *x);
}


// base
template< typename T , typename Alloc>
bs_list<T, Alloc>::bs_list(const std::list<T> &rhs):
    impl((T*)_bs_list_::null())
{
    form(rhs);
}
template< typename T , typename Alloc>
std::list<T> bs_list<T, Alloc>::to_std()
{
    std::list<T> retlist;

    foreach (const iterator &itr , *this)
        retlist.push_back(*itr);
    return retlist;
}
template< typename T , typename Alloc>
void bs_list<T, Alloc>::form(const std::list<T> &rhs)
{
    detach();
    foreach (const T &var , rhs)
        append(var);
    //std::copy(rhs.begin(), rhs.end(), std::back_inserter(*this));
}

template< typename T , typename Alloc>
bs_list<T, Alloc>& bs_list<T, Alloc>::operator = (const std::list<T> &rhs)
{
    form(rhs);
    return *this;
}

template< typename T , typename Alloc>
bs_list<T, Alloc> &bs_list<T, Alloc>::operator +=(const bs_list<T, Alloc> &rhs)
{
    foreach (const T &var , rhs)
        push_back(var);

    return *this;
}
template< typename T , typename Alloc>
bs_list<T, Alloc> &bs_list<T, Alloc>::operator +=(const T &val)
{
    push_back(val);
    return *this;
}
template< typename T , typename Alloc>
bs_list<T, Alloc> bs_list<T, Alloc>::operator +(const bs_list<T, Alloc> &rhs) const
{
    bs_list<T, Alloc> retlist(*this);
    foreach (const T &var , rhs)
        retlist.push_back(var);
    return retlist;
}
template< typename T , typename Alloc>
bs_list<T, Alloc> &bs_list<T, Alloc>::operator << (const T &val)
{
    *this += val;
    return *this;
}
template< typename T , typename Alloc>
bs_list<T, Alloc> &bs_list<T, Alloc>::operator <<(const bs_list<T, Alloc> &rhs)
{
    *this += rhs;
    return *this;
}


template< typename T , typename Alloc>
bool bs_list<T, Alloc>::is_empty() const
{
    return count() == 0 && begin() == end();
}

template< typename T , typename Alloc>
size_t bs_list<T, Alloc>::count() const
{
    return impl->count;
}

template< typename T , typename Alloc>
void bs_list<T, Alloc>::assign(size_t inx, const T &x )
{
    replace(inx, x);
}

template< typename T , typename Alloc>
bool bs_list<T, Alloc>::operator ==(const bs_list<T, Alloc> &rhs) const
{
    if (header()->count != rhs.count())
        return false;
    if (list == rhs.list)
        return true;

    list_node *i = list->next;
    list_node *irhs = rhs.list->next;
    while (i != list)
    {
        if (! (i->data == irhs->data))
            return false;
        i = i->next;
        irhs = irhs->next;
    }
    return true;
}
template< typename T , typename Alloc>
void bs_list<T, Alloc>::append(const T& val)
{
    detach();
    list_node *nnode = newnode(val);
    nnode->next = list;
    nnode->prev = list->prev;
    nnode->prev->next = list->prev = nnode;
    ++header()->count;
}

template< typename T , typename Alloc>
void bs_list<T, Alloc>::append(const bs_list<T, Alloc>& val)
{
    foreach (const T &var, val)
        append(var);
}
template< typename T , typename Alloc>
void bs_list<T, Alloc>::prepend(const T& val)
{
    detach();
    list_node *nnode = newnode(val);
    nnode->data = val;
    nnode->next = list->next;
    nnode->prev = list;
    nnode->next->prev = list->next = nnode;
    ++header()->count;
}
template< typename T , typename Alloc>
void bs_list<T, Alloc>::prepend(const bs_list<T, Alloc>& val)
{
    foreach (const T &var, val)
        prepend(var);
}

template< typename T , typename Alloc>
void bs_list<T, Alloc>::replace(size_t i, const T &t)
{
    detach();
    list_node *n = (this->begin() +i).ope;
    n->data = t;
}
template< typename T , typename Alloc>
void bs_list<T, Alloc>::move(size_t inx, size_t to)
{
    if (inx == to)
        return ;

    detach();
    list_node *pinx = (this->begin() +inx).ope;
    list_node *toinx = (this->begin() +to).ope;

    if (pinx != list && toinx != list)
    {
        // 断开原来的
        pinx->next->prev = pinx->prev;
        pinx->prev->next = pinx->next;
        // 修改自身的指向
        pinx->next= toinx;
        pinx->prev = toinx->prev;

        // 修改原来位置的指向
        toinx->prev->next = pinx;
        toinx->prev = pinx;
    }
}

template< typename T , typename Alloc>
void bs_list<T, Alloc>::swap(iterator x, iterator y)
{
    size_t posx = x - begin(), posy = y - begin();
    swap(posx, posy);
}

template< typename T , typename Alloc>
void bs_list<T, Alloc>::remove(size_t pos)
{
    detach();
    pos = bs_min(pos, size()-1);
    list_node *p = (begin()+pos).ope;

    if (p != list)
    {
        p->next->prev = p->prev;
        p->prev->next = p->next;
        freenode(p);
        -- header()->count;
    }
}

template< typename T , typename Alloc>
void bs_list<T, Alloc>::swap(size_t i, size_t j)
{
    detach();

    list_node *x = (this->begin() +bs_bound(size_t(0), bs_min(i, j), size()-1)).ope;
    list_node *y = (this->begin() +bs_bound(size_t(0), bs_max(i, j), size()-1)).ope;
    if (x == y)
        return ;

    x->prev->next = y;
    y->prev->next = x;

    list_node *tmp = x->next;
    x->next = y->next;
    y->next = tmp;
    tmp = x->prev;
    x->prev = y->prev;
    y->prev = tmp;
}
template< typename T , typename Alloc>
bool bs_list<T, Alloc>::contains(const T &t) const
{
    list_node *i = list;
    while ((i = i->next) != list)
        if (i->data == t)
            return true;
    return false;
}
template< typename T , typename Alloc>
int bs_list<T, Alloc>::find(const T &t) const
{
    int fi = 0;
    list_node *i = list;
    while ((i = i->next) != list)
    {
        fi++;
        if (i->data == t)
            return fi;
    }
    return -1;
}
template< typename T , typename Alloc>
typename bs_list<T, Alloc>::opelist_node bs_list<T, Alloc>::opelist_node::operator +(int c)
{
    opelist_node node = *this;
    if (c > 0)
        while (c--) ++node;
    else
        while (c++) --node;
    return (node);
}
template< typename T , typename Alloc>
const typename bs_list<T, Alloc>::opelist_node bs_list<T, Alloc>::opelist_node::operator +(int c)const
{
    opelist_node node = *this;
    if (c > 0)
        while (c--) ++node;
    else
        while (c++) --node;
    return (node);
}

template< typename T , typename Alloc>
const typename bs_list<T, Alloc>::opelist_node& bs_list<T, Alloc>::opelist_node::operator++()const
{
    opelist_node *take = (opelist_node *)this;
    take->ope = ope->next;
    return *this;
}
template< typename T , typename Alloc>
const typename bs_list<T, Alloc>::opelist_node bs_list<T, Alloc>::opelist_node::operator++(int)const
{
    opelist_node old = *this;
    ope = ope->next;
    return old;
}
template< typename T , typename Alloc>
const typename bs_list<T, Alloc>::opelist_node& bs_list<T, Alloc>::opelist_node::operator--()const
{
    opelist_node *take = (opelist_node *)this;
    take->ope = ope->prev;
    return *this;
}
template< typename T , typename Alloc>
const typename bs_list<T, Alloc>::opelist_node bs_list<T, Alloc>::opelist_node::operator--(int)const
{
    opelist_node old = *this;
    ope = ope->prev;
    return old;
}
template< typename T , typename Alloc>
typename bs_list<T, Alloc>::opelist_node& bs_list<T, Alloc>::opelist_node::operator++()
{
    ope = ope->next;
    return *this;
}
template< typename T , typename Alloc>
typename bs_list<T, Alloc>::opelist_node bs_list<T, Alloc>::opelist_node::operator++(int)
{
    opelist_node old = *this;
    ope = ope->next;
    return old;
}
template< typename T , typename Alloc>
typename bs_list<T, Alloc>::opelist_node& bs_list<T, Alloc>::opelist_node::operator--()
{
    ope = ope->prev;
    return *this;
}
template< typename T , typename Alloc>
typename bs_list<T, Alloc>::opelist_node bs_list<T, Alloc>::opelist_node::operator--(int)
{
    opelist_node old = *this;
    ope = ope->prev;
    return old;
}
template< typename T , typename Alloc>
int bs_list<T, Alloc>::opelist_node::operator -(const opelist_node &rhs)const
{
    bs_list<T, Alloc>::opelist_node *take = (bs_list<T, Alloc>::opelist_node*)this;
    int retval = 0;
    list_node * nt = ope;
    forever (ope != NULL && ope != rhs.ope)
    {
        this->operator --();
        ++retval;
    }
    if (ope == NULL)
    {
        take->ope = nt;
        retval = 0;
        forever (ope != NULL && ope != rhs.ope)
        {
            this->operator ++();
            --retval;
        }
    }
    take->ope = nt;
    return retval;
}


#endif // BS_LIST_INL

