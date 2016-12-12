#ifndef IPLATFORM
#define IPLATFORM

#include "bs_define.h"
#include "tools/bs_string.h"
#include "tools/bs_image.h"
#include "maths/bs_vec4.h"
#include "bs_event.h"
#include "bs_object.h"
#include "iwcontext.h"

/*!
 * @brief The tPlatformDevices struct 平台设备信息
 * @struct tPlatformDevices
 */
typedef struct
{
    winid adapter;    ///< 当前设备对应的适配器
    bs_string id;
    bs_string key;
    bs_string name;
    bs_string string;
}tPlatformDevices;

typedef struct
{
    bs_vector<ushort> red;
    bs_vector<ushort> green;
    bs_vector<ushort> blue;
}tGammaRamps;

typedef struct
{
    float   dpi;              ///< 像素精度
    int     bpp;              ///< 像素位数
    int     fps;              ///< 刷新频率
    int     bits;             ///< 颜色通道位数 [red,green,blue]
    ivec2   size;             ///< 显示器尺寸
}tMonitorColors;
inline bool operator == (const tMonitorColors &mc1, const tMonitorColors &mc2)
{
    return bsEqualf(mc1.dpi, mc2.dpi) &&
            mc1.bpp == mc2.bpp &&
            mc1.fps == mc2.fps &&
            mc1.bits == mc2.bits &&
            mc1.size == mc2.size;
}

/*!
 * @brief The tMonitorInofs struct 监视器信息
 * @struct tPlatformDevices
 */
typedef struct
{
    tPlatformDevices            dev;           ///< 设备信息
    bs_string                   name;          ///< 显示器名称
    tGammaRamps                 gamma;         ///< 伽马控制值
    bs_vector<tMonitorColors>   color;         ///< 颜色信息
    vec2                        pos ;

    int                         index;         ///< 显示器索引
    winid                       id;            ///< 显示器句柄

    vec2                        inches;        ///< 英寸数
    vec2                        millimeters;   ///< 毫米数 *25.4 = 英寸数
    vec2                        ppi;           ///< 每英寸的像素数
}tMonitorInofs;

typedef enum
{
    Cursor_Arrow,
    Cursor_IBeam,
    Cursor_Cross,
    Cursor_Hand,
    Cursor_Wait,
    Cursor_DragMove,
    Cursor_DragCopy,
    Cursor_DragLink,
    Cursor_WhatsThis,
    Cursor_Forbidden,
    Cursor_Not
}eCursorTypes;
/*!
 * @brief The iplatform class 系统
 * @class iplatform
 */
bsPackage iplatform
{
    /*!
     * @brief The iwindow class 系统窗口接口
     * @class iwindow
     */
    bsPackage iwindow :public bs_object, public iwcontext
    {
        /// 获取窗口当前状态
        //eWindowStates
        int state;

        bsObject(iwindow)
        iwindow(iwindow *parent = NULL):bs_object(parent){}
        virtual ~iwindow(){}

        /// 获取是否隐藏，关闭，激活
        bool is_hide() const{return state & Window_Hide;}
        bool is_show()const {return state & Window_Show;}
        bool is_enable() const{return state & Window_Enable;}
        bool is_disable()const {return state & Window_Disable;}
        bool is_top()const {return state & Window_AlwaysTop;}
        bool is_full() const {return state & Window_FullScreen;}

        /// 全屏窗口
        virtual void fullscreen(bool s = true) {state |= (s ? Window_FullScreen: Window_Show);}
        /// 置顶窗口
        virtual void top(bool s = true) {state |= (s ? Window_AlwaysTop: Window_Show);}
        /// 关闭窗口
        /// 最小化
        virtual void minimum() {state |= Window_Minimum;}
        /// 最大化
        virtual void maximun(){state |= Window_Maximum;}
        /// 正常显示
        virtual void normal(){state |= Window_Show | Window_Enable;}

        /// 设置窗口是否为开启状态
        virtual void enable(bool s = true) {state |= (s ? Window_Enable: Window_Disable);}
        virtual void disable(bool s = true) {state |= (s ? Window_Disable: Window_Enable);}

        /// 设置窗口是否为隐藏状态
        virtual void hide(bool s = true) {state |= (s ? Window_Hide: Window_Show);}
        virtual void show(bool s = true) {state |= (s ? Window_Show: Window_Hide);}

        /// 窗口的尺寸
        virtual void size(const ivec2 &rect) {resize(rect);}
        virtual void resize(const ivec2 &rect) {geometry(ivec4(format.geometry.xy(), rect));}

        /// 窗口的位置
        virtual void pos(const ivec2 &p) {geometry(ivec4(p, format.geometry.zw ()));}
        virtual void move(const ivec2 &p) {geometry(ivec4(p, format.geometry.zw ()));}

        /// 窗口内的客户区域几何大小
        virtual void geometry(const ivec4 &rect) {format.geometry = rect;}
        virtual void title(const bs_string &til)  {format.title = til;}
    };

#define iWindow bs_ptr(iwindow)
    typedef iWindow p_window;
#undef iWindow

    //iplatform(const piwcontext &wctx);
    virtual ~iplatform(){}

    // window
    virtual int dispatch()  = 0;
    virtual void post(const tWindowNexus &evt) = 0;

    // monitor
    //!
    //! \brief change 更改监视器的设置信息
    //!
    virtual bool monitor_change(const tMonitorInofs &mi, bool is_fullscreen = false) = 0;
    virtual tMonitorInofs monitor(const int idx = 0) = 0;
    virtual int monitor_count()const = 0;

    // mouse
    virtual bool cursor_set(const bs_image &) = 0;
    virtual bs_image cursor_get() const= 0;
    virtual void cursor_set(eCursorTypes ct) = 0;
    virtual void cursor_show(bool state = true) = 0;
    virtual void cursor_position(const ivec2 &s) = 0;
    virtual ivec2 cursor_position() const = 0;
    virtual void cursor_event(const bs_event &e) = 0;

    //static p_window create();
};
/*#define iPlat bs_ptr(iplatform)
typedef iPlat piplatform;
#undef iPlat*/
typedef iplatform::p_window piwindow;


#endif // IWINDOW

