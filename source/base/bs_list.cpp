
#include "bs_list.h"

const _bs_list_ _bs_list_::shared_null =
{
    (_bs_list_*)&_bs_list_::shared_null,
    (_bs_list_*)&_bs_list_::shared_null,
    bs_ref::refStatic, 0
};

bool _bs_list_::is_null (intptr p)
{
    return p == (intptr)&shared_null;
}
intptr _bs_list_::null()
{
    return (intptr)&shared_null;
}
