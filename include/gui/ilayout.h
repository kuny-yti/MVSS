#ifndef ILAYOUT_H
#define ILAYOUT_H

#include "iwidget.h"

bsPackage ilayout
{
    vec4 margin; //边的尺寸
    vec2 spacing; //部件之间的间隔
    eLayoutModes mode; //布局模式
    eUiOrientations orientation;//布局方向
    eLayoutAligns align;//布局对齐

    ilayout()
    {
        margin = vec4(4,4,4,4);
        spacing = vec2(2,2);
        mode = Layout_Box;
        orientation = Ui_Horizontal;
        align = Align_Fill;
    }

    virtual ~ilayout() {}

    //!
    //! \brief insert 插入ui (Layout_Horizontal Layout_Vertical模式)
    //! \param ui
    //! \param sizt 插入位置
    //! \param width 横/竖向占取的份数
    //!
    virtual void insert(const piwidget &ui, int sizt, int width/* = 1*/) = 0;
    virtual void insert(const piwidget &ui, ivec2 &rp) {insert (ui, rp.x, rp.y);}

    //!
    //! \brief append 添加ui (Layout_Horizontal Layout_Vertical模式)
    //! \param ui
    //! \param width 横/竖向占取的份数
    //!
    virtual void append(const piwidget &ui, int width = 1) = 0;
    //!
    //! \brief remove 删除ui (Layout_Horizontal Layout_Vertical模式)
    //! \param ui 需要删除的ui
    //! \param sizt 需要删除的位置
    //!
    virtual void remove(const piwidget &ui) = 0;
    virtual void remove(int sizt) = 0;

    //!
    //! \brief replace 替换ui (Layout_Horizontal Layout_Vertical模式)
    //! \param sui 需要替换的ui
    //! \param dui 替换后的ui
    //! \param width 替换后占取的份数: -1 原有份数
    //!
    virtual void replace(const piwidget &sui, const piwidget &dui, int width = -1) = 0;
    //!
    //! \brief replace 替换ui (Layout_Horizontal Layout_Vertical模式)
    //! \param sizt 需要替换的ui位置
    //! \param dui 替换后的ui
    //! \param width 替换后占取的份数: -1 原有份数
    //!
    virtual void replace(int sizt, const piwidget &dui, int width = -1) = 0;

    virtual void adjust(const piwidget &ui, int width = -1) = 0;
    virtual void adjust(int ssizt, int width = -1) = 0;


    //!
    //! \brief insert 插入ui (Layout_Horizontal Layout_Vertical模式)
    //! \param ui
    //! \param sizt 插入位置
    //! \param width 横/竖向占取的份数
    //!
    virtual void insert(const piwidget &ui, int siztx, int sizty, int width/* = 1*/, int height/* = 1*/) = 0;
    virtual void insert(const piwidget &ui, ivec4 &rp) {insert (ui, rp.x, rp.y, rp.z, rp.w);}

    //!
    //! \brief append 添加ui (Layout_Horizontal Layout_Vertical模式)
    //! \param ui
    //! \param width 横/竖向占取的份数
    //!
    virtual void append(const piwidget &ui, int sizty, ivec2 wh) = 0;
    //!
    //! \brief remove 删除ui (Layout_Horizontal Layout_Vertical模式)
    //! \param ui 需要删除的ui
    //! \param sizt 需要删除的位置
    //!
    virtual void remove(const ivec2 &sizt) = 0;

    //!
    //! \brief replace 替换ui (Layout_Horizontal Layout_Vertical模式)
    //! \param sui 需要替换的ui
    //! \param dui 替换后的ui
    //! \param width 替换后占取的份数: -1 原有份数
    //!
    virtual void replace(const piwidget &sui, const piwidget &dui, ivec2 wh = ivec2(-1,-1)) = 0;
    //!
    //! \brief replace 替换ui (Layout_Horizontal Layout_Vertical模式)
    //! \param sizt 需要替换的ui位置
    //! \param dui 替换后的ui
    //! \param width 替换后占取的份数: -1 原有份数
    //!
    virtual void replace(const ivec2 sizt, const piwidget &dui, ivec2 wh = ivec2(-1,-1)) = 0;

    virtual void adjust(const piwidget &ui, const ivec4 &rp) = 0;
    virtual void adjust(const ivec4 &rp) = 0;
};
#define PLAY bs_ptr(ilayout)
typedef  PLAY pilayout;
#undef PLAY

#endif // ILAYOUT_H
