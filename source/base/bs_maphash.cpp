#include "bs_maphash.h"

const _bs_maphash_ _bs_maphash_::shared_null =
{
    bs_ref::refStatic, 0, 0, 0
};

float _bs_maphash_::sMaxUsageRate = .9f;   ///< 桶的最大使用率
float _bs_maphash_::sGrowRate = 1.7312543f;///< 桶的增长率

bool _bs_maphash_::is_null(intptr p)
{
    return p == (intptr)&_bs_maphash_::shared_null;
}
intptr _bs_maphash_::null()
{
    const _bs_maphash_ *nl = (&_bs_maphash_::shared_null);
    return (intptr)(nl++);
}
