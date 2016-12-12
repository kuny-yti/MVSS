#ifndef UI_LAYOUT_H
#define UI_LAYOUT_H
#include "gui/ilayout.h"
#include "ui_define.h"


class ui_layout :public ilayout, public iwidget
{
public:
    explicit ui_layout(iwidget *parent = 0);
    virtual ~ui_layout();

    virtual void insert(const piwidget &ui, int sizt, int width = 1);

    virtual void append(const piwidget &ui, int width = 1);

    virtual void remove(const piwidget &ui);
    virtual void remove(int sizt);

    virtual void replace(const piwidget &sui, const piwidget &dui, int width = -1);

    virtual void replace(int sizt, const piwidget &dui, int width = -1);

    virtual void adjust(const piwidget &ui, int width = -1);
    virtual void adjust(int ssizt,  int width = -1);


    virtual void insert(const piwidget &ui, int siztx, int sizty, int width = 1, int height = 1);

    virtual void append(const piwidget &ui, int sizty, ivec2 wh);

    virtual void remove(const ivec2 &sizt);

    virtual void replace(const piwidget &sui, const piwidget &dui, ivec2 wh = ivec2(-1,-1));
    virtual void replace(const ivec2 sizt, const piwidget &dui, ivec2 wh = ivec2(-1,-1));

    virtual void adjust(const piwidget &ui, const ivec4 &rp);
    virtual void adjust(const ivec4 &rp);

protected:
    virtual bool event(const bs_event &);
    virtual void paint();
    virtual fsize fit() const;

private:
    pilayout layout;

    void check();
};


#endif // UI_LAYOUT_H
