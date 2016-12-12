


template<typename T, typename Alloc>
bool bs_array<T, Alloc>::is_null()const
{
    return _bs_array_::is_null((intptr)impl);
}

// C++11
#if bsLanguage >= bsLanguage11
template<typename T, typename Alloc>
bs_array<T, Alloc>::bs_array(bs_array&& rhs):
    Alloc(),
    impl((T*)_bs_array_::null())
{
    *this = rhs;
}
template<typename T, typename Alloc>
bs_array<T, Alloc>& bs_array<T, Alloc>::operator =(bs_array&& rhs)
{
    if (!is_null() && header()->lessref())
    {
        this->destroy((T*)header());
        impl = (T*)_bs_array_::null();
    }
    if (rhs.is_null())
        return *this;
    impl = rhs.impl;
    rhs.impl = (T*)_bs_array_::null();
    return *this;
}
#endif

template<typename T, typename Alloc>
bs_array<T, Alloc>::bs_array():
    impl((T*)_bs_array_::null())
{
}
template<typename T, typename Alloc>
bs_array<T, Alloc>::bs_array(size_t size):
    impl((T*)_bs_array_::null())
{
    reserve(size + arrayCapacityDefaults);
}
template<typename T, typename Alloc>
bs_array<T, Alloc>::bs_array(const bs_array<T, Alloc> &rhs):
    impl((T*)_bs_array_::null())
{
    if (rhs.impl == impl)
        return ;
    impl = rhs.impl;
    if (rhs.header()->has_shareable())
        header()->addref();
    else
        detach();
}
template<typename T, typename Alloc>
bs_array<T, Alloc>::bs_array(const T *buf, size_t len):
    impl((T*)_bs_array_::null())
{
    append(buf, len);
}
template<typename T, typename Alloc>
bs_array<T, Alloc>::~bs_array()
{
    if (!is_null() && header()->lessref())
        this->destroy((T*)header());
    impl = NULL;
}

template<typename T, typename Alloc>
size_t bs_array<T, Alloc>::capacity() const
{
    return header()->cnum;
}
template<typename T, typename Alloc>
void bs_array<T, Alloc>::reserve(size_t size)
{
    if (!is_null() && header()->lessref())
        this->destroy((T*)header());

    _data_ *ph = (_data_*)this->alloc((size+1) * sizeOf + sizeof(_bs_array_));
    ph->header.cnum = size+1;
    ph->header.num = 0;
    ph->header.set(bs_ref::refShareableDetach);
    impl = &ph->d;
    /*impl[size] = T();*/
    clear();
}
template<typename T, typename Alloc>
size_t bs_array<T, Alloc>::size()const
{
    return header()->num;
}
template<typename T, typename Alloc>
size_t bs_array<T, Alloc>::bytecount()const
{
    return size() * sizeOf;
}
template<typename T, typename Alloc>
void bs_array<T, Alloc>::resize(size_t s)
{
    if (s < 1) return;
    if (s > capacity())
        reserve(s+ arrayCapacityDefaults);
    else
        clear();
    header()->num = s;
}
template<typename T, typename Alloc>
void bs_array<T, Alloc>::resize(size_t s, const T &v)
{
    resize(s);
    fill(v, s);
}
template<typename T, typename Alloc>
bool bs_array<T, Alloc>::is_empty() const
{
    return size() == 0;
}

template<typename T, typename Alloc>
bs_array<T, Alloc>::operator T *() const
{
    return this->data();
}
template<typename T, typename Alloc>
bs_array<T, Alloc>::operator void *() const
{
    return (void*)this->operator const T *();
}
template<typename T, typename Alloc>
T *bs_array<T, Alloc>::data()
{
    detach();
    return impl;
}
template<typename T, typename Alloc>
T *bs_array<T, Alloc>::data() const
{
    bs_array<T, Alloc> *take = (bs_array<T, Alloc>*) this;
    take->detach();
    return impl;
}
template<typename T, typename Alloc>
void bs_array<T, Alloc>::copy()
{
    if (size())
    {
        _bs_array_ *tmh = header();
        T *tmp = impl;
        impl = (T*)_bs_array_::null();
        reserve(tmh->num);
        header()->num = tmh->num;
        bs_memory::copy_sse(data(), tmp, bytecount());
        if (tmh->lessref())
            this->destroy((T*)tmh);
    }
}
template<typename T, typename Alloc>
void bs_array<T, Alloc>::detach()
{
    if (!is_null() && header()->has_detach() && header()->is_shared())
        copy();
}
template<typename T, typename Alloc>
void bs_array<T, Alloc>::clear()
{
    detach();
    if (!is_null() && capacity())
        bs_memory::set_c(data(), '\0', capacity()*sizeOf);
}

template<typename T, typename Alloc>
void bs_array<T, Alloc>::fill(const T &c, size_t len, size_t pos)
{
    detach();
    if (!is_null() && capacity())
    {
        len = ((len+pos) > capacity() ? capacity()-pos : len);
        do
            data()[pos++] = c;
        while (--len);
    }
}
template<typename T, typename Alloc>
T bs_array<T, Alloc>::at(size_t i)
{
    return (*this)[i];
}
template<typename T, typename Alloc>
T bs_array<T, Alloc>::at(size_t i)const
{
    return (*this)[i];
}
template<typename T, typename Alloc>
T &bs_array<T, Alloc>::operator[](size_t i)
{
    static T at = T();
    if (i > size() || is_null())
        return at;
    detach();
    return data()[i];
}
template<typename T, typename Alloc>
const T &bs_array<T, Alloc>::operator[](size_t i) const
{
    static T at = T();
    if ( i > size() || is_null())
        return at;
    return data()[i];
}

template<typename T, typename Alloc>
bs_array<T, Alloc> &bs_array<T, Alloc>::prepend(const T &c)
{
    return insert(0, c);
}
template<typename T, typename Alloc>
bs_array<T, Alloc> &bs_array<T, Alloc>::prepend(const T *s, int len)
{
    return insert(0, s, len);
}
template<typename T, typename Alloc>
bs_array<T, Alloc> &bs_array<T, Alloc>::prepend(const bs_array<T, Alloc> &a)
{
    return insert(0, a);
}
template<typename T, typename Alloc>
bs_array<T, Alloc> &bs_array<T, Alloc>::append(const T &c)
{
    return insert(size(), c);
}
template<typename T, typename Alloc>
bs_array<T, Alloc> &bs_array<T, Alloc>::append(const T *s, size_t len)
{
    return insert(size(), s, len);
}
template<typename T, typename Alloc>
bs_array<T, Alloc> &bs_array<T, Alloc>::append(const bs_array<T, Alloc> &a)
{
    return insert(size(), a);
}
template<typename T, typename Alloc>
bs_array<T, Alloc> &bs_array<T, Alloc>::operator =(const bs_array<T, Alloc> &rhs)
{
    if (impl == rhs.impl)
        return *this;
    if (!is_null() && header()->lessref())
        this->destroy((T*)header());
    impl = rhs.impl;
    if (rhs.header()->has_shareable())
        header()->addref();
    else
        detach();
    return *this;
}
template<typename T, typename Alloc>
bs_array<T, Alloc> &bs_array<T, Alloc>::insert(size_t i, const T& c)
{
    return insert(i, &c, 1);
}
template<typename T, typename Alloc>
bs_array<T, Alloc> &bs_array<T, Alloc>::insert(size_t i, const T *s, size_t len)
{
    if (is_null())
        reserve(len + arrayCapacityDefaults);
    detach();
    if (size() + len >= capacity())
    {
        bs_array<T, Alloc> temp = *this;
        reserve(temp.size() + len + arrayCapacityDefaults);
        header()->num = temp.size();
        bs_memory::copy_sse(data(), temp.data(), temp.bytecount());
    }

    if (i < size())
    {
        bs_memory::move_c(data()+i+len, data()+i, (size() - i)*sizeOf);
        bs_memory::copy_sse(data()+i, s, len*sizeOf);
        header()->num += len;
    }
    else
    {
        bs_memory::copy_sse(data()+i, s, len*sizeOf);
        header()->num += len;
    }

    return *this;
}
template<typename T, typename Alloc>
bs_array<T, Alloc> &bs_array<T, Alloc>::insert(size_t i, const bs_array<T, Alloc> &a)
{
    return insert(i, a.data(), a.size());
}
template<typename T, typename Alloc>
bs_array<T, Alloc> &bs_array<T, Alloc>::remove(size_t index, size_t len)
{
    static bs_array<T, Alloc> sT;
    if (is_null())
        return sT;

    detach();
    if (index < size())
    {
        bs_memory::set_c(data()+index, 0, ((len+index) < capacity() ? len : capacity()-index)*sizeOf);
        if (index + len < size())
        {
            bs_memory::move_c(data()+index, data()+index+len, (size()-index-len)*sizeOf);
            //bs_memory::copy_sse(data()+index, data()+index+len, (size()-index-len)*sizeOf);
            bs_memory::set_c(data()+size() - len, 0, len*sizeOf);
        }
        header()->num -= len;
    }
    return *this;
}
template<typename T, typename Alloc>
bs_array<T, Alloc> &bs_array<T, Alloc>::replace(size_t index, const T&c)
{
    return replace(index, 1, &c, 1);
}
template<typename T, typename Alloc>
bs_array<T, Alloc> &bs_array<T, Alloc>::replace(size_t index, size_t len, const T *s, size_t alen)
{
    static bs_array<T, Alloc> sT;
    if (is_null())
        return sT;

    detach();
    if (len != alen)
    {
        int templen = len - alen;
        if (templen > 0)
        {
            // 要替换的比替换的长
            remove(index+alen, templen); // 删除多余的，然后直接覆盖需要替换的
            bs_memory::copy_sse(data()+index, s, alen*sizeOf);
        }
        else
        {
            // 要替换的没有替换的长
            remove(index, len); // 删除需要替换的，然后插入替换的
            insert(index, s, alen);
        }
    }
    else
    {
        // 要替换的和替换的相等
        bs_memory::copy_sse(data()+index, s, alen*sizeOf);
    }
    return *this;
}
template<typename T, typename Alloc>
bs_array<T, Alloc> &bs_array<T, Alloc>::replace(size_t index, size_t len, const bs_array<T, Alloc> &s)
{
    return replace(index, len, s.data(), s.size());
}

template<typename T, typename Alloc>
size_t bs_array<T, Alloc>::shrink()
{
    size_t cap = capacity() - size();
    if (cap == capacity())
    {
    }
    else
        copy();
    return cap;
}
template<typename T, typename Alloc>
void bs_array<T, Alloc>::swap(bs_array<T, Alloc> &rhs)
{
    if (rhs.impl != impl)
    {
        T *tmp = impl;
        impl = rhs.impl;
        rhs.impl = tmp;
    }
}
template<typename T, typename Alloc>
bs_array<T, Alloc> bs_array<T, Alloc>::extract( int pos, int count)const
{
    bs_array<T, Alloc> tmp(this->data(), this->size());
    if (count > 0)
        tmp.remove(pos, count);
    else
        tmp.remove(pos -abs(count), abs(count));
    return tmp;
}
template<typename T, typename Alloc>
bs_array<T, Alloc> bs_array<T, Alloc>::extract( int pos )const
{
    bs_array<T, Alloc> tmp(this->data(), this->size());
    tmp.remove(0, pos);
    return tmp;
}
template<typename T, typename Alloc>
bs_array<T, Alloc> bs_array<T, Alloc>::start( int count)const
{
    bs_array<T, Alloc> tmp(this->data(), this->size());
    tmp.remove(count, this->size() - count);
    return tmp;
}
template<typename T, typename Alloc>
bs_array<T, Alloc> bs_array<T, Alloc>::end( int count)const
{
    bs_array<T, Alloc> tmp(this->data(), this->size());
    tmp.remove(0, this->size() - count);
    return tmp;
}
