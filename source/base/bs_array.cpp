#include "bs_array.h"

const _bs_array_ _bs_array_::shared_null = {};

bool _bs_array_::is_null (intptr p)
{
    return p == (intptr)&shared_null;
}
intptr _bs_array_::null()
{
    return (intptr)&shared_null;
}

static const char b64string[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
bs_byte base64enc(const bs_byte &s)
{
    bs_byte out;
    out.resize (s.size ());

    size_t len = s.size ();
    char *fromp = (char*)s.data ();
    char *top = (char*)out.data ();
    uchar cbyte;
    uchar obyte;
    char end[3];

    for (; len >= 3; len -= 3)
    {
        cbyte = *fromp++;
        *top++ = b64string[(int)(cbyte >> 2)];
        obyte = (cbyte << 4) & 0x30;		/* 0011 0000 */

        cbyte = *fromp++;
        obyte |= (cbyte >> 4);			/* 0000 1111 */
        *top++ = b64string[(int)obyte];
        obyte = (cbyte << 2) & 0x3C;		/* 0011 1100 */

        cbyte = *fromp++;
        obyte |= (cbyte >> 6);			/* 0000 0011 */
        *top++ = b64string[(int)obyte];
        *top++ = b64string[(int)(cbyte & 0x3F)];/* 0011 1111 */
    }

    if (len)
    {
        end[0] = *fromp++;
        if (--len)
            end[1] = *fromp++;
        else
            end[1] = 0;
        end[2] = 0;

        cbyte = end[0];
        *top++ = b64string[(int)(cbyte >> 2)];
        obyte = (cbyte << 4) & 0x30;		/* 0011 0000 */

        cbyte = end[1];
        obyte |= (cbyte >> 4);
        *top++ = b64string[(int)obyte];
        obyte = (cbyte << 2) & 0x3C;		/* 0011 1100 */

        if (len)
            *top++ = b64string[(int)obyte];
        else
            *top++ = '=';
        *top++ = '=';
    }
    *top = 0; //return top - to;
    //out.shrink ();
    return out;
}
#define badchar(c,p) (!(p = (char*)memchr(b64string, c, 64)))
bs_byte base64dec(const bs_byte &d)
{
    bs_byte out;
    out.resize (d.size ());
    size_t len = d.size ();
    char *fromp = (char*)d.data ();
    char *top = (char*)out.data ();
    char *p;
    uchar cbyte;
    uchar obyte;
    int padding = 0;

    for (; len >= 4; len -= 4)
    {
        if ((cbyte = *fromp++) == '=')
            cbyte = 0;
        else
        {
            if (badchar(cbyte, p))
                return bs_byte();
            cbyte = (p - b64string);
        }
        obyte = cbyte << 2;		/* 1111 1100 */

        if ((cbyte = *fromp++) == '=')
            cbyte = 0;
        else
        {
            if (badchar(cbyte, p))
                return bs_byte();
            cbyte = p - b64string;
        }
        obyte |= cbyte >> 4;		/* 0000 0011 */
        *top++ = obyte;

        obyte = cbyte << 4;		/* 1111 0000 */
        if ((cbyte = *fromp++) == '=')
        {
            cbyte = 0;
            padding++; }
        else
        {
            padding = 0;
            if (badchar (cbyte, p))
                return bs_byte();
            cbyte = p - b64string;
        }
        obyte |= cbyte >> 2;		/* 0000 1111 */
        *top++ = obyte;

        obyte = cbyte << 6;		/* 1100 0000 */
        if ((cbyte = *fromp++) == '=')
        {
            cbyte = 0;
            padding++;
        }
        else
        {
            padding = 0;
            if (badchar (cbyte, p))
                return bs_byte();
            cbyte = p - b64string;
        }
        obyte |= cbyte;			/* 0011 1111 */
        *top++ = obyte;
    }

    *top = 0;
    if (len)
        return bs_byte();
    return out;
    //return (top - to) - padding;
}
