#include "x11_drag_drop.h"

x11_drag_drop::x11_drag_drop(x11_extension *ext):
    ext(ext)
{
    source = NULL;
    // Xdnd 拖放原子
    XdndAware = XInternAtom(ext->display(), "XdndAware", False);
    XdndEnter = XInternAtom(ext->display(), "XdndEnter", False);
    XdndPosition = XInternAtom(ext->display(), "XdndPosition", False);
    XdndStatus = XInternAtom(ext->display(), "XdndStatus", False);
    XdndActionCopy = XInternAtom(ext->display(), "XdndActionCopy", False);
    XdndDrop = XInternAtom(ext->display(), "XdndDrop", False);
    XdndLeave = XInternAtom(ext->display(), "XdndLeave", False);
    XdndFinished = XInternAtom(ext->display(), "XdndFinished", False);
    XdndSelection = XInternAtom(ext->display(), "XdndSelection", False);
}
x11_drag_drop::~x11_drag_drop()
{

}
void x11_drag_drop::setProtocol()
{
    if (XdndAware)
    {
        // Announce support for Xdnd (drag and drop)
        const Atom version = 5;
        XChangeProperty(ext->display(), ext->handle(), XdndAware, XA_ATOM, 32,
                        PropModeReplace, (uchar*) &version, 1);
    }
}
bool x11_drag_drop::event(const XEvent &evt)
{
    if (evt.xclient.message_type == XdndEnter)
    {
        // A drag operation has entered the window
        // TODO: Check if UTF-8 string is supported by the source
        return true;
    }
    else if (evt.xclient.message_type == XdndDrop)
    {
        // The drag operation has finished dropping on
        // the window, ask to convert it to a UTF-8 string
        source = evt.xclient.data.l[0];
        XConvertSelection(ext->display(), XdndSelection, ext->UTF8_STRING,
                          XdndSelection, ext->handle(), CurrentTime);
        return true;
    }
    else if (evt.xclient.message_type == XdndPosition)
    {
        // The drag operation has moved over the window
        const int absX = (evt.xclient.data.l[2] >> 16) & 0xFFFF;
        const int absY = (evt.xclient.data.l[2]) & 0xFFFF;
        int x, y;

        //_glfwPlatformGetWindowPos(window, &x, &y);
        //_glfwInputCursorPos(window, absX - x, absY - y);

        // Reply that we are ready to copy the dragged data
        XEvent reply;
        memset(&reply, 0, sizeof(reply));

        reply.type = ClientMessage;
        reply.xclient.window = evt.xclient.data.l[0];
        reply.xclient.message_type = XdndStatus;
        reply.xclient.format = 32;
        reply.xclient.data.l[0] = ext->handle();
        reply.xclient.data.l[1] = 1; // Always accept the dnd with no rectangle
        reply.xclient.data.l[2] = 0; // Specify an empty rectangle
        reply.xclient.data.l[3] = 0;
        reply.xclient.data.l[4] = XdndActionCopy;

        XSendEvent(ext->display(), evt.xclient.data.l[0], False, NoEventMask, &reply);
        XFlush(ext->display());
        return true;
    }
    return false;
}

bool x11_drag_drop::drop(const XEvent &evt)
{
    if (evt.xselection.property)
    {
        // The converted data from the drag operation has arrived
        char* data = NULL;
        const int result = ext->property(evt.xselection.requestor, evt.xselection.property,
                                         evt.xselection.target, (unsigned char**) &data);

        if (result)
            text = data;

        XFree(data);

        XEvent reply;
        memset(&reply, 0, sizeof(reply));

        reply.type = ClientMessage;
        reply.xclient.window = source;
        reply.xclient.message_type = XdndFinished;
        reply.xclient.format = 32;
        reply.xclient.data.l[0] = ext->handle();
        reply.xclient.data.l[1] = result;
        reply.xclient.data.l[2] = XdndActionCopy;

        // Reply that all is well
        XSendEvent(ext->display(), source, False, NoEventMask, &reply);
        XFlush(ext->display());
        return true;
    }
    return false;
}
