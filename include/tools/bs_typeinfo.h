#ifndef CHECK_TYPE_H___
#define CHECK_TYPE_H___

#include "bs_define.h"
#include <typeinfo>     // typeid
#include <sstream>      // std::ostringstream, std::string
#include <type_traits>  // std::is_array
#include <utility>      // std::move

#if bsCompiler == bsCompiler_GNUC
#include <memory>       // std::unique_ptr
#include <cxxabi.h>     // abi::__cxa_demangle
#endif

namespace base_type {

template <typename T, bool IsBase = false>
struct bs_typeinfo;

/*
    Output state management
*/

class bs_output
{
    bool is_compact_ = true;

    template <typename T>
    bool check_empty(const T&) { return false; }
    bool check_empty(const char* val)
    {
        return (!val) || (val[0] == 0);
    }

    template <typename T>
    void out(const T& val)
    {
        if (check_empty(val)) return;
        if (!is_compact_) sr_ += " ";
        using ss_t = std::ostringstream;
        sr_ += static_cast<ss_t&>(ss_t() << val).str();
        is_compact_ = false;
    }

    std::string& sr_;

public:
    bs_output(std::string& sr) : sr_(sr) {}

    bs_output& operator()(void) { return (*this); }

    template <typename T1, typename... T>
    bs_output& operator()(const T1& val, const T&... args)
    {
        out(val);
        return operator()(args...);
    }

    bs_output& compact(void)
    {
        is_compact_ = true;
        return (*this);
    }
};

// ()

template <bool>
struct bs_bracket
{
    bs_output& out_;

    bs_bracket(bs_output& out, const char* = nullptr) : out_(out)
    { out_("(").compact(); }

    ~bs_bracket(void)
    { out_.compact()(")"); }
};

template <>
struct bs_bracket<false>
{
    bs_bracket(bs_output& out, const char* str = nullptr)
    { out(str); }
};

// [N]

template <size_t N = 0>
struct bs_bound
{
    bs_output& out_;

    bs_bound(bs_output& out) : out_(out) {}
    ~bs_bound(void)
    {
        if (N == 0) out_("[]");
        else        out_("[").compact()
                        ( N ).compact()
                        ("]");
    }
};

// (P1, P2, ...)

template <bool, typename... P>
struct bs_parameter;

template <bool IsStart>
struct bs_parameter<IsStart>
{
    bs_output& out_;

    bs_parameter(bs_output& out) : out_(out) {}
    ~bs_parameter(void)
    { bs_bracket<IsStart> { out_ }; }
};

template <bool IsStart, typename P1, typename... P>
struct bs_parameter<IsStart, P1, P...>
{
    bs_output& out_;

    bs_parameter(bs_output& out) : out_(out) {}
    ~bs_parameter(void)
    {
        [this](bs_bracket<IsStart>&&)
        {
            bs_typeinfo<P1> { out_ };
            bs_parameter<false, P...> { out_.compact() };
        } (bs_bracket<IsStart> { out_, "," });
    }
};

// Do output at destruct

struct at_destruct
{
    bs_output&     out_;
    const char* str_;

    at_destruct(bs_output& out, const char* str = nullptr)
        : out_(out)
        , str_(str)
    {}
    ~at_destruct(void)
    { out_(str_); }

    void set_str(const char* str = nullptr)
    { str_ = str; }
};

/*
    CV-qualifiers, references, pointers
*/

template <typename T, bool IsBase>
struct bs_typeinfo
{
    bs_output out_;
    bs_typeinfo(const bs_output& out) : out_(out)
    {
#   if bsCompiler == bsCompiler_GNUC
        const char* typeid_name = typeid(T).name();
        auto deleter = [](char* p)
        {
            if (p) free(p);
        };
        std::unique_ptr<char, decltype(deleter)> real_name
        {
            abi::__cxa_demangle(typeid_name, nullptr, nullptr, nullptr), deleter
        };
        out_(real_name ? real_name.get() : typeid_name);
#   else
        out_(typeid(T).name());
#   endif
    }
};

#define BS_TYPEINFO__(OPT) \
    template <typename T, bool IsBase> \
    struct bs_typeinfo<T OPT, IsBase> : bs_typeinfo<T, true> \
    { \
        using base_t = bs_typeinfo<T, true>; \
        using base_t::out_; \
        bs_typeinfo(const bs_output& out) : base_t(out) { out_(#OPT); } \
    };

BS_TYPEINFO__(const)
BS_TYPEINFO__(volatile)
BS_TYPEINFO__(const volatile)
BS_TYPEINFO__(&)
BS_TYPEINFO__(&&)
BS_TYPEINFO__(*)

#undef BS_TYPEINFO__

/*
    Arrays
*/

#define BS_TYPEINFO_ARRAY__(CV_OPT, BOUND_OPT, ...) \
    template <typename T, bool IsBase __VA_ARGS__> \
    struct bs_typeinfo<T CV_OPT [BOUND_OPT], IsBase> : bs_typeinfo<T CV_OPT, !std::is_array<T>::value> \
    { \
        using base_t = bs_typeinfo<T CV_OPT, !std::is_array<T>::value>; \
        using base_t::out_; \
    \
        bs_bound<BOUND_OPT> bound_   = out_; \
        bs_bracket<IsBase>  bracket_ = out_; \
    \
        bs_typeinfo(const bs_output& out) : base_t(out) {} \
    };

#define BS_TYPEINFO_ARRAY_CV__(BOUND_OPT, ...) \
    BS_TYPEINFO_ARRAY__(, BOUND_OPT, ,##__VA_ARGS__) \
    BS_TYPEINFO_ARRAY__(const, BOUND_OPT, ,##__VA_ARGS__) \
    BS_TYPEINFO_ARRAY__(volatile, BOUND_OPT, ,##__VA_ARGS__) \
    BS_TYPEINFO_ARRAY__(const volatile, BOUND_OPT, ,##__VA_ARGS__)

#define BS_TYPEINFO_PLACEHOLDER__
BS_TYPEINFO_ARRAY_CV__(BS_TYPEINFO_PLACEHOLDER__)
#if bsCompiler == bsCompiler_GNUC
BS_TYPEINFO_ARRAY_CV__(0)
#endif
BS_TYPEINFO_ARRAY_CV__(N, size_t N)

#undef BS_TYPEINFO_PLACEHOLDER__
#undef BS_TYPEINFO_ARRAY_CV__
#undef BS_TYPEINFO_ARRAY__

/*
    Functions
*/

template <typename T, bool IsBase, typename... P>
struct bs_typeinfo<T(P...), IsBase> : bs_typeinfo<T, true>
{
    using base_t = bs_typeinfo<T, true>;
    using base_t::out_;

    bs_parameter<true, P...> parameter_ = out_;
    bs_bracket<IsBase>       bracket_   = out_;

    bs_typeinfo(const bs_output& out) : base_t(out) {}
};

/*
    Pointers to members
*/

template <typename T, bool IsBase, typename C>
struct bs_typeinfo<T C::*, IsBase> : bs_typeinfo<T, true>
{
    using base_t = bs_typeinfo<T, true>;
    using base_t::out_;

    bs_typeinfo(const bs_output& out) : base_t(out)
    {
        bs_typeinfo<C> { out_ };
        out_.compact()("::*");
    }
};

/*
    Pointers to member functions
*/

#define BS_TYPEINFO_MEM_FUNC__(...) \
    template <typename T, bool IsBase, typename C, typename... P> \
    struct bs_typeinfo<T(C::*)(P...) __VA_ARGS__, IsBase> \
    { \
        at_destruct cv_ = base_.out_; \
        bs_typeinfo<T(P...), true> base_; \
        bs_output& out_ = base_.out_; \
    \
        bs_typeinfo(const bs_output& out) : base_(out) \
        { \
            cv_.set_str(#__VA_ARGS__); \
            bs_typeinfo<C> { out_ }; \
            out_.compact()("::*"); \
        } \
    };

BS_TYPEINFO_MEM_FUNC__()
BS_TYPEINFO_MEM_FUNC__(const)
BS_TYPEINFO_MEM_FUNC__(volatile)
BS_TYPEINFO_MEM_FUNC__(const volatile)

#undef BS_TYPEINFO_MEM_FUNC__

}

/*
    Get the name of the given type

    check_type<const volatile void *>()
    -->
    void const volatile *
*/

template <typename T>
inline std::string bs_typeinfo(void)
{
    std::string str;
    base_type::bs_typeinfo<T> { str };
    return std::move(str);
}

#define isType(Type, V) (typeid(Type) == typeid(V))

#define bs_is_type(TT) \
    (isType(TT, bool) | \
    isType(TT, char) | \
    isType(TT, short) | \
    isType(TT, int) | \
    isType(TT, long) | \
    isType(TT, float) | \
    isType(TT, uchar) | \
    isType(TT, ushort) | \
    isType(TT, uint) | \
    isType(TT, ulong) | \
    isType(TT, real) | \
    isType(TT, lreal) | \
    isType(TT, ulonglong) | \
    isType(TT, longlong) | \
    isType(TT, int64) | \
    isType(TT, uint64))

#endif
