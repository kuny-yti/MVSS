
#ifndef ISOCKET
#define ISOCKET

#include "bs_define.h"
#include "tools/bs_string.h"
#include "bs_ptr.h"

//! Socket 选项
typedef enum
{
    Option_OPT,            //IP_OPTIONS, ptr,len
    Option_TOS,            //IP_TOS, uchar [get]
    Option_TTL,            //IP_TTL, int [get]
    Option_MulticastTTL,   //IP_MULTICAST_TTL, int [get]
    Option_MulticastLoop,  //IP_MULTICAST_LOOP, int(0-1)
    Option_AddMembership,  //IP_ADD_MEMBERSHIP, struct ip_mreqn, struct ip_mreq
    Option_DropMembership, //IP_DROP_MEMBERSHIP, struct ip_mreqn, struct ip_mreq
    Option_ReuseAddr,      //SO_REUSEADDR, int(0-1)
    Option_KeepAlive,      //SO_KEEPALIVE, int(0-1)
    Option_AcceptConn,     //SO_ACCEPTCONN, int [get]-
    Option_Broadcast,      //SO_BROADCAST, int(0-1)
    Option_Debug,          //SO_DEBUG, int(0-1)
    Option_Error,          //SO_ERROR, int [get]-
    Option_DontRoute,      //SO_DONTROUTE, int(0-1)
    Option_Linger,         //SO_LINGER, struct linger
    Option_OOBInline,      //SO_OOBINLINE, int(0-1)
    Option_RcvLowat,       //SO_RCVLOWAT, int
    Option_SndLowat,       //SO_SNDLOWAT, int
    Option_RcvTimeo,       //SO_RCVTIMEO, struct timeval
    Option_SndTimeo,       //SO_SNDTIMEO, struct timeval
    Option_RcvBuf,         //SO_RCVBUF, int [get]
    Option_SndBuf,         //SO_SNDBUF, int [get]
    Option_Type,           //SO_TYPE, int [get]
    Option_PktInfo ,       //IP_PKTINFO, int(0-1)
    Option_Hdrincl ,       //IP_HDRINCL, int(0-1)

    Option_RecvTOS ,       //IP_RECVTOS, int(0-1)
    Option_RecvTTL ,       //IP_RECVTTL, int(0-1)
    Option_RecvOpts ,      //IP_RECVOPTS, int(0-1)
    Option_RetOpts,        //IP_RETOPTS, int(0-1)
    Option_RecvErr ,       //IP_RECVERR, int(0-1)
    Option_MtuDiscover ,   //IP_MTU_DISCOVER, int(0-1)
    Option_Mtu ,           //IP_MTU, int [get]-
    Option_RouterAlert ,   //IP_ROUTER_ALERT, int(0-1)
    Option_NoSigPipe ,     //SO_NOSIGPIPE, int(0-1)
    Option_BSDCompat ,     //SO_BSDCOMPAT, int(0-1)
    Option_BindToDevive ,  //SO_BINDTODEVICE, char ptr, len
    Option_PassCred ,      //SO_PASSCRED, int(0-1)
    Option_PeerCred ,      //SO_PEERCRED, struct ucred
    Option_Priority ,      //SO_PRIORITY, int
    Option_RcvBuffOrce ,   //SO_RCVBUFFORCE, int
    Option_SndBuffOrce ,   //SO_SNDBUFFORCE, int
    Option_Timestamp ,    //SO_TIMESTAMP, int(0-1)

}eNetworkOptions;

//! Socket 模式
typedef enum
{
    Socket_Not,
    Socket_TCP,
    Socket_UDP,
    Socket_Raw
}eNetworkSockets;

//! Socket 协议
typedef enum  {
    Protocol_Not,
    Protocol_IPv4,
    Protocol_IPv6,
    Protocol_AnyIP
}eNetworkProtocols;

//! Socket 协议基地址
typedef struct
{
    ushort	sa_family;
    char	sa_data[14];
}sockaddr_c;

typedef int sockhd;

#define iAddress bs_ptr(bsPackage inetaddr)
typedef iAddress pinetaddr;
#undef iAddress

//! ipv4和ipv6地址类
bsPackage inetaddr
{
    uint16 _ports_;
    bs_string _addrs_;

    virtual ~inetaddr() {}

    //!
    //! \brief local_ip 返回本地ip
    //! \return
    //!
    virtual bs_string local_ip()const = 0;

    //!
    //! \brief ip 设置ip地址
    //! \param i  以"."组成的4位符号地址
    //!
    virtual void ip(const bs_string &i){_addrs_ = i;}
    virtual bs_string ip() {return _addrs_;}

    //!
    //! \brief port 设置端口号
    //! \param p
    //!
    virtual void port(uint16 p){_ports_ = p;}
    virtual uint16 port() {return _ports_;}

    //!
    //! \brief socklen 返回socket()返回的指针长度
    //! \return
    //!
    virtual size_t socklen() const= 0;
    //!
    //! \brief socket 返回socket需要使用的地址信息
    //! \return
    //!
    virtual sockaddr_c *socket() = 0;

    //!
    //! \brief is_ipv4 是否ip内指定的为合法ipv4地址
    //! \param ip
    //! \return        true为合法,否则不合法
    //!
    virtual bool is_ipv4(const bs_string &ip)
    {
        int dots = 0;
        // %! ignore :port?
        for (size_t i = 0; i < ip.size(); ++i)
        {
            if (ip[i] == '.')
                dots++;
            else
                if (!ip[i].is_digit ())
                    return false;
        }
        if (dots != 3)
            return false;
        return true;

    }
    //!
    //! \brief is_ipv6 是否ip内指定的为合法ipv6地址
    //! \param ip
    //! \return        true为合法,否则不合法
    //!
    virtual bool is_ipv6(const bs_string& )
    {
        return true;
    }
    //!
    //! \brief is_ipv4 是否支持ipv4
    //! \return        true为支持,否则不支持
    //!
    virtual bool is_ipv4()const = 0;
    //!
    //! \brief is_ipv6 是否支持ipv6
    //! \return        true为支持,否则不支持
    //!
    virtual bool is_ipv6()const = 0;
    //!
    //! \brief is_valid 判断socket()返回的是否是有效地址
    //! \return         true为有效,否则无效
    //!
    virtual bool is_valid()const = 0;
    //!
    //! \brief operator == 判断i的地址是否和本地址相同
    //! \param i
    //! \return            true为相同,否则不相同
    //!
    virtual bool operator==(const pinetaddr &i)
    {
        return i->port() == port() &&
                i->ip() == ip();
    }
    //!
    //! \brief operator = 将i的地址赋值给本地址
    //! \param i
    //! \return           返回本地址
    //!
    virtual inetaddr &operator=(const pinetaddr &i)
    {
        flow_info(i->flow_info());
        scope_id(i->scope_id());
        ip(i->ip());
        port(i->port());
        return (*this);
    }

    //!
    //! \brief clone 拷贝一个和本地址相同的地址
    //! \return
    //!
    virtual pinetaddr clone() = 0;


    //!
    //! \brief scope_id 设置范围id(在IPV6中才使用)
    //! \param id
    //!
    virtual void scope_id(ulong) {}
    virtual ulong scope_id() const{return 0;}

    //!
    //! \brief flow_info 设置信息流(在IPV6中才使用)
    //! \param info
    //!
    virtual void flow_info(ulong) {}
    virtual ulong flow_info() const{return 0;}

};

//! Socket 抽象接口
bsPackage isocket
{
    virtual ~isocket(){}

    //!
    //! \brief create   创建Socket
    //! \param mode     TCP或UDP
    //! \param protocol IPV4或IPV6及ANYIP
    //! \return         true 成功, 否则失败
    //!
    virtual bool create(eNetworkSockets mode, eNetworkProtocols protocol = Protocol_IPv4) = 0;
    virtual bool is_create() = 0;

    //!
    //! \brief connect 链接服务端
    //! \param ip      服务端地址信息和端口号(内部区分IPV4和IPV6)
    //! \return        true 成功, 否则失败
    //!
    virtual bool connect(const pinetaddr &ip) = 0;

    //!
    //! \brief bind 绑定Socket
    //! \param ip   需要绑定的地址和端口号(内部区分IPV4和IPV6)
    //! \return     true 成功, 否则失败
    //!
    virtual bool bind(const pinetaddr &ip) = 0;

    //!
    //! \brief listen 监听Socket
    //! \param count  同时能够被Accept接受的客户端数
    //! \return       true 成功, 否则失败
    //!
    virtual bool listen(int count = 20) = 0;

    //!
    //! \brief accept 接受客户端的链接
    //! \param addrs  接受到的客户端地址信息
    //! \return       链接后分配的socket句柄
    //!
    virtual int accept(pinetaddr *addrs) = 0;
    //!
    //! \brief close  关闭创建的socket
    //!
    virtual void close() = 0;
    //!
    //! \brief nonblocking 设置socket是否为阻塞模式
    //! \param nb          true为非阻塞模式，否则阻塞模式
    //! \return            true设置成功，否则失败
    //!
    virtual bool nonblocking(bool nb = true) = 0;

    //!
    //! \brief is_nonblocking 返回当前socket是否是非阻塞
    //! \return               true为非阻塞模式，否则阻塞模式
    //!
    virtual bool is_nonblocking() = 0;

    //!
    //! \brief option_ip ip选项设置
    //! \param opt       选项
    //! \param dat       数据指针
    //! \param len       数据指针地址长度
    //! \return          true成功，否则失败
    //!
    virtual bool option_ip(eNetworkOptions opt, void *dat, int len) = 0;
    virtual bool option_sol(eNetworkOptions opt, void *dat, int len) = 0;
    virtual bool option_ip(eNetworkOptions opt, void *dat, int *maxlen) = 0;
    virtual bool option_sol(eNetworkOptions opt, void *dat, int *maxlen) = 0;

    //!
    //! \brief read   从socket接受maxlen长度的数据存储到data中
    //! \param data   存储接受的数据(指针需要先申请)
    //! \param maxlen 存储数据的空间(申请时的长度)
    //! \return       接受到的长度
    //!
    virtual int64 read(char *data, int64 maxlen) = 0;
    //!
    //! \brief write 向socket中发送len长度的data
    //! \param data  需要发送的数据
    //! \param len   需要发送的数据的长度
    //! \return      返回发送的长度
    //!
    virtual int64 write(const char *data, int64 len) = 0;
    //!
    //! \brief read   从socket读取maxlen长度的数据存储到data中
    //! \param data   存储接受的数据(指针需要先申请)
    //! \param maxlen 存储数据的空间(申请时的长度)
    //! \param ip     数据从ip中发来的
    //! \return       读取到的长度
    //!
    virtual int64 read(char *data, int64 maxlen, pinetaddr *ip)=0;
    //!
    //! \brief write 向socket中发送len长度的data
    //! \param data  需要发送的数据
    //! \param len   需要发送的数据的长度
    //! \param ip    将数据发送到ip目标
    //! \return      返回发送的长度
    //!
    virtual int64 write(const char *data, int64 len, const pinetaddr &ip)=0;

    //!
    //! \brief address  根据协议类型返回一个地址指针
    //! \param protocol 协议类型
    //! \return         返回地址指针(不使用需要使用bsFree将其释放)
    //!
    virtual pinetaddr netaddr(eNetworkProtocols protocol = Protocol_IPv4) = 0;
    virtual sockhd handle() const= 0;
    virtual void handle(sockhd h) = 0;
};
#define iSocket bs_ptr(isocket)
typedef iSocket pisocket;
#undef iSocket
#endif // ISOCKET

