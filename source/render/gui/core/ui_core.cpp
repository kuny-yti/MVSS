#include "ui_core.h"
#include "gui/theme/ui_themecore.h"

// 排序根节点，用于排列选中的先后顺序
void tWidgetManages::sqrt(iwidget *ui)
{
    if (ui->parent() != NULL)
        return;

    for (uint i = 0; i < root.count(); ++i)
        if (ui == root[i])
            root.move(i, 0);
}
// 向根节点添加父部件
bool tWidgetManages::append(iwidget *p)
{
    if (root.contains (p))
        return false;
    if (p->parent() == NULL)
    {
        root.append (p);
        return true;
    }
    return false;
}
// 向根节点删除父部件
bool tWidgetManages::remove(iwidget *oui)
{
    //! 若是删除根对象则其他子对象全部删除
    if (oui && (oui->parent() == NULL))
    {
        for (uint i = 0; i < root.count(); ++i)
        {
            if (root[i] == oui)
            {
                root.remove(i);
                return true;
            }
        }
    }
    return false;
}

// 根据事件所在的父部件进行事件派遣
bool tWidgetManages::dispatch(const bs_event &evt)
{
    bool dispret = false;

    // 若激活部件为真则有部件还在事件模式状态
    if (active)
        dispret = posting(active, evt);
    // 当前有焦点对象则将事件下发到焦点对象，用于键盘事件
    else if (focus && (evt.key () != Event_KeyNot || focus->contains (fpoint(evt.x(), evt.y()))))
        dispret = posting(focus, evt);
    // 当鼠标进入窗口后又进入到子部件中，此时则记录的窗口对象
    else if (ui_parent && ui_parent->contains (fpoint(evt.x(), evt.y())))
    {
        if (enter && enter->contains (fpoint(evt.x(), evt.y())))
            dispret = posting(enter, evt);
        else
            dispret = posting(ui_parent, evt);
    }
    /*else if (enter && enter->contains (fpoint(evt.x(), evt.y())))
        dispret = posting(enter, evt);*/
    else
    {
        ui_parent = NULL;
        enter = NULL;
        foreach (iwidget * &wg , root)
        {
            // 若鼠标位置在指定部件内则将其设为进入状态
            if (wg->contains (fpoint(evt.x(), evt.y())))
            {
                if (!ui_parent || (ui_parent && ui_parent->type &Ui_Window))
                {
                    ui_parent = wg;
                    leave = NULL;
                }
                dispret = posting(wg, evt);
                break;
            }
            else if (!wg->is_hide () && !wg->is_freeze ())
                wg->state = Ui_Normal;
        }
    }
    // 排序当前被激活的窗口部件
    if (active && (active->type & Ui_Window))
        sqrt (active);

    if (!dispret && focus)
    {
        focus->state = Ui_Normal;
        focus = NULL;
    }

    return dispret;
}
bool tWidgetManages::posting(iwidget *gui, const bs_event &evt)
{
    bool outR = false;
    // 若为隐藏则不处理部件事件
    if (gui->is_hide ())
        return true;

    if (!active)
    {
        // 接受本组件下的子组件处理部件事件
        for (bs_object::child uw = gui->begin(); uw != gui->end(); ++uw)
        {
            iwidget *fake = bsAsCast(iwidget *) (*uw);
            if (fake->contains (fpoint(evt.x(), evt.y())))
                outR |= posting(fake, evt);
            else if (!fake->is_hide () || !gui->is_freeze ())
                fake->state = Ui_Normal;
        }
    }

    if (!outR)
    {
        if (enter != gui)
            leave = enter;
        enter = gui;

        return gui->event (evt);
        // 父部件响应
        /*if ((gui->parent() == NULL) && (gui->type & Ui_Window))
            return evtwindow (evt, gui);
        // 本部件为子部件响应-扩展部件也需要响应
        else if (gui->parent() && ((gui->type & Ui_Widget) || (gui->type & Ui_Extension)))
        {
            iwidget *tmp_parent = gui->parent ()->as<iwidget>();
            // 当本部件和父对象都为部件时需要对父部件进行时间响应
            if (tmp_parent && tmp_parent->type & Ui_Widget)
                return evtwidget (evt, tmp_parent);
            return evtwidget(evt, gui);
        }*/
    }
    return outR;
}

void tWidgetManages::render(float width, float height)
{
    // 处理焦点控件
    if (focus && (focus != active))
        focus->state = Ui_Focus;

    //! 取反序原因是，OpenGL 渲染原因
    for (int i = root.count()-1; i >= 0 ; --i)
        paint(root[i], width, height);

    // 绘制置顶部件
    if (top)
        paint(top, width, height);

    if (dump != Dump_Not)
    {
        //! 取反序原因是，OpenGL 渲染原因
        for (int i = root.count()-1; i >= 0 ; --i)
        {
            root[i]->dump ();
            for (bs_object::child it = root[i]->begin(); it != root[i]->end(); ++it)
            {
                iwidget *renui = (*it)->as<iwidget>();
                if (renui)
                    renui->dump ();
            }
        }

        // 绘制置顶部件
        if (top)
            top->dump ();
    }
}
void tWidgetManages::paint (iwidget *gui, float width, float height)
{
    //! 若对象为隐藏则不绘制
    if (!gui->is_hide())
    {
        iwidget *tmp_parent = NULL;
        if (gui->parent())
            tmp_parent = gui->parent()->as<iwidget>();

        //! 计算适应的尺寸()
        fsize fit_size = gui->fit();

        if (gui->type & Ui_Widget || gui->type & Ui_Window)
        {
            //! 本类为窗口-设置窗口的绝对位置和适合尺寸
            if (gui->type &Ui_Window && !tmp_parent)
            {
                //! 若为最大化需要更新绘制的尺寸
                if (gui->is_maximun)
                    gui->resize (fsize(width, height));
                gui->geometrys[iwidget::Geom_Abs].xy () = gui->pos ();
            }

            //! 除了父对象为布局-需要根据父窗口的默认位置设置部件的位置和尺寸
            else if (tmp_parent && !(tmp_parent->type & Ui_Layout))
                gui->geometrys[iwidget::Geom_Abs].xy () = tmp_parent->location;

            //! 1.当前类型为窗口类型时需要设置尺寸
            //! 2.或者父类型补位Ui_Layout类型时需要设置尺寸，当为Ui_Layout类型时由布局控件进行设置子部件
            if ((gui->type & Ui_Window && !tmp_parent) || (tmp_parent && !(tmp_parent->type & Ui_Layout)))
            {
                //! 若当前几何尺寸大于适应的尺寸时使用设置的几何尺寸
                if (gui->size().width > fit_size.width)
                    gui->geometrys[iwidget::Geom_Abs].width = gui->geometrys[iwidget::Geom].width;
                else
                {
                    if (gui->type & Ui_Window && !tmp_parent)
                        gui->size(fsize(fit_size.width, gui->size ().height));
                    gui->geometrys[iwidget::Geom_Abs].width = fit_size.width;
                }
                if (gui->size().height > fit_size.height)
                    gui->geometrys[iwidget::Geom_Abs].height = gui->geometrys[iwidget::Geom].height;
                else
                {
                    if (gui->type & Ui_Window && !tmp_parent)
                        gui->size(fsize(gui->size ().width, fit_size.height));
                    gui->geometrys[iwidget::Geom_Abs].height = fit_size.height;
                }
            }
        }

        //! 设置部件依父对象位置为准
        if (tmp_parent)
        {
            if (tmp_parent->type & Ui_Window)
            {
                if (gui->type & Ui_Layout)
                {
                    fpoint loc = tmp_parent->pos();
                    loc.y += tmp_parent->location.y;
                    gui->geometrys[iwidget::Geom_Abs].xy () = loc;
                    //! 若当前几何尺寸大于适应的尺寸时使用设置的几何尺寸
                    if (tmp_parent->size().width > fit_size.width)
                        gui->geometrys[iwidget::Geom_Abs].width = tmp_parent->size().width;
                    else
                        gui->geometrys[iwidget::Geom_Abs].width = fit_size.width;
                    if (tmp_parent->size().height > fit_size.height)
                        gui->geometrys[iwidget::Geom_Abs].height = tmp_parent->size().height - tmp_parent->location.y*2;
                    else
                        gui->geometrys[iwidget::Geom_Abs].height = fit_size.height - tmp_parent->location.y*2;

                    //处理布局的对齐方式
                    /*ilayout *lay = bsAsCast(ilayout)(gui);
                            if (lay->align == Align_Minimum)
                            {
                                gui->geometrys_abs.size.width() = fit_size.width();
                                gui->geometrys_abs.size.height() = fit_size.height();
                            }
                            else if (lay->align == Align_Middle)
                            {
                                gui->geometrys_abs.position.x += (gui->geometrys_abs.size.width()*0.5 -fit_size.width());
                                gui->geometrys_abs.position.y += (gui->geometrys_abs.size.height()*0.5 -fit_size.height());
                            }*/
                }
                else
                {
                    fpoint loc = (gui->pos () + tmp_parent->pos());
                    loc.y += tmp_parent->location.y;
                    gui->geometrys[iwidget::Geom_Abs].xy () = loc;
                    //! 若当前几何尺寸大于适应的尺寸时使用设置的几何尺寸
                    if (gui->size().width > fit_size.width)
                        gui->geometrys[iwidget::Geom_Abs].width = gui->size().width;
                    else
                        gui->geometrys[iwidget::Geom_Abs].width = fit_size.width;
                    if (gui->size().height > fit_size.height)
                        gui->geometrys[iwidget::Geom_Abs].height = gui->size().height;
                    else
                        gui->geometrys[iwidget::Geom_Abs].height = fit_size.height;
                }
            }
            else if (tmp_parent->type & Ui_Widget && !(tmp_parent->type & Ui_Layout))
            {
                gui->geometrys[iwidget::Geom_Abs] = tmp_parent->geometrys[iwidget::Geom_Abs];
                gui->geometrys[iwidget::Geom_Abs].xy () += tmp_parent->location;
                gui->geometrys[iwidget::Geom_Abs].zw () -= tmp_parent->location;
            }
        }

        if (gui->themes == NULL)
            gui->themes = &(*ui_themecore::instance ())[gui->type] ;

        gui->paint();

        //! 根据根节点id 渲染子节点
        for (bs_object::child it = gui->begin(); it != gui->end(); ++it)
        {
            iwidget *renui = (*it)->as<iwidget>();
            if (renui)
                paint(renui, width, height);
        }
    }
}
