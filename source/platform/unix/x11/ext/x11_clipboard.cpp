#include "x11_clipboard.h"

#include <sys/select.h>

x11_clipboard::x11_clipboard(x11_extension *ext):
    ext(ext)
{
    // 字符串格式原子
    NULL_ = XInternAtom(ext->display(), "NULL", False);
    COMPOUND_STRING = XInternAtom(ext->display(), "COMPOUND_STRING", False);
    ATOM_PAIR = XInternAtom(ext->display(), "ATOM_PAIR", False);

    // 自定义选择属性原子
    CLIPBOARD_SELECTION = XInternAtom(ext->display(), "CLIPBOARD_SELECTION", False);

    // ICCCM 标准剪贴板原子
    TARGETS = XInternAtom(ext->display(), "TARGETS", False);
    MULTIPLE = XInternAtom(ext->display(), "MULTIPLE", False);
    CLIPBOARD = XInternAtom(ext->display(), "CLIPBOARD", False);

    // 剪贴板管理原子
    CLIPBOARD_MANAGER = XInternAtom(ext->display(), "CLIPBOARD_MANAGER", False);
    SAVE_TARGETS = XInternAtom(ext->display(), "SAVE_TARGETS", False);
}
x11_clipboard::~x11_clipboard()
{

}
Atom x11_clipboard::writeTargetToProperty(const XSelectionRequestEvent* request)
{
    const Atom formats[] = { ext->UTF8_STRING, COMPOUND_STRING, XA_STRING };
    const int formatCount = sizeof(formats) / sizeof(formats[0]);

    if (request->property == None)
        return None;
    else if (request->target == TARGETS)
    {
        const Atom targets[] = { TARGETS, MULTIPLE, ext->UTF8_STRING, COMPOUND_STRING,  XA_STRING };

        XChangeProperty(ext->display(), request->requestor, request->property,
                        XA_ATOM, 32, PropModeReplace, (uchar*) targets, sizeof(targets) / sizeof(targets[0]));

        return request->property;
    }
    else if (request->target == MULTIPLE)
    {
        Atom* targets;
        ulong count = ext->property(request->requestor, request->property, ATOM_PAIR, (uchar**) &targets);

        for (ulong i = 0;  i < count;  i += 2)
        {
            int j;
            for (j = 0;  j < formatCount;  j++)
            {
                if (targets[i] == formats[j])
                    break;
            }

            if (j < formatCount)
            {
                bs_utf8 utftext = text.to_utf8();
                XChangeProperty(ext->display(), request->requestor, targets[i + 1], targets[i],
                                8, PropModeReplace, (uchar*)utftext.data(), utftext.size());
            }
            else
                targets[i + 1] = None;
        }

        XChangeProperty(ext->display(), request->requestor, request->property,
                        ATOM_PAIR, 32,  PropModeReplace, (uchar*) targets, count);

        XFree(targets);

        return request->property;
    }
    else if (request->target == SAVE_TARGETS)
    {
        XChangeProperty(ext->display(), request->requestor, request->property,
                        NULL_, 32, PropModeReplace, NULL, 0);

        return request->property;
    }

    for (int i = 0;  i < formatCount;  i++)
    {
        if (request->target == formats[i])
        {
            bs_utf8 utftext = text.to_utf8();
            XChangeProperty(ext->display(), request->requestor, request->property,
                            request->target, 8, PropModeReplace, (uchar*)utftext.data(), utftext.size());

            return request->property;
        }
    }

    return None;
}
bool x11_clipboard::event(const XEvent &evt)
{
    if (evt.type == SelectionRequest)
    {
        const XSelectionRequestEvent* request = &evt.xselectionrequest;

        XEvent reply;
        memset(&reply, 0, sizeof(reply));

        reply.xselection.property = writeTargetToProperty(request);
        reply.xselection.type = SelectionNotify;
        reply.xselection.display = request->display;
        reply.xselection.requestor = request->requestor;
        reply.xselection.selection = request->selection;
        reply.xselection.target = request->target;
        reply.xselection.time = request->time;

        XSendEvent(ext->display(), request->requestor, False, 0, &reply);
        return true;
    }
    else if (evt.type == SelectionClear)
    {
        text = bs_string();
        return true;
    }
    return false;
}

bool x11_clipboard::set (const bs_string &t)
{
    text = t;
    XSetSelectionOwner(ext->display(), CLIPBOARD, ext->handle(), CurrentTime);

    if (XGetSelectionOwner(ext->display(), CLIPBOARD) != ext->handle())
    {
        printf("X11: Failed to become owner of clipboard selection\n");
        return false;
    }
    return true;
}

bs_string x11_clipboard::get()
{
    size_t i;
    const Atom formats[] = { ext->UTF8_STRING, COMPOUND_STRING, XA_STRING };
    const size_t formatCount = sizeof(formats) / sizeof(formats[0]);

    void* window;

    if (XFindContext(ext->display(),  XGetSelectionOwner(ext->display(), CLIPBOARD),
                     XUniqueContext(), (XPointer*) &window) == 0)
        return text;

    text = bs_string();

    for (i = 0;  i < formatCount;  i++)
    {
        char* data;
        XEvent event;

        XConvertSelection(ext->display(), CLIPBOARD, formats[i], CLIPBOARD_SELECTION,
                          ext->handle(), CurrentTime);

        while (!XCheckTypedEvent(ext->display(), SelectionNotify, &event))
            ext->select(NULL);

        if (event.xselection.property == None)
            continue;

        if (ext->property(event.xselection.requestor, event.xselection.property,
                          event.xselection.target, (uchar**) &data))
            text = data;

        XFree(data);

        XDeleteProperty(ext->display(), event.xselection.requestor, event.xselection.property);

        if (!text.is_empty())
            break;
    }

    if (!text.is_empty())
        printf("X11: Failed to convert clipboard to string\n");

    return text;
}
