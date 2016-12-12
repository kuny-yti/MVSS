#ifndef BS_UUID
#define BS_UUID

#include "bs_define.h"
#include "bs_string.h"
#include "bs_datetime.h"

struct bs_uuid
{
    uint32   data1;
    uint16   data2;
    uint16   data3;
    uchar    data4[8];

    bs_string to_string();
    bs_datetime to_timestamp();
    bool is_uuid(const char *uuid);

    bool operator ==(const bs_uuid &rhs)const
    {
        return data1 == rhs.data1 &&
                data2 == rhs.data2 &&
                data3 == rhs.data3 &&
                data4[0] == rhs.data4[0] &&
                data4[1] == rhs.data4[1] &&
                data4[2] == rhs.data4[2] &&
                data4[3] == rhs.data4[3] &&
                data4[4] == rhs.data4[4] &&
                data4[5] == rhs.data4[5] &&
                data4[6] == rhs.data4[6] &&
                data4[7] == rhs.data4[7] ;
    }
    bool operator !=(const bs_uuid &rhs)const
    {
        return !(*this == rhs);
    }
    bs_uuid &operator =(const bs_uuid &rhs)
    {
         data1 = rhs.data1;
         data2 = rhs.data2;
         data3 = rhs.data3 ;
         data4[0] = rhs.data4[0];
         data4[1] = rhs.data4[1];
         data4[2] = rhs.data4[2];
         data4[3] = rhs.data4[3];
         data4[4] = rhs.data4[4];
         data4[5] = rhs.data4[5];
         data4[6] = rhs.data4[6] ;
         data4[7] = rhs.data4[7] ;
         return *this;
    }

    static bs_uuid create();
    static bs_string create_string();
    static bs_uuid  create_external(const char *external);
};

const bs_uuid uuid_null = {0,0,0,{0,0,0,0,0,0,0,0}};
const bs_uuid uuid_max = {0xFFFFFFFF, 0xFFFF, 0xFFFF, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};

bs_bstream &operator << (bs_bstream &in, const bs_uuid &v);
bs_bstream &operator >> (bs_bstream &out, bs_uuid &v);
bs_tstream &operator << (bs_tstream &in, const bs_uuid &v);
bs_tstream &operator >> (bs_tstream &out, bs_uuid &v);
#endif // BS_UUID

