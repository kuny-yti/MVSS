
#include "bs_application.h"
#include "bs_platform.h"
#include "bs_render.h"
#include "bs_graphs.h"
#include "bs_gui.h"

/**
  1.内部包含了tcmalloc内存管理
  tools  工具库
bs_singleton  单例模式继承[模板类] - bs_utils.h
bs_array      连续数组类[模板类] - bs_array.h
bs_list       链表类[模板类]    - bs_list.h
bs_map        键值/数值对应类[模板类]，使用红黑树实现- bs_map.h
bs_vector     容器类[模板类] - bs_vector.h
bs_queue      队列类[模板类] - bs_queue.h
bs_stack      栈类[模板类] - bs_stack.h

bs_stream     数据流类[模板类] - bs_stream.h
bs_bstream    二进制数据流 - bs_stream.h
bs_tstream    文本数据流 - bs_stream.h

bs_datetime   日期时间 - bs_datetime.h
bs_image      图像 - bs_image.h
bs_md5        MD5 - bs_md5.h
bs_memory     内存操作 - bs_memory.h
bs_parser     解析器 - bs_parser.h
bs_string     字符串 - bs_string.h
bs_timer      计时器和定时器 - bs_timer.h
bs_typeinfo   类信息  - bs_typeinfo.h
bs_url        url解析  - bs_url.h
bs_uuid       uuid - bs_uuid.h

基础类
bs_application 应用程序  - bs_application.h
bs_atomic      原子操作  - bs_atomic.h
bs_event       基本事件  - bs_event.h
bs_filesystem  文件系统 - bs_filesystem.h
bs_ref         对象引用计数  - bs_define.h
bs_object      对象基类 - bs_object.h
bs_ptr         对象智能指针 - bs_ptr.h
bs_thread      系统线程  - bs_thread.h
bs_noncopy     禁止拷贝基类  - bs_utils.h

基础接口
ievent        系统事件基类  - ievent.h
iio           系统io操作基类  - iio.h
isocket       网络操作基类  - isocket.h

algorlthm 算法库 - algorlthm.h
bs_isort 插入排序
bs_qsort 快速排序
bs_hsort 堆排序
bs_bsort 冒泡排序
bs_msort 归并排序
bs_hash  哈希算法

maths 数学库
bs_rad2deg
bs_deg2rad
bs_sign
bs_isign
bs_ifloor
bs_iceil
bs_fix
bs_ifix
bs_round
bs_iround
bs_clamp
bs_isNumber
bs_isNaN
bs_isFiniteNumber
bs_fmap
bs_rand

bs_aabb 轴向平行包围盒[模板类] - bs_aabb.h (iaabb, faabb, daabb)
bs_color 颜色类[模板类] - bs_color.h (color)
bs_line2 [模板类] bs_line2.h iline2 fline2 dline2
bs_line3 [模板类] bs_line3.h iline3 fline3 dline3
bs_mat2x2 [模板类] bs_mat2x2.h bs_mat2X2f bs_mat2X2d mat2 fmat2 dmat2
bs_mat3x3 [模板类] bs_mat3x3.h bs_mat3X3f bs_mat3X3d mat3 fmat3 dmat3
bs_mat4x4 [模板类] bs_mat4x4.h bs_mat4X4f bs_mat4X4d mat4 fmat4 dmat4 bs_matrix bs_xform
bs_matMxN [模板类] bs_matMxN.h bs_matMxNf bs_matMxNd bs_matMxNi imatMxN fmatMxN dmatMxN
bs_plane [模板类] bs_plane.h iplane fplane dplane
bs_quaternion [模板类] bs_quaternion.h quat fquat dquat
bs_vec2 [模板类] bs_vec2.h vec2 ivec2 fvec2 dvec2 svec2 bvec2 isize fsize dsize ipoint fpoint dpoint bs_vector2f, bs_vector2d
bs_vec3 [模板类] bs_vec3.h vec3 ivec3 fvec3 dvec3 svec3 bvec3 isize3 fsize3 dsize3 ipoint3 fpoint3 dpoint3 bs_vector3f bs_vector3d vector
bs_vec4 [模板类] bs_vec4.h vec4 ivec4 fvec4 dvec4 svec4 bvec4 bs_vector4f bs_vector4d
*/
/*
struct _bs_octree_ : bs_ref
{
    bs_atomic<int> count;

    static const _bs_octree_ shared_null;

    static bool is_null(intptr p);
    static intptr null();
};
const _bs_octree_ _bs_octree_::shared_null = {bs_ref::refStatic, 0};
bool _bs_octree_::is_null (intptr p)
{
    return p == null();
}
intptr _bs_octree_::null ()
{
    _bs_octree_ *h = &shared_null;
    return ++h;
}

template <typename T, typename BoxT = float, typename Alloc = bs_alloc<void> >
class bs_octree : Alloc
{
    typedef T tNodeObject;
    typedef bs_aabb<BoxT> tBounds;

protected:
    // 节点
    struct octnode
    {
        octnode* parent;
        tBounds  bound;
        bool     leaf;

        virtual ~octnode(){}

        bool is_root()const{return parent == NULL;}
        bool is_leaf()const{return !is_root() && leaf;}
        bool is_branch()const{return !is_root() && !is_leaf();}
    };
    // 分支节点
    struct octbranch : octnode
    {
        octnode* child[2][2][2];

    public:
        explicit octbranch(const bs_aabb& box)
        {
            bound = box;
            leaf = false;
            bs_memory::set_c (child, 0, sizeof(child));
        }
        ~octbranch(){}
    };
    // 叶子节点
    struct octleaf : octnode
    {
    public:
        octleaf(const tNodeObject &o):obj(o){
            leaf = true;
        }
        ~octleaf(){}
    private:
        tNodeObject obj;
    };

public:
    bool is_null(){_bs_octree_::is_null (root_);}

public:
    bs_octree(): root_((octnode*)_bs_octree_::null ()){ }
    explicit bs_octree(const tBounds& box):
        root_(create(sizeof(octbranch)))
    {
        root_->bound = box;
    }
    ~bs_octree()
    {
        destory();
    }

    void append(const tBounds &box, const tNodeObject &v);
    void remove(const tNodeObject &v);
    const tNodeObject &find(const tBounds &box)const;
    tNodeObject &find(const tBounds &box);

protected:
    static octnode *create(int alg)
    {
        struct{_bs_octree_ *d; octnode *e;}t;

        t.d = (_bs_octree_*)Alloc::alloc (sizeof(_bs_octree_)+alg);
        t.d->set (bs_ref::refShareableDetach);
        t.d->count = 0;
        t.e = (octnode*)(t.d + 1);
        return t.e;
    }
    void destory()
    {

    }

    octnode *newnode(const tBounds &box)
    {
        octnode *tmp = root_;
        while (tmp && tmp->is_branch ())
        {
            ivec3 idx = boxindex(tmp->bound, box);
            octbranch *fackn = bsAsCast(octbranch)(tmp);
            tmp = fackn ? fackn->child[idx.x][idx.y][idx.z] : NULL;
        }
        if (tmp && tmp->is_leaf ())
    }
    octnode *search(const tBounds &box)
    {
        return search(root_, box);
    }
    static octnode *search(octnode* node, const tBounds &box)
    {
        if (node == NULL)
            return NULL;
        if (node->is_branch ())
        {
            ivec3 idx = boxindex(node->bound, box);
            octbranch* fackn = bsAsCast(octbranch)(node);
            if (fackn && fackn->child[idx.x][idx.y][idx.z]->is_leaf ())
                return &fackn->child[idx.x][idx.y][idx.z];
            else if (fackn && fackn->child[idx.x][idx.y][idx.z]->is_branch ())
                return search(fackn->child[idx.x][idx.y][idx.z], box);
        }
        else if (node->is_leaf ())
            return node;
        else if (node->is_root ())
            return search(node, box);
        return NULL;
    }
    static ivec3 boxindex(const tBounds &sbox, const tBounds &dbox)
    {
        tBounds::tElemTypes center = sbox.center ();
        tBounds::tElemTypes subcenter = dbox.center ();
        ivec3 oidx(0,0,0);
        if (subcenter.x > center.x)
            oidx.x = 1;
        else
            oidx.x = 0;

        if (subcenter.y > center.y)
            oidx.y = 1;
        else
            oidx.y = 0;

        if (subcenter.z > center.z)
            oidx.z = 1;
        else
            oidx.z = 0;
    }
private:
    octnode *root_;

    _bs_octree_ *header(){
        _bs_octree_ *h = (_bs_octree_*)root_;
        if (h) --h;
        return h;
    }
    _bs_octree_ *header()const{
        _bs_octree_ *h = (_bs_octree_*)root_;
        if (h) --h;
        return h;
    }
};
*/

int bs_main(int argc, char **argv)
{
    app()->enter(argc, argv);

    piwindow win = bs_platform::create ();
    bs_platform::instance ()->set (win);

    tWindowFormats wf;
    wf.event = Window_InputOut;
    wf.minisize = isize(64,64);
    wf.maxisize = isize(1920, 1080);
    wf.geometry = ivec4(0,0,1280,720);
    wf.title = "MVSS -v1.0.0.1";
    wf.style = (Window_Resizable | Window_MultiThread | Window_AlwaysRun);
    tRenderFormats rf;
    rf.swap = (Swap_DoubleBuffer);
    rf.client = (Render_Default);
    rf.target = (Render_Window);
    rf.depth_bits= (Channel_Bit24);
    rf.stencil_bits= (Channel_Bit8);
    rf.color_bits= (Channel_RGBA8);
    rf.accum_bits= (Channel_BitNot);
    rf.samples= (16);
    rf.enable_stereo= (0);
    rf.has_alpha= (1);
    rf.swap_sync= (0);
    rf.has_debug= (1);

    if (!bs_render::instance ()->create(win.get(), rf))
        return -1;
    if (!win->create(bs_render::instance ()->context.get (), wf))
        return -1;
    win->show();

    bs_graphs::instance ()->set (pigui(bs_gui::instance ()));
    bs_graphs::instance ()->initialize (win, bs_render::instance ());

    return app()->exec();
}

