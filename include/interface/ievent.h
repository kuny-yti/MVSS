#ifndef BS_SYSTEM_H
#define BS_SYSTEM_H

#include "bs_define.h"

/* 标志位宽 */
#define Flags_Wide 0x01
/* 事件定义宏 */
#define Event_Define(val, field) (val << field)
/* 事件定义获取宏 */
#define Event_GetDef(val, field) (val >> field)

/* 系统事件标志位定义 */
#define System_Flags 31
/* 系统事件掩码段定义 */
#define System_MaskFlags (Flags_Wide << System_Flags)

/* 键盘事件标志位定义 */
#define Keybord_Flags 30
/* 键盘事件掩码段定义 */
#define Keybord_MaskFlags (Flags_Wide << Keybord_Flags)

/* 鼠标事件标志位定义 */
#define Mouse_Flags 29
/* 鼠标事件掩码段定义 */
#define Mouse_MaskFlags (Flags_Wide << Mouse_Flags)

/* 窗口事件标志位定义 */
#define Window_Flags 28
/* 窗口事件掩码段定义 */
#define Window_MaskFlags (Flags_Wide << Window_Flags)

/* 拖放事件标志位定义 */
#define DragDrop_Flags 27
/* 拖放事件掩码段定义 */
#define DragDrop_MaskFlags (Flags_Wide << DragDrop_Flags)

/* 剪切板事件标志位定义 */
#define Clipboard_Flags 26
/* 剪切板事件掩码段定义 */
#define Clipboard_MaskFlags (Flags_Wide << Clipboard_Flags)


typedef enum
{
    Event_System = Event_Define(Flags_Wide , System_Flags),
    Event_Keybord = Event_Define(Flags_Wide, Keybord_Flags),
    Event_Mouse = Event_Define(Flags_Wide, Mouse_Flags),
    Event_Window = Event_Define(Flags_Wide, Window_Flags),
    Event_DragDrop = Event_Define(Flags_Wide, DragDrop_Flags),
    Event_Clipboard = Event_Define(Flags_Wide, Clipboard_Flags),

    Event_FlagMask = Event_System | Event_Keybord | Event_Mouse | Event_Window |
                     Event_DragDrop | Event_Clipboard
}eEvents;

/* 系统事件定义宏 */
#define SystemEventDef(val) (Event_System | val)
/* 键盘事件定义宏 */
#define KeybordEventDef(val) (Event_Keybord | val)
/* 鼠标事件定义宏 */
#define MouseEventDef(val) (Event_Mouse | val)
/* 窗口事件定义宏 */
#define WindowEventDef(val) (Event_Window | val)
/* 拖放事件定义宏 */
#define DragDropEventDef(val) (Event_DragDrop | val)
/* 剪切板事件定义宏 */
#define ClipboardEventDef(val) (Event_Clipboard | val)

class ievent
{
public:
    ievent(int code = 0): _code(code),_time(0) {}
    virtual ~ievent(){}

    int code()_fix_{return _code;}
    bool is_system()_fix_{return _code & Event_System;}
    bool is_keybord()_fix_{return _code & Event_Keybord;}
    bool is_mouse()_fix_{return _code & Event_Mouse;}
    bool is_window()_fix_{return _code & Event_Window;}
    bool is_dragdrop()_fix_{return _code & Event_DragDrop;}
    bool is_clipboard()_fix_{return _code & Event_Clipboard;}
    bool is_ext()_fix_{return !(_code & Event_FlagMask);}

    int64 &timestamp(){return _time;}
    int64 timestamp()_fix_{return _time;}

private:
    int _code;
    int64 _time;    ///< 事件发生时间戳
};


#endif // BS_SYSTEM_H
