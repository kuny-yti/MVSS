#ifndef X11_WINDOW_MANAGER_H
#define X11_WINDOW_MANAGER_H

#include "x11_extension.h"
#include "tools/bs_image.h"

class x11_window_manager
{
public:
    x11_window_manager(x11_extension *ext);
    ~x11_window_manager();

    void enableError();
    void disableError();

    bool is_exit()const;

    eWindowStates getWindowState();
    void setWindowState(int s, int full_inx = -1);

    void setWindowTitle(const char* title);
    void setWindowIcon(int count, const bs_image* images);
    void setWindowPos(const ivec2 &size);
    void setWindowSize(const ivec2 &size);
    void sendEventToWM(Atom type, long a, long b, long c, long d, long e);
    void setWMProtocol();

    vec2 getWindowPos();
    vec2 getWindowSize();

    bool event(const XEvent &evt);
private:
    void toggle_fullscreen(int index = -1);
    void normal_window(const ivec2 &size);

    bool detectEWMH();
private:
    x11_extension *ext;
    bool req_exit;

    Atom        UTF8_STRING;
    // Window manager atoms
    Atom        WM_PROTOCOLS;
    Atom        WM_STATE;
    Atom        WM_DELETE_WINDOW;
    Atom        NET_WM_NAME;
    Atom        NET_WM_ICON_NAME;
    Atom        NET_WM_ICON;
    Atom        NET_WM_PID;
    Atom        NET_WM_PING;
    Atom        NET_WM_WINDOW_TYPE;
    Atom        NET_WM_WINDOW_TYPE_NORMAL;
    Atom        NET_WM_STATE;
    Atom        NET_WM_STATE_ABOVE;
    Atom        NET_WM_STATE_FULLSCREEN;
    Atom        NET_WM_STATE_MAXIMIZED_VERT;
    Atom        NET_WM_STATE_MAXIMIZED_HORZ;
    Atom        NET_WM_BYPASS_COMPOSITOR;
    Atom        NET_WM_FULLSCREEN_MONITORS;
    Atom        NET_ACTIVE_WINDOW;
    Atom        NET_FRAME_EXTENTS;
    Atom        NET_REQUEST_FRAME_EXTENTS;
    Atom        MOTIF_WM_HINTS;
};

#endif // X11_WINDOW_MANAGER_H
