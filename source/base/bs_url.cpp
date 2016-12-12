#include "bs_url.h"
#include "bs_filesystem.h"
bs_url::bs_url()
{

}
bs_url::bs_url(const bs_string& url, eUrlProtocols pro)
{

    if (pro == Protocol_File)
    {
       url_ = url;
       path_ = url;
       scheme_ = "file";
    }
    else
         resolve(url);
}
bs_url::~bs_url()
{

}
bs_string bs_url::url() const
{
    return url_;
}
bool bs_url::is_empty()const
{
    return url_.is_empty ();
}
bool bs_url::is_valid()const
{
    return !scheme().is_empty ();
}
eUrlProtocols bs_url::protocol()const
{
    if (scheme() == "file")
        return Protocol_File;
    else if (scheme() == "ftp")
        return Protocol_Ftp;
    else if (scheme() == "gopher")
        return  Protocol_Gopher ;
    else if (scheme() == "http")
        return Protocol_Http ;
    else if (scheme() == "https")
        return Protocol_Https ;
    else if (scheme() == "mailto")
        return Protocol_Mailto ;
    else if (scheme() == "mms")
        return Protocol_MMS ;
    else if (scheme() == "telnet")
        return Protocol_Telnet ;
    else if (scheme() == "news")
        return Protocol_News;
    return Protocol_UrlNot;
}
static bool IsSchemeChar(bs_char c)
{
    return (!c.is_letter ()&& '+' != c && '-' != c && '.' != c) ? 0 : 1;
}
bool bs_url::resolve(const bs_string& url)const
{
    bs_url *tack = (bs_url *)this;
    tack->url_ = url;
    bs_char * curstr = (bs_char *)url.data ();
    bs_char * tmpstr;
    int pos = bs_string(curstr, url.count ()).find (":");
    if (pos <= 0)
        return false;
    else if (pos == 1)
    {
        bs_fileinfo finfo(url);
        if (finfo.is_exists ())
        {
            tack->scheme_ = "file";
            tack->path_ = url;
            return true;
        }
    }
    /* Check restrictions */
    for (int i = 0; i < pos; i++ ) {
        if ( !IsSchemeChar(curstr[i]) ) {
            /* Invalid format */
            return false;
        }
    }
    tack->scheme_ = bs_string(curstr, pos);
    tack->scheme_.lower ();

    curstr += pos+1;
    for (int i = 0; i < 2; i++ )
    {
        if ( '/' != *curstr ) {
            return false;
        }
        curstr++;
    }

    int userpass_flag = 0, bracket_flag = 0;
    tmpstr = curstr;
    while ( '\0' != *tmpstr )
    {
        if ( '@' == *tmpstr )
        {
            /* Username and password are specified */
            userpass_flag = 1;
            break;
        } else if ( '/' == *tmpstr ) {
            /* End of <host>:<port> specification */
            userpass_flag = 0;
            break;
        }
        tmpstr++;
    }

    /* User and password specification */
    tmpstr = curstr;
    if ( userpass_flag )
    {
        /* Read username */
        while ( '\0' != *tmpstr && ':' != *tmpstr && '@' != *tmpstr )
        {
            tmpstr++;
        }
        int len = tmpstr - curstr;
        tack->username_ = bs_string(curstr, len);

        /* Proceed current pointer */
        curstr = tmpstr;
        if ( ':' == *curstr ) {
            /* Skip ':' */
            curstr++;
            /* Read password */
            tmpstr = curstr;
            while ( '\0' != *tmpstr && '@' != *tmpstr ) {
                tmpstr++;
            }
            len = tmpstr - curstr;
            tack->password_ = bs_string(curstr, len);
            curstr = tmpstr;
        }
        /* Skip '@' */
        if ( '@' != *curstr ) {
            return false;
        }
        curstr++;
    }

    if ( '[' == *curstr ) {
        bracket_flag = 1;
    } else {
        bracket_flag = 0;
    }
    /* Proceed on by delimiters with reading host */
    tmpstr = curstr;
    while ( '\0' != *tmpstr ) {
        if ( bracket_flag && ']' == *tmpstr ) {
            /* End of IPv6 address. */
            tmpstr++;
            break;
        } else if ( !bracket_flag && (':' == *tmpstr || '/' == *tmpstr) ) {
            /* Port number is specified. */
            break;
        }
        tmpstr++;
    }
    int len = tmpstr - curstr;
    tack->host_ = bs_string(curstr, len);
    curstr = tmpstr;

    /* Is port number specified? */
    if ( ':' == *curstr ) {
        curstr++;
        /* Read port number */
        tmpstr = curstr;
        while ( '\0' != *tmpstr && '/' != *tmpstr ) {
            tmpstr++;
        }
        len = tmpstr - curstr;
        tack->port_ = bs_string(curstr, len);
        curstr = tmpstr;
    }

    /* End of the string */
    if ( '\0' == *curstr ) {
        return true;
    }

    /* Skip '/' */
    //if ( '/' != *curstr ) {
    //return false;
    //}
    //curstr++;

    /* Parse path */
    tmpstr = curstr;
    while ( '\0' != *tmpstr && '#' != *tmpstr  && '?' != *tmpstr ) {
        tmpstr++;
    }
    len = tmpstr - curstr;

    tack->path_ = bs_string(curstr, len);
    curstr = tmpstr;

    /* Is query specified? */
    if ( '?' == *curstr ) {
        /* Skip '?' */
        curstr++;
        /* Read query */
        tmpstr = curstr;
        while ( '\0' != *tmpstr && '#' != *tmpstr ) {
            tmpstr++;
        }
        len = tmpstr - curstr;
        tack->query_ = bs_string(curstr, len);
        curstr = tmpstr;
    }

    /* Is fragment specified? */
    if ( '#' == *curstr ) {
        /* Skip '#' */
        curstr++;
        /* Read fragment */
        tmpstr = curstr;
        while ( '\0' != *tmpstr ) {
            tmpstr++;
        }
        len = tmpstr - curstr;
        tack->fragment_ = bs_string(curstr, len);
        curstr = tmpstr;
    }
    return true;
}
bs_string bs_url::package()const
{
    return bs_string();
}

bs_string bs_url::scheme()const
{
    return scheme_;
}
bs_string bs_url::host()const
{
    return host_;
}
bs_string bs_url::port()const
{
    return port_;
}
bs_string bs_url::path()const
{
    return path_;
}
bs_string bs_url::query()const
{
    return query_;
}
bs_string bs_url::fragment()const
{
    return fragment_;
}
bs_string bs_url::username()const
{
    return username_;
}
bs_string bs_url::password()const
{
    return password_;
}
#include "bs_stream.h"
bs_bstream &operator << (bs_bstream &in, const bs_url &v)
{
    in << (int)v.protocol();
    in << v.url() ;
    return in;
}
bs_bstream &operator >> (bs_bstream &out, bs_url &v)
{
    int prot = 0;
    bs_string urls;
    out >> prot;
    out >> urls;
    v = bs_url(urls, (eUrlProtocols)prot);
    return out;
}

