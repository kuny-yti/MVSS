#include "hash_bucket.h"
#include "tools/bs_memory.h"

const hash_bucket hash_bucket::shared_null = {bs_vector<uint32>(),NULL, bs_ref::refStatic, 0, 0};

bool hash_bucket::is_null (intptr p)
{
    return p == (intptr)&shared_null;
}
intptr hash_bucket::null()
{
    return (intptr)&shared_null;
}

size_t hash_bucket::getprime(size_t size)
{
    const int _PrimeSize = 28;
    static const unsigned long _PrimeList[_PrimeSize] =
    {
        53ul, 97ul, 193ul, 389ul, 769ul,
        1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
        49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
        1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
        50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
        1610612741ul, 3221225473ul, 4294967291ul
    };
    for (int i = 0; i < _PrimeSize; ++i)
    {
        if (_PrimeList[i] > size)
            return _PrimeList[i];
    }
    return 4294967291ul;
}
bucket_node* hash_bucket::first()
{
    bucket_node *e = (bucket_node*)this;
    bucket_node **bucket = buckets;
    int n = bucket_capacity.value ();
    while (n--) {
        if (*bucket != e)
            return *bucket;
        ++bucket;
    }
    return e;
}
bucket_node *hash_bucket::next(bucket_node *node)
{
    union {
        bucket_node *next;
        bucket_node *e;
        hash_bucket *d;
    };
    next = node->next;
    if (next->next)
        return next;

    int start = (node->hash % d->bucket_capacity.value ()) + 1;
    bucket_node **bucket = d->buckets + start;
    int n = d->bucket_capacity.value () - start;
    while (n--) {
        if (*bucket != e)
            return *bucket;
        ++bucket;
    }
    return e;
}
bucket_node *hash_bucket::prev(bucket_node *node)
{
    union {
        bucket_node *e;
        hash_bucket *d;
    };

    e = node;
    while (e->next)
        e = e->next;

    int start;
    if (node == e)
        start = d->bucket_capacity.value ()- 1;
    else
        start = node->hash % d->bucket_capacity.value ();

    bucket_node *sentinel = node;
    bucket_node **bucket = d->buckets + start;
    while (start >= 0) {
        if (*bucket != sentinel) {
            bucket_node *prev = *bucket;
            while (prev->next != sentinel)
                prev = prev->next;
            return prev;
        }

        sentinel = e;
        --bucket;
        --start;
    }
    return e;
}
