#ifndef BS_STACK
#define BS_STACK
#include "bs_debug.h"
#include <stack>
#include "bs_list.h"

template <typename T>
class bs_stack
{
public:
    bs_stack(): sequence() {}
    bs_stack(const bs_stack<T> &s): sequence(s.sequence) {}
    bs_stack(const std::stack<T> &s):sequence() {form(s);}

    std::stack<T> &to_std(){return sequence.to_std ();}
    void form (const std::stack<T> &rhs)
    {

    }

    T &top(){return sequence.back ();}
    const T &top() const{return sequence.back ();}

    void push(const T& val){sequence.push_back (val);}
#if bsLanguage >= bsLanguage11
    void push(T && v){sequence.push_back (v);}
#endif
    T pop(){T out = sequence.back (); remove ();return out;}
    void remove(){sequence.pop_back();}
    void swap(bs_stack& s){bs_swap (sequence, s.sequence);}
    friend void bs_swap (bs_stack &a, bs_stack &b){a.swap (b);}

    size_t size() const{return sequence.size ();}
    size_t count() const {return size();}

    bool empty() const {return sequence.empty ();}
    bool is_empty() const{return empty ();}
protected:
    bs_list <T> sequence;
};

#endif // BS_STACK

