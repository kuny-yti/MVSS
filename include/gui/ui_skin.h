#ifndef UI_SKIN_H
#define UI_SKIN_H
#include "guidef.h"
#include "maths/bs_vec4.h"
#include "maths/bs_color.h"
#include "tools/bs_image.h"

typedef enum
{
    Skin_Color, /// 使用颜色皮肤
    Skin_Image  /// 使用图片皮肤
}eSkinTypes;

//!
//! \brief The ui_skin class 皮肤类定义
//!
struct ui_skin
{
    //!
    //! \brief The tTitles struct 标题
    //!
    struct tTitles
    {
        vec2 pos;
        int size;
        float blur;
        float spacing;
        float height;
        bs_string family;
        bs_color color;
        int align;
        tTitles(){}
    };
    //!
    //! \brief The tTooltips struct 提示
    //!
    struct tTooltips
    {
        int size;
        float blur;
        float spacing;
        float height;
        bs_string family;
        bs_color color;
        int align;

        tTooltips(){}
    };

    //!
    //! \brief The tColors struct 颜色皮肤结构
    //!
    struct tColors
    {
        //!
        //! \brief The eGradualTypes enum 渐变类型
        //!
        enum eGradualTypes
        {
            Gradual_Not,
            Gradual_Linear,
            Gradual_Box,  /// 盒型渐变
            Gradual_Round /// 圆型渐变
        };
        //!
        //! \brief The tGraduals struct 渐变结构
        //!
        struct tGraduals
        {
            bs_color initial; /// 渐变的开始颜色
            bs_color final;   /// 渐变的结束颜色
            float deviate;    /// 偏离值
            int angle;        /// 角度
            float radiu;      /// 锐角半径 Corner
            float feather;    /// 羽化
            eGradualTypes type;/// 渐变的类型

            tGraduals()
            {
                type = Gradual_Not;
                initial = bs_color(0, 0, 0, 255);
                final = bs_color(0, 0, 0, 0);
                deviate = 0;
                angle = 0;
                radiu = 0.f;
                feather = 0.f;
            }
            tGraduals(const bs_color &i, const bs_color &f)
            {
                type = Gradual_Not;
                initial = i;
                final = f;
                deviate = 0;
                angle = 0;
                radiu = 0.f;
                feather = 0.f;
            }

        };

        struct tOutLines
        {
            bs_color dark ;
            bs_color light ;
            bs_color medium ;
            float    width ;
            tOutLines()
            {
                dark = bs_color(0,0,0,255);
                light = bs_color(20,20,20,2255);
                medium = bs_color(0,0,0,255);
                width = 1.f;
            }
        };

        tOutLines  outline;/// 轮廓颜色
        tGraduals  fill;   /// 填充颜色
        tGraduals  shadow; /// 阴影 Gradual

        tColors():outline(),
            fill(), shadow() {}
        ~tColors() {}
    };
    //! 当类型为Skin_Image 时使用本结构
    struct tImages
    {
        vec2 min;
        vec2 max;
        ivec2 id;

        tImages():min(),max(),id(){}
    };
    //! 当类型为Skin_Image时本结构内为需要的素材数据
    struct tFodders
    {
        bs_image fodder;
        isize  dim;
        vec4   margin;
        isize  grid;

        tFodders():  fodder(),
            dim(), margin(), grid(){}
    };

    struct tStateIcons
    {
        uint icon;
        bs_color outline;
        bs_color color;
    };

    bool has_fodder;
    bool has_icon;
    tFodders fodder;
    eSkinTypes type;
    tTitles    title;  /// 标题信息
    tTooltips  tooltip;/// 提示信息
    bs_string icon_family;
    tStateIcons icon[Gui_GetDef(Ui_StateCount, UiState_Field)];
    tColors color[Gui_GetDef(Ui_StateCount, UiState_Field)];
    tImages image[Gui_GetDef(Ui_StateCount, UiState_Field)];

    //!
    //! \brief ui_skin 构造函数，默认为图片皮肤
    //! \param st
    //!
    explicit ui_skin(eSkinTypes st = Skin_Image, bool has_fodder = true)
    {
        _has_fodder = has_fodder;
        _type = st;
    }
    ~ui_skin(){}

    //!
    //! \brief state 返回本状态的颜色、图像
    //! \param st
    //! \param color
    //! \return
    //!
    inline bool states(int st, tColors* &c)
    {
        if (type != Skin_Color)
            return false;

        st -= 1;
        if (Gui_GetDef(st, UiState_Field) >= Ui_StateCount)
            c = &color[Gui_GetDef(Ui_Normal , UiState_Field)];
        else
            c = &color[Gui_GetDef(st , UiState_Field) ];

        return true;
    }
    inline bool states(int st, tImages* &img)
    {
        if (type != Skin_Color)
            return false;

        st -= 1;
        if (Gui_GetDef(st, UiState_Field)  >= Ui_StateCount)
            img = &image[Gui_GetDef(Ui_Normal , UiState_Field) ];
        else
            img = &image[Gui_GetDef(st , UiState_Field) ];

        return true;
    }
    inline tColors *colors(int st)
    {
        static tColors stacol;
        if (type != Skin_Color)
            return &stacol;

        st -= 1;
        if (Gui_GetDef(st, UiState_Field)  >= Ui_StateCount)
            return &color[Gui_GetDef(Ui_Normal, UiState_Field)];

        return &color[Gui_GetDef(st , UiState_Field)];
    }
    inline tImages *images(int st)
    {
        static tImages staimg;
        if (type != Skin_Image)
            return &staimg;

        st -= 1;
        if (Gui_GetDef(st, UiState_Field)  >= Ui_StateCount)
            return &image[Gui_GetDef(Ui_Normal, UiState_Field)];

        return &image[Gui_GetDef(st , UiState_Field)];
    }
    inline const tColors *colors(int st)const
    {
        static tColors stacol;
        if (type != Skin_Color)
            return &stacol;

        st -= 1;
        if (Gui_GetDef(st, UiState_Field)  >= Ui_StateCount)
            return &color[Gui_GetDef(Ui_Normal, UiState_Field) ];

        return &color[Gui_GetDef(st , UiState_Field) ];
    }
    inline const tImages *images(int st)const
    {
        static tImages staimg;
        if (type != Skin_Image)
            return &staimg;

        st -= 1;
        if (Gui_GetDef(st, UiState_Field) >= Ui_StateCount)
            return &image[Gui_GetDef(Ui_Normal, UiState_Field) ];

        return &image[Gui_GetDef(st , UiState_Field) ];
    }

    //!
    //! \brief fodder 返回使用资源
    //! \return
    //!

    //!
    //! \brief title 返回使用标题
    //! \return
    //!

    //!
    //! \brief tooltip 返回使用提示
    //! \return
    //!

    inline tStateIcons *icons(int st)
    {
        st -= 1;
        if (Gui_GetDef(st, UiState_Field)  >= Ui_StateCount)
            return &icon[Gui_GetDef(Ui_Normal, UiState_Field)];

        return &icon[Gui_GetDef(st , UiState_Field)];
    }
    inline const tStateIcons *icons(int st)const
    {
        st -= 1;
        if (Gui_GetDef(st, UiState_Field)  >= Ui_StateCount)
            return &icon[Gui_GetDef(Ui_Normal, UiState_Field)];

        return &icon[Gui_GetDef(st , UiState_Field)];
    }

};

typedef bs_list<ui_skin> ui_skinlist;
#endif // UI_SKIN_H
