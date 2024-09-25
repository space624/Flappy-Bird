#include "birditem.h"

#include <QGraphicsScene>
#include <QTimer>
#include <QPropertyAnimation>
#include <QPainter>
#include <QKeyEvent>

#define FLY_BIRD_SIZE 45

BirdItem::BirdItem(QGraphicsScene* scene)
    :m_curflyStatus(0)
    ,m_IsLandFall(true)
    ,m_IsRaise(true)
{
    scene->addItem(this);   //将当前小鸟对象添加到游戏场景中
    m_scene=scene;

    //定时刷新小鸟
    m_birdRefreshTimer=new QTimer(this);
    connect(m_birdRefreshTimer,SIGNAL(timeout()),this,SLOT(onRefreshBird()));
    m_birdRefreshTimer->start(10);  //定时器间隔10ms

    //小鸟位置属性动画,通过设置小鸟的位置属性值,达到动画效果
    m_flyAnimation=new QPropertyAnimation(this,"pos");
    connect(m_flyAnimation,SIGNAL(finished()),
            this,SLOT(onFlyRaiseAnimationFinished()));
}

BirdItem::~BirdItem()
{

}

//重写绘图区域
QRectF BirdItem::boundingRect() const
{
    return QRectF(60,FLY_BIRD_SIZE*5,FLY_BIRD_SIZE,FLY_BIRD_SIZE);
}

//小鸟上升动画
void BirdItem::flyRaiseAnimation()
{
    if(m_IsRaise)
    {
        m_IsRaise=false;
        m_flyAnimation->stop(); //动画停止

        int nMove=pos().y();  //鸟儿移动的y值
        if(nMove>-180)
        {
            m_flyAnimation->setDuration(300);   //动画时长
            m_flyAnimation->setEndValue(QPoint(pos().x(),pos().y()-FLY_BIRD_SIZE));
        }
        else    //小鸟飞到了顶上
        {
            m_flyAnimation->setDuration(300);
            m_flyAnimation->setEndValue(pos());
        }

        m_flyAnimation->setEasingCurve(QEasingCurve::Linear);   //动画曲线效果
        m_flyAnimation->start();
    }
}

//下降动画
void BirdItem::flyLandfallAnimation()
{
    if(m_IsLandFall)
    {
        m_flyAnimation->stop();
        m_flyAnimation->setDuration(1500);
        m_flyAnimation->setEndValue(QPoint(pos().x(),m_scene->height()/*pos().y()+FLY_BIRD_SIZE*/)); //下降一个小鸟高度
        m_flyAnimation->setEasingCurve(QEasingCurve::InQuad);
        m_flyAnimation->start();
        m_IsLandFall=false;
    }
}

bool BirdItem::checkIsCollided()
{
    //collidingItems返回与当前游戏项“冲突”(碰撞)的所有图形项
    if(!collidingItems().isEmpty())
        return true;
    else
        return false;
}

//重写绘图事件
void BirdItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if(m_curflyStatus<10)   //翅膀向上
    {
        m_curflyStatus++;
        painter->drawImage(boundingRect(),QImage(":/BirdGame/Resources/image/bird1.png"));
    }
    else if(m_curflyStatus>=10&&m_curflyStatus<20)  //翅膀剧中
    {
        m_curflyStatus++;
        painter->drawImage(boundingRect(),QImage(":/BirdGame/Resources/image/bird2.png"));
    }
    else if(m_curflyStatus>=20&&m_curflyStatus<30)  //翅膀往下
    {
        m_curflyStatus++;
        painter->drawImage(boundingRect(),QImage(":/BirdGame/Resources/image/bird3.png"));
    }
    else
    {
        m_curflyStatus=0;
    }
}

void BirdItem::keyPressEvent(QKeyEvent *event)
{

    //键盘事件是空格键的情况下
    if(event->key()==Qt::Key_Space)
    {
        m_IsRaise=true;
        flyRaiseAnimation();
    }
}

void BirdItem::onRefreshBird()
{
    update();   //刷新小鸟
}

void BirdItem::onFlyRaiseAnimationFinished()
{
    m_IsLandFall=true;
    flyLandfallAnimation();
}
