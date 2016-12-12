#ifndef UI_TRIGGER_H
#define UI_TRIGGER_H

#include "ui_state.h"

class ui_trigger :public ui_state
{
    uiWidgetDefine(ui_state)
public:
    ui_trigger(iwidget *parent  = 0);
    ~ui_trigger();

protected:
    virtual void mouse_down(eEventMouses em, int modifier, const ipoint &pos);
    virtual void mouse_up(eEventMouses em, int modifier, const ipoint &pos);

    virtual void keybord_down(eEventKeys ek, int modifier);
    virtual void keybord_up(eEventKeys ek, int modifier);

protected:
    tButtons nexus;
};

#endif // UI_TRIGGER_H
