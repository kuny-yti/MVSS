#include "ui_layout.h"

namespace impl {

class box_layout :public ilayout
{
public:
    box_layout(ui_layout *p):
        ilayout()
    {
        parent = p;
        total_power = 0;
    }
    virtual void insert(const piwidget &ui, int sizt, int width)
    {
        // 当插入的位置多余当前实际的数量时，进行补空
        if (sizt > list_ui.count ())
        {
            for (uint i = list_ui.count(); i < sizt; ++i)
            {
                list_ui.append(piwidget(NULL));
                list_vec.append(ivec2(i, 1));
                list_rect.append(frect(0,0,0,0));

                total_power += 1;
            }
        }
        if (sizt >= list_ui.count ())
        {
            list_ui.append(ui);
            list_vec.append(ivec2(sizt, width));
            list_rect.append(frect(0,0,0,0));
            total_power += width;
        }
        else
        {
            piwidget tmwid = list_ui[sizt];
            if (!tmwid && (uint)sizt < list_ui.count())
            {
                list_ui[sizt] = ui;
                list_vec[sizt] = ivec2(sizt, width);
            }
            else
            {
                list_ui.insert (sizt, ui);
                list_vec.insert (sizt, ivec2(sizt, width));
                list_rect.insert (sizt, frect(0,0,0,0));
                total_power += width;
            }
        }

        // 设置他的父对象
        piwidget tmpui(ui);
        if (tmpui)
            tmpui->parent(parent);
    }

    virtual void append(const piwidget &ui, int width = 1)
    {
        list_ui.append(ui);
        list_vec.append(ivec2(list_vec.count (), width));
        list_rect.append(frect(0,0,0,0));
        total_power += width;

        // 设置他的父对象
        piwidget tmpui(ui);
        if (tmpui)
            tmpui->parent(parent);
    }

    virtual void remove(const piwidget &ui)
    {
        for (uint pos = 0; pos < list_ui.count(); ++pos)
        {
            if (ui.get() == list_ui[pos].get ())
            {
                total_power -= list_vec[pos].y;
                list_ui.remove(pos);
                list_vec.remove(pos);
                list_rect.remove(pos);
                return ;
            }
        }
    }
    virtual void remove(int sizt)
    {
        if (sizt >= 0 && sizt < list_ui.count ())
        {
            total_power -= orientation == list_vec[sizt].y;
            list_ui.remove(sizt);
            list_vec.remove(sizt);
            list_rect.remove(sizt);
        }
    }

    virtual void replace(const piwidget &sui, const piwidget &dui, int width = -1)
    {

        for (uint pos = 0; pos < list_ui.count(); ++pos)
        {
            if (sui.get() == list_ui[pos].get ())
            {
                replace(pos, dui, width);
                return ;
            }
        }
    }

    virtual void replace(int sizt, const piwidget &dui, int width = -1)
    {
        if (sizt >= 0 && sizt < list_ui.count ())
        {
            list_ui[sizt]->parent (dui->parent ());
            list_ui.replace (sizt, dui);
            if (width != -1)
            {
                total_power += (width - list_vec[sizt].y);
                list_vec[sizt].y = width;
            }
        }
    }

    void move(const piwidget &ui1, const piwidget &ui2)
    {
        uint src = 0, dst = 0;
        for (uint i = 0; i < list_ui.count (); ++i)
        {
            if (list_ui[i].get () == ui1.get ())
                src = i;
            else if (list_ui[i].get () == ui2.get ())
                dst = i;
        }

        move (src, dst);
    }
    void move(int sizt1, int sizt2)
    {
        if (sizt1 >=0 && sizt1 < list_ui.count () && sizt2 >=0 && sizt2 < list_ui.count ())
        {
            list_vec[sizt1].x = sizt2;
            list_ui.move (sizt1, sizt2);
            list_vec.move (sizt1, sizt2);
        }
    }
    void move(const piwidget &ui1, int sizt2)
    {
        uint src = 0;
        for (uint i = 0; i < list_ui.count (); ++i)
        {
            if (list_ui[i].get () == ui1.get ())
                src = i;
        }

        move (src, sizt2);
    }
    void move(int sizt1, const piwidget &ui2)
    {
        uint dst = 0;
        for (uint i = 0; i < list_ui.count (); ++i)
        {
            if (list_ui[i].get () == ui2.get ())
                dst = i;
        }

        move (sizt1, dst);
    }

    void swap(const piwidget &ui1, const piwidget &ui2)
    {
        uint src = 0, dst = 0;
        for (uint i = 0; i < list_ui.count (); ++i)
        {
            if (list_ui[i].get () == ui1.get ())
                src = i;
            else if (list_ui[i].get () == ui2.get ())
                dst = i;
        }

        swap (src, dst);
    }
    void swap(int sizt1, int sizt2)
    {
        if (sizt1 >=0 && sizt1 < list_ui.count () && sizt2 >=0 && sizt2 < list_ui.count ())
        {
            list_ui.swap (sizt1, sizt2);
            list_vec.swap (sizt1, sizt2);
        }
    }
    void swap(const piwidget &ui1, int sizt2)
    {
        uint src = 0;
        for (uint i = 0; i < list_ui.count (); ++i)
        {
            if (list_ui[i].get () == ui1.get ())
                src = i;
        }

        swap (src, sizt2);
    }
    void swap(int sizt1, const piwidget &ui2)
    {
        uint dst = 0;
        for (uint i = 0; i < list_ui.count (); ++i)
        {
            if (list_ui[i].get () == ui2.get ())
                dst = i;
        }

        swap (sizt1, dst);
    }

    virtual void adjust(const piwidget &ui, int width = -1)
    {
        for (uint i = 0; i < list_ui.count (); ++i)
        {
            if (list_ui[i].get () == ui.get ())
            {
                adjust(i, width);
                return ;
            }
        }
    }
    virtual void adjust(int ssizt, int width = -1)
    {
        if (ssizt >= 0 && ssizt < list_ui.count ())
        {
            if (width != -1)
            {
                total_power += (width - list_vec[ssizt].y);
                list_vec[ssizt].y = width;
            }
        }
    }

public:
    virtual bool event(const bs_event &)
    {
        return false;
    }
    virtual void paint(frect rect)
    {
        //去除边缘的幅度
        rect.x += margin.x;
        rect.y += margin.y;
        rect.width -= margin.z + margin.x;
        rect.height -= margin.w + margin.y;

        if (orientation == Ui_Horizontal)
        {
            //去除间隔的幅度和总份数
            rect.width  -= spacing.x * (list_ui.count() -1);
            //计算每份占用的尺寸
            int one_power = rect.width / total_power;

            for (uint pos = 0; pos < list_ui.count(); ++pos)
            {
                // 计算绝对位置
                if (pos == 0)
                    list_rect[pos] = frect(rect.x, rect.y, one_power*list_vec[pos].y, rect.height);
                else
                {
                    fpoint loc = list_rect[pos-1].xy();
                    loc.x += list_rect[pos-1].width + spacing.x;

                    list_rect[pos] = frect(loc.x, loc.y, one_power*list_vec[pos].y, rect.height);
                }
                list_ui[pos]->geometrys[iwidget::Geom_Abs] = list_rect[pos];
            }
        }
        else if (orientation == Ui_Vertical)
        {
            //去除间隔的幅度和总份数
            int one_power = 0;
            rect.height -= spacing.x * (list_ui.count() -1);
            //计算每份占用的尺寸
            one_power = rect.height / total_power;

            for (uint pos = 0; pos < list_ui.count(); ++pos)
            {
                // 计算绝对位置
                if (pos == 0)
                    list_rect[pos] = frect(rect.x, rect.y, rect.width, one_power*list_vec[pos].y);
                else
                {
                    fpoint loc = list_rect[pos-1].xy();
                    loc.y += list_rect[pos-1].height + spacing.y;

                    list_rect[pos] = frect(loc.x, loc.y, rect.width, one_power*list_vec[pos].y);
                }
                 list_ui[pos]->geometrys[iwidget::Geom_Abs] = list_rect[pos];
            }
        }
    }
    virtual fsize fit() const
    {
        fsize fit_size;
        fsize max_fit;

        fit_size.width = margin.z + margin.x;
        fit_size.height = margin.w + margin.y;

        for (uint i = 0; i < list_ui.count (); ++i)
        {
            fsize chdfit = list_ui[i]->fit();
            if (orientation == Ui_Horizontal)
            {
                if (chdfit.height > max_fit.height)
                    max_fit.height = chdfit.height;

                fit_size.width += chdfit.width + spacing.x;
            }
            else if (orientation == Ui_Vertical)
            {
                if (chdfit.width > max_fit.width)
                    max_fit.width = chdfit.width;

                fit_size.height += chdfit.height + spacing.y;
            }
        }
        if (orientation == Ui_Horizontal)
        {
            fit_size.width -= spacing.x;
            fit_size.height += max_fit.height;
        }
        else if (orientation == Ui_Vertical)
        {
            fit_size.height -= spacing.y;
            fit_size.width += max_fit.width;
        }

        return fit_size;
    }

private:
    ui_layout *parent;
    int total_power;
    bs_list<piwidget> list_ui;
    bs_list<ivec2 > list_vec;
    bs_list<frect> list_rect;

private:
    void insert(const piwidget &, int , int , int , int){}

    void append(const piwidget &, int ,ivec2 ){}

    void remove(const ivec2 &){}
    void replace(const piwidget &, const piwidget &, ivec2  ){}
    void replace(const ivec2 , const piwidget &, ivec2  ){}
    void adjust(const piwidget &, const ivec4 &){}
    void adjust(const ivec4 &){}
};
class grid_layout :public ilayout
{
protected:
    void insert_row (int sizt)
    {
        int row_count = list_ui.count () ? list_ui[0].count (): 0;

        if (sizt >= list_ui.count ())
        {
            for (uint j = list_ui.count (); j <= sizt; ++j)
            {
                bs_list<piwidget> lui;
                bs_list<ivec4> lvec;
                bs_list<frect> lrect;
                for (uint i = 0; i < row_count; ++i)
                {
                    lui.append (piwidget());
                    lvec.append (ivec4(i, sizt, 0,0));
                    lrect.append (frect());
                }
                list_ui.append (lui);
                list_vec.append (lvec);
                list_rect.append (lrect);
            }
        }
    }
    void insert_col(int sizt)
    {
        int row_count = list_ui.count () ? list_ui[0].count (): 0;
        if (sizt < row_count)
        {
            for (uint j = 0; j < list_ui.count (); ++j)
            {
                list_ui[j].insert (sizt, piwidget());
                list_vec[j].insert (sizt, ivec4(sizt, j, 0,0));
                list_rect[j].insert (sizt, frect());
            }
        }
        else if (sizt >= row_count)
        {
            for (uint j = 0; j < list_ui.count (); ++j)
            {
                for (uint i = list_ui[j].count (); i <= sizt; ++i)
                {
                    list_ui[j].append (piwidget());
                    list_vec[j].append (ivec4(i, j, 0,0));
                    list_rect[j].append (frect());
                }
            }
        }
    }
public:
    grid_layout(ui_layout *p):
        ilayout()
    {
        parent = p;
    }

    void insert(const piwidget &ui, int siztx, int sizty, int width, int height)
    {
        int row_count = list_ui.count () ? list_ui[0].count (): 0;
        if (sizty >= list_ui.count ())
            insert_row (sizty);
        else
            insert_col (siztx);
        if (row_count == 0 || siztx > row_count)
            insert_col (siztx);
        list_ui[sizty][siztx] = ui;
        list_vec[sizty][siztx] = ivec4(siztx, sizty, width, height);

        // 设置他的父对象
        piwidget tmpui(ui);
        if (tmpui)
            tmpui->parent(parent);
    }

    void append(const piwidget &ui, int sizty, ivec2 rp){
        if (sizty >= list_ui.count ())
            insert_row (sizty);

        int cols = list_vec[sizty].count ();
        insert_col(cols);
        list_ui[sizty][cols] = (ui);
        list_vec[sizty][cols] = (ivec4(list_vec[sizty].count (), sizty, rp.x, rp.y));
        // 设置他的父对象
        piwidget tmpui(ui);
        if (tmpui)
            tmpui->parent(parent);
    }

    void remove(const piwidget &ui)
    {
        for (uint j = 0; j < list_ui.count (); ++j)
        {
            for (uint i = 0; i < list_ui[j].count (); ++i)
                if (list_ui[j][j].get () == ui.get ())
                {
                    list_ui[j][j] = piwidget();
                    list_vec[j][i].z = 0;
                    list_vec[j][i].w = 0;
                }
        }
    }
    void remove(const ivec2 &r)
    {
        int row_count = list_ui.count () ? list_ui[0].count (): 0;
        if (r.x >= 0 && r.x < row_count && r.y >=0 && r.y < list_ui.count ())
        {
            list_ui[r.y][r.x] = piwidget();
            list_vec[r.y][r.x].z = 0;
            list_vec[r.y][r.x].w = 0;
        }
    }
    void replace(const piwidget &, const piwidget &, ivec2  ){

    }
    void replace(const ivec2 , const piwidget &, ivec2  ){

    }
    void adjust(const piwidget &, const ivec4 &){

    }
    void adjust(const ivec4 &){

    }

public:
    virtual bool event(const bs_event &)
    {
        return false;
    }
    virtual void paint(frect rect)
    {
        //去除边缘的幅度
        rect.x += margin.x;
        rect.y += margin.y;
        rect.width -= margin.z+margin.x;
        rect.height -= margin.w+margin.y;

        //计算每份占用的尺寸
        rect.width += spacing.x;
        rect.height += spacing.y;

        int y_p = list_ui.count ();
        for (uint y = 0; y < list_ui.count(); ++y)
        {
            int x_p = list_ui[y].count ();
            for (uint x = 0; x < list_ui[y].count(); ++x)
            {
                float power_w = rect.width / x_p;
                float power_h = rect.height / y_p;

                // 计算绝对位置
                if (y == 0)
                {
                    if (x == 0)
                        list_rect[y][x] = frect(rect.x, rect.y, power_w*list_vec[y][x].z, power_h*list_vec[y][x].w);
                    else
                    {
                        frect prve = list_rect[y][x-1];
                        fpoint loc = prve.xy();
                        loc.x += prve.width + spacing.x;

                        list_rect[y][x] = frect(loc.x, loc.y, power_w*list_vec[y][x].z, power_h*list_vec[y][x].w);
                    }
                }
                else
                {
                    if (x == 0 )
                    {
                        frect prve = list_rect[y-1][x];
                        fpoint loc = prve.xy();
                        loc.y += prve.height + spacing.y;

                        list_rect[y][x] = frect(loc.x, loc.y, power_w*list_vec[y][x].z, power_h*list_vec[y][x].w);
                    }
                    else
                    {
                        frect prve = list_rect[y-1][x-1];
                        fpoint loc = prve.xy();
                        loc.y += prve.height + spacing.y;
                        loc.x += prve.width + spacing.x;

                        list_rect[y][x] = frect(loc.x, loc.y, power_w*list_vec[y][x].z, power_h*list_vec[y][x].w);
                    }
                }

                list_ui[y][x]->geometrys[iwidget::Geom_Abs] = list_rect[y][x];
            }
        }
    }
    virtual fsize fit() const
    {
        fsize fit_size;

        for (uint y = 0; y < list_ui.count(); ++y)
        {
            fsize max_fit;
            for (uint x = 0; x < list_ui[y].count(); ++x)
            {
                fsize chdfit = list_ui[y][x]->fit();
                if (max_fit.height < chdfit.height)
                    max_fit.height = chdfit.height;
                max_fit.width += chdfit.width + spacing.x;

            }
            if (max_fit.width > fit_size.width)
                fit_size.width = max_fit.width- spacing.x;
            fit_size.height += max_fit.height+ spacing.y;
        }

        fit_size.height -= spacing.y;
        fit_size.width += margin.z+margin.x;
        fit_size.height += margin.w+margin.y;

        return fit_size;
    }

private:
    ui_layout *parent;
    bs_list<bs_list<piwidget> > list_ui;
    bs_list<bs_list<ivec4 > > list_vec;
    bs_list<bs_list<frect> > list_rect;

private:
    virtual void insert(const piwidget &, int , int ){}

    virtual void append(const piwidget &, int){}

    virtual void remove(int ){}

    virtual void replace(const piwidget &, const piwidget &, int ){}

    virtual void replace(int , const piwidget &, int ){}

    virtual void adjust(const piwidget &, int ){}
    virtual void adjust(int , int ){}
};

}

ui_layout::ui_layout(iwidget *parent):
    iwidget(parent)
{
    type = Ui_Layout;
}
ui_layout::~ui_layout()
{
}

void ui_layout::check()
{
    if (mode == Layout_Box && !layout.get ())
        layout = bs_malloc (impl::box_layout, this);
    else if (mode == Layout_Grid && !layout.get ())
        layout = bs_malloc (impl::grid_layout, this);
    else if (mode == Layout_Group)
    {

    }

    layout->orientation = this->orientation;
    layout->align = this->align;
    layout->margin = this->margin;
    layout->spacing = this->spacing;
}
void ui_layout::insert(const piwidget &ui, int sizt, int width)
{
    check();
    layout->insert (ui, sizt, width);
}

void ui_layout::append(const piwidget &ui, int width)
{
    check();
    layout->append (ui, width);
}

void ui_layout::remove(const piwidget &ui)
{
    if (!layout.get ())
        return ;
    layout->remove (ui);
}
void ui_layout::remove(int sizt)
{
    if (mode != Layout_Box && !layout.get ())
        return ;
    layout->remove (sizt);
}

void ui_layout::replace(const piwidget &sui, const piwidget &dui, int width)
{
    if (mode != Layout_Box && !layout.get ())
        return ;
    layout->replace (sui, dui, width);
}

void ui_layout::replace(int sizt, const piwidget &dui, int width)
{
    if (mode != Layout_Box && !layout.get ())
        return ;
    layout->replace (sizt, dui, width);
}

void ui_layout::adjust(const piwidget &ui,int width)
{
    if (mode != Layout_Box && !layout.get ())
        return ;
    layout->adjust (ui, width);
}
void ui_layout::adjust(int ssizt, int width)
{
    if (mode != Layout_Box && !layout.get ())
        return ;
    layout->adjust (ssizt, width);
}


void ui_layout::insert(const piwidget &ui, int siztx, int sizty, int width, int height)
{
    check();
    layout->insert (ui, siztx,sizty, width, height);
}

void ui_layout::append(const piwidget &ui, int sizty, ivec2 wh)
{
    check();
    layout->append (ui, sizty, wh);
}

void ui_layout::remove(const ivec2 &sizt)
{
    if (mode != Layout_Grid && !layout.get ())
        return ;
    layout->remove (sizt);
}

void ui_layout::replace(const piwidget &sui, const piwidget &dui, ivec2 wh )
{
    if (mode != Layout_Grid && !layout.get ())
        return ;
    layout->replace (sui, dui, wh);
}
void ui_layout::replace(const ivec2 sizt, const piwidget &dui, ivec2 wh )
{
    if (mode != Layout_Grid && !layout.get ())
        return ;
    layout->replace (sizt, dui, wh);
}

void ui_layout::adjust(const piwidget &ui, const ivec4 &rp)
{
    if (mode != Layout_Grid && !layout.get ())
        return ;
    layout->adjust (ui, rp);
}
void ui_layout::adjust(const ivec4 &rp)
{
    if (mode != Layout_Grid && !layout.get ())
        return ;
    layout->adjust (rp);
}
bool ui_layout::event(const bs_event &e)
{
    if (!layout.get ())
        return false;
    if (mode == Layout_Box)
    {
        impl::box_layout *bl = layout.as<impl::box_layout>().get ();
        if (bl)
            return bl->event (e);
    }
    else if (mode == Layout_Grid)
    {
        impl::grid_layout *gl = layout.as<impl::grid_layout>().get ();
        if (gl)
            return gl->event (e);
    }
    return false;
}
fsize ui_layout::fit() const
{
    if (!layout.get ())
        return fsize(0,0);
    if (mode == Layout_Box)
    {
        impl::box_layout *bl = layout.as<impl::box_layout>().get ();
        if (bl)
            return bl->fit ();
    }
    else if (mode == Layout_Grid)
    {
        impl::grid_layout *gl = layout.as<impl::grid_layout>().get ();
        if (gl)
            return gl->fit ();
    }
    return fsize(0,0);
}
void ui_layout::paint()
{
    if (!layout.get ())
        return ;
    if (mode == Layout_Box)
    {
        impl::box_layout *bl = layout.as<impl::box_layout>().get ();
        if (bl)
            return bl->paint (this->absolute ());
    }
    else if (mode == Layout_Grid)
    {
        impl::grid_layout *gl = layout.as<impl::grid_layout>().get ();
        if (gl)
            return gl->paint (this->absolute ());
    }
}
