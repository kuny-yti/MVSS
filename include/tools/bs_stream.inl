#ifndef BS_STREAM_INL
#define BS_STREAM_INL

template <typename OpeT>
bs_stream<OpeT>::bs_stream(const bs_byte &ay):
    impl(bsAsCast(tStreamDatas*)(this))
{
    if (!impl)
        return ;
    impl->border = (bsArchEndian);
    impl->array = ((bs_byte*)&ay);

    int bv = *(eByteOrders*)impl->array->data();
    if (bv == Endian_Big)
        impl->border = Endian_Big;
    else if (bv == Endian_Little)
        impl->border = Endian_Little;
    else
        impl->array = NULL;

    if (impl->array)
        impl->array->remove(0, sizeof(eByteOrders));
}
template <typename OpeT>
bs_stream<OpeT>::bs_stream(bs_byte *ay):
    impl(bsAsCast(tStreamDatas*)(this))
{
    if (!impl)
        return ;
    impl->border = (bsArchEndian);
    impl->array = (ay);
    if (impl->array)
        impl->put((char*)&impl->border, sizeof(eByteOrders));
}
template <typename OpeT>
bs_stream<OpeT>::~bs_stream(){impl = NULL;}
template <typename OpeT>
size_t bs_stream<OpeT>::size()const
{
    return impl && impl->array ? impl->array->size():0;
}
template <typename OpeT>
bs_stream<OpeT> & operator << (bs_stream<OpeT> &in, const bs_stream<OpeT> & v)
{
    size_t vs = v.size();
    if (vs && v.impl)
    {
        in.put(&vs, sizeof(size_t));
        char *tmpbuf = (char*)v.take(&vs);
        in.put(tmpbuf, vs);
    }
    return in;
}

template <typename OpeT>
bs_stream<OpeT> & operator >> (bs_stream<OpeT> &out, bs_stream<OpeT> & v)
{
    if (out.impl && out.size() && v.impl)
    {
        int len = sizeof(size_t);
        size_t vs = *(size_t*)out.take(&len);
        v.put(&vs, len);

        char *vd = (char*)out.take(&vs);
        v.put(vd, vs);
    }
    return out;
}

template <typename OpeT>
bs_stream<OpeT> &operator << (bs_stream<OpeT> &in, const bs_byte &v)
{
    size_t vs = v.size();
    in.put(&vs, sizeof(size_t));
    in.put(v.data(), vs);
    return in;
}
template <typename OpeT>
bs_stream<OpeT> &operator >> (bs_stream<OpeT> &out, bs_byte &v)
{
    if (out.impl && out.size())
    {
        int len = sizeof(size_t);
        len = *(size_t*) out.take(&len);
        char *buf = (char*)out.take(&len);
        v.append((bs_byte::array_t*)buf, len);
    }
    return out;
}


#endif // BS_STREAM_INL
