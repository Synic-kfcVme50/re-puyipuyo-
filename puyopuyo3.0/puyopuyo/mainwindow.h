#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTimer>
#include<QPainter>
#include<QKeyEvent>
#include<vector>
#include<string>

using namespace std;

const int ROW=15;
const int COL=7;
const int CELL=45;

enum PuyoColor{
    EMPTY=0,
    RED,
    GREEN,
    BLUE,
    YELLOW
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    enum Direction{UP=0,RIGHT,DOWN,LEFT};
    Direction curDir=UP;
    bool hasActivePuyo;

    const int dx[4] = {0, 1, 0, -1};
    const int dy[4] = {-1, 0, 1, 0}; //!!!!


protected://游戏画面
    void paintEvent(QPaintEvent *event)override;
    void keyPressEvent(QKeyEvent *event)override;

private slots:
    void autoDrop();
    void chainNext();

private:
    int map[ROW][COL];
    int curX,curY;
    int color1,color2;

    QTimer *timer;

    void newPuyo();
    bool checkhere(int x,int y);
    void fixPuyo();

    QTimer*chainTimer;
    bool isChaining;
    int chainStep;

    bool canPlacePuyo(int x1,int y1,int x2,int y2);



};

#endif // MAINWINDOW_H
