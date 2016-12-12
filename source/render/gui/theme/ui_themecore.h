#ifndef UI_THEMECORE_H
#define UI_THEMECORE_H

#include "bs_utils.h"
#include "gui/ui_theme.h"


class ui_themecore :public bs_singleton<ui_themecore>
{
public:
    ui_themecore();
    ~ui_themecore();

    void append(const ui_theme &t);
    void remove(const ui_theme &t);

    const ui_theme &get(const int te)const;
    ui_theme &operator [](const int te);

private:
    ui_thememap map;
};

#endif // UI_THEMECORE_H
