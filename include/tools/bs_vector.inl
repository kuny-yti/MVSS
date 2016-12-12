#ifndef BS_VECTOR_INL
#define BS_VECTOR_INL

template <typename T >
bs_vector<T>::bs_vector():
    VecBase()
{

}

template <typename T >
bs_vector<T>::bs_vector(size_t count):
    VecBase()
{
    VecBase::resize(count);
}

template <typename T >
bs_vector<T>::bs_vector(const bs_vector& rhs):
    VecBase(rhs)
{

}


template <typename T >
bs_vector<T>& bs_vector<T>::operator =(const bs_vector& rhs)
{
    VecBase::operator =(rhs);
    return *this;
}

template <typename T >
typename bs_vector<T>::iterator bs_vector<T>::begin()
{
    return iterator(VecBase::data());
}
template <typename T >
typename bs_vector<T>::iterator bs_vector<T>::end()
{
    return iterator(VecBase::data()+VecBase::size());
}
template <typename T >
typename bs_vector<T>::const_iterator bs_vector<T>::begin()const
{
    return const_iterator(VecBase::data());
}
template <typename T >
typename bs_vector<T>::const_iterator bs_vector<T>::end()const
{
    return const_iterator(VecBase::data()+VecBase::size());
}

template <typename T >
bool bs_vector<T>::empty() const
{
    return VecBase::is_empty();
}

template <typename T >
T &bs_vector<T>::front()
{
    return *begin();
}
template <typename T >
const T &bs_vector<T>::front()const
{
    return *begin();
}

template <typename T >
T &bs_vector<T>::back()
{
    return *(end()-1);
}
template <typename T >
const T &bs_vector<T>::back()const
{
    return *(end()-1);
}

template <typename T >
void bs_vector<T>::push_back(const T& v)
{
    VecBase::append(v);
}
template <typename T >
void bs_vector<T>::pop_back()
{
    VecBase::remove(VecBase::size()-1);
}

#if bsLanguage < bsLanguage11
template <typename T >
iterator bs_vector<T>::insert(iterator pos, const T& v)
{
    VecBase::insert(pos.ope - begin().ope, v);
    return pos+1;
}
template <typename T >
void bs_vector<T>::insert(iterator pos, size_t count, const T& v)
{
    VecBase::fill(v, count, pos.ope - begin().ope);
}
template <typename T >
iterator bs_vector<T>::erase(iterator pos)
{
    if (pos == end())
        VecBase::remove(count() -1);
    else
        VecBase::remove(pos.ope - begin().ope);
    return begin()+(pos.ope - begin().ope);
}
template <typename T >
iterator bs_vector<T>::erase(iterator first, iterator last)
{
    if (first == end())
        return end();
    size_t offset = (first.ope - begin().ope);
    size_t len = (last.ope - first.ope);
    VecBase::remove(offset, len);
    return begin() + offset;
}
#endif

// C++11
#if bsLanguage >= bsLanguage11

template <typename T >
bs_vector<T>::bs_vector(bs_vector&& rhs):
    VecBase(rhs)
{

}

template <typename T >
void bs_vector<T>::push_back(T&& v)
{
    VecBase::append(v);
}
template <typename T >
typename bs_vector<T>::iterator bs_vector<T>::insert(const_iterator pos, const T& v)
{
    VecBase::insert(pos.ope - begin().ope, v);
    return iterator(pos+1);
}
template <typename T >
typename bs_vector<T>::iterator bs_vector<T>::insert(const_iterator pos, T&& v)
{
    VecBase::insert(pos.ope - begin().ope, v);
    return iterator(pos.ope+1);
}
template <typename T >
typename bs_vector<T>::iterator bs_vector<T>::insert(const_iterator pos, size_t count, const T& v)
{
    VecBase::fill(v, count, pos.ope - begin().ope);
    return iterator(pos+count);
}
template <typename T >
typename bs_vector<T>::iterator bs_vector<T>::erase(const_iterator pos)
{
    if (pos == end())
        VecBase::remove(count() -1);
    else
        VecBase::remove(pos.ope - begin().ope);
    return begin()+(pos.ope - begin().ope);
}
template <typename T >
typename bs_vector<T>::iterator bs_vector<T>::erase(const_iterator first, const_iterator last)
{
    if (first == end())
        return end();
    size_t offset = (first.ope - begin().ope);
    size_t len = (last.ope - first.ope);
    VecBase::remove(offset, len);
    return begin() + offset;
}
#endif

// base
template <typename T >
bs_vector<T>::bs_vector(const std::vector<T> &rhs):VecBase()
{
    form(rhs);
}

template <typename T >
std::vector<T> bs_vector<T>::to_std()
{
    std::vector<T> dat;
    dat.resize(VecBase::size());
    bs_memory::copy_sse(dat.data(), VecBase::data(), VecBase::bytecount());
    return dat;
}
template <typename T >
void bs_vector<T>::form(const std::vector<T> &rhs)
{
    VecBase::resize(rhs.size());
    bs_memory::copy_sse(VecBase::data(), rhs.data(), VecBase::bytecount());
}
template <typename T >
bs_vector<T>& bs_vector<T>::operator =(const std::vector<T>& rhs)
{
    form(rhs);
    return *this;
}
template <typename T >
bool bs_vector<T>::operator ==(const bs_vector<T> &v) const
{
    return VecBase::data() == v.data() && count() == v.count();
}

template <typename T >
bool bs_vector<T>::contains(const T &t) const
{
    for (iterator it = begin(); it != end(); ++it)
        if (*it == t)
            return true;
    return false;
}
template <typename T >
size_t bs_vector<T>::count()const
{
    return VecBase::size();
}

template <typename T >
bs_vector<T> &bs_vector<T>::prepend(T c)
{
    return *(bs_vector<T>*)&VecBase::prepend(c);
}
template <typename T >
bs_vector<T> &bs_vector<T>::prepend(const T *s, int len)
{
    return *(bs_vector<T>*)&VecBase::prepend(s, len);
}
template <typename T >
bs_vector<T> &bs_vector<T>::prepend(const bs_vector<T> &a)
{
    return *(bs_vector<T>*)&VecBase::prepend(a);
}

template <typename T >
bs_vector<T> &bs_vector<T>::append(T c)
{
    return *(bs_vector<T>*)&VecBase::append(c);
}
template <typename T >
bs_vector<T> &bs_vector<T>::append(const T *s, size_t len)
{
    return *(bs_vector<T>*)&VecBase::append(s, len);
}
template <typename T >
bs_vector<T> &bs_vector<T>::append(const bs_vector<T> &a)
{
    return *(bs_vector<T>*)&VecBase::append(a);
}

template <typename T >
bs_vector<T> &bs_vector<T>::insert(size_t i, T c)
{
    return *(bs_vector<T>*)&VecBase::insert(i, c);
}
template <typename T >
bs_vector<T> &bs_vector<T>::insert(size_t i, const T *s, size_t len)
{
    return *(bs_vector<T>*)&VecBase::insert(i, s, len);
}
template <typename T >
bs_vector<T> &bs_vector<T>::insert(size_t i, const bs_vector<T> &a)
{
    return *(bs_vector<T>*)&VecBase::insert(i, a);
}

template <typename T >
bs_vector<T> &bs_vector<T>::remove(size_t index, size_t len)
{
    return *(bs_vector<T>*)&VecBase::remove(index, len);
}

template <typename T >
bs_vector<T> &bs_vector<T>::replace(size_t index, size_t len, const T *s, size_t alen)
{
    return *(bs_vector<T>*)&VecBase::replace(index, len, s, alen);
}
template <typename T >
bs_vector<T> &bs_vector<T>::replace(size_t index, size_t len, const bs_vector<T> &s)
{
    return *(bs_vector<T>*)&VecBase::replace(index, len, s);
}

template <typename T >
bs_vector<T> bs_vector<T>::extract( int pos, int count)
{
    return *(bs_vector<T>*)&VecBase::extract(pos, count);
}
template <typename T >
bs_vector<T> bs_vector<T>::extract( int pos )
{
    return *(bs_vector<T>*)&VecBase::extract(pos);
}
template <typename T >
bs_vector<T> bs_vector<T>::start( int count)
{
    return *(bs_vector<T>*)&VecBase::start(count);
}
template <typename T >
bs_vector<T> bs_vector<T>::end( int count)
{
    return *(bs_vector<T>*)&VecBase::end(count);
}
template <typename T >
void bs_vector<T>::swap(bs_vector<T> &rhs)
{
    VecBase::swap(rhs);
}

template <typename T >
typename bs_vector<T>::opevec_node bs_vector<T>::opevec_node::operator +(int c)
{
    opevec_node node = *this;
    if (c > 0)
        while (c--) ++node;
    else
        while (c++) --node;
    return (node);
}
template <typename T >
const typename bs_vector<T>::opevec_node bs_vector<T>::opevec_node::operator +(int c)const
{
    opevec_node node = *this;
    if (c > 0)
        while (c--) ++node;
    else
        while (c++) --node;
    return (node);
}

template <typename T >
const typename bs_vector<T>::opevec_node & bs_vector<T>::opevec_node::operator++()const
{
    opevec_node *take = (opevec_node *)this;
    ++take->ope;
    return *this;
}
template <typename T >
const typename bs_vector<T>::opevec_node bs_vector<T>::opevec_node::operator++(int)const
{
    opevec_node old = *this;
    ++ope;
    return old;
}
template <typename T >
const typename bs_vector<T>::opevec_node& bs_vector<T>::opevec_node::operator--()const
{
    opevec_node *take = (opevec_node *)this;
    --take->ope;
    return *this;
}
template <typename T >
const typename bs_vector<T>::opevec_node bs_vector<T>::opevec_node::operator--(int)const
{
    opevec_node old = *this;
    --ope;
    return old;
}
template <typename T >
typename bs_vector<T>::opevec_node& bs_vector<T>::opevec_node::operator++()
{
    ++ope;
    return *this;
}
template <typename T >
typename bs_vector<T>::opevec_node bs_vector<T>::opevec_node::operator++(int)
{
    opevec_node old = *this;
    ++ope;
    return old;
}
template <typename T >
typename bs_vector<T>::opevec_node& bs_vector<T>::opevec_node::operator--()
{
    --ope;
    return *this;
}
template <typename T >
typename bs_vector<T>::opevec_node bs_vector<T>::opevec_node::operator--(int)
{
    opevec_node old = *this;
    --ope;
    return old;
}
template <typename T >
int bs_vector<T>::opevec_node::operator - (const opevec_node &rhs)
{
    return int(ope - rhs.ope);
}
#endif // BS_VECTOR_INL

