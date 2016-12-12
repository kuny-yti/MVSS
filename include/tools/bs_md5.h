#ifndef BS_MD5
#define BS_MD5
#include "bs_define.h"
#include "bs_string.h"

class bs_md5 {

public:
    bs_md5();
    bs_md5(const void *input, size_t length);
    bs_md5(const bs_string &str);

    void update(const void *input, size_t length);
    void update(const bs_string &str);

    const uchar *digest();
    bs_string to_string();

    void reset();
private:
    void update(const uchar *input, size_t length);

    void final();
    void transform(const uchar block[64]);

    void encode(const uint32_t *input, uchar *output, size_t length);
    void decode(const uchar *input, uint32_t *output, size_t length);

    bs_string byte_hex(const uchar *input, size_t length);

    /* class uncopyable */
    bs_md5(const bs_md5&);
    bs_md5& operator=(const bs_md5&);

private:
    uint32_t _state[4];	/* state (ABCD) */
    uint32_t _count[2];	/* number of bits, modulo 2^64 (low-order word first) */
    uchar _buffer[64];	/* input buffer */
    uchar _digest[16];	/* message digest */
    bool _finished;		/* calculate finished ? */

    static const uchar PADDING[64];	/* padding for calculate */
    static const char HEX[16];
    static const size_t BUFFER_SIZE = 1024;

};
#endif // BS_MD5

