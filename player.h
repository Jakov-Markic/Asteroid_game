#ifndef PLAYER_H
#define PLAYER_H

#include <QPainter>

class Player{
private:
    int x,y;
    int size;
public:
    float rotation;

    Player();

    void setPos(int, int);
    void setRotation(float);
    int getX();
    int getY();
    int getSize();

    void drawPlayer(QPainter&);
};

Player::Player(){
    x = 0;
    y = 0;
    size = 20;
    rotation = 0;
}

void Player::setPos(int posX, int posY){
    x = posX;
    y = posY;
}
void Player::setRotation(float rot){
    rotation = rot;
}

int Player::getX(){ return x;}
int Player::getY(){ return y;}
int Player::getSize(){ return size;}

void Player::drawPlayer(QPainter& painter){
    int halfSide;

    halfSide = (int)(size / 2);

    QPointF *points = new QPointF[3];
    points[0] = QPointF((int) x -  (halfSide * cos(rotation)), (int)y -  (halfSide * sin(rotation)));

    points[1] = QPointF( (int)x +  (halfSide * cos(rotation)), (int)y +  (halfSide * sin(rotation)));

    points[2] = QPointF( (int)x +  (2 * size * sin(rotation)), (int) y - (2 * size * cos(rotation)));

    painter.drawPolygon(points,3);
}

#endif // PLAYER_H
