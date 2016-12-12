#ifndef BACKSTAGE_H
#define BACKSTAGE_H

#include "bs_define.h"
#include "bs_ptr.h"
#include "bs_event.h"
#include "maths/bs_vec4.h"

/*!
 * @brief The eWindowStates enum 窗口的状态
 * @enum eWindowStates
 */
typedef enum
{
    Window_Enable =1,            //!< 启用
    Window_Disable= (~1),        //!< 禁用
    Window_Show = 1<< 1,         //!< 显示
    Window_Hide= ~(1<< 1),       //!< 隐藏
    Window_Minimum =1 << 2,      //!< 最小化
    Window_Maximum = ~(1 << 2),  //!< 最大化
    Window_StateNot
}eWindowStates;

/*!
 * @brief The eWindowEvents enum 窗口的事件
 * @enum eWindowStates
 */
typedef enum
{
    Window_Input = WindowEventDef(0),      //!< 输入
    Window_Initialize = WindowEventDef(1), //!< 初始化
    Window_Paint = WindowEventDef(2),      //!< 绘制
    Window_Resize = WindowEventDef(3),     //!< 窗口尺寸更新
    Window_Exit = WindowEventDef(4),       //!< 退出

    Window_EventNot
}eWindowEvents;
/*!
 * @brief The eWindowStyles enum 窗口的装饰样式
 * @enum eWindowStyles
 */
typedef enum
{
    Window_Rimless = 1 << 3,    //!< 窗口无边框
    Window_FullScreen = 1 << 4, //!< 窗口全屏
    Window_Resizable = 1 << 5,  //!< 窗口可调整
    Window_AlwaysTop = 1 << 6,  //!< 窗口置顶
    Window_MultiThread = 1 << 7,//!< 将窗口渲染部分分离到线程中,有事件触发时渲染
    Window_AlwaysRun = 1 << 8,  //!< 将窗口渲染分离到线程后一直渲染
    Window_StyleNot
}eWindowStyles;

/*!
 * @brief The eWindowEvents enum 窗口可使用的事件
 * @enum eWindowEvents
 */
typedef enum
{
    Window_InputOnly  = 1 ,      //!< 支持窗口事件,其他事件都不能用
    Window_Mouse      = 1 << 1,  //!< 支持鼠标事件
    Window_Keybord    = 1 << 2,  //!< 支持键盘事件
    Window_DragDrop   = 1 << 3,  //!< 支持拖放事件
    Window_Clipboard  = 1 << 4,  //!< 支持剪切板事件
    Window_MouseTrack = 1 << 5,  //!< 支持鼠标跟踪
    Window_InputOut   = 1 << 6 | Window_Clipboard | Window_DragDrop | Window_Keybord | Window_Mouse | Window_MouseTrack,
    Window_SupportEventNot
}eWindowSupportEvents;

/*!
 * @brief The tWindowNexus struct 窗口对象关系
 * @struct tWindowNexus
 */
typedef struct :tObjectNexus
{
    eWindowStates state;
    bs_event event;
}tWindowNexus;

/*!
 * @brief The tWindowFormats struct 窗口格式信息
 * @class tWindowFormats
 */
typedef struct _WindowFormat_
{
    ivec2              minisize;   ///< 窗口可调整最小尺寸
    ivec2              maxisize;   ///< 窗口可调整最大尺寸
    vec2               minaspect;  ///< 窗口调整的最小比例
    vec2               maxaspect;  ///< 窗口调整的最大比例
    ivec4              geometry;   ///< 窗口的区域
    bs_string          title;      ///< 窗口的标题
    int                style;      ///< 窗口的装饰样式
    int                event;      ///< 窗口可使用的事件

    _WindowFormat_():
        minisize(0,0),
        maxisize(65536, 65536),
        minaspect(4, 3),
        maxaspect(4, 3),
        geometry(0,0,1280,720),
        title(),
        style(Window_Resizable | Window_MultiThread | Window_AlwaysRun),
        event(Window_InputOut)
    {

    }
    bool is_valid()const
    {
        return geometry.width > 0 && geometry.height > 0;
    }
    bool operator !=(const _WindowFormat_ &rhs)const
    {
        return !((*this) == rhs);
    }
    bool operator ==(const _WindowFormat_ &rhs)const
    {
        return (rhs.geometry == geometry) &&
                (rhs.title == title) &&
                (rhs.style == style)&&
                (rhs.event ==  event) &&
                (rhs.maxaspect == maxaspect) &&
                (rhs.minaspect == minaspect) &&
                (rhs.maxisize == maxisize) &&
                (rhs.minisize == minisize);
    }
}tWindowFormats;

bsPackage ircontext;
bsPackage iwcontext;
#define iContext bs_ptr(iwcontext)
typedef iContext piwcontext;
#undef iContext
/*!
 * @brief The iwcontext class 窗口上下文
 * @class iwcontext
 */
bsPackage iwcontext
{
    winid              parents;     ///< 对于x11则为display, 对于win32则为本窗口的父

    winid              handles;     ///< 本窗口的句柄
    ircontext *        rcontext;    ///< 本窗口的关联的渲染上下文

    tWindowFormats     format;

    iwcontext()
    {
        parents = NULL;
        handles= NULL;
        rcontext = NULL;
    }
    virtual ~iwcontext(){}

    template<typename T>
    T parent() {return (T)parents;}
    template<typename T>
    T handle() {return (T)handles;}

    virtual bool is_valid()const {return parents && handles;}

    virtual bool create(ircontext *rctx, const tWindowFormats &wf) {rcontext = rctx; format = wf;return true;}
    virtual void close() {parents = NULL; handles = NULL; rcontext = NULL;}
    //!
    //! \brief dispatch 所有派遣和寄送的窗口事件。
    //! \return
    //!
    virtual int dispatch() = 0;
    virtual void post(const tWindowNexus &evt) = 0;
};

#endif // BACKSTAGE_H
