#include"mainwindow.h"
#include<QRandomGenerator>
//绘制地图和噗哟
void MainWindow::paintEvent(QPaintEvent* event){
    Q_UNUSED(event);
    QPainter p(this);

    QColor colors[] = { Qt::white, Qt::red, Qt::green, Qt::blue, Qt::yellow };

    // 画网格
    p.setPen(Qt::lightGray);
    for(int i=0; i <= ROW; i++)
        p.drawLine(0, i*CELL, COL*CELL, i*CELL);
    for(int j=0; j <= COL; j++)
        p.drawLine(j*CELL, 0, j*CELL, ROW*CELL);

    // 画固定在地图上的噗哟
    for(int i=0; i < ROW; i++){
        for(int j=0; j < COL; j++){
            int c = map[i][j];
            if(c != EMPTY){
                p.setBrush(colors[c]);
                p.drawEllipse(j*CELL+2, i*CELL+2, CELL-4, CELL-4);
            }
        }
    }

    // 画活动噗哟（只有当有活动噗哟）
    if(hasActivePuyo){
        int rotX = curX + dx[curDir];
        int rotY = curY + dy[curDir];

        // 画主噗哟
        p.setBrush(colors[color1]);
        p.drawEllipse(curX*CELL+2, curY*CELL+2, CELL-4, CELL-4);

        // 画旋转噗哟（在地图范围内）
        if(rotX >= 0 && rotX < COL && rotY >= 0 && rotY < ROW){
            p.setBrush(colors[color2]);
            p.drawEllipse(rotX*CELL+2, rotY*CELL+2, CELL-4, CELL-4);
        }
    }
}