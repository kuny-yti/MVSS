#ifndef UI_THEME_H
#define UI_THEME_H

#include "tools/bs_maphash.h"
#include "guidef.h"
#include "ui_style.h"
#include "ui_skin.h"
#include "ui_effect.h"
#include "ui_script.h"
#include "ui_font.h"

struct ui_theme
{
    int type;
    /// 主题样式(几何图形的样子)
    ui_style style;
    /// 主题特效(状态特效和事件特效)
    ui_effect effect;
    /// 主题皮肤(使用颜色或者纹理)
    ui_skin skin;
    /// 主题脚本(特效或是事件等等脚本)
    ui_script script;
};
typedef bs_maphash<int, ui_theme> ui_thememap;

#endif // UI_THEME_H
