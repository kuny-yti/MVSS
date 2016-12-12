#ifndef EVENT_H
#define EVENT_H
#include "interface/ievent.h"
#include "tools/bs_string.h"


/* 状态事件占用的段定义 */
#define State_Field 20   //20 ~ 24
/* 状态事件位宽 */
#define State_Wide 0x0f
/* 状态事件定义宏 */
#define State_EventDef(val) (val << State_Field)
/* 状态事件掩码段定义 */
#define State_MaskField (State_Wide << State_Field)

/* 修饰按键事件占用的段定义 */
#define Modifier_Field 16
/* 修饰按键事件位宽 */
#define Modifier_Wide 0x0f
/* 修饰按键事件定义宏 */
#define Modifier_EventDef(val) (val << Modifier_Field)
/* 修饰按键事件掩码段定义 */
#define Modifier_MaskField (Modifier_Wide << State_Field)

/* 键盘按键事件占用的段定义 */
#define Keybord_Field 0
/* 键盘按键事件位宽 */
#define Keybord_Wide 0x1ff
/* 键盘按键事件定义宏 */
#define Keybord_EventDef(val) (KeybordEventDef(val << Modifier_Field))
/* 键盘按键事件掩码段定义 */
#define Keybord_MaskField (Modifier_Wide << Keybord_Field)

/* 鼠标按键事件占用的段定义 */
#define Mouse_Field 0
/* 鼠标按键事件位宽 */
#define Mouse_Wide 0x0f
/* 鼠标按键事件定义宏 */
#define Mouse_EventDef(val) (MouseEventDef(val << Modifier_Field))
/* 鼠标按键事件掩码段定义 */
#define Mouse_MaskField (Modifier_Wide << Keybord_Field)


/*!
 * @brief The eEventStates enum
 * @enum eEventStates
 */
typedef enum
{
    Event_StateNot = State_EventDef(0),

    Event_StateIm = State_EventDef(1 << 1),        ///< 输入法
    Event_StateDown = State_EventDef(1 << 2),      ///< 按下
    Event_StateUp = State_EventDef(1 << 3),        ///< 弹起
    Event_StateImDown = Event_StateIm | Event_StateDown,
    Event_StateImUp = Event_StateIm | Event_StateUp,
    Event_StateMove= State_EventDef(1 << 4)        ///< 移动

}eEventStates;
/*!
 * @brief The eEventMouses enum
 * @enum eEventMouses
 */
typedef enum
{
    Event_MouseNot = Mouse_EventDef(0),

    Event_MouseLeft = Mouse_EventDef(1 << 1),  ///左键
    Event_MouseRight = Mouse_EventDef(1 << 2),   ///右键
    Event_MouseMiddle = Mouse_EventDef(1 << 3),  ///中键
    Event_MouseWheel = Mouse_EventDef(1 << 4)    ///滚轮
}eEventMouses;
/*!
 * @brief The eEventModifiers enum
 * @enum eEventModifiers
 */
typedef enum
{
    Event_ModifierNot = Modifier_EventDef(0),

    Event_ModifierShift = Modifier_EventDef(1<<1),
    Event_ModifierControl = Modifier_EventDef(1<<2),
    Event_ModifierAlt = Modifier_EventDef(1<<3),
    Event_ModifierMeta = Modifier_EventDef(1<<4),
    Event_ModifierSuper = Event_ModifierMeta
}eEventModifiers;

/*
0	NUT	32	(space)	64	@	96	、
1	SOH	33	!	65	A	97	a
2	STX	34	"	66	B	98	b
3	ETX	35	#	67	C	99	c
4	EOT	36	$	68	D	100	d
5	ENQ	37	%	69	E	101	e
6	ACK	38	&	70	F	102	f
7	BEL	39	,	71	G	103	g
8	BS	40	(	72	H	104	h
9	HT	41	)	73	I	105	i
10	LF	42	*	74	J	106	j
11	VT	43	+	75	K	107	k
12	FF	44	,	76	L	108	l
13	CR	45	-	77	M	109	m
14	SO	46	.	78	N	110	n
15	SI	47	/	79	O	111	o
16	DLE	48	0	80	P	112	p
17	DCI	49	1	81	Q	113	q
18	DC2	50	2	82	R	114	r
19	DC3	51	3	83	S	115	s
20	DC4	52	4	84	T	116	t
21	NAK	53	5	85	U	117	u
22	SYN	54	6	86	V	118	v
23	TB	55	7	87	W	119	w
24	CAN	56	8	88	X	120	x
25	EM	57	9	89	Y	121	y
26	SUB	58	:	90	Z	122	z
27	ESC	59	;	91	[	123	{
28	FS	60	<	92	/	124	|
29	GS	61	=	93	]	125	}
30	RS	62	>	94	^	126	`
31	US	63	?	95	_	127	DEL
*/
//键盘键码
typedef enum
{
    Event_KeyNot =0,

    Event_KeySpace = 0x20,   // "Space" 空格键
    Event_KeyExclam,         // "!" 感叹号
    Event_KeyQuoteDbl,       // """ 双引号
    Event_KeyNumberSign,     // "#" 井号
    Event_KeyDollar ,        // "$" 美元号
    Event_KeyPercent ,       // "%" 百分号
    Event_KeyAmpersand ,     // "&" 号
    Event_KeyApostrophe,     // "'" 单引号-闭单引号
    Event_KeyParenLeft ,     // "(" 左小括号
    Event_KeyParenRight,     // ")" 右小括号
    Event_KeyAsterisk,       // "*" 星号
    Event_KeyMultiply = Event_KeyAsterisk,
    Event_KeyPlus,           // "+" 加号
    Event_KeyAdd = Event_KeyPlus,
    Event_KeyComma ,         // "," 逗号
    Event_KeyMinus ,         // "-" 减号
    Event_KeySubtract = Event_KeyMinus,
    Event_KeyPeriod ,        // "." 句号
    Event_KeyDecimal = Event_KeyPeriod,
    Event_KeySlash,          // "/" 斜杠
    Event_KeyDivide = Event_KeySlash,

    Event_Key0 = 0x30,        // 数字键
    Event_Key1 = 0x31,
    Event_Key2 = 0x32,
    Event_Key3 = 0x33,
    Event_Key4 = 0x34,
    Event_Key5 = 0x35,
    Event_Key6 = 0x36,
    Event_Key7 = 0x37,
    Event_Key8 = 0x38,
    Event_Key9 = 0x39,
    Event_KeyColon ,     // ":" 冒号
    Event_KeySemicolon , // ";" 分号
    Event_KeyLess ,      // "<" 小于号
    Event_KeyEqual ,     // "=" 等号
    Event_KeyGreater ,   // ">" 大于号
    Event_KeyQuestion ,  // "?" 问号
    Event_KeyAt ,        // "@" 号
    Event_KeyA = 0x41,         // 字母键
    Event_KeyB = 0x42,
    Event_KeyC = 0x43,
    Event_KeyD = 0x44,
    Event_KeyE = 0x45,
    Event_KeyF = 0x46,
    Event_KeyG = 0x47,
    Event_KeyH = 0x48,
    Event_KeyI = 0x49,
    Event_KeyJ = 0x4a,
    Event_KeyK = 0x4b,
    Event_KeyL = 0x4c,
    Event_KeyM = 0x4d,
    Event_KeyN = 0x4e,
    Event_KeyO = 0x4f,
    Event_KeyP = 0x50,
    Event_KeyQ = 0x51,
    Event_KeyR = 0x52,
    Event_KeyS = 0x53,
    Event_KeyT = 0x54,
    Event_KeyU = 0x55,
    Event_KeyV = 0x56,
    Event_KeyW = 0x57,
    Event_KeyX = 0x58,
    Event_KeyY = 0x59,
    Event_KeyZ = 0x5a,
    Event_KeyBracketsLeft, // "[" 左方括号
    Event_KeyBackslash,    // "\" 反斜杠
    Event_KeyBracketsRight,// "]" 右方括号
    Event_KeyCircum,       // "^" 脱字符 AsciiCircum
    Event_KeyUnderscore,   // "_" 下划线
    Event_KeyGraveAccent,  // "`" 键盘左上角那个点-开单引号

    Event_Keya = 0x61,         // 字母键
    Event_Keyb = 0x62,
    Event_Keyc = 0x63,
    Event_Keyd = 0x64,
    Event_Keye = 0x65,
    Event_Keyf = 0x66,
    Event_Keyg = 0x67,
    Event_Keyh = 0x68,
    Event_Keyi = 0x69,
    Event_Keyj = 0x6a,
    Event_Keyk = 0x6b,
    Event_Keyl = 0x6c,
    Event_Keym = 0x6d,
    Event_Keyn = 0x6e,
    Event_Keyo = 0x6f,
    Event_Keyp = 0x70,
    Event_Keyq = 0x71,
    Event_Keyr = 0x72,
    Event_Keys = 0x73,
    Event_Keyt = 0x74,
    Event_Keyu = 0x75,
    Event_Keyv = 0x76,
    Event_Keyw = 0x77,
    Event_Keyx = 0x78,
    Event_Keyy = 0x79,
    Event_Keyz = 0x7a,

    Event_KeyBraceLeft,    // "{" 左花括号
    Event_KeyBar,          // "|" 竖线
    Event_KeyBraceRight ,  // "}" 右花括号
    Event_KeyTilde,   // "~" 波浪号

    Event_KeyF1 = 0x100,                // 功能键
    Event_KeyF2 ,
    Event_KeyF3 ,
    Event_KeyF4 ,
    Event_KeyF5 ,
    Event_KeyF6,
    Event_KeyF7,
    Event_KeyF8,
    Event_KeyF9 ,
    Event_KeyF10,
    Event_KeyF11,
    Event_KeyF12,

    Event_KeyEscape,       // "Esc" Esc键
    Event_KeyTab ,         // "Tap" Tap键
    Event_KeyBacktab,      // "Backtab" 退格键
    Event_KeyBackspace,    // "Backspace" 退格键
    Event_KeyReturn,       // Event_KeyEnter
    Event_KeyEnter = Event_KeyReturn,        // "Enter" 回车键
    Event_KeyInsert,       // "Insert" 插入键
    Event_KeyDelete,       // "Delete" 删除键
    Event_KeyPause,        // "Pause/Break" Pause/Break键
    Event_KeyPrint,        // "PrintScreen" 截屏键
    Event_KeySysReq,       // Event_KeyPrint
    Event_KeyClear,        // 清除键
    Event_KeyHome ,        // "Home" 起始键
    Event_KeyEnd,          // "End" 结尾键
    Event_KeyLeft ,        // "←" 左箭头
    Event_KeyUp,           // "↑" 上箭头
    Event_KeyRight,        // "→" 右箭头
    Event_KeyDown,         // "↓" 下箭头
    Event_KeyPageUp ,      // "PageUp" 向上翻页键
    Event_KeyPageDown,     // "PageDown" 向下翻页键
    Event_KeyShift,        // "Shift" 上档转换键
    Event_KeyShiftLeft,    // "Shift" 左上档转换键
    Event_KeyShiftRight,   // "Shift" 右上档转换键
    Event_KeyControl,      // "Ctrl" 键
    Event_KeyControlLeft,  // "Ctrl" 左键
    Event_KeyControlRight, // "Ctrl" 右键
    Event_KeyMeta,
    Event_KeyMetaLeft,
    Event_KeyMetaRight,
    Event_KeySuper,
    Event_KeySuperLeft,
    Event_KeySuperRight,
    Event_KeyAlt,          // "Alt" 交替换档键
    Event_KeyAltLeft,      // "Alt" 左交替换档键
    Event_KeyAltRight,     // "Alt" 右交替换档键
    Event_KeyCapsLock,     // "CapsLock" 大小写锁定键
    Event_KeyNumLock ,     // "NumLock" 小键盘锁定键
    Event_KeyScrollLock ,  // "ScrollLock" 滚动锁定键

    Event_KeyMenu ,        //  菜单键
    Event_KeyHelp,         //  帮助键

    Event_KeyFN              // "FN" 扩展键
}eEventKeys;

//浏览器键码
typedef enum
{
    Event_KeyBack,       //  Browser Back key
    Event_KeyForward,    //  Browser Forward key
    Event_KeyRefresh,    //  Browser Refresh key
    Event_KeyStop,       //  Browser Stop key
    Event_KeySearch,     //  Browser Search key
    Event_KeyFavorites,  //  Browser Favorites key
    Event_KeyHomePage,   //  Browser Start and Home key
}eEventKeyBrowsers;

//多媒体键码
typedef enum
{
    Event_KeyVolumeMute, //  Volume Mute key
    Event_KeyVolumeDown, //  Volume Down key
    Event_KeyVolumeUp,   //  Volume Up key
    Event_KeyMediaNext,  //  Next Track key
    Event_KeyMediaPrev, // Previous Track key
    Event_KeyMediaStop,  //  Stop Media key
    Event_KeyMediaPlay,  //  Play/Pause Media key
}eEventKeyMedias;

//其他键码
typedef enum
{
    Event_KeySleep,          //  95   0x5F   VK_SLEEP
    Event_KeyMediaPause,     //  47   0x2f   APPCOMMAND_MEDIA_PAUSE
    Event_KeyMediaRecord,    //  48   0x30   APPCOMMAND_MEDIA_RECORD
    Event_KeyAudioForward,   //  49   0x31   APPCOMMAND_MEDIA_FAST_FORWARD
    Event_KeyAudioRewind,    //  50   0x32   APPCOMMAND_MEDIA_REWIND
    Event_KeyChannelDown,    //  51   0x33   APPCOMMAND_MEDIA_CHANNEL_DOWN
    Event_KeyChannelUp,      //  52   0x34   APPCOMMAND_MEDIA_CHANNEL_UP
    Event_KeyUndo,           //  34   0x22   APPCOMMAND_UNDO
    Event_KeyRedo,           //  35   0x23   APPCOMMAND_REDO
    Event_KeyCopy,           //  36   0x24   APPCOMMAND_COPY
    Event_KeyCut,            //  37   0x25   APPCOMMAND_CUT
    Event_KeyPaste,          //  38   0x26   APPCOMMAND_PASTE
    Event_KeyReply,          //  39   0x27   APPCOMMAND_REPLY_TO_MAIL
    Event_KeyMailForward,    //  40   0x28   APPCOMMAND_FORWARD_MAIL
    Event_KeySend,           //  41   0x29   APPCOMMAND_SEND_MAIL
    Event_KeySpell,          //  42   0x2a   APPCOMMAND_SPELL_CHECK
    Event_KeyFind,           //  28   0x1c   APPCOMMAND_FIND
    Event_KeyNew,            //  29   0x1d   APPCOMMAND_NEW
    Event_KeyOpen,           //  30   0x1e   APPCOMMAND_OPEN
    Event_KeyClose,          //  31   0x1f   APPCOMMAND_CLOSE
    Event_KeySave,           //  32   0x20   APPCOMMAND_SAVE
    Event_KeyBassDown,       //  19   0x13   APPCOMMAND_BASS_DOWN
    Event_KeyBassBoost,      //  20   0x14   APPCOMMAND_BASS_BOOST
    Event_KeyBassUp,         //  21   0x15   APPCOMMAND_BASS_UP
    Event_KeyTrebleDown,     //  22   0x16   APPCOMMAND_TREBLE_DOWN
    Event_KeyTrebleUp,       //  23   0x17   APPCOMMAND_TREBLE_UP
    Event_KeyMicMute,        //  24   0x18   APPCOMMAND_MICROPHONE_VOLUME_MUTE
    Event_KeyMicVolumeDown,  //  25   0x19   APPCOMMAND_MICROPHONE_VOLUME_DOWN
    Event_KeyMicVolumeUp,    //  26   0x1a   APPCOMMAND_MICROPHONE_VOLUME_UP
    Event_KeyMediaTogglePlayPause,   //  14   0x0e   APPCOMMAND_MEDIA_PLAYPAUSE

    Event_KeyCancel,     //   Control-Break processing
    Event_KeyModeSwitch, //   IME mode change request
    Event_KeySelect,     //   Select key
    Event_KeyPrinter,    //   Print key
    Event_KeyExecute,    //   Execute key
    Event_KeyMassyo,     //  'Unregister word' key
    Event_KeyTouroku,    //  'Register word' key
    Event_KeyLaunchMail, //  Start Mail key
    Event_KeyLaunchMedia,//  Select Media key
    Event_KeyLaunch0,    //  Start Application 1 key
    Event_KeyLaunch1,    //  Start Application 2 key
    Event_KeyPlay,       //  Play key
    Event_KeyZoom,       //  Zoom key
}eEventKeyOthers;

class bs_event : public ievent
{
public:
    bs_event(): ievent(), _detle(0), _pos_x(0), _pos_y(0) {}

    //! 构建窗口事件
    explicit bs_event(int evt): ievent(evt),_detle(0), _pos_x(0), _pos_y(0) {}

    explicit bs_event(_in_ bs_event &e): ievent(e.code ()), _detle(e.wheel()),
        _pos_x(e.x()), _pos_y(e.y()) {timestamp () = e.timestamp();}

    //! 构建键盘事件
    explicit bs_event(int key, eEventModifiers modif, int64 ts):
        ievent(KeybordEventDef(key) | modif),
        _detle(0), _pos_x(0), _pos_y(0) {timestamp () = ts;}

    //! 构建鼠标事件
    explicit bs_event(int mouse, int m_x,int m_y,
                      eEventModifiers modif, int64 ts):
        ievent(MouseEventDef(mouse) | modif),
        _detle(0), _pos_x(m_x), _pos_y(m_y) {timestamp () = ts;}

    //! 构建鼠标滚轮事件
    explicit bs_event(int mouse, int m_w, eEventModifiers modif, int64 ts):
        ievent(MouseEventDef(mouse) | modif),
        _detle(m_w), _pos_x(0), _pos_y(0) {timestamp () = ts;}

    int &wheel() {return _detle;}
    float &x() {return _pos_x;}
    float &y() {return _pos_y;}

    int wheel()_fix_ { return _detle;}
    float x()_fix_ {return _pos_x;}
    float y()_fix_ {return _pos_y;}
    int button()_fix_ {return (code() & Mouse_MaskField) | (code() & State_MaskField);}
    int key()_fix_ {return (code() & Keybord_MaskField) | (code() & State_MaskField);}
    int modifier()_fix_ {return code() & Modifier_MaskField;}

    bool operator == (const bs_event &Rhs)
    {
        return (this->code () == Rhs.code ()) &&
                (this->x () == Rhs.x ()) &&
                (this->y () == Rhs.y ()) &&
                (this->timestamp () == Rhs.timestamp ()) &&
                (this->wheel () == Rhs.wheel ());
    }
    bool operator != (const bs_event &Rhs)
    {
        return !((*this) == Rhs);
    }

    inline bs_string dump()const;

protected:
    int   _detle;   ///< 滚轮值
    float _pos_x;   ///< 鼠标位置
    float _pos_y;
};

inline bs_string bs_event::dump()const
{
    static struct
    {
        int code;
        const char * const str;
    }sDumpStateStr [] =
    {
    {Event_StateDown, "Down"},
    {Event_StateUp, "Up"},
    {Event_StateMove, "Move"},

    {Event_StateNot, "Not"}
};
    static struct
    {
        int code;
        const char * const str;
    }sDumpMouseStr [] =
    {
    {Event_MouseLeft , "Left"},
    {Event_MouseRight , "Right"},
    {Event_MouseMiddle , "Middle"},
    {Event_MouseWheel , "Wheel"},

    {Event_MouseNot , "Not"}
};
    static struct
    {
        int code;
        const char * const str;
    }sDumpModifierStr [] =
    {
    {Event_ModifierShift , "Shift"},
    {Event_ModifierControl , "Control"},
    {Event_ModifierAlt , "Alt"},
    {Event_ModifierMeta ,"Meta"},

    {Event_ModifierNot ,"Not"}
};
    static struct
    {
        int code;
        const char * const str;
    }sDumpKeyStr [] =
    {
    {Event_KeySpace , "space"},
    {Event_KeyExclam, "!"},
    {Event_KeyQuoteDbl, "\""},       // """ 双引号
    {Event_KeyNumberSign,"#"},     // "#" 井号
    {Event_KeyDollar ,"$"},        // "$" 美元号
    {Event_KeyPercent ,"%"},       // "%" 百分号
    {Event_KeyAmpersand ,"&"},     // "&" 号
    {Event_KeyApostrophe,"'"},     // "'" 单引号
    {Event_KeyParenLeft , "("},    // "(" 左小括号
    {Event_KeyParenRight,  ")"},   // ")" 右小括号
    {Event_KeyAsterisk,   "*"},    // "*" 星号
    {Event_KeyPlus, "+"},          // "+" 加号
    {Event_KeyComma ,","},         // "," 逗号
    {Event_KeyMinus , "-"},        // "-" 减号
    {Event_KeyPeriod , "."},       // "." 句号
    {Event_KeySlash, "/"},         // "/" 斜杠

    {Event_Key0 ,"0"},        // 数字键
    {Event_Key1 ,"1"},
    {Event_Key2 ,"2"},
    {Event_Key3 ,"3"},
    {Event_Key4 ,"4"},
    {Event_Key5 ,"5"},
    {Event_Key6 ,"6"},
    {Event_Key7 ,"7"},
    {Event_Key8 ,"8"},
    {Event_Key9 ,"9"},
    {Event_KeyColon ,":"},     // ":" 冒号
    {Event_KeySemicolon ,";"},// ";" 分号
    {Event_KeyLess ,"<"},     // "<" 小于号
    {Event_KeyEqual ,"="},     // "=" 等号
    {Event_KeyGreater,">"},   // ">" 大于号
    {Event_KeyQuestion ,"?"},  // "?" 问号
    {Event_KeyAt ,"@"},       // "@" 号
    {Event_KeyA ,"A"},         // 字母键
    {Event_KeyB ,"B"},
    {Event_KeyC ,"C"},
    {Event_KeyD ,"D"}, //44,
    {Event_KeyE ,"E"}, //45,
    {Event_KeyF ,"F"}, //46,
    {Event_KeyG ,"G"}, //47,
    {Event_KeyH ,"H"}, //48,
    {Event_KeyI ,"I"}, //49,
    {Event_KeyJ ,"J"}, //4a,
    {Event_KeyK ,"K"}, //4b,
    {Event_KeyL ,"L"}, //4c,
    {Event_KeyM ,"M"}, //4d,
    {Event_KeyN ,"N"}, //4e,
    {Event_KeyO ,"O"}, //4f,
    {Event_KeyP ,"P"}, //50,
    {Event_KeyQ ,"Q"}, //51,
    {Event_KeyR ,"R"}, //52,
    {Event_KeyS ,"S"}, //53,
    {Event_KeyT ,"T"}, //54,
    {Event_KeyU ,"U"}, //55,
    {Event_KeyV ,"V"}, //56,
    {Event_KeyW ,"W"}, //57,
    {Event_KeyX ,"X"}, //58,
    {Event_KeyY ,"Y"}, //59,
    {Event_KeyZ ,"Z"}, //5a,
    {Event_KeyBracketsLeft,"["}, // "[" 左方括号
    {Event_KeyBackslash,"\\"},    // "\" 反斜杠
    {Event_KeyBracketsRight,"]"},// "]" 右方括号

    {Event_Keya ,"a"},         // 字母键
    {Event_Keyb ,"b"},
    {Event_Keyc ,"c"},
    {Event_Keyd ,"d"}, //44,
    {Event_Keye ,"e"}, //45,
    {Event_Keyf ,"f"}, //46,
    {Event_Keyg ,"g"}, //47,
    {Event_Keyh ,"h"}, //48,
    {Event_Keyi ,"i"}, //49,
    {Event_Keyj ,"j"}, //4a,
    {Event_Keyk ,"k"}, //4b,
    {Event_Keyl ,"l"}, //4c,
    {Event_Keym ,"m"}, //4d,
    {Event_Keyn ,"n"}, //4e,
    {Event_Keyo ,"o"}, //4f,
    {Event_Keyp ,"p"}, //50,
    {Event_Keyq ,"q"}, //51,
    {Event_Keyr ,"r"}, //52,
    {Event_Keys ,"s"}, //53,
    {Event_Keyt ,"t"}, //54,
    {Event_Keyu ,"u"}, //55,
    {Event_Keyv ,"v"}, //56,
    {Event_Keyw ,"w"}, //57,
    {Event_Keyx ,"x"}, //58,
    {Event_Keyy ,"y"}, //59,
    {Event_Keyz ,"z"}, //5a,

    {Event_KeyCircum,"^"},  // // "^" 次方号
    {Event_KeyUnderscore,"_"},   // "_" 下划线

    {Event_KeyGraveAccent,"`" },  // "`" 键盘左上角那个点
    {Event_KeyBraceLeft,"{"},    // "{" 左花括号
    {Event_KeyBar,"|"},          // "|" 竖线
    {Event_KeyBraceRight ,"}"},  // "}" 右花括号
    {Event_KeyTilde,"~"},   // "~" 波浪号

    {Event_KeyF1, "F1"},                // 功能键
    {Event_KeyF2 ,"F2"},
    {Event_KeyF3 ,"F3"},
    {Event_KeyF4 ,"F4"},
    {Event_KeyF5 ,"F5"},
    {Event_KeyF6,"F6"},
    {Event_KeyF7,"F7"},
    {Event_KeyF8,"F8"},
    {Event_KeyF9 ,"F9"},
    {Event_KeyF10,"F10"},
    {Event_KeyF11,"F11"},
    {Event_KeyF12,"F12"},

    {Event_KeyEscape, "Esc"},       // "Esc" Esc键
    {Event_KeyTab ,   "Tap"},      // "Tap" Tap键
    {Event_KeyBacktab,"Backtab"},    // "Backtab" 退格键
    {Event_KeyBackspace,"Backspace"},  // "Backspace" 退格键
    {Event_KeyReturn,   "Return, Enter"},// Event_KeyEnter
    {Event_KeyInsert,  "Insert" },// "Insert" 插入键
    {Event_KeyDelete,  "Delete" },// "Delete" 删除键
    {Event_KeyPause,   "Pause/Break" },// "Pause/Break" Pause/Break键
    {Event_KeyPrint,   "PrintScreen" },// "PrintScreen" 截屏键
    {Event_KeySysReq,  "SysReq"     },// Event_KeyPrint
    {Event_KeyClear,   "Clear"},// 清除键
    {Event_KeyHome ,   "Home" },// "Home" 起始键
    {Event_KeyEnd,      "End" },// "End" 结尾键
    {Event_KeyLeft ,    "←"},// "←" 左箭头
    {Event_KeyUp,       "↑" },// "↑" 上箭头
    {Event_KeyRight,     "→" },// "→" 右箭头
    {Event_KeyDown,      "↓" },// "↓" 下箭头
    {Event_KeyPageUp ,  "PageUp" },// "PageUp" 向上翻页键
    {Event_KeyPageDown, "PageDown"},// "PageDown" 向下翻页键
    {Event_KeyShift,    "Shift"},// "Shift" 上档转换键
    {Event_KeyShiftLeft, "Left-Shift"},// "Shift" 左上档转换键
    {Event_KeyShiftRight,"Right-Shift"},// "Shift" 右上档转换键
    {Event_KeyControl,   "Control"},// "Ctrl" 键
    {Event_KeyControlLeft, "Left-Control" },// "Ctrl" 左键
    {Event_KeyControlRight,"Right-Control" }, // "Ctrl" 右键
    {Event_KeyMeta,"Meta"},
    {Event_KeyMetaLeft,"Left-Meta"},
    {Event_KeyMetaRight,"Right-Meta"},
    {Event_KeySuper,"Super"},
    {Event_KeySuperLeft,"Left-Super"},
    {Event_KeySuperRight,"Right-Super"},
    {Event_KeyAlt,   "Alt" },   // "Alt" 交替换档键
    {Event_KeyAltLeft,"Left-Alt" },   // "Alt" 左交替换档键
    {Event_KeyAltRight, "Right-Alt" },   // "Alt" 右交替换档键
    {Event_KeyCapsLock, "CapsLock" },   // "CapsLock" 大小写锁定键
    {Event_KeyNumLock ,"NumLock"},     // "NumLock" 小键盘锁定键
    {Event_KeyScrollLock ,"ScrollLock"},  // "ScrollLock" 滚动锁定键

    {Event_KeyMenu , "Menu"},       //  菜单键
    {Event_KeyHelp,"Help"},         //  帮助键

    {Event_KeyFN ,"FN"},             // "FN" 扩展键
};

    bs_string outstr;
    outstr.format("dump event:\n  timestamp= %ld \n", this->timestamp());

    const char* state = NULL;

    if (this->is_keybord ())
    {
        for (int i = 0; i < sizeof(sDumpStateStr)/ sizeof(sDumpStateStr[0]); ++i)
            if (sDumpStateStr[i].code & this->key())
            {
                state = sDumpStateStr[i].str;
                break;
            }
        for (int i = 0; i < sizeof(sDumpKeyStr)/ sizeof(sDumpKeyStr[0]); ++i)
            if (sDumpKeyStr[i].code & this->key())
            {
                outstr.format("  key= %s[%s]\n", sDumpKeyStr[i].str, state);
                break;
            }
    }
    else if (this->is_mouse ())
    {
        for (int i = 0; i < sizeof(sDumpStateStr)/ sizeof(sDumpStateStr[0]); ++i)
            if (sDumpStateStr[i].code & this->button())
            {
                state = sDumpStateStr[i].str;
                break;
            }
        for (int i = 0; i < sizeof(sDumpMouseStr)/ sizeof(sDumpMouseStr[0]); ++i)
            if (sDumpMouseStr[i].code & this->button())
            {
                outstr.format("  button= %s[%s]", sDumpMouseStr[i].str, state);
                break;
            }
        if (this->button() & Event_MouseWheel)
            outstr.format("  wheel[%d]", this->wheel());
        outstr.format("  pos[x=%f,y=%f] \n", this->x(), this->y());
    }

    // modifier
    outstr.format("  modifier= ");
    for (int i = 0; i < sizeof(sDumpModifierStr)/ sizeof(sDumpModifierStr[0]); ++i)
        if (sDumpModifierStr[i].code & this->modifier())
            outstr.format("%s ", sDumpModifierStr[i].str);
    outstr.format(" \n");
    return outstr;
}
#endif // EVENT_H
