#ifndef UI_COMPOSER_H
#define UI_COMPOSER_H

#include "gui/iwidget.h"


/*!
 * \brief The ui_composer class UI 合成器
 * 1.合成方法利用fbo，每个窗口一个fbo.
 * 2.当窗口有更新时对界面进行一次离屏渲染.
 * 3.需要进行界面更新时组合器负责将每个窗口的离屏的图像进行合成并渲染出来
 */
class ui_composer
{
public:
    ui_composer(const piwidget &ui);

    void render();
};

#endif // UI_COMPOSER_H
