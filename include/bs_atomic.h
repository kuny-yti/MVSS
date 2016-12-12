#ifndef BS_ATOMIC
#define BS_ATOMIC

#include <pthread.h>
/*
 * Atomic operations
 */
template<typename T>
class bs_atomic
{
public:
    bs_atomic() {ope_value = T(0);}
    bs_atomic(T v) {ope_value = v;}
    T value()const {return ope_value;}
    operator T () {return ope_value;}
    operator const T ()const {return ope_value;}
    operator T ()volatile {return ope_value;}
    operator const T ()const volatile {return ope_value;}
public:
    // 后置
    T operator ++ (int) {return __sync_fetch_and_add (&ope_value, 1);}
    T operator -- (int) {return __sync_fetch_and_sub (&ope_value, 1);}
    T operator ++ (int)volatile {return __sync_fetch_and_add (&ope_value, 1);}
    T operator -- (int)volatile {return __sync_fetch_and_sub (&ope_value, 1);}

    // 前置
    T operator ++ () {return __sync_add_and_fetch(&ope_value, 1);}
    T operator -- () {return __sync_sub_and_fetch(&ope_value, 1);}
    T operator ++ ()volatile {return __sync_add_and_fetch(&ope_value, 1);}
    T operator -- ()volatile {return __sync_sub_and_fetch(&ope_value, 1);}

    T operator + (T value) {return __sync_fetch_and_add (&ope_value, value);}
    T operator - (T value) {return __sync_fetch_and_sub (&ope_value, value);}
    T operator + (T value)volatile {return __sync_fetch_and_add (&ope_value, value);}
    T operator - (T value)volatile {return __sync_fetch_and_sub (&ope_value, value);}

    /*T operator & (T value) {return __sync_fetch_and_and (&ope_value, value);}
    T operator | (T value) {return __sync_fetch_and_or (&ope_value, value);}
    T operator ^ (T value) {return __sync_fetch_and_xor (&ope_value, value);}
    //T operator ~ (T value) //非
    //{return __sync_fetch_and_nand (&ope_value, value);}
    T operator & (T value)volatile {return __sync_fetch_and_and (&ope_value, value);}
    T operator | (T value)volatile {return __sync_fetch_and_or (&ope_value, value);}
    T operator ^ (T value)volatile {return __sync_fetch_and_xor (&ope_value, value);}
    //T operator ~ (T value)volatile //非
    //{return __sync_fetch_and_nand (&ope_value, value);}*/

public:
    T operator = (T value) {return __sync_lock_test_and_set (&ope_value, value);}
    T operator = (T value)volatile {return __sync_lock_test_and_set (&ope_value, value);}

    bs_atomic<T> & operator = (const bs_atomic<T> &rhs) {__sync_lock_test_and_set (&ope_value, rhs.ope_value); return *this;}
    bs_atomic<T> & operator = (const bs_atomic<T> &rhs)volatile {__sync_lock_test_and_set (&ope_value, rhs.ope_value); return *this;}

public:
    T operator += (T value) {return __sync_add_and_fetch (&ope_value, value);}
    T operator -= (T value) {return __sync_sub_and_fetch (&ope_value, value);}
    T operator += (T value)volatile {return __sync_add_and_fetch (&ope_value, value);}
    T operator -= (T value)volatile {return __sync_sub_and_fetch (&ope_value, value);}

    T operator &= (T value) {return __sync_and_and_fetch (&ope_value, value);}
    T operator |= (T value) {return __sync_or_and_fetch (&ope_value, value);}
    T operator ^= (T value) {return __sync_xor_and_fetch (&ope_value, value);}
    //T operator ~= (T value) //非
    //{return __sync_nand_and_fetch (&ope_value, value);}
    T operator &= (T value)volatile {return __sync_and_and_fetch (&ope_value, value);}
    T operator |= (T value)volatile {return __sync_or_and_fetch (&ope_value, value);}
    T operator ^= (T value)volatile {return __sync_xor_and_fetch (&ope_value, value);}
    //T operator ~ (T value)volatile //非
    //{return __sync_fetch_and_nand (&ope_value, value);}

public:
    bool bcswap(bool oldval, bool newval) {return __sync_bool_compare_and_swap(&ope_value, oldval, newval);}
    T vcswap(T oldval, T newval) {return __sync_val_compare_and_swap(&ope_value, oldval, newval);}
    bool bcswap(bool oldval, bool newval)volatile { return __sync_bool_compare_and_swap(&ope_value, oldval, newval);}
    T vcswap(T oldval, T newval)volatile {return __sync_val_compare_and_swap(&ope_value, oldval, newval);}

private:
    T ope_value;
};


#endif // BS_ATOMIC

