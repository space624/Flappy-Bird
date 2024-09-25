#include "buttonitem.h"

#include <QGraphicsScene>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

ButtonItem::ButtonItem(const QPixmap& pixmap,QGraphicsScene* scene)
    :m_pix(pixmap)
{
    scene->addItem(this);   //场景中添加当前按钮图形象
    setCursor(QCursor(Qt::PointingHandCursor)); //鼠标形状设置成手形
}

ButtonItem::~ButtonItem()
{

}

//重写按钮图形象的绘图区域
QRectF ButtonItem::boundingRect() const
{
    return QRectF(QPointF(0,0),m_pix.size());
}

void ButtonItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
        emit clicked();
}

void ButtonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->drawPixmap(0,0,m_pix);
}
