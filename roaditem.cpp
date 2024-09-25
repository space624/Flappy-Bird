#include "roaditem.h"

#include <QPainter>
#include <QPropertyAnimation>

#define ROAD_ITEM_HEIGHT 38

RoadItem::RoadItem(QGraphicsScene* scene)
    :m_scene(scene)
{
    scene->addItem(this);   //将当前道路图形项添加到游戏场景中去

    startMove();
}

RoadItem::~RoadItem()
{

}

//重写绘图区域
QRectF RoadItem::boundingRect() const
{
    //游戏窗口大小:584*448
    //马路大小:384*64
    return QRectF(0,m_scene->height()-ROAD_ITEM_HEIGHT,
                  m_scene->width()*2,ROAD_ITEM_HEIGHT);
}

//重写绘图事件
void RoadItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //画前后两段马路
    painter->drawImage(QRectF(0,m_scene->height()-ROAD_ITEM_HEIGHT,
                              m_scene->width(),ROAD_ITEM_HEIGHT),
                       QImage(":/BirdGame/Resources/image/road.png"));
    painter->drawImage(QRectF(m_scene->width(),
                              m_scene->height()-ROAD_ITEM_HEIGHT,
                              m_scene->width(),ROAD_ITEM_HEIGHT),
                       QImage(":/BirdGame/Resources/image/road.png"));
}

void RoadItem::startMove()
{
    QPropertyAnimation* moveAnimation=new QPropertyAnimation(this,"pos");
    moveAnimation->setLoopCount(-1);    //设置马路动画无限循环,0--不启动,-1--无限循环
    moveAnimation->setDuration(6000);
    moveAnimation->setStartValue(QPoint(0,pos().y()));    //动画启动值
    moveAnimation->setEndValue(QPoint(0-m_scene->width(),pos().y()));   //动画结束值
    moveAnimation->setEasingCurve(QEasingCurve::Linear);
    moveAnimation->start();
}
