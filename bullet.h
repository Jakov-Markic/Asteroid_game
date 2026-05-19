#ifndef BULLET_H
#define BULLET_H

#include <QPainter>

class Bullet{
public:
    float x, y;
    int size;
    float speed, rotation;
    Bullet();
    Bullet(float, float);
    void setPos(float, float);
    void setSize(int);
    void changeSpeed(float);
    void drawBullet(QPainter&);
};
Bullet::Bullet(){
    x = 0;
    y = 0;
    size = 10;
    speed = 2;
    rotation = 0;
}
Bullet::Bullet(float nSpeed, float nRotation){
    x = 0;
    y = 0;
    size = 10;
    speed = nSpeed;
    rotation = nRotation;
}

void Bullet::setPos(float posX, float posY){
    x = posX;
    y = posY;
}
void Bullet::setSize(int nSize){
    size = nSize;
}

void Bullet::changeSpeed(float nSpeed){
    speed = nSpeed;
}

void Bullet::drawBullet(QPainter& painter){

    QPointF * points = new QPointF[3];
    points[0] = QPointF( x -  ((size/2) * cos(rotation)), y -  ((size/2) * sin(rotation)));

    points[1] = QPointF( x +  ((size/2) * cos(rotation)), y +  ((size/2) * sin(rotation)));
    painter.drawPolygon(points, 2);
}



#endif // BULLET_H
