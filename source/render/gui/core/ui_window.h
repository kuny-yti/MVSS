#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include "ui_state.h"

class ui_window : public ui_state
{
    uiWidgetDefine(ui_window)
public:
    ui_window(iwidget *parent  = 0);
    ~ui_window();
};

#endif // UI_WINDOW_H
