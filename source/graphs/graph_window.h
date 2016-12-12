#ifndef graph_window_H
#define graph_window_H
#include "window_base.h"
#include "graphs/igraphs.h"

namespace impl {
class graphs_window : public window_base
{
    bsObject(graphs_window)
public:
    typedef bs_list <piscene > VecPaint_Ptr;
    typedef bs_list <int>  VecPaintIs;
    typedef bs_list <picamera> VecCamera_Ptr;
    typedef bs_list <picamcontrol > VecCameraControl_Ptr;

    pipipeline pipe;
    int64 render_elapsed_time; // 渲染用的时间
    int64 frame_elapsed_time; // 一帧完成需要的时间

    VecCamera_Ptr camera;//默认相机
    picamera camera_default;
    VecCameraControl_Ptr camera_ctrl;

    VecPaint_Ptr paint;
    VecPaintIs   is_paint_init;

    pigui paint_gui;

    graphs_window();
    ~graphs_window();

    piscene find_scene(const bs_string &name);
    void insert_scene(const piscene &pat);
    void delete_scene(const piscene &pat);
    void delete_scene(const bs_string &name);
    void event_scene(const bs_event &e);
    void render_scene(const isize &sz);
    picamcontrol find_control(const bs_string &name);
    void insert_control(const picamcontrol &ctrl);
    void delete_control(const picamcontrol &ctrl);
    void delete_control(const bs_string &name);

    picamera find_camera(const bs_string &name);
    void insert_camera(const picamera &cam);
    void delete_camera(const picamera &cam);
    void delete_camera(const bs_string &name);

    void set_control_camera(const picamera &cam);
    void event_control(const bs_event &e);
    bool is_control_camera();

    virtual void callback(tObjectNexus *param);
    virtual void paints();
    virtual void initializes();
    virtual void events(const bs_event &e);
};
}

#endif // graph_window_H
