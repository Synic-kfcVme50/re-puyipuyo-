#include "mainwindow.h"
#include<QRandomGenerator>

MainWindow::MainWindow(QWidget*parent):QMainWindow(parent){
    setFixedSize(COL*CELL+100,ROW*CELL+50);

    for(int i=0;i<ROW;i++){
        for(int j=0;j<COL;j++){
            map[i][j]=EMPTY;
        }
    }
    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,&MainWindow::autoDrop);
    timer->start(700);

    //停顿计时器
    chainTimer=new QTimer(this);
    chainTimer->setSingleShot(true);
    connect(chainTimer,&QTimer::timeout,this,&MainWindow::chainNext);

    isChaining=false;



    newPuyo();

}
MainWindow::~MainWindow(){

}
//新的噗哟出生
// 初始化新噗哟
void MainWindow::newPuyo(){
    curX = COL/2;
    curY = 1; // 第一行或第二行

    color1 = QRandomGenerator::global()->bounded(1,5);
    color2 = QRandomGenerator::global()->bounded(1,5);
    curDir = UP; // 默认竖向生成

    int rotX = curX + dx[curDir];
    int rotY = curY + dy[curDir];

    // 确保生成位置合法
    if(!canPlacePuyo(curX, curY, rotX, rotY)){
        // 游戏结束
        timer->stop();
    }
}










