#ifndef IWIDGET_H
#define IWIDGET_H

#include "guidef.h"
#include "tools/bs_string.h"
#include "tools/bs_vector.h"
#include "tools/bs_image.h"
#include "bs_event.h"
#include "maths/bs_mat4X4.h"
#include "tools/bs_datetime.h"
#include "bs_ptr.h"

#define UiDebugDump

#define UiEventClickLimt 250 //! 两次点击在250ms内认为是双击
#define UiEventHoverLimt 500//! 鼠标移到到部件上停留500ms认为是悬浮状态
#define UiEventMoveSize 15 //! 窗口部件移动选择的大小

class ui_font;
class ui_theme;

bsPackage iwidget;
#define IWid bs_ptr(iwidget)
typedef IWid piwidget;
#undef IWid

typedef enum
{
    Dump_Not,
    Dump_BoundingBox = 1,
    Dump_Coord = 2,
    Dump_Event = 4
}eUiDebugDumps;

/*!
 * \brief The tWidgetManages struct 部件的管理器
 */
typedef struct tWidgetManages
{
    friend bsPackage iwidget;

    bs_list<iwidget *> root;      //!< 根节点
    iwidget *          focus;     //!< 当前焦点部件
    iwidget *          hover;     //!< 当前鼠标所在的部件
    iwidget *          active;    //!< 当前鼠标点击的部件
    iwidget *          enter;     //!< 当前鼠标进入的部件
    iwidget *          ui_parent; //!< 当前所在的父窗口
    iwidget *          leave;     //!< 离开的部件
    iwidget *          top;       //!< 被置顶的部件-在root根节点内不能记录被置顶的部件

    int dump;                     //!< eUiDebugDumps 调试

    //!
    //! \brief dispatch 全局GUI事件派遣
    //! \param evt
    //! \return
    //!
    bool dispatch(const bs_event &evt);

    //!
    //! \brief render 全局渲染GUI对象
    //! \param width
    //! \param height
    //!
    void render(float width, float height);

protected:
    //!
    //! \brief sqrt 排序根节点，用于排列选中的先后顺序
    //! \param ui
    //!
    void sqrt(iwidget *ui);
    //!
    //! \brief append 向根节点添加父部件
    //! \param p
    //! \return
    //!
    bool append(iwidget *p);
    //!
    //! \brief remove 向根节点删除父部件
    //! \param oui
    //! \return
    //!
    bool remove(iwidget *oui);

private:
    //!
    //! \brief paint 主调用，主要内部处理绝对位置和关联部件几何信息修改
    //! \param ui 需要绘制的ui对象
    //! \param base ui对象绘制的基础数据
    //! \param width root 的尺寸
    //! \param height
    //!
    void paint (iwidget *ui, float width, float height);

    //!
    //! \brief posting 处理部件事件和子部件事件分发
    //! \param gui 需要派遣的ui
    //! \param evt
    //! \return
    //!
    bool posting(iwidget *gui, const bs_event &evt);
}tWidgetManages;

typedef enum
{
    Icon_Font,
    Icon_Image
}eWidgetIconTypes;
/*!
 * \brief The tWidgetIcons struct 部件的图标对象
 */
typedef struct tWidgetIcons
{
    eWidgetIconTypes type;
    bs_image         image;
    uint             code;
}tWidgetIcons;

/*!
 * \brief The tWidgetBackstages struct 部件的后台数据
 */
typedef struct tWidgetBackstages
{
    uint            is_maximun:1;    //!< 窗口是否最大化
    uint            is_minimun:1;    //!< 窗口是否最小化
    uint            is_always_top:1; //!< 窗口是否置顶

    uint            click:2;         //!< 鼠标在部件上点击次数
    int             state;           //!< 部件的当前状态
    int             type;            //!< 部件的类型
    eUiOrientations orientaions;     //!< 部件的方向

    enum {Geom, Geom_Abs, Geom_Backup, Geom_Count};
    frect           geometrys[Geom_Count];//!< 部件的几何信息
    fsize           minsizes;        //!< 部件的最小尺寸
    fsize           maxsizes;        //!< 部件的最大尺寸

    bs_string       titles;          //!< 部件的标题
    bs_string       tooltips;        //!< 部件的提示
    bs_image        images;          //!< 部件的图像
    tWidgetIcons    icons;           //!< 部件的图标
    ui_font*        fonts;
    ui_theme*       themes;          //!< 部件的主题

    fpoint          location;        //! 子部件布局开始的默认位置
    bs_event        last_event;      //! 上次所触发事件

    tWidgetManages *manage;


    //!
    //! \brief save 部件的信息保存到文件中
    //!
    virtual void save(const bs_string &){}
    //!
    //! \brief load 从文件中加载部件信息
    //!
    virtual void load(const bs_string &) {}
    //!
    //! \brief serialize 部件的信息序列化
    //! \return
    //!
    virtual bs_string serialize()const {return bs_string();}
    //!
    //! \brief serialize 序列化中设置部件的信息
    //!
    virtual void serialize(const bs_string &) {}

}tWidgetBackstages;


#define uiWidgetDefine(ui) bsObject(ui)
/*!
 * \brief The iwidget struct 图形用户接口-部件基类
 */
bsPackage iwidget :public bs_object, public tWidgetBackstages
{
    friend struct tWidgetManages;

    uiWidgetDefine(iwidget)
    explicit iwidget (iwidget *parent = NULL):
            bs_object(parent)
    {
        type = Ui_Widget;
        state = Ui_Normal;
        maxsizes = (fsize(65536, 65536));
        minsizes = (fsize(0, 0));
        is_maximun = false;
        is_minimun = false;
        is_always_top = false;
        click = 0;
        themes = NULL;
        manage = NULL;
        fonts = NULL;
    }
    virtual ~iwidget(){}

    //!
    //! \brief is_freeze 是否为冻结状态
    //! \return
    //!
    bool is_freeze() const{return state  == Ui_Freeze;}
    //!
    //! \brief is_hide 是否为隐藏状态
    //! \return
    //!
    bool is_hide() const{return state  == Ui_Hide;}
    //!
    //! \brief is_thaw 是否是解冻状态
    //! \return
    //!
    bool is_thaw() const{return !is_freeze();}
    //!
    //! \brief is_show 是否是显示状态
    //! \return
    //!
    bool is_show() const {return !is_hide();}
    //!
    //! \brief is_visible 是否可见
    //! \return
    //!
    bool is_visible()const{return is_show();}

    //!
    //! \brief pos 返回几何位置
    //! \return
    //!
    fpoint pos()const{return geometrys[Geom].xy ();}
    //!
    //! \brief size 返回几何尺寸
    //! \return
    //!
    fsize size()const{return geometrys[Geom].zw ();}

    //! 对窗口部件有用
    //!
    //! \brief close 关闭窗口
    //!
    virtual void close()  {}
    //!
    //! \brief minimum 最小化窗口
    //!
    virtual void minimum(){is_minimun = true; hide();}
    //!
    //! \brief maximun 最大化窗口
    //!
    virtual void maximun()
    {
        if (!this->is_maximun)
        {
            geometrys[Geom_Backup] = geometrys[Geom_Abs];
            is_maximun= true;
        }
        else
        {
            normal();
            is_maximun = false;
        }
    }
    //!
    //! \brief normal 正常显示窗口
    //!
    virtual void normal()
    {
        if (this->is_maximun)
        {
            geometrys[Geom_Abs] = geometrys[Geom_Backup];
            this->is_maximun= false;
        }
    }

    //! 正常显示还是冻结显示
    //!
    //! \brief thaw 解冻状态
    //! \param is
    //!
    virtual void thaw(bool is = true)
    {
        state = is ? Ui_Normal : Ui_Freeze;
    }
    //!
    //! \brief freeze 冻结状态
    //! \param is
    //!
    virtual void freeze(bool is = true)
    {
        state = is ? Ui_Freeze : Ui_Normal;
    }

    //! 显示部件还是隐藏部件
    //!
    //! \brief hide  隐藏状态
    //! \param is
    //!
    virtual void hide(bool is = true)
    {
        state = is ? Ui_Hide : Ui_Normal;
    }
    //!
    //! \brief show 显示窗口
    //! \param is
    //!
    virtual void show(bool is = true)
    {
        state = is ? Ui_Normal : Ui_Hide;
    }

    //! 调整部件
    //!
    //! \brief move 移动窗口
    //! \param val
    //!
    virtual void move(const fpoint &val){geometrys[Geom].xy ()+= val;}
    //!
    //! \brief resize 重新设置窗口大小
    //! \param val
    //!
    virtual void resize(const fsize &val)
    {
        if (minsizes > val)
            geometrys[Geom].zw () = minsizes;
        else if (maxsizes < val)
            geometrys[Geom].zw () = maxsizes;
        else
            geometrys[Geom].zw () = val;
    }

    //! 设置部件
    virtual void pos(const fpoint &p){geometrys[Geom].xy () = p;}
    virtual void size(const fsize &p){geometrys[Geom].zw () = p;}

    //! \brief 部件几何设置
    virtual void geometry(const frect &geom){geometrys[Geom] = geom;}

    //! \brief 部件最小、最大大小设置
    virtual void minsize(const fsize & ms){minsizes = ms;}
    virtual void maxsize(const fsize & ms){maxsizes = ms;}

    //! \brief 部件标题
    virtual void title( const bs_string & stl){titles = stl;}
    virtual void image(const bs_image &img){images = img;}
    virtual void icon(const tWidgetIcons &icon){icons = icon;}
    virtual void theme(const ui_theme *f){themes = (ui_theme *)f;}
    virtual void font(const ui_font *f){fonts = (ui_font*)f;}

    //! \brief 部件提示信息
    virtual void tooltip(const bs_string & stl){tooltips = stl;}

    //! \brief 部件置顶
    virtual void top( const bool & stl){is_always_top = stl;}

    virtual void orientaion( const eUiOrientations & uior){orientaions = uior;}

public:
    //!
    //! \brief paint 继承后完成部件的绘制
    //!
    virtual void paint() = 0;
    //!
    //! \brief event 继承后完成部件的事件响应
    //! \return
    //!
    virtual bool event(const bs_event &) = 0;
    //!
    //! \brief fit 返回部件的最适合的尺寸
    //! \return
    //!
    virtual fsize fit()const = 0;

    //!
    //! \brief dump
    //!
    virtual void dump() {}

    //!
    //! \brief contains 给定位置是否在部件内部
    //! \param p
    //! \return
    //!
    bool contains(const fpoint &p){return ::contains (geometrys[Geom_Abs], p);}

    frect absolute()const{return geometrys[Geom_Abs];}
};

#endif // IGUI_H

