#ifndef BS_UTILS_H
#define BS_UTILS_H

#include "bs_define.h"
#include "bs_ptr.h"

//!
//! \brief The bs_noncopy class 禁止拷贝基类
//!
class bs_noncopy
{
protected:
    bs_noncopy(){}
    virtual ~bs_noncopy(){}

private:
    bs_noncopy(const bs_noncopy&);
    bs_noncopy &operator = (const bs_noncopy &);
};

template<typename sT>
class bs_singleton
{
public:
    typedef bs_ptr(sT) singT;

public:
    static singT instance()
    {
        static singT instp;
        if (instp.get () == NULL)
            instp = bs_malloc(sT);
        return instp;
    }
};

#endif
