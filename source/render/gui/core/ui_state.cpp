#include "ui_state.h"
#include "ui_define.h"

ui_state::ui_state(iwidget *parent):
    iwidget(parent)
{
}
ui_state::~ui_state()
{
}
void ui_state::mouse_down(eEventMouses , int , const ipoint &)
{
}
void ui_state::mouse_move(const ipoint &)
{
}
void ui_state::mouse_wheel(const int )
{
}
void ui_state::mouse_up(eEventMouses , int , const ipoint &)
{
}

void ui_state::keybord_down(eEventKeys , int)
{
}
void ui_state::keybord_up(eEventKeys , int )
{
}
void ui_state::keybord_im(const bs_event &)
{
}
// 窗口事件处理
class window_event
{
public:
    window_event(eEventMouses butkey = Event_MouseLeft);

public:
    bool process(const bs_event &evt, iwidget *ui);

protected:
    bool mouse_down(const bs_event &evt, iwidget *ui);
    bool mouse_up(const bs_event &evt, iwidget *ui);
    bool mouse_down_move(const bs_event &evt, iwidget *ui);
    bool mouse_move(const bs_event &evt, iwidget *ui);

private:
    bool is_down;
    eEventMouses mouse_button;
    bool is_adjust;
};

// 子部件事件处理
class widget_event
{
public:
    widget_event(eEventMouses butkey = Event_MouseLeft);
public:
    bool process(const bs_event &evt, iwidget *ui);

protected:
    bool mouse_down(const bs_event &evt, iwidget *ui);
    bool mouse_up(const bs_event &evt, iwidget *ui);
    bool mouse_down_move(const bs_event &evt, iwidget *ui);
    bool mouse_move(const bs_event &evt, iwidget *ui);

private:
    bool is_down;
    eEventMouses mouse_button;
};
bool ui_state::event(const bs_event &e)
{
    // 父部件响应
    if ((this->parent() == NULL) && (this->type & Ui_Window))
    {
        static window_event wevt;
        wevt.process (e, this);
    }
    // 本部件为子部件响应-扩展部件也需要响应
    else if (this->parent() && ((this->type & Ui_Widget) || (this->type & Ui_Extension)))
    {
        static widget_event wevt;
        iwidget *tmp_parent = this->parent ()->as<iwidget>();
        // 当本部件和父对象都为部件时需要对父部件进行时间响应
        if (tmp_parent && tmp_parent->type & Ui_Widget)
            wevt.process (e, tmp_parent);
        else
            wevt.process (e, this);
    }

    frect widget_geom = absolute();

    if (e.button () & Event_StateDown)
    {
        if (e.button () & Event_MouseWheel)
            mouse_wheel (e.wheel ());
        else
            mouse_down (eEventMouses(e.button () & Mouse_MaskField), e.modifier (), ipoint(e.x ()-widget_geom.x, (e.y ()-widget_geom.y)));
    }
    else if (e.button () & Event_StateMove )
        mouse_move (ipoint(e.x ()-widget_geom.x, (e.y ()-widget_geom.y)));
    else if (e.button () & Event_StateUp)
    {
        if (e.button () & Event_MouseWheel)
            ;
        else
            mouse_up (eEventMouses(e.button ()), e.modifier (), ipoint(e.x ()-widget_geom.x, (e.y ()-widget_geom.y)));
    }
    else if (e.key () & Event_StateDown)
        keybord_down (eEventKeys(e.key ()), e.modifier ());
    else if (e.key () & Event_StateUp)
        keybord_up (eEventKeys(e.key ()), e.modifier ());
    return true;
}


window_event::window_event(eEventMouses butkey )
{
    is_adjust = false;
    mouse_button = butkey;
    is_down = false;
}

bool window_event::process(const bs_event &evt, iwidget *ui)
{
    //! 指定按键被按下
    if ((evt.button() & Event_StateDown) && (evt.button() & mouse_button))
    {
        is_down = true;
        return mouse_down(evt, ui);
    }
    //! 指定按键被弹起
    else if ((evt.button() & Event_StateUp)&& (evt.button() & mouse_button))
    {
        is_down = false;
        return mouse_up(evt, ui);
    }
    //! 指定按键被按下并切移动
    else if ((evt.button() & Event_StateMove) && is_down)
        return mouse_down_move(evt, ui);
    //! 鼠标在移动
    else if (evt.button() & Event_StateMove)
        return mouse_move(evt, ui);

    return true;
}

bool window_event::mouse_down(const bs_event &evt, iwidget *ui)
{
    // 若是冻结状态则不能响应事件
    if (ui->is_freeze ())
        return true;

    // 修改状态为激活状态
    ui->state = Ui_Active;
    {
        ui->manage->active = ui;
        if (ui->manage->focus)
            ui->manage->focus->state = Ui_Normal;
        ui->manage->focus = NULL;
    }

    // 若是窗口部件需要判断是移动还是调整
    if (ui->type & Ui_Window)
    {
        // 判断是否是对窗口进行调整
        float x = ui->pos().x + ui->size().width;
        float y = ui->pos().y + ui->size().height;

        // 在右下角则为调整窗口。true为调整，false为移动
        is_adjust = ui_contains(x - UiEventMoveSize, y - UiEventMoveSize,
                                UiEventMoveSize, UiEventMoveSize, evt.x(), evt.y());
    }
    ui->last_event = evt;
    return true;
}
bool window_event::mouse_up(const bs_event &evt, iwidget *ui)
{
    // 若是冻结状态则不能响应事件
    if (ui->is_freeze ())
        return true;

    // 松开鼠标后设置为焦点状态
    if (ui->state == Ui_Active)
        ui->state = Ui_Focus;
    {
        if (ui->manage->focus)
            ui->manage->focus->state = Ui_Normal;
        ui->manage->focus = ui->manage->active;
        ui->manage->active = NULL;
    }
    return true;
}
bool window_event::mouse_down_move(const bs_event &evt, iwidget *ui)
{
    // 若是冻结状态则不能响应事件
    if (ui->is_freeze ())
        return true;

    // 移动窗口，
    if (ui->type & Ui_Window)
    {
        float last_x = ui->last_event.x();
        float last_y = ui->last_event.y();

        // 计算移动量
        last_x = evt.x() - last_x;
        last_y = evt.y() - last_y;

        if (is_adjust)
        {
            fsize tem ;
            tem.width  = ui->size().width + last_x;
            tem.height  = ui->size().height + last_y;

            if ((tem.width  > ui->minsizes.width )&&
                    (tem.width  < ui->maxsizes.width ) &&
                    (tem.height  > ui->minsizes.height ) &&
                    (tem.height  < ui->maxsizes.height )/* &&
                    tem < ui_screen::geometry ().size ()*/)
            {
                ui->size (tem);
            }
        }
        // 移动窗口
        else
        {
            ui->state = Ui_Drag;
            ui->pos(fpoint(last_x, last_y) + ui->pos());
        }
        ui->last_event = evt;
    }
    return true;
}
bool window_event::mouse_move(const bs_event &evt, iwidget *ui)
{
    // 鼠标停留时间是否大于指定时间.当前问题是被动刷新不能检测悬浮时间
    if ((ui->last_event.x() == evt.x()) && (ui->last_event.y() == evt.y()))
    {
        if (evt.timestamp() - ui->last_event.timestamp ()>= UiEventHoverLimt)
        {
            ui->state = Ui_Hover;
            ui->manage->hover = ui;
            //ui->manage->cursor_pos.x = evt.x ();
            //ui->manage->cursor_pos.y = evt.y ();
        }
    }
    else
        ui->state = Ui_Normal;

    ui->last_event = evt;
    return true;
}


widget_event::widget_event(eEventMouses butkey)
{
    mouse_button = butkey;
    is_down = false;
}

bool widget_event::process(const bs_event &evt, iwidget *ui)
{
    if ((evt.button() & Event_StateDown) && (evt.button() & mouse_button))
    {
        is_down = true;
        return mouse_down(evt, ui);
    }
    else if ((evt.button() & Event_StateUp)&& (evt.button() & mouse_button))
    {
        is_down = false;
        return mouse_up(evt, ui);
    }
    else if ((evt.button() & Event_StateMove) && is_down)
        return mouse_down_move(evt, ui);

    return mouse_move(evt, ui);
}

bool widget_event::mouse_down(const bs_event &evt, iwidget *ui)
{
    // 若是冻结状态则不能响应事件
    if (ui->is_freeze ())
        return true;

    // 修改状态为激活状态
    ui->state = Ui_Active;
    {
        ui->manage ->active = ui;
        if (ui->manage->focus && ui->manage->focus != ui)
            ui->manage->focus->state = Ui_Normal;
        ui->manage->focus = NULL;
    }

    if (evt.timestamp() - ui->last_event.timestamp() <= UiEventClickLimt)
        ui->click +=1;
    else
        ui->click = 1;
    ui->last_event = evt;
    return true;
}
bool widget_event::mouse_up(const bs_event &evt, iwidget *ui)
{
    // 若是冻结状态则不能响应事件
    if (ui->is_freeze ())
        return true;

    // 松开鼠标后设置为焦点状态
    if (ui->state == Ui_Active)
        ui->state = Ui_Focus;
    {
        // 修改所有部件只有一个为焦点
        if (ui->manage->focus)
            ui->manage->focus->state = Ui_Normal;
        ui->manage->focus = ui->manage->active;
        ui->manage->active = NULL;
    }
    return true;
}
bool widget_event::mouse_down_move(const bs_event &evt, iwidget *ui)
{
    // 将来实现拖拽功能
    return true;
}
bool widget_event::mouse_move(const bs_event &evt, iwidget *ui)
{
    // 鼠标停留时间是否大于指定时间
    if ((ui->last_event.x() == evt.x()) && (ui->last_event.y() == evt.y()))
    {
        if (evt.timestamp() - ui->last_event.timestamp () >= UiEventHoverLimt)
        {
            ui->state = Ui_Hover;
            ui->manage->hover = ui;
            //ui->manage->cursor_pos.x = evt.x ();
            //ui->manage->cursor_pos.y = evt.y ();
        }
    }
    else
        ui->state = Ui_Normal;
    ui->last_event = evt;
    return true;
}
