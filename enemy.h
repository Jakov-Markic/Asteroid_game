#ifndef ENEMY_H
#define ENEMY_H

#include <QPainter>

class Enemy{
public:
    float x, y;
    int size, sideNum;
    float speed, rotation;
    double angle;

    Enemy();
    Enemy(float, float);

    void setPos(float, float);
    void setSize(int);
    void setSpeed(float);
    void setRotation(float);
    void setAngle(double);

    void drawEnemy(QPainter&);
};

Enemy::Enemy(){
    x = 0;
    y = 0;
    size = 15;
    sideNum = 8;
    speed = 2;
    rotation = 0;
    angle = 0;
}

Enemy::Enemy(float newX, float newY){
    x = newX;
    y = newY;
    size = 15;
    sideNum = 8;
    speed = 2;
    rotation = 0;
    angle = 0;
}
void Enemy::setPos(float newX, float newY){
    x = newX;
    y = newY;
}
void Enemy::setSize(int nSize){
    size = nSize;
}
void Enemy::setRotation(float nRotation){
    rotation = nRotation;
}
void Enemy::setSpeed(float nSpeed){
    speed = nSpeed;
}
void Enemy::setAngle(double nAngle){
    angle = nAngle;
}

void Enemy::drawEnemy(QPainter& painter){
    QPointF * points = new QPointF[sideNum];
    for(int i = 0; i < sideNum; i++){
        points[i] = QPointF(x - (size) * cos((M_PI / ((float)sideNum / 2)) * i),
                            y - (size) * sin((M_PI / ((float)sideNum / 2)) * i));
    }


    painter.drawPolygon(points, sideNum);
}

#endif // ENEMY_H
