#include "x11_event.h"
#include "tools/bs_datetime.h"
#include "tools/bs_timer.h"
#include "x11_header.h"

namespace event {
const static int aMouseTrackingTime = 40;
class mouse_tracking :public bs_timer
{
public:
    mouse_tracking(x11_event *ev, x11_extension *ex):
        bs_timer(TimerMode_Timing, aMouseTrackingTime)
    {
        posx = posy = 0;
        ext = ex;
        evt = ev;
        is_tracking = false;
        this->connect(this);
    }
    void begin(uint ev, uint modif)
    {
        if (!ext || !evt)
            return ;
        if (is_tracking.bcswap(false, true))
            this->start(aMouseTrackingTime);
        button = ev;
        modifier = modif;
    }
    void end()
    {
        if (!ext || !evt)
            return ;
        if (is_tracking.bcswap(true, false))
            this->stop();
    }
public:
    virtual void callback(tObjectNexus *)
    {
        Window formroot, tmpwin;
        int wposx, wposy, rposx, rposy;
        uint tmpnull;
        if (is_tracking)
        {
            XQueryPointer(ext->display(), ext->root(), &formroot, &tmpwin,
                          &rposx, &rposy, &wposx, &wposy, &tmpnull);
            if (wposx != posx || wposy != posy)
                evt->send_event(bs_event(button | Event_StateMove, wposx, wposy, (eEventModifiers)modifier.value (),
                                         bs_datetime::microsecond()));
            posx = wposx;
            posy = wposy;
        }
    }

    bs_atomic<uint> button;
    bs_atomic<uint> modifier;
    bs_atomic<bool> is_tracking;
    int posx, posy;
    x11_event *evt;
    x11_extension *ext;
};
}

x11_event::x11_event(x11_extension *ext):
    x11_inputmethod(ext)
{
    x11_ext = ext;
}

x11_event::~x11_event()
{
}

bool x11_event::route(const XEvent &xEvent)
{
    /*if (x11_ext->randr.available)
    {
        if (xEvent.type == x11_ext->randr.eventBase + RRNotify)
        {
            XRRUpdateConfiguration(&xEvent);
            return true;
        }
    }*/

    if (windows(xEvent))
        return true;
    else if (mouse(xEvent))
        return true;
    else if (keybord(xEvent))
        return true;
    return false;
}

bool x11_event::windows(const XEvent &xEvent)
{
    switch (xEvent.type)
    {
    case Expose:
    case ConfigureNotify:
    case UnmapNotify:
    case MapNotify:
    case DestroyNotify:
    case EnterNotify:
    case LeaveNotify:
    case ReparentNotify:
    case VisibilityNotify:
        send_event(xEvent);
        return true;
    case ClientMessage:
        if (xEvent.xclient.message_type == None)
            break;
        send_event(xEvent);
        return true;
    }
    return false;
}
bool x11_event::mouse(const XEvent &xEvent)
{
    bs_event evt;
    evt.timestamp() = bs_datetime::microsecond();

    switch (xEvent.type)
    {
    case ButtonPress:
    {
        int stk = getmousestate(xEvent);
        if (stk & Event_MouseWheel)
            send_event(bs_event(Event_StateDown | getmousestate(xEvent), getmounsewhlee(xEvent),
                                (eEventModifiers)getmodifiers(xEvent), bs_datetime::microsecond()));
        else
            send_event(bs_event(Event_StateDown | getmousestate(xEvent), xEvent.xbutton.x, xEvent.xbutton.y,
                                 (eEventModifiers)getmodifiers(xEvent), bs_datetime::microsecond()));
        return true;
    }
    case ButtonRelease:
        send_event(bs_event(Event_StateUp | getmousestate(xEvent), xEvent.xbutton.x, xEvent.xbutton.y,
                             (eEventModifiers)getmodifiers(xEvent), bs_datetime::microsecond()));
        return true;
    case MotionNotify:
        send_event(bs_event(Event_StateMove | getmousestate(xEvent), xEvent.xbutton.x, xEvent.xbutton.y,
                             (eEventModifiers)getmodifiers(xEvent), bs_datetime::microsecond()));
        return true;
    }
    return false;
}
bool x11_event::keybord(const XEvent &xEvent)
{
    static int tmpkey = 0;
    bs_event evt;
    evt.timestamp() = bs_datetime::microsecond();
    switch (xEvent.type)
    {
    case KeyPress:
        if (!x11_inputmethod::event(xEvent))
        {
            tmpkey = getkey(xEvent);
            send_event(bs_event(Event_StateDown | tmpkey , (eEventModifiers)getmodifiers(xEvent), bs_datetime::microsecond()));
        }
        return true;
    case KeyRelease:
        if (!x11_inputmethod::event(xEvent))
        {
            send_event(bs_event(Event_StateUp | tmpkey , (eEventModifiers)getmodifiers(xEvent), bs_datetime::microsecond()));
            tmpkey = 0;
        }
        return true;
    }
    return false;
}

int x11_event::getmodifiers(const XEvent &event)
{
    int ret = 0;

    if( event.xkey.state & ( ShiftMask | LockMask ) )
        ret |= Event_ModifierShift;
    if( event.xkey.state & ControlMask )
        ret |= Event_ModifierControl;
    if( event.xkey.state & Mod1Mask )
        ret |= Event_ModifierAlt;
    if (event.xkey.state & Mod4Mask)
        ret |= Event_ModifierSuper;

    return ret;
}
uint x11_event::getmounsewhlee(const XEvent& event)
{
    switch( event.xbutton.button )
    {
        case Button4: return 1;
        case Button5: return -1;
        case 6: return 1;
        case 7: return -1;
        default: return 0;
    }
    return 0;
}
uint x11_event::getmousestate(const XEvent& event )
{
    const int xState = event.xbutton.state;
    uint   state  = 0;

    if( xState & Button1Mask ) state |= Event_MouseLeft;
    if( xState & Button2Mask ) state |= Event_MouseMiddle;
    if( xState & Button3Mask ) state |= Event_MouseRight;
    if( xState & Button4Mask ) state |= Event_MouseWheel;
    if( xState & Button5Mask ) state |= Event_MouseWheel;

    return state | getmouseaction( event );
}
uint x11_event::getmouseaction(const XEvent& event )
{
    switch( event.xbutton.button )
    {
        case Button1: return Event_MouseLeft;
        case Button2: return Event_MouseMiddle;
        case Button3: return Event_MouseRight;
        case Button4:
        case Button5:
        case 6:
        case 7: return Event_MouseWheel;
        default: return Event_MouseNot;
    }
}

uint x11_event::getkey (const XEvent &event)
{
    int mk = getmodifiers(event) & Event_ModifierShift;
    return xkb_translatekc(event.xany.display, event.xkey.keycode, mk);
}
static bool xkb_has (Display *display)
{
    int major = 1;
    int minor = 0;
    int majorOpcode;
    int eventBase;
    int errorBase;
    int available = XkbQueryExtension(display, &majorOpcode, &eventBase,
                                      &errorBase, &major, &minor);

    if (available)
    {
        int supported;
        if (XkbSetDetectableAutoRepeat(display, True, &supported))
        {
            if (supported)
                return true;
        }
    }
    return false;
}
// 当按下CapsLock时 按键大小写问题需要解决
int x11_event::xkb_translatekc (Display *display, int scancode, int is_shift)
{
    static bool is_xkb = xkb_has(display);
    int keySym = 0;

    if (scancode < 8 || scancode > 255)
        return Event_KeyNot;

    if (is_xkb)
    {
        keySym = XkbKeycodeToKeysym(display, scancode, 0, 1);
        switch (keySym)
        {
        case XK_KP_0:           return Event_Key0;
        case XK_KP_1:           return Event_Key1;
        case XK_KP_2:           return Event_Key2;
        case XK_KP_3:           return Event_Key3;
        case XK_KP_4:           return Event_Key4;
        case XK_KP_5:           return Event_Key5;
        case XK_KP_6:           return Event_Key6;
        case XK_KP_7:           return Event_Key7;
        case XK_KP_8:           return Event_Key8;
        case XK_KP_9:           return Event_Key9;
        case XK_KP_Separator:
        case XK_KP_Decimal:     return Event_KeyDecimal;
        case XK_KP_Equal:       return Event_KeyEqual;
        case XK_KP_Enter:       return Event_KeyEnter;
        default:                break;
        }

        keySym = XkbKeycodeToKeysym(display, scancode, 0, 0);
    }
    else
    {
        int dummy;
        KeySym* keySyms;

        keySyms = XGetKeyboardMapping(display, scancode, 1, &dummy);
        keySym = keySyms[0];
        XFree(keySyms);
    }

    if (keySym >= XK_A && keySym <= XK_Z)
        return !is_shift ? (Event_KeyA + (keySym - XK_A)) : Event_Keya + (keySym - XK_A);
    else if (keySym >= XK_a && keySym <= XK_z)
        return !is_shift ? Event_Keya + (keySym - XK_a) : (Event_KeyA + (keySym - XK_a));
    else
    {
        switch (keySym)
        {
        case XK_Escape:         return Event_KeyEscape;
        case XK_Tab:            return Event_KeyTab;
        case XK_Shift_L:        return Event_KeyShiftLeft;
        case XK_Shift_R:        return Event_KeyShiftRight;
        case XK_Control_L:      return Event_KeyControlLeft;
        case XK_Control_R:      return Event_KeyControlRight;
        case XK_Meta_L:
        case XK_Alt_L:          return Event_KeyAltLeft;
        case XK_Mode_switch: // Mapped to Alt_R on many keyboards
        case XK_ISO_Level3_Shift: // AltGr on at least some machines
        case XK_Meta_R:
        case XK_Alt_R:          return Event_KeyAltRight;
        case XK_Super_L:        return Event_KeySuperLeft;
        case XK_Super_R:        return Event_KeySuperRight;
        case XK_Menu:           return Event_KeyMenu;
        case XK_Num_Lock:       return Event_KeyNumLock;
        case XK_Caps_Lock:
            return Event_KeyCapsLock;
        case XK_Print:          return Event_KeyPrint;
        case XK_Scroll_Lock:    return Event_KeyScrollLock;
        case XK_Pause:          return Event_KeyPause;
        case XK_Delete:         return Event_KeyDelete;
        case XK_BackSpace:      return Event_KeyBackspace;
        case XK_Return:         return Event_KeyReturn;
        case XK_Home:           return Event_KeyHome;
        case XK_End:            return Event_KeyEnd;
        case XK_Page_Up:        return Event_KeyPageUp;
        case XK_Page_Down:      return Event_KeyPageDown;
        case XK_Insert:         return Event_KeyInsert;
        case XK_Left:           return Event_KeyLeft;
        case XK_Right:          return Event_KeyRight;
        case XK_Down:           return Event_KeyDown;
        case XK_Up:             return Event_KeyUp;
        case XK_F1:             return Event_KeyF1;
        case XK_F2:             return Event_KeyF2;
        case XK_F3:             return Event_KeyF3;
        case XK_F4:             return Event_KeyF4;
        case XK_F5:             return Event_KeyF5;
        case XK_F6:             return Event_KeyF6;
        case XK_F7:             return Event_KeyF7;
        case XK_F8:             return Event_KeyF8;
        case XK_F9:             return Event_KeyF9;
        case XK_F10:            return Event_KeyF10;
        case XK_F11:            return Event_KeyF11;
        case XK_F12:            return Event_KeyF12;

            // Numeric keypad
        case XK_KP_Divide:      return Event_KeyDivide;
        case XK_KP_Multiply:    return Event_KeyMultiply;
        case XK_KP_Subtract:    return Event_KeySubtract;
        case XK_KP_Add:         return Event_KeyAdd;

            // These should have been detected in secondary keysym test above!
        case XK_KP_Insert:      return Event_Key0;
        case XK_KP_End:         return Event_Key1;
        case XK_KP_Down:        return Event_Key2;
        case XK_KP_Page_Down:   return Event_Key3;
        case XK_KP_Left:        return Event_Key4;
        case XK_KP_Right:       return Event_Key6;
        case XK_KP_Home:        return Event_Key7;
        case XK_KP_Up:          return Event_Key8;
        case XK_KP_Page_Up:     return Event_Key9;
        case XK_KP_Delete:      return Event_KeyDecimal;
        case XK_KP_Equal:       return Event_KeyEqual;
        case XK_KP_Enter:       return Event_KeyEnter;

        case XK_1:              return !is_shift ?Event_Key1 : Event_KeyExclam;
        case XK_2:              return !is_shift ?Event_Key2 : Event_KeyAt;
        case XK_3:              return !is_shift ?Event_Key3 : Event_KeyNumberSign;
        case XK_4:              return !is_shift ?Event_Key4 : Event_KeyDollar;
        case XK_5:              return !is_shift ?Event_Key5 : Event_KeyPercent;
        case XK_6:              return !is_shift ?Event_Key6 : Event_KeyCircum;
        case XK_7:              return !is_shift ?Event_Key7 : Event_KeyAmpersand;
        case XK_8:              return !is_shift ?Event_Key8 : Event_KeyAsterisk;
        case XK_9:              return !is_shift ?Event_Key9 : Event_KeyParenLeft;
        case XK_0:              return !is_shift ?Event_Key0 : Event_KeyParenRight;
        case XK_space:          return Event_KeySpace;
        case XK_minus:          return !is_shift ?Event_KeyMinus : Event_KeyUnderscore;
        case XK_equal:          return !is_shift ?Event_KeyEqual : Event_KeyPlus;
        case XK_bracketleft:    return !is_shift ?Event_KeyBracketsLeft : Event_KeyBraceLeft;
        case XK_bracketright:   return !is_shift ?Event_KeyBracketsRight: Event_KeyBraceRight;
        case XK_backslash:      return !is_shift ?Event_KeyBackslash : Event_KeyBar;
        case XK_semicolon:      return !is_shift ?Event_KeySemicolon : Event_KeyColon;
        case XK_apostrophe:     return !is_shift ?Event_KeyApostrophe : Event_KeyQuoteDbl;
        case XK_grave:          return !is_shift ?Event_KeyGraveAccent : Event_KeyTilde;
        case XK_comma:          return !is_shift ?Event_KeyComma : Event_KeyLess;
        case XK_period:         return !is_shift ?Event_KeyPeriod : Event_KeyGreater;
        case XK_slash:          return !is_shift ?Event_KeySlash : Event_KeyQuestion;

        case XK_less:           return Event_KeyLess;
        case XK_greater:        return Event_KeyGreater;
        case XK_exclam :        return Event_KeyExclam;
        case XK_quotedbl:       return Event_KeyQuoteDbl;
        case XK_numbersign:     return Event_KeyNumberSign;
        case XK_dollar:         return Event_KeyDollar;
        case XK_percent:        return Event_KeyPercent;
        case XK_ampersand :     return Event_KeyAmpersand;
        case XK_parenleft:      return Event_KeyParenLeft;
        case XK_parenright:     return Event_KeyParenRight;
        case XK_asterisk:       return Event_KeyAsterisk;
        case XK_plus:           return Event_KeyPlus;
        case XK_colon:          return Event_KeyColon;
        case XK_question:       return Event_KeyQuestion;
        case XK_at:             return Event_KeyAt;
        case XK_asciicircum:    return Event_KeyCircum;
        case XK_underscore:     return Event_KeyUnderscore;
        case XK_braceleft:      return Event_KeyBraceLeft;
        case XK_bar :           return Event_KeyBar;
        case XK_braceright:     return Event_KeyBraceRight;
        case XK_asciitilde:     return Event_KeyTilde;

        default:                break;
        }
    }

    return Event_KeyNot;
}
