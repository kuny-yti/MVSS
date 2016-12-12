#ifndef BS_NUMBER
#define BS_NUMBER
#include "bs_string.h"


enum { AsciiSpaceMask = (1 << (' ' - 1)) |
                        (1 << ('\t' - 1)) |   // 9: HT - horizontal tab
                        (1 << ('\n' - 1)) |   // 10: LF - line feed
                        (1 << ('\v' - 1)) |   // 11: VT - vertical tab
                        (1 << ('\f' - 1)) |   // 12: FF - form feed
                        (1 << ('\r' - 1)) };  // 13: CR - carriage return

static bool ascii_isspace(uchar c)
{
    return c >= 1U && c <= 32U && (uint(AsciiSpaceMask) >> uint(c - 1)) & 1U;
}
static char* ulonglong_string(ulonglong l, int base)
{
    static char buff[65]; // length of MAX_ULLONG in base 2
    char *p = buff + 65;

    if (base != 10)
    {
        while (l != 0)
        {
            int c = l % base;
            --p;

            if (c < 10)
                *p = '0' + c;
            else
                *p = c - 10 + 'a';

            l /= base;
        }
    }
    else
    {
        while (l != 0)
        {
            int c = l % base;

            *(--p) = '0' + c;

            l /= base;
        }
    }

    p[65 - (p - buff)] = '\0';
    return p;
}
static char* longlong_string(longlong l, int base)
{
    return ulonglong_string(l < 0 ? -l : l, base);
}

static ulonglong string_ulonglong(const char *nptr, const char **endptr, int base)
{
    const char *s = nptr;
    ulonglong acc;
    unsigned char c;
    ulonglong qbase, cutoff;
    int any, cutlim;

    s = nptr;
    do {
        c = *s++;
    } while (ascii_isspace(c));

    if (c == '-') {
        if (endptr != 0)
            *endptr = s - 1;
        return 0;
    } else {
        if (c == '+')
            c = *s++;
    }
    if ((base == 0 || base == 16) &&
        c == '0' && (*s == 'x' || *s == 'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;
    qbase = unsigned(base);
    cutoff = ulonglong(ULLONG_MAX) / qbase;
    cutlim = ulonglong(ULLONG_MAX) % qbase;
    for (acc = 0, any = 0;; c = *s++)
    {
        if (!isascii(c))
            break;
        if (isdigit(c))
            c -= '0';
        else if (isalpha(c))
            c -= isupper(c) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc *= qbase;
            acc += c;
        }
    }
    if (any == 0) {
    } else if (any < 0) {
        acc = ULLONG_MAX;
    }
    if (endptr != 0)
        *endptr = (any ? s - 1 : nptr);
    return acc;
}
static longlong string_longlong(const char *nptr, const char **endptr, int base)
{
    const char *s;
    ulonglong acc;
    unsigned char c;
    ulonglong qbase, cutoff;
    int neg, any, cutlim;

    s = nptr;
    do {
        c = *s++;
    } while (ascii_isspace(c));
    if (c == '-') {
        neg = 1;
        c = *s++;
    } else {
        neg = 0;
        if (c == '+')
            c = *s++;
    }
    if ((base == 0 || base == 16) &&
        c == '0' && (*s == 'x' || *s == 'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;


    qbase = unsigned(base);
    cutoff = neg ? ulonglong(0-(LLONG_MIN + LLONG_MAX)) + LLONG_MAX : LLONG_MAX;
    cutlim = cutoff % qbase;
    cutoff /= qbase;
    for (acc = 0, any = 0;; c = *s++) {
        if (!isascii(c))
            break;
        if (isdigit(c))
            c -= '0';
        else if (isalpha(c))
            c -= isupper(c) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc *= qbase;
            acc += c;
        }
    }
    if (any < 0) {
        acc = neg ? LLONG_MIN : LLONG_MAX;
    } else if (neg) {
        acc = (~acc) + 1;
    }
    if (endptr != 0)
        *endptr = (any >= 0 ? s - 1 : nptr);

    return acc;
}

template <typename T> static
T toIntegral_helper(const char *data, int len, int base)
{
    len = len;
    longlong val = string_longlong(data, NULL, base);
    if (T(val) != val)
        val = 0;

    return T(val);
}
template <typename T> static
T toUIntegral_helper(const char *data, int len, int base)
{
    len = len;
    ulonglong val = string_ulonglong(data, NULL, base);
    if (T(val) != val)
        val = 0;

    return T(val);
}
#endif // BS_NUMBER

