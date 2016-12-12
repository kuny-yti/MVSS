#include "bs_object.h"
#include "bs_thread.h"

namespace impl {

class object : public bs_ref // 本对象的引用计数
{
public:
    bs_list<bs_object *> slot; // 记录本对象调用的关联对象。
    bs_object * sig;           // 记录本对象被何对象调用

    bs_object *parent;         // 父对象
    bs_list<bs_object *> child;// 本对象的所有子对象
    idx_t _this_id_;           // 本对象在父对像的链表索引号

    bs_thread *thread;          // 本对象所处线程的id

    object():
        bs_ref(),
        slot(),sig(NULL),
        parent(NULL),
        child(),
        _this_id_(-1),
        thread(NULL)
    {
    }
};
}


bs_object::bs_object(bs_object *parent):
    impl(bsNew (impl::object()))
{
    impl->parent = parent;
    if (parent)
        parent->append(this);

    impl->thread = bs_thread::current();
}

void bs_object::callback(tObjectNexus *param)
{
    param = param;
}
bs_thread *bs_object::thread() const
{
    return impl->thread;
}
void bs_object::addref()
{
    impl->addref();
}

void bs_object::lessref()
{
    if (impl->lessref())
    {
        //! 在父对象中去掉自己
        if (parent())
            parent()->remove(this);

        //! 在关联对象中去除关联关系
        impl->sig->disconnect(this);

        //! 处理孩子
        for (child slt = impl->child.begin(); slt != impl->child.end(); ++slt)
            (*slt)->lessref();

        bsDelete (impl);
        bsDelete (this);
    }
}
void bs_object::parent(const bs_object *p)
{
    if (impl->parent)
        impl->parent->remove(this);
    impl->parent = (bs_object *)p;
    impl->parent->append(this);
}
bs_object *bs_object::parent()const
{
    return impl->parent;
}
uint bs_object::child_count()const
{
    return impl->child.count();
}
void bs_object::append(bs_object *obj)
{
    for (child it = begin(); it != end(); ++it)
        if (obj == *it)
            return ;
    impl->child.append(obj);
    obj->impl->_this_id_ = impl->child.count() -1;
}
void bs_object::remove(const child &it)
{
    impl->child.erase(it);
    idx_t ids = 0;
    if (parent())
    {
        for (child chd = parent()->impl->child.begin(); chd != parent()->impl->child.end(); ++chd, ++ids)
            (*chd)->impl->_this_id_ = ids;
    }
}
void bs_object::remove(bs_object *obj)
{
    for (child it = begin(); it != end(); ++it)
        if (obj == *it)
        {
            remove(it);
            return ;
        }
}
bs_object::child bs_object::begin()
{
    return impl->child.begin();
}
const bs_object::child bs_object::begin()const
{
    return impl->child.begin();
}
bs_object::child bs_object::end()
{
    return impl->child.end();
}
const bs_object::child bs_object::end()const
{
    return impl->child.end();
}

// 处理关联的类
void bs_object::emit(tObjectNexus *param)
{
    for (bs_object *to : impl->slot)
        to->callback(param);
}
void bs_object::connect(bs_object *slt)
{
    for (bs_object *to : impl->slot)
        if (to == slt)
            return;

    impl->slot.append(slt);
    slt->impl->sig = (this);
}
void bs_object::disconnect(bs_object *slt)
{
    for (uint i = 0 ; i < impl->slot.count(); ++i)
        if (impl->slot[i] == slt)
            impl->slot.remove(i);
}

void bs_object::connect(bs_object *sig, bs_object *slt)
{
    sig->connect(slt);
}
void bs_object::disconnect(bs_object *sig, bs_object *slt)
{
    sig->disconnect(slt);
}
void bs_object::emit(bs_object *sig, tObjectNexus *param)
{
    return sig->emit(param);
}
