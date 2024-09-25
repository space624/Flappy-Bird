#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "GameScene.h"
#include "birdItem.h"

#include <QWidget>
#include <QParallelAnimationGroup>
#include <QMediaPlayer>
#include <QMediaPlaylist>

QT_BEGIN_NAMESPACE
namespace Ui { class GameWindow; }
QT_END_NAMESPACE

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

protected:
    //重写绘图事件
    void paintEvent(QPaintEvent* event) override;
    //重写按键事件
    void keyPressEvent(QKeyEvent* event) override;

private:
    void initControl();
    void initBackGroundMusic(); //初始化背景音乐
    void loadStyleSheet(const QString& sheetName);  //加载样式
    void startWelcome();    //游戏欢迎
    void GameOver();

private slots:
    void onStartBtnCLicked();
    void onCheckGameStstus();

private:
    Ui::GameWindow *ui;
    QMediaPlayer* m_welcomePlayer;  //欢迎音乐
    QMediaPlaylist* m_welcomePlayerList;    //片头音乐播放列表
    GameScene* m_scene;    //游戏场景
    BirdItem* m_birdItem;   //鸟儿对象
    bool m_startGame;   //游戏开始标志
    QParallelAnimationGroup* m_lettersGroupFading;    //渐渐消失动画组
    QTimer* m_checkGameStatusTimer; //检测游戏状态
};
#endif // GAMEWINDOW_H
