#include "bs_graphs.h"
#include "graph_window.h"

bs_graphs::bs_graphs():
    impl(bsNew (impl::graphs_window()))
{
}

bs_graphs::~bs_graphs()
{
    bsDelete (impl);
}

void bs_graphs::initialize(const piwindow &window, const pirender &rbase)
{
    bsASSERT(window.get() == NULL, "graph initialize window handle null.");
    bsASSERT(rbase.get() == NULL, "graph initialize render base null.");

    impl->initialize(window, rbase);
}

void bs_graphs::append(const piscene &pat)
{
    impl->insert_scene (pat);
}
#include "camera/camera_target.h"
#include "camera/camera_eye.h"
picamcontrol bs_graphs::append(eCameraControls ctrl)
{
    picamcontrol ctl;
    switch ((int) ctrl)
    {
    case Camera_Eye:
        ctl = bs_malloc( camera_eye);
        break;
    case Camera_Target:
        ctl = bs_malloc( camera_target);
        break;
    case Camera_Fly:
        break;
    default:
        ctl = bs_malloc( camera_target);
        break;
    }
    append(ctl);
    return ctl;
}
#include "camera/camera_impl.h"
void bs_graphs::append(const picamera &cam)
{
    if (cam.get () == NULL)
        impl->insert_camera(bs_malloc (camera_impl));
    else
        impl->insert_camera(cam);
}
void bs_graphs::append(const picamcontrol &ctrl)
{
    impl->insert_control (ctrl);
}

void bs_graphs::remove(const picamcontrol &ctrl)
{
    impl->delete_control (ctrl);
}
void bs_graphs::remove(const picamera &cam)
{
    impl->delete_camera (cam);
}
void bs_graphs::remove(const piscene &pat)
{
    impl->delete_scene (pat);
}

void bs_graphs::set(const picamera &cam)
{
    impl->camera_default = cam;
    impl->set_control_camera(cam);
}
void bs_graphs::set(const picamcontrol &)
{

}
void bs_graphs::set(const pigui &ui)
{
    impl->paint_gui = ui;
}
picamera bs_graphs::camera()
{
    return impl->camera_default;
}
picamcontrol bs_graphs::control()
{
    return picamcontrol();
}

#include "ray_pick.h"
bool bs_graphs::pick(const ipoint &p, fpoint3 &_src)
{
    fpoint3 _near, _far;
    impl->pipe->ray_line(p, _near, _far);
    return ray_pick::pick(_src, _near, _far);
}

