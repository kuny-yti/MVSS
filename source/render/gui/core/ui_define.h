#ifndef UI_DEFINE
#define UI_DEFINE
#include "gui/iwidget.h"
#include "maths/bs_line2.h"


static bool ui_contains(float x1, float y1, float w1, float h1, float x, float y)
{
    x -= x1;
    y -= y1;
    if ((x >= 0) && (y >= 0)
        && (x < w1) && (y < h1))
        return true;
    return false;
}

typedef struct Point
{
    int x;
    int y;
}Point;
//排斥实验
inline bool IsRectCross(const Point &p1,const Point &p2,const Point &q1,const Point &q2)
{
    bool ret = std::min(p1.x,p2.x) <= std::max(q1.x,q2.x)    &&
               std::min(q1.x,q2.x) <= std::max(p1.x,p2.x) &&
               std::min(p1.y,p2.y) <= std::max(q1.y,q2.y) &&
               std::min(q1.y,q2.y) <= std::max(p1.y,p2.y);
    return ret;
}
//跨立判断
static bool IsLineSegmentCross(const Point &pFirst1,const Point &pFirst2,const Point &pSecond1,const Point &pSecond2)
{
    long line1,line2;
    line1 = pFirst1.x * (pSecond1.y - pFirst2.y) +
            pFirst2.x * (pFirst1.y - pSecond1.y) +
            pSecond1.x * (pFirst2.y - pFirst1.y);
    line2 = pFirst1.x * (pSecond2.y - pFirst2.y) +
            pFirst2.x * (pFirst1.y - pSecond2.y) +
            pSecond2.x * (pFirst2.y - pFirst1.y);
    if (((line1 ^ line2) >= 0) && !(line1 == 0 && line2 == 0))
        return false;

    line1 = pSecond1.x * (pFirst1.y - pSecond2.y) +
            pSecond2.x * (pSecond1.y - pFirst1.y) +
            pFirst1.x * (pSecond2.y - pSecond1.y);
    line2 = pSecond1.x * (pFirst2.y - pSecond2.y) +
            pSecond2.x * (pSecond1.y - pFirst2.y) +
            pFirst2.x * (pSecond2.y - pSecond1.y);
    if (((line1 ^ line2) >= 0) && !(line1 == 0 && line2 == 0))
        return false;
    return true;
}

static bool GetCrossPoint(const Point &p1,const Point &p2,const Point &q1,const Point &q2,long &x,long &y)
{
    if(IsRectCross(p1,p2,q1,q2))
    {
        if (IsLineSegmentCross(p1,p2,q1,q2))
        {
            //求交点
            long tmpLeft,tmpRight;
            tmpLeft = (q2.x - q1.x) * (p1.y - p2.y) - (p2.x - p1.x) * (q1.y - q2.y);
            tmpRight = (p1.y - q1.y) * (p2.x - p1.x) * (q2.x - q1.x) + q1.x * (q2.y - q1.y) * (p2.x - p1.x) - p1.x * (p2.y - p1.y) * (q2.x - q1.x);

            x = (int)((double)tmpRight/(double)tmpLeft);

            tmpLeft = (p1.x - p2.x) * (q2.y - q1.y) - (p2.y - p1.y) * (q1.x - q2.x);
            tmpRight = p2.y * (p1.x - p2.x) * (q2.y - q1.y) + (q2.x- p2.x) * (q2.y - q1.y) * (p1.y - p2.y) - q2.y * (q1.x - q2.x) * (p2.y - p1.y);
            y = (int)((double)tmpRight/(double)tmpLeft);
            return true;
        }
    }
    return false;
}

class ui_composer
{

};
#endif // UI_DEFINE

