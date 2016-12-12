#ifndef BS_QUEUE
#define BS_QUEUE
#include "bs_debug.h"
#include <queue>
#include "bs_list.h"

template <typename T>
class bs_queue
{
public:
    bs_queue() :sequence() {}
    bs_queue(const bs_queue &v) :sequence(v.sequence) {}
    bs_queue(const std::queue<T> &v) :sequence() {form(v);}

    std::queue<T> &std()
    {
        return sequence.to_std ();
    }
    void form (const std::queue<T> &v)
    {

    }

    T& front(){return sequence.front ();}
    const T& front()const{return sequence.front ();}
    T& back(){return sequence.back ();}
    const T& back()const{return sequence.back ();}

    void push(const T& val) {sequence.push_back (val);}
#if bsLanguage >= bsLanguage11
    void push(T && v){sequence.push_back (v);}
#endif

    void remove() {sequence.pop_front ();}
    void clear() {sequence.clear ();}
    T pop() {T out = sequence.front (); remove (); return out;}
    size_t size()const {return sequence.size ();}
    size_t count()const {return size();}

    bool empty()const {return sequence.empty ();}
    bool is_empty()const {return empty();}

protected:
    bs_list<T> sequence;
};

#endif // BS_QUEUE

