#ifndef UI_FONT_H
#define UI_FONT_H

#include "tools/bs_string.h"
#include "tools/bs_map.h"

class ui_font
{
public :
    struct tFonts
    {
        bs_string name;
        bs_string family;
        bs_byte file;
        int font_id;
    };
    typedef bs_map<bs_string, tFonts> tFontsMap;

public:
    ui_font(){}
    ui_font(const tFontsMap &fm):maps(fm){}

    virtual ~ui_font(){}

    bs_string_list familys() {return maps.keys ();}
    tFonts &operator [](const bs_string family){return maps[family];}

private:
    tFontsMap maps;
};

#endif // UI_FONT_H
