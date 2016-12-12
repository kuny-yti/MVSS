#ifndef UI_TEXT_H
#define UI_TEXT_H

#include "ui_state.h"

class ui_text :public ui_state
{
    uiWidgetDefine(ui_text)
public:
    ui_text(iwidget *parent  = 0);
    ~ui_text();

protected:
    virtual void paint ();
};
#endif // UI_TEXT_H
