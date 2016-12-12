#ifndef BS_OBJECT_H
#define BS_OBJECT_H

#include "tools/bs_typeinfo.h"
#include "tools/bs_string.h"
#include "tools/bs_list.h"
#include "bs_thread.h"

namespace impl {
class object;
}

typedef struct
{
    class bs_object *source;
    class bs_object *target;
}tObjectNexus;

/// 当前此类不是线程安全的
///
/// 1.类的RTTI
///   a.可继承后用name可以查看类型继承关系
///   b.继承is可判断是否兼容类型
/// 2.关联类的调用
///   a.connect可以将被本类调用的函数链接到被调用池
///   b.disconnect断开连接
///   c.emit执行调用池内的对象
///   d.被调用者需要继承callback
/// 3.子父对象的结构
///   用于关系操作
/// 4.对象自动管理
///
//! 类的RTTI
#define bsObject(type) public:\
    virtual bs_string name() const{return bs_typeinfo<type>();} \
    virtual bool is(bs_object *rhs)const \
    {\
        type *tmps = bsAsCast(type *)(rhs);\
        if (tmps != NULL)\
            return true;\
        return false; \
    }

class bs_object
{
    bsObject(bs_object)
    template<typename PtrT>
    friend class bs_ptrref;
protected:
    void lessref();
    void addref();

public:
    explicit bs_object(bs_object *parent = NULL);

    template <typename T>
    T* as() {return bsAsCast(T *)(this);}

    //! 处理关联的类
    void emit(tObjectNexus *param);
    void connect(bs_object *slt);
    void disconnect(bs_object *slt);

    //! 被connect指定的对象通过emit时调用此函数
    virtual void callback(tObjectNexus *param);

    bs_thread *thread() const;

public:
    static void connect(bs_object *sig, bs_object *slt);
    static void disconnect(bs_object *sig, bs_object *slt);
    static void emit(bs_object *sig, tObjectNexus *param);

public:
    typedef bs_list <bs_object*>::iterator child;

    uint child_count()const;
    bs_object *parent()const;
    void parent(const bs_object *p);

protected:
    virtual ~bs_object(){}

protected:
    //!
    //! \brief append 添加孩子
    //! \param obj
    //!
    void append(bs_object *obj);
    //!
    //! \brief remove 删除孩子
    //! \param it
    //!
    void remove(const child &it);
    void remove(bs_object *obj);
    //!
    //! \brief begin 孩子的开始节点
    //! \return
    //!
    child begin();
    const child begin()const;
    //!
    //! \brief end 孩子的结束节点
    //! \return
    //!
    child end();
    const child end()const;

private:
    impl::object *impl;
};



template<typename PtrT>
class bs_ptrref
{
public:
    bs_ptrref():ptr(NULL),ref(NULL){}
    bs_ptrref (PtrT *p, bs_ref::eRefFlags c = bs_ref::refShareable)
    {
        if (typeid(PtrT) != typeid(bs_object))
            ref = bsNew (bs_ref(c));
        else
            ref = NULL;
        ptr = p;
    }

    bs_ptrref(const bs_ptrref<PtrT> &rhs):
        ptr(rhs.ptr),
        ref(rhs.ref)
    {
        addref();
    }

#if bsLanguage >= bsLanguage11
    bs_ptrref(bs_ptrref<PtrT> &&rhs) :
        ptr(rhs.ptr),
        ref(rhs.ref)
    {
        rhs.ptr = NULL;
        rhs.ref = NULL;
    }
    bs_ptrref<PtrT>& operator =(bs_ptrref<PtrT>&& rhs)
    {
        if (*this != rhs)
        {
            lessref();
            ptr = rhs.ptr;
            ref = rhs.ref;
            rhs.ptr = NULL;
            rhs.ref = NULL;
        }
        return *this;
    }
#endif

    //! 其他类到本类转换
    template <class PtrU>
    bs_ptrref(const bs_ptrref <PtrU> &rhs):
            ptr(bsAsCast(PtrT *)(rhs.ptr))
    {
        if (ptr)
        {
            ref = rhs.ref;
            addref();
        }
        else
            ref = NULL;
    }

    //! 作为其他类使用
    template <class PtrU>
    bs_ptrref <PtrU> as()const
    {
        PtrU *pptr = bsAsCast(PtrU*)(ptr);
        bs_ptrref <PtrU> oref;
        if (pptr)
        {
            oref.ptr = pptr;
            oref.ref = ref;
            oref.addref();
        }

        return oref;
    }

    ~bs_ptrref() {lessref();}
    //! 其他类到本类转换
    template <class PtrU>
    bs_ptrref<PtrT>& operator =(const bs_ptrref<PtrU>& rhs)
    {
        lessref();
        ptr = bsAsCast(PtrT *)(rhs.ptr);
        if (ptr)
        {
            ref = rhs.ref;
            addref();
        }
        else
            ref = NULL;
        return *this;
    }

    bs_ptrref<PtrT>& operator =(const bs_ptrref<PtrT>& rhs)
    {
        if (ptr != rhs.ptr && ref != rhs.ref)
        {
            lessref();
            ptr = rhs.ptr;
            ref = rhs.ref;
            addref();
        }
        return *this;
    }

    bs_ptrref<PtrT>& operator = (PtrT *otp)
    {
        if (otp != ptr)
        {
            lessref();
            if (typeid(PtrT) != typeid(bs_object))
                ref = bsNew (bs_ref());
            else
                ref = NULL;
            ptr = otp;
        }
        return *this;
    }

    bool operator ==(const bs_ptrref<PtrT> &rhs) const { return ptr == rhs.ptr && ref == rhs.ref; }
    bool operator !=(const bs_ptrref<PtrT> &rhs) const { return !(this->operator ==(rhs)); }
    bool operator ==(const PtrT* rhs) const { return ptr == rhs; }
    bool operator !=(const PtrT* rhs) const { return !(this->operator ==(rhs)); }

    PtrT* operator->() { return ptr; }
    PtrT* operator->() const { return ptr; }
    PtrT* get() { return ptr; }
    const PtrT* get() const { return ptr; }
    PtrT& operator*() { return *ptr; }
    const PtrT& operator*() const { return *ptr; }
    operator PtrT* () { return ptr; }
    operator bool() const { return ptr != NULL; }

    void reset(){ lessref(); ptr = NULL;ref = NULL;}

public:
    PtrT *ptr;
    bs_ref *ref;

    void lessref()
    {
        bs_object *toj = bsAsCast(bs_object*)((bs_object*)ptr);
        if (typeid(PtrT) == typeid(bs_object))
            toj->lessref();
        else if (ref && ref->lessref())
        {
            bsDelete (ptr);
            bsDelete (ref);
            ptr = NULL;
            ref = NULL;
        }
    }
    void addref()
    {
        bs_object *toj = bsAsCast(bs_object*)((bs_object*)ptr);
        if (typeid(PtrT) == typeid(bs_object))
            toj->addref();
        else if (ref != NULL)
            ref->addref();
    }
};
#endif // BS_OBJECT_H
