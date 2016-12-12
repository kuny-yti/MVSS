#ifndef BS_GUI_H
#define BS_GUI_H
#include "gui/igui.h"
#include "bs_utils.h"

namespace impl {
struct screen;
}
class bs_gui :public igui, public bs_singleton<bs_gui>
{
public:
    bs_gui(bs_object *parent = NULL);
    virtual ~bs_gui();

    virtual void event(const bs_event &e) ;
    virtual void paint();

    //!
    //! \brief create 创建一个部件
    //! \param ut (eUiTypes | eGuiTypes)
    //! \param p
    //! \return
    //!
    static piwidget create(int ut, const piwidget &p);

private:
    impl::screen* impl;
};

#endif
