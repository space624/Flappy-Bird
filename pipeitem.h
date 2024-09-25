#ifndef PIPEITEM_H
#define PIPEITEM_H

#include <QGraphicsObject>
#include <QGraphicsScene>

class PipeItem : public QGraphicsObject
{
    Q_OBJECT
public:
    PipeItem(QGraphicsScene* scene);
     ~PipeItem();

protected:
    QRectF boundingRect() const;    //重写图形项的绘图区域
    void paint(QPainter* painter,const QStyleOptionGraphicsItem*,QWidget*); //重写绘图事件
    QPainterPath shape() const;

private:
    void startMove();   //管道移动
    void createPipeHeight();    //创建管道的高度

private:
    QGraphicsScene* m_scene;
    int m_upPipeHeight; //上管道高度
    int m_downPipeHeight;   //下管道高度

};

#endif // PIPEITEM_H
