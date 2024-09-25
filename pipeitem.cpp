#include "pipeitem.h"

#include <QPainter>
#include <QPropertyAnimation>

#define PIPE_WIDTH 60

PipeItem::PipeItem(QGraphicsScene* scene)
    :m_scene(scene)
{
    //游戏场景中添加管道图形项
    scene->addItem(this);
    createPipeHeight();
    startMove();
}

PipeItem::~PipeItem()
{

}

QRectF PipeItem::boundingRect() const
{
    return QRectF(m_scene->width(),0,PIPE_WIDTH,m_scene->height());
}

void PipeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //绘画上管道
    painter->drawImage(QRectF(m_scene->width(),0,
                              PIPE_WIDTH,m_upPipeHeight),
                       QImage(":/BirdGame/Resources/image/tubeup.png"));
    //绘画下管道
    painter->drawImage(QRectF(m_scene->width(),m_upPipeHeight+140,
                              PIPE_WIDTH,m_downPipeHeight),
                       QImage(":/BirdGame/Resources/image/tubedown.png"));
}

QPainterPath PipeItem::shape() const
{
    QPainterPath path;
    path.addRect(QRectF(m_scene->width(),0,
                        PIPE_WIDTH,m_upPipeHeight));
    path.addRect(QRectF(m_scene->width(),m_upPipeHeight+140,
                        PIPE_WIDTH,m_downPipeHeight));
    return path;
}

void PipeItem::startMove()
{
    //移动的动画(位置属性动画)
    QPropertyAnimation* moveAnimation=new QPropertyAnimation(this,"pos");
    moveAnimation->setLoopCount(-1);
    moveAnimation->setDuration(3000);
    moveAnimation->setStartValue(QPoint(0,pos().y()));
    moveAnimation->setEndValue(QPoint(0-m_scene->width()-PIPE_WIDTH,pos().y()));
    moveAnimation->start();

    //动画循环次数改变时则创建管道
    connect(moveAnimation,&QPropertyAnimation::currentLoopChanged,
            [this](){
        createPipeHeight();
    });
}

void PipeItem::createPipeHeight()
{
    m_upPipeHeight=qrand()%100+80;  //上管道高度
    m_downPipeHeight=m_scene->height()-m_upPipeHeight-178;  //下管道的高度
}
