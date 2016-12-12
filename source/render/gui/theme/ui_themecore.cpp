#include "ui_themecore.h"

ui_themecore::ui_themecore()
{
    map.clear ();
}
ui_themecore::~ui_themecore()
{

}
void ui_themecore::append(const ui_theme &t)
{
    if (map.contains (t.type))
        return ;
    map.append (t.type , t);
}
void ui_themecore::remove(const ui_theme &t)
{
    map.remove (t.type );
}
const ui_theme &ui_themecore::get(const int te)const
{
    return map.value (te);
}
ui_theme &ui_themecore::operator [](const int te)
{
    return map[te];
}

static const ui_theme itheme[] =
{
    {
        Gui_PushButton, // type
        {},// style
        {},// effect
        {
            false, // has_fodder
            false, // has_icon
            {

            }, // fodder
            Skin_Color, // type
            {

            },    // title  /// 标题信息
            {

            },  // tooltip;/// 提示信息
            bs_string(), //icon_family;
            {

            }, // icon
            {

            }, // color
            {

            } // image
        },// skin
        {} // script
    }
};
