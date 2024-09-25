#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "gamescene.h"
#include "buttonitem.h"
#include "roaditem.h"
#include "pipeitem.h"

#include <QFile>
#include <QGraphicsView>
#include <QRectF>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QStyleOption>
#include <QTimer>
#include <QDebug>

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWindow)
    , m_startGame(false)
{
    ui->setupUi(this);
    initControl();
    initBackGroundMusic();
    this->setWindowTitle("飞翔的小鸟");
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::paintEvent(QPaintEvent *event)
{
    //自定义paint事件
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);

    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    if(m_startGame)
        m_birdItem->keyPressEvent(event);
}

void GameWindow::initControl()
{
    loadStyleSheet("GameWindow");   //加载样式

    const QRectF gameRect=rect();
    m_scene=new GameScene(this,gameRect);   //初始化游戏场景
    //初始化游戏试图(将场景添加到视图中)
    QGraphicsView* view=new QGraphicsView(m_scene,this);
    view->setScene(m_scene);    //设置游戏场景
    //视图无边框，透明
    view->setStyleSheet("border:none;background:transparent;");
    view->setCacheMode(QGraphicsView::CacheBackground); //设置缓存为背景模式
    startWelcome();

}

void GameWindow::initBackGroundMusic()
{
    m_welcomePlayer=new QMediaPlayer(this);


    /*  只会播放一次
    m_welcomePlayer->setMedia(QUrl("qrc:/BirdGame/Resources/sound/welcome.mp3"));
    m_welcomePlayer->setVolume(100);
    m_welcomePlayer->play();
    */

    m_welcomePlayerList=new QMediaPlaylist(this);
    m_welcomePlayerList->addMedia(QUrl("qrc:/BirdGame/Resources/sound/welcome.mp3"));
    //...添加其他播放音乐

    //循环播放当前列表中的曲目
    m_welcomePlayerList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    m_welcomePlayer->setPlaylist(m_welcomePlayerList);  //设置播放列表
    m_welcomePlayer->play();
}

//加载样式
void GameWindow::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/BirdGame/Resources/qss/"+sheetName+".css");
    if(file.open(QFile::ReadOnly))
    {
        QString strStyleSheet=this->styleSheet();   //当前样式
        strStyleSheet+=file.readAll();
        this->setStyleSheet(strStyleSheet);
        file.close();
    }
}

//启动欢迎
void GameWindow::startWelcome()
{
    RoadItem *roadItem=new RoadItem(m_scene);   //道路
    PipeItem *pipeItem=new PipeItem(m_scene);   //管道
    m_birdItem=new BirdItem(m_scene);   //小鸟
    m_checkGameStatusTimer=new QTimer(this);    //游戏状态检测
    //定时检测游戏状态
    connect(m_checkGameStatusTimer,SIGNAL(timeout()),this,SLOT(onCheckGameStstus()));

    //字幕 "飞吧，像风一样自由，无法挽留..."
    const int nLetters=15;
    struct{
        char const *pix;
        qreal initX,inity;
        qreal destX,destY;
    }letterData[nLetters]={
        {"飞", -1000, -1000, 250, 100 },
        {"吧", -800, -1000, 300, 100 },
        {",", -600, -1000, 350, 100 },
        {"像", -400, -1000, 150, 160 },
        {"风", 1000, 2000, 200, 160 },
        {"一", 800, 2000, 250, 160 },
        {"样", 600, 2000, 300, 160 },
        {"自", 400, 2000, 350, 160 },
        {"由", 200, 2000, 400, 160 },
        {",", 0, 2000, 450, 160 },
        {"无", 0, 2000, 175, 220 },
        {"法", 0, 2000, 225, 220 },
        {"挽", 0, 2000, 275, 220 },
        {"留", 0, 2000, 325, 220 },
        {"...", 0, 2000, 375, 220 }
    };

    //连续动画组
    QSequentialAnimationGroup* letterGroupMoving=new QSequentialAnimationGroup(this);

    //并行动画组
    m_lettersGroupFading=new QParallelAnimationGroup(this);

    for(int i=0;i<nLetters;i++)
    {
        //文本图形象
        QString htmlText=QString("<span style=\"font_-family:'Berlin Sans FB';font-size:38px;font-weight:600;color:#194819\">%1</span>").arg(letterData[i].pix);
        QGraphicsTextItem* letter=new QGraphicsTextItem();
        letter->setHtml(htmlText);
        letter->setPos(letterData[i].initX,letterData[i].inity);

        //文本动画属性设置
        QPropertyAnimation *moveAnimation=new QPropertyAnimation(letter,"pos",letterGroupMoving);
        moveAnimation->setEndValue(QPointF(letterData[i].destX,letterData[i].destY));   //设置字符动画结束的位置
        moveAnimation->setDuration(200);    //动画时长200ms
        moveAnimation->setEasingCurve(QEasingCurve::OutElastic);    //动画曲线效果
        letterGroupMoving->addPause(50);

        //渐渐消失动画
        QPropertyAnimation *fadeAnimation=new QPropertyAnimation(letter,"opacity",m_lettersGroupFading);
        fadeAnimation->setDuration(1000);
        fadeAnimation->setEndValue(0);
        fadeAnimation->setEasingCurve(QEasingCurve::OutQuad);

        m_scene->addItem(letter);   //往游戏场景中添加字符图形象
    }

    //启动字符动画
    letterGroupMoving->start(QAbstractAnimation::DeleteWhenStopped);    //动画停止后删除

    //按钮图形象
    QPixmap pix=QPixmap(":/BirdGame/Resources/image/startButton.png");
    ButtonItem* btnItem=new ButtonItem(pix,m_scene);
    btnItem->setPos(QPointF(240,340));

    //按钮渐渐消失动画
    QPropertyAnimation *fadeAnimation=new QPropertyAnimation(btnItem,"opacity",m_lettersGroupFading);
    fadeAnimation->setDuration(600);
    fadeAnimation->setEndValue(0);  //opacity为0就全透明，1不透明
    fadeAnimation->setEasingCurve(QEasingCurve::OutQuad);
    connect(btnItem,SIGNAL(clicked()),this,SLOT(onStartBtnCLicked()));
    connect(fadeAnimation,&QPropertyAnimation::finished,
            [this](){
       m_startGame=true;
       m_checkGameStatusTimer->start(50);
       m_birdItem->flyLandfallAnimation();

       m_welcomePlayer->stop();
       m_welcomePlayerList->clear();
       m_welcomePlayerList->addMedia(QUrl("qrc:/BirdGame/Resources/sound/background.mp3"));
       m_welcomePlayer->play();
    });
}

void GameWindow::GameOver()
{
    //哎呀,游戏失败啦!
    const int nLetters=9;

    struct{
        char const *pix;
        qreal initX,inity;
        qreal destX,destY;
    }letterData[nLetters]={
        {"哎", -1000, -1000, 250, 100 },
        {"呀", -800, -1000, 300, 100 },
        {",", -600, -1000, 350, 100 },
        {"游", -400, -1000, 150, 160 },
        {"戏", 1000, 2000, 200, 160 },
        {"失", 800, 2000, 250, 160 },
        {"败", 600, 2000, 300, 160 },
        {"啦", 400, 2000, 350, 160 },
        {"!", 200, 2000, 400, 160 }
    };

    //游戏结束文字动画组
    QParallelAnimationGroup* lettersGroupMoving=new QParallelAnimationGroup(this);

    for(int i=0;i<nLetters;i++)
    {
        //文本图形象
        QString htmlText=QString("<span style=\"font_-family:'Berlin Sans FB';font-size:38px;font-weight:600;color:#194819\">%1</span>").arg(letterData[i].pix);
        QGraphicsTextItem* letter=new QGraphicsTextItem();
        letter->setHtml(htmlText);
        letter->setPos(letterData[i].initX,letterData[i].inity);

        //文本动画属性设置
        QPropertyAnimation *moveAnimation=new QPropertyAnimation(letter,"pos",lettersGroupMoving);
        moveAnimation->setEndValue(QPointF(letterData[i].destX,letterData[i].destY));   //设置字符动画结束的位置
        moveAnimation->setDuration(200);    //动画时长200ms
        moveAnimation->setEasingCurve(QEasingCurve::OutElastic);    //动画曲线效果

        //文本图形项添加到游戏场景中去
        m_scene->addItem(letter);
    }

    lettersGroupMoving->start(QAbstractAnimation::DeleteWhenStopped);

    //将小鸟图形项移除
    m_scene->removeItem(m_birdItem);

    //游戏结束音效
    m_welcomePlayer->stop();
    m_welcomePlayerList->clear();
    m_welcomePlayer->setMedia(QUrl("qrc:/BirdGame/Resources/sound/gameover.wav"));
    m_welcomePlayer->setVolume(100);
    m_welcomePlayer->play();
}

void GameWindow::onStartBtnCLicked()
{
    //启动渐渐消失动画，动画结束后删除动画
    m_lettersGroupFading->start(QAbstractAnimation::DeleteWhenStopped);
}

void GameWindow::onCheckGameStstus()
{
    if(m_birdItem->checkIsCollided())   //小鸟是否与其他场景有冲突
        GameOver();
}

