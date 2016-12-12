#ifndef BS_URL
#define BS_URL

#include "bs_string.h"
#include "bs_stream.h"

typedef enum
{
    Protocol_File,
    Protocol_Ftp,
    Protocol_Gopher ,
    Protocol_Http ,
    Protocol_Https ,
    Protocol_Mailto ,
    Protocol_MMS ,
    Protocol_Telnet ,
    Protocol_News ,
    Protocol_UrlNot
}eUrlProtocols;

class bs_url
{
public:
    bs_url();
    explicit bs_url(const bs_string& url, eUrlProtocols = Protocol_File);
    ~bs_url();

    bool is_empty()const;
    bool is_valid()const;

    eUrlProtocols protocol()const;

    bool resolve(const bs_string& url)const;
    bs_string package()const;

    bs_string scheme()const;
    bs_string host()const;
    bs_string port()const;
    bs_string path()const;
    bs_string query()const;
    bs_string fragment()const;
    bs_string username()const;
    bs_string password()const;
    bs_string url() const;
private:
    bs_string url_;

    bs_string scheme_;
    bs_string host_;
    bs_string port_;
    bs_string path_;
    bs_string query_;
    bs_string fragment_;
    bs_string username_;
    bs_string password_;
};

bs_bstream &operator << (bs_bstream &in, const bs_url &v);
bs_bstream &operator >> (bs_bstream &out, bs_url &v);
#endif // BS_URL

