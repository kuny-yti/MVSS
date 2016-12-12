#ifndef UI_IMAGE_H
#define UI_IMAGE_H

#include "ui_state.h"

class ui_image :public ui_state
{
    uiWidgetDefine(ui_image)
public:
    ui_image(iwidget *parent  = 0);
    ~ui_image();
};

#endif // UI_IMAGE_H
