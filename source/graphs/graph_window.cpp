#include "graph_window.h"
#include "ray_pick.h"
#include "bs_render.h"
#include "bs_application.h"
#include "tools/bs_timer.h"

namespace impl {

static const char * const mvpSourceVs =
        "#version 330 \n"
        "layout (location = 0) in vec3 inVertex; \n"
        "layout (location = 1) in vec4 inColor; \n"
        "layout (location = 2) in vec3 inNormal; \n"
        "layout (location = 3) in vec2 inCoord; \n"
        "out vec4 fcolor; \n"
        "out vec2 fcoord; \n"
        ""
        "uniform mat4 matModel; \n"
        "uniform mat4 matView; \n"
        "uniform mat4 matProjection; \n"
        "void main() { \n"
        "fcolor = inColor; \n"
        "fcoord = inCoord; \n"
        "gl_Position = matModel * matView * matProjection * vec4(inVertex, 1.0); \n"
        "}";
static const char * const mvpSourceFs =
        "#version 330 \n"
        "in vec4 fcolor; \n"
        "in vec2 fcoord; \n"
        "out vec4 outColor; \n"
        "out vec2 outCoord; \n"
        "void main(){outColor = fcolor; outCoord = fcoord;}";

struct mvp
{
    bool is_init;
    pipipeline pipe;
    pishader vshader;
    pishader fshader;
    piprogram prog;
    mvp():is_init(false),vshader(),prog(){}

    bool initialize(const pirender &ird)
    {
        pipe = ird->pipeline;
        vshader = ird->shader();
        fshader = ird->shader();
        prog = ird->program();
        if (!vshader && !prog && !fshader)
            return false;
        vshader->create(Shader_Vertex);
        bs_string errstr = vshader->compile(mvpSourceVs);
        if (!errstr.is_empty())
        {
            log_warning(errstr.to_latin1());
            vshader->destroy();
            return false;
        }
        fshader->create(Shader_Fragment);
        errstr = vshader->compile(mvpSourceFs);
        if (!errstr.is_empty())
        {
            log_warning(errstr.to_latin1());
            vshader->destroy();
            fshader->destroy();
            return false;
        }

        prog->create();
        prog->append(vshader);
        prog->append(fshader);
        errstr = prog->link();
        if (!errstr.is_empty())
        {
            log_warning(errstr.to_latin1());
            vshader->destroy();
            fshader->destroy();
            prog->destroy();
            return false;
        }
        is_init = true;
        return true;
    }

    void bind(const mat4 &matProjection, const mat4 &matView, const mat4 &matModel = mat4::identity)
    {
        prog->bind();
        prog->uniform("matProjection", matProjection);
        prog->uniform("matView", matView);
        prog->uniform("matModel", matModel);
    }
    void unbind()
    {
        prog->unbind();
    }
};

}

using namespace impl;
graphs_window::graphs_window():
    window_base()
{
    camera.clear ();
    camera_ctrl.clear ();
    paint.clear ();
    is_paint_init.clear ();
}
graphs_window::~graphs_window()
{
    paint.clear ();
    is_paint_init.clear ();
}

piscene graphs_window::find_scene(const bs_string &name)
{
    for (size_t i = 0; i < paint.count(); ++i)
    {
        if (paint[i]->name() == name)
            return paint[i];
    }
    return piscene ();
}
void graphs_window::insert_scene(const piscene &pat)
{
    pat->connect(this);
    paint.append (pat);
    is_paint_init.append (false);
}
void graphs_window::delete_scene(const piscene &pat)
{
    for (size_t i = 0; i < paint.count(); ++i)
    {
        if (paint[i] == pat)
        {
            paint.remove (i);
            is_paint_init.remove (i);
        }
    }
}
void graphs_window::delete_scene(const bs_string &name)
{
    piscene paint = find_scene (name);
    if (paint)
        delete_scene (paint);

}
void graphs_window::event_scene(const bs_event &e)
{
    for (size_t i = 0; i < paint.count(); ++i)
    {
        if (paint[i])
            paint[i]->event(e);
    }
}
void graphs_window::render_scene(const isize &sz)
{
    for (size_t i = 0; i < paint.count(); ++i)
    {
        if (paint[i])
        {
            if (!is_paint_init[i])
            {
               paint[i]->initialize();
                is_paint_init[i] = true;
            }
            paint[i]->paint(sz);
        }
    }
}

picamcontrol graphs_window::find_control(const bs_string &name)
{
    for (size_t i = 0; i < camera_ctrl.count(); ++i)
    {
        if (camera_ctrl[i]->name == name)
            return camera_ctrl[i];

    }
    return picamcontrol();
}
void graphs_window::insert_control(const picamcontrol &ctrl)
{
    if (camera_default)
        ctrl->camera (camera_default);
    camera_ctrl.append ( ctrl);
}
void graphs_window::delete_control(const picamcontrol &ctrl)
{
    for (size_t i = 0; i < camera_ctrl.count(); ++i)
    {
        if (camera_ctrl[i] == ctrl)
            camera_ctrl.remove (i);
    }
}
void graphs_window::delete_control(const bs_string &name)
{
    picamcontrol ctrl = find_control (name);
    if (ctrl)
        delete_control (ctrl);

}

picamera graphs_window::find_camera(const bs_string &name)
{
    for (size_t i = 0; i < camera.count(); ++i)
    {
        if (camera[i]->name == name)
            return camera[i];
    }
    return picamera();
}
void graphs_window::insert_camera(const picamera &cam)
{
    if (!camera.count ())
        camera_default = cam;

    if (is_control_camera())
        set_control_camera(camera_default);
    camera.append (cam);
}
void graphs_window::delete_camera(const picamera &cam)
{
    for (size_t i = 0; i < camera.count(); ++i)
    {
        if (camera[i] == cam)
            camera.remove ( i);
    }
}
void graphs_window::delete_camera(const bs_string &name)
{
    picamera f = find_camera(name);
    if (f)
        delete_camera(f);
}

void graphs_window::set_control_camera(const picamera &cam)
{
    for (size_t i = 0; i < camera_ctrl.count(); ++i)
    {
        if (camera_ctrl[i])
            camera_ctrl[i]->camera(cam);
    }
}
void graphs_window::event_control(const bs_event &e)
{
    for (size_t i = 0; i < camera_ctrl.count(); ++i)
    {
        if (camera_ctrl[i])
            camera_ctrl[i]->event(e);
    }
}
bool graphs_window::is_control_camera()
{
    for (size_t i = 0; i < camera_ctrl.count(); ++i)
    {
        if (camera_ctrl[i])
            if (!camera_ctrl[i]->cameras)
                return true;
    }
    return false;
}

void graphs_window::callback(tObjectNexus *param)
{
    if (!param)
        this->update();
    else
        window_base::callback(param);
}
void graphs_window::paints()
{
    static bool is_runt = false;
    static bs_timer datetime, timer;
    static int frames = 0;
    if (!is_runt)
    {
        timer.start ();
        datetime.start ();
        is_runt = true;
    }

    bsASSERT(context () == 0, "context not!");

    frame_elapsed_time = datetime.restart ();

    pipe->viewport (*window_base::viewport);
    //if (!mvpope.is_init)
    //    mvpope.initialize(irender());

    if (camera_default.get() != NULL)
    {
        mat4 mode, view, projection;
        mode = mat4::identity;
        camera_default->window_size(window_base::viewport->zw());

        view = camera_default->matrix_view();
        view = transpose(view);

        projection = camera_default->matrix_projection();
        projection = transpose(projection);

        //mvpope.bind(projection, view, mode);
        pipe->matrix_identity(Matrix_Projection);
        pipe->matrix_load(projection);
        pipe->matrix_identity(Matrix_ModelView);
        pipe->matrix_load(view);
    }
    else
    {
        mat4 mode, view, projection;
        mode = mat4::identity;
        view = mat4::createLookAt(vec3(0,2,3), vec3(0,0,0), vec3(0,1,0));
        view = transpose(view);
        projection = mat4::createPerspective(bs_deg2rad(45.f), 1280.f/720.f, 0.1f, 100.f);
        projection = transpose(projection);

        pipe->matrix_identity(Matrix_Projection);
        pipe->matrix_load(projection);
        pipe->matrix_identity(Matrix_ModelView);
        pipe->matrix_load(view);
        //mvpope.bind(projection, view, mode);
    }

    render_scene (window_base::viewport->zw());

    //mvpope.unbind();
    render_elapsed_time = datetime.elapsed ();

    if (paint_gui.get ())
        paint_gui->paint ();

    /*if (analyzer)
    {
        int cfps = 0;
        int64 cum = timer.elapsed ();

        if (cum >= 1000)
        {
            cfps = frames * 1000. / cum;
            frames = 0;
            timer.restart();
        }
        else
            ++frames;

        analyzer->update (render_elapsed_time, frame_elapsed_time, cfps);
    }*/
}
void graphs_window::initializes()
{

    bsASSERT(!bs_render::instance ()->initialize (), "render initialize not");
    pipe = bs_render::instance ()->pipeline;

    picapability capa = bs_render::instance ()->capability();
    log_warnings("engine gl version: %s", capa->version.to_latin1());
    log_warnings("engine glsl version: %s", capa->version_glsl.to_latin1());
    log_warnings("engine gl renderer: %s", capa->renderer.to_latin1());
    log_warnings("engine gl vendor: %s", capa->vendor.to_latin1());

}
void graphs_window::events(const bs_event &e)
{
    if (paint_gui.get ())
        paint_gui->event (e);

    event_control (e);
    event_scene (e);
    this->update();
}
