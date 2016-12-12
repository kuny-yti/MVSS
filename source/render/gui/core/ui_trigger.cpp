#include "ui_trigger.h"

ui_trigger::ui_trigger(iwidget *parent):
    ui_state(parent)
{
    type = Ui_ExtTrigger | Ui_Widget;
    nexus.state = false;
}
ui_trigger::~ui_trigger()
{

}
void ui_trigger::mouse_down(eEventMouses , int , const ipoint &)
{
    nexus.source = this;
    nexus.state = true;
    this->emit(&nexus);
}

void ui_trigger::mouse_up(eEventMouses , int , const ipoint &)
{
    nexus.source = this;
    nexus.state = false;
    this->emit(&nexus);
}

void ui_trigger::keybord_down(eEventKeys , int)
{
    nexus.source = this;
    nexus.state = true;
    this->emit(&nexus);
}
void ui_trigger::keybord_up(eEventKeys , int )
{
    nexus.source = this;
    nexus.state = false;
    this->emit(&nexus);
}
