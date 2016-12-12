#ifndef X11_CLIPBOARD_H
#define X11_CLIPBOARD_H

#include "x11_extension.h"
#include "tools/bs_string.h"

class x11_clipboard
{
public:
    x11_clipboard(x11_extension *ext);
    ~x11_clipboard();

    bool event(const XEvent &evt);

    bool set (const bs_string &text);
    bs_string get ();
protected:
    Atom writeTargetToProperty(const XSelectionRequestEvent* request);

private:
    x11_extension *ext;

    bs_string text;
    // Selection (clipboard) atoms
    Atom        TARGETS;
    Atom        MULTIPLE;
    Atom        CLIPBOARD;
    Atom        CLIPBOARD_MANAGER;
    Atom        SAVE_TARGETS;
    Atom        NULL_;
    Atom        COMPOUND_STRING;
    Atom        ATOM_PAIR;
    Atom        CLIPBOARD_SELECTION;
};

#endif // X11_CLIPBOARD_H
