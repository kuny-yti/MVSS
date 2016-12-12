#ifndef UI_STATE_H
#define UI_STATE_H

#include "gui/iwidget.h"

class ui_state :public iwidget
{
    uiWidgetDefine(ui_state)
public:
    ui_state(iwidget *parent  = 0);
    ~ui_state();

protected:
    virtual void mouse_down(eEventMouses em, int modifier, const ipoint &pos);
    virtual void mouse_move(const ipoint &pos);
    virtual void mouse_wheel(const int val);
    virtual void mouse_up(eEventMouses em, int modifier, const ipoint &pos);

    virtual void keybord_down(eEventKeys ek, int modifier);
    virtual void keybord_up(eEventKeys ek, int modifier);
    virtual void keybord_im(const bs_event &e);

private:
    bool event(const bs_event &e);
};

#endif // UI_STATE_H
