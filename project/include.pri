
include (platform.pri)

INCLUDEPATH += $${PWD}/../include
VPATH += $${PWD}/../include

Base_Math_Header = \
    maths/bs_maths.h \
    maths/bs_vec4.h \
    maths/bs_vec3.h \
    maths/bs_vec2.h \
    maths/bs_mat4X4.h \
    maths/bs_mat3x3.h \
    maths/bs_mat2x2.h \
    maths/bs_quaternion.h \
    maths/bs_color.h \
    maths/bs_plane.h \
    maths/bs_matMxN.h \
    maths/bs_line3.h \
    maths/bs_line2.h \
    maths/bs_aabb.h \
    maths/bs_quadtree.h

Base_Tool_Header = \
    tools/bs_algorlthm.h \
    tools/bs_vector.h \
    tools/bs_uuid.h \
    tools/bs_url.h \
    tools/bs_typeinfo.h \
    tools/bs_timer.h \
    tools/bs_string.h \
    tools/bs_stream.h \
    tools/bs_stack.h \
    tools/bs_queue.h \
    tools/bs_parser.h \
    tools/bs_memory.h \
    tools/bs_md5.h \
    tools/bs_mapdata.h \
    tools/bs_map.h \
    tools/bs_maphash.h \
    tools/bs_list.h \
    tools/bs_image.h \
    tools/bs_datetime.h \
    tools/bs_array.h \
    tools/bs_vector.inl \
    tools/bs_stream.inl \
    tools/bs_map.inl \
    tools/bs_list.inl \
    tools/bs_array.inl \
    tools/bs_maphash.inl

Base_Interface_Header = \
    interface/iio.h \
    interface/isocket.h\
    interface/ievent.h \
    interface/iwcontext.h \
    interface/ircontext.h \
    interface/iplatform.h

Render_Header = \
    render/rdefine.h \
    render/ibobject.h \
    render/icapability.h \
    render/ifbo.h \
    render/iglsl.h \
    render/iibo.h \
    render/imesh.h \
    render/ipbo.h \
    render/ipipeline.h \
    render/itexture.h \
    render/iubo.h \
    render/ivao.h \
    render/ivbo.h

Geometry_Header = \
    geometry/geomdefine.h \
    geometry/ialgorithm.h \
    geometry/igeometry.h \
    geometry/iprimitive.h \
    geometry/iproperty.h \
    geometry/ishow.h \
    geometry/icurve.h \
    geometry/isurface.h \

Graphs_Header = \
    graphs/ifrustum.h \
    graphs/icamera.h \
    graphs/icamcontrol.h \
    graphs/iscene.h \
    graphs/igraphs.h \

Gui_Header = \
   gui/igui.h \
   gui/guidef.h \
   gui/ilayout.h \
   gui/iwidget.h \
   gui/ui_style.h \
   gui/ui_skin.h \
   gui/ui_script.h \
   gui/ui_font.h \
   gui/ui_effect.h \
   gui/ui_theme.h


Base_Header = \
    bs_main.h \
    bs_intrin.h \
    bs_define.h \
    bs_atomic.h \
    bs_thread.h \
    bs_debug.h \
    bs_ptr.h \
    bs_utils.h \
    bs_filesystem.h \
    bs_event.h \
    bs_object.h \
    bs_application.h \
    bs_platform.h \
    bs_render.h \
    bs_geometry.h \
    bs_graphs.h \
    bs_gui.h

HEADERS += \
    $${Base_Math_Header} \
    $${Base_Tool_Header} \
    $${Base_Interface_Header} \
    $${Render_Header} \
    $${Geometry_Header} \
    $${Graphs_Header} \
    $${Gui_Header} \
    $${Base_Header}

LIBS += -L$${PWD}/bulid/$${BulidLevel}/$${Platform} -lbase -lplatform -lrender -lgraphs  -lpthread -lunwind

QMAKE_CFLAGS += -fshort-wchar -fPIC
#QMAKE_CFLAGS += -msse4a -mavx2 -mavx -mbmi2 -mbmi -msse2 -mf16c -mfma4 -mfma -mrdrnd -mfsgsbase -mlwp -mlzcnt -m3dnow -mmmx -msse4.2 -msse3 -mpopcnt -msse4 -mtbm -mssse3 -maes -mpclmul -msse -mxop

QMAKE_CXXFLAGS += -fshort-wchar -fPIC
#QMAKE_CXXFLAGS += -msse4a -mavx2 -mavx -mbmi2 -mbmi -msse2 -mf16c -mfma4 -mfma -mrdrnd -mfsgsbase -mlwp -mlzcnt -m3dnow -mmmx -msse4.2 -msse3 -mpopcnt -msse4 -mtbm -mssse3 -maes -mpclmul -msse -mxop
CONFIG += C++14
