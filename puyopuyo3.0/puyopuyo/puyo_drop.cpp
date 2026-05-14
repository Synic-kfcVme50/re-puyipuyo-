#include "mainwindow.h"
#include "clear.h"
#include<QRandomGenerator>
#include<QThread>

//检测可否落到此处
bool MainWindow::checkhere(int x,int y){
    int y2=y-1;
    int y1=y;
    if(x<0||x>=COL)
        return false;
    if(y1>=ROW)
        return false;
    if(y1>=0&&map[y1][x]!=EMPTY)
        return false;


    //防止噗哟穿透？
    if(y2>=0){
        if(y2>=ROW)
            return false;
        if(map[y2][x]!=EMPTY)
            return false;
    }
    return true;
}
//下落程序
void MainWindow::autoDrop() {
    if(!hasActivePuyo) return;

    int rotX = curX + dx[curDir];
    int rotY = curY + dy[curDir];

    // 判断下一帧是否阻挡
    bool mainBlocked = (curY + 1 >= ROW) || (map[curY + 1][curX] != EMPTY);
    bool rotBlocked  = (rotX < 0 || rotX >= COL || rotY + 1 >= ROW || map[rotY + 1][rotX] != EMPTY);

    if(!mainBlocked && !rotBlocked) {
        curY++; // 正常下落
    } else {
        fixPuyo();       // 固定到地图
        hasActivePuyo = false;
        isChaining = true;
        chainNext();     // 开始消除
    }

    update();
}


//------先把能消除的噗哟全消除-----
//AI之力

// 标记阶段
std::vector<PuyoToRemove> markedForRemoval;

void MainWindow::chainNext() {
    bool hasMore = true;

    while(hasMore) {
        // 找出所有可消除噗哟
        std::vector<PuyoToRemove> marked;
        scanAndMark(map, marked);
        if(marked.empty()) {
            hasMore = false;
            break;
        }

        // 找出当前有哪些颜色
        std::set<int> colors;
        for(auto &p : marked) {
            int color = map[p.y][p.x];
            if(color != EMPTY)
                colors.insert(color);
        }

        // 按颜色顺序消除
        for(int c : colors) {
            std::vector<PuyoToRemove> batch;
            for(auto &p : marked) {
                if(map[p.y][p.x] == c)
                    batch.push_back(p);
            }

            // 显示消除动画
            update();
            QThread::msleep(200); // 消除动画时间

            // 真正删除这一颜色
            for(auto &p : batch)
                map[p.y][p.x] = EMPTY;

            // 下落
            dropAll(map);
            update();
            QThread::msleep(150); // 下落动画
        }
    }

    isChaining = false;
    newPuyo();
}


//把噗哟固定在桌面上 2.0
enum Direction { UP = 0, RIGHT, DOWN, LEFT };
Direction curDir = UP;

const int dx[4] = {0, 1, 0, -1};
const int dy[4] = {-1, 0, 1, 0};


bool MainWindow::canPlacePuyo(int x1,int y1,int x2,int y2){
    if(x1<0 || x1>=COL || y1<0 || y1>=ROW) return false;
    if(x2<0 || x2>=COL || y2<0 || y2>=ROW) return false;
    if(map[y1][x1]!=EMPTY) return false;
    if(map[y2][x2]!=EMPTY) return false;
    return true;
}


//AI之力
void MainWindow::fixPuyo() {
    int rotX = curX + dx[curDir];
    int rotY = curY + dy[curDir];

    // 主噗哟
    if(curY >= 0 && curY < ROW && curX >= 0 && curX < COL)
        map[curY][curX] = color1;

    // 旋转噗哟
    if(rotX >= 0 && rotX < COL && rotY >= 0 && rotY < ROW)
        map[rotY][rotX] = color2;
}



//实现键盘操控噗哟移动功能
void MainWindow::keyPressEvent(QKeyEvent *e) {
    if(!hasActivePuyo) return;

    int rotX = curX + dx[curDir];
    int rotY = curY + dy[curDir];

    switch(e->key()) {
    case Qt::Key_Left:
        if(checkhere(curX-1, curY) && checkhere(rotX-1, rotY))
            curX--;
        break;

    case Qt::Key_Right:
        if(checkhere(curX+1, curY) && checkhere(rotX+1, rotY))
            curX++;
        break;

    case Qt::Key_Down:
        autoDrop();
        break;

    case Qt::Key_Up: {
        // 顺时针旋转
        Direction newDir = (Direction)((curDir + 1) % 4);
        int newRotX = curX + dx[newDir];
        int newRotY = curY + dy[newDir];

        // 检查旋转后的格子是否可用（不超出边界且为空）
        if(newRotX >= 0 && newRotX < COL && newRotY >= 0 && newRotY < ROW &&
            map[newRotY][newRotX] == EMPTY) {
            curDir = newDir;
        }
        break;
    }
    }

    update();
}


