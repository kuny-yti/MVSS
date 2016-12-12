#ifndef GUI_DEFINE
#define GUI_DEFINE
#include "bs_define.h"
#include "tools/bs_string.h"
#include "bs_object.h"

#define Gui_Define(val, field) (val << field)
#define Gui_GetDef(val, field) (val >> field)
#define Gui_Field 16
#define Gui_MaskField 0xffff << Gui_Field
#define Ui_Field 24
#define Ui_MaskField 0xff << Ui_Field
#define UiExt_Field 8
#define UiExt_MaskField 0xff << UiExt_Field
#define UiState_Field 0
#define UiState_MaskField 0xff

/// GUI的类型
typedef enum
{
    Ui_Window = Gui_Define(0x01, Ui_Field),  //窗口类型
    Ui_Widget = Gui_Define(0x02, Ui_Field),  //窗口组件类型
    Ui_Layout = Gui_Define(0x04, Ui_Field),  //窗口布局类型
    Ui_Extension = Gui_Define(0x08, Ui_Field)
}eUiTypes;

/// GUI的状态
typedef enum
{
    Ui_Enter = Gui_Define(0x01, UiState_Field),       // 鼠标进入部件
    Ui_Hover = Gui_Define(0x02, UiState_Field),       // 悬浮状态(鼠标悬浮)
    Ui_Active = Gui_Define(0x03, UiState_Field),      // 激活状态(鼠标触发部件)
    Ui_Focus = Gui_Define(0x04, UiState_Field),       // 焦点状态(键盘选中部件，或鼠标选中后松开)
    Ui_Normal = Gui_Define(0x05, UiState_Field),      // 正常状态(部件默认状态)
    Ui_Freeze = Gui_Define(0x06, UiState_Field),      // 冻结状态(部件处于不可选状态)
    Ui_Drag = Gui_Define(0x07, UiState_Field),        // 选中拖动状态
    Ui_StateCount= Gui_Define(0x08, UiState_Field),

    Ui_Hide = Gui_Define(0x10, UiState_Field)         // 隐藏状态(部件处于隐藏状态)
}eUiStates;

/// GUI的部件
/// 可以加上扩展项，[eGuiTypes | eGuiExtTypes] 例如
///   1. Gui_PushButton 此时此按钮只有几何样式和Ui_ExtToggled触发方式
///   2. Gui_PushButton | Ui_ExtText 此时此按钮带有文本的按钮
///   3. Gui_PushButton | Ui_ExtImage 此时此按钮带有图像的按钮
typedef enum
{
    Gui_Window = Gui_Define(0x0001, Gui_Field),      //! 窗口
    Gui_PushButton = Gui_Define(0x0002, Gui_Field),  //! 按钮
    Gui_ToolButton = Gui_Define(0x0003, Gui_Field),  //! 工具按钮
    Gui_CheckButton = Gui_Define(0x0004, Gui_Field), //! 多选按钮
    Gui_RadioButton = Gui_Define(0x0005, Gui_Field), //! 单选按钮
    Gui_ToggleButton = Gui_Define(0x0006, Gui_Field),//! 开关按钮
    Gui_CircleButton = Gui_Define(0x0007, Gui_Field),//!

    Gui_MenuBar = Gui_Define(0x0008, Gui_Field),     //! 菜单条
    Gui_MenuAction = Gui_Define(0x0009, Gui_Field),  //! 菜单项

    Gui_SpinBox = Gui_Define(0x000a, Gui_Field),     //! 数字框
    Gui_ComboBox = Gui_Define(0x000b, Gui_Field),    //! 复选框
    Gui_TextEdit = Gui_Define(0x000c, Gui_Field),    //! 文本编辑
    Gui_ScrollBar = Gui_Define(0x000d, Gui_Field),   //! 滚动条
    Gui_Slider = Gui_Define(0x000e, Gui_Field),      //! 滑块
    Gui_SliderTwo = Gui_Define(0x000f, Gui_Field),   //! 双向滑块

    Gui_Label = Gui_Define(0x0010, Gui_Field),       //! 标签显示
    Gui_ProgressBar = Gui_Define(0x0011, Gui_Field), //! 进度条
    Gui_ImageView = Gui_Define(0x0012, Gui_Field),   //! 图像视图

    Gui_Layout = Gui_Define(0x0013, Gui_Field),       //! 进度条
    Gui_VisualBox = Gui_Define(0x0014, Gui_Field),    //! 可视化控件
    Gui_Analyzer = Gui_Define(0x0015, Gui_Field)
}eGuiTypes;

/// GUI的扩展类型
typedef enum
{
    Gui_ExtButton = Ui_Extension | Gui_Define(0x01, UiExt_Field),
    Gui_ExtRect = Ui_Extension | Gui_Define(0x02, UiExt_Field),
    Gui_ExtFan = Ui_Extension | Gui_Define(0x03, UiExt_Field),

    Ui_ExtTrigger = Gui_Define(0x04, UiExt_Field),
    Ui_ExtText = Gui_Define(0x05, UiExt_Field),
    Ui_ExtImage =  Gui_Define(0x06, UiExt_Field),
    Ui_ExtIcon =  Gui_Define(0x07, UiExt_Field),
}eGuiExtTypes;

typedef enum
{
    Spin_Integer,
    Spin_Double
}eSpinModes;

typedef enum
{
    Combo_Normal,
    Combo_Change,
    Combo_Select,
}eComboStates;

typedef enum
{
    Ui_Horizontal ,
    Ui_Vertical,
}eUiOrientations;

typedef enum
{
    Layout_Group, //组布局
    Layout_Grid,  //网格布局
    Layout_Box,   //横向布局
    Layout_Auto  //自动布局
}eLayoutModes;

typedef enum
{
    Align_Minimum,
    Align_Middle,
    Align_Maximum,
    Align_Fill
}eLayoutAligns;

//! 部件关系
typedef tObjectNexus tWidgetNexus;

//! 按钮
struct tButtons :public tWidgetNexus
{
    bool state; //! 状态
};
//! 工具按钮
struct tToolButtons :public tWidgetNexus
{
    bool state; //! 状态
    bs_string text; //! 字符串
};
//! 微调框
struct tSpinBoxs :public tWidgetNexus
{
    real range_min;//! 最小范围
    real range_max;//! 最大范围
    real step;     //! 步进值
    real value;    //! 当前值

    eSpinModes mode;
};
//! 组合框
struct tComboBoxs :public tWidgetNexus
{
    eComboStates state;
    int current; //! 被选中的
    int index;   //! 索引
    bs_string text; //! 索引对应的文本
};
//! 文本编辑框
struct tTextEdits :public tWidgetNexus
{
    bs_string text; //! 编辑的文本
};
typedef enum
{
    Scroll_Step,  //! 根据步进值滚动
    Scroll_Assign,//! 根据鼠标点击位置
    Scroll_Page  //! 根据当前的尺寸翻页

}eScrollModes;
//! 滚动条
struct tScrollBars :public tWidgetNexus
{
    //! 滚动的总高度或是总宽度
    float maxsize;
    eScrollModes mode;
    float step; //! 步进值
    //! 0-1
    float percent; //! 百分比
};
//! 滑块
struct tSliders :public tWidgetNexus
{
    real range_min;//! 最小范围
    real range_max;//! 最大范围
    real step;     //! 步进值
    real value;    //! 当前值
};
//! 进度条
struct tProgressBars :public tWidgetNexus
{
    //! 0-100
    float percent; //! 百分比
};

#endif // GUI_DEFINE

