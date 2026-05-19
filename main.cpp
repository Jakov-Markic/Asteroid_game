#include <QApplication>
#include <QMainWindow>
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "gameMenu.h"

#include <QGraphicsView>
#include <QPainter>
#include <QLabel>
#include <QString>
#include <QList>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QVector>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QFont>
#include <QPushButton>
#include <QCoreApplication>

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>

class MainWindow : public QMainWindow
{

public:
    int width, height;
    int menuWidth, menuHeight;
    int level, score;
    int spawnLocations, numLevel;
    bool isPlayerDead, isGamePaused;

    Player player;
    QList<Bullet *> bulletList;
    QList<Bullet *>::Iterator iterBullet;
    QVector<Enemy *> enemyList;
    QVector<Enemy *>::Iterator iterEnemy;
    QLabel * levelCount;

    gameMenu * menu;
    QWidget * positionLayoutCenter;

    QMenu* SwitchMenu;
    QMenu* FileMenu;
    QAction* FileSaveAs;
    QAction* FileOpen;
    QTimer* timer;
    QTimer * enemyTimer;
    QTimer * levelUpTimer;

    MainWindow();

    void paintEvent(QPaintEvent * event) override;
    void updatePainter(void);
    void generateEnemy(void);
    void levelUp(void);

    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent * event) override;
    void keyPressEvent(QKeyEvent * event)override;

    void startMenu();
    void startNewGame();
    void continueGame();
    void exitGame();
    void playerIsDead();

    void FileSaveAsMenu();
    void FileOpenMenu();
};

MainWindow::MainWindow(){
    setMouseTracking(true);
    width = 700; height = 700;
    menuWidth = 500; menuHeight = 500;
    level = 1;
    score = 0;
    numLevel = 0;
    spawnLocations = 30;
    isPlayerDead = false;
    isGamePaused = true;
    this->setFixedSize(width, height);
    player.setPos((int)width / 2, (int)height / 2);

    positionLayoutCenter = new QWidget(this);
    positionLayoutCenter->setFixedWidth(menuWidth);
    positionLayoutCenter->setMinimumWidth(200);
    positionLayoutCenter->setFixedHeight(menuHeight);
    positionLayoutCenter->move(player.getX() -(menuWidth / 2), player.getY() - (menuHeight / 2));

    menu = new gameMenu(positionLayoutCenter);

    levelCount = new QLabel(this);
    levelCount->setText("Level: " + QString::number(level) + "      Score: " + QString::number(score));
    levelCount->move(20,40);
    levelCount->hide();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updatePainter);
    timer->start(16);
    timer->stop();

    enemyTimer = new QTimer(this);
    connect(enemyTimer, &QTimer::timeout, this, &MainWindow::generateEnemy);
    enemyTimer->start(1000);
    enemyTimer->stop();

    levelUpTimer = new QTimer(this);
    connect(levelUpTimer, &QTimer::timeout, this, &MainWindow::levelUp);
    levelUpTimer->start(5000);
    levelUpTimer->stop();

    FileSaveAs = new QAction(tr("&Save As..."), this);
    FileSaveAs->setShortcut(tr("CTRL+S"));
    connect(FileSaveAs, &QAction::triggered, this, &MainWindow::FileSaveAsMenu);

    FileOpen = new QAction(tr("&Open..."), this);
    FileOpen->setShortcut(tr("CTRL+O"));
    connect(FileOpen, &QAction::triggered, this, &MainWindow::FileOpenMenu);

    connect(menu->button_continue, &QPushButton::clicked, this, &MainWindow::continueGame);
    connect(menu->button_startNewGame, &QPushButton::clicked, this, &MainWindow::startNewGame);
    connect(menu->button_loadGame, &QPushButton::clicked, this, &MainWindow::FileOpenMenu);
    connect(menu->button_saveGame, &QPushButton::clicked, this, &MainWindow::FileSaveAsMenu);
    connect(menu->button_exitGame, &QPushButton::clicked, this, &MainWindow::exitGame);

    FileMenu = menuBar()->addMenu(tr("&File"));
    FileMenu->addAction(FileSaveAs);
    FileMenu->addAction(FileOpen);
}
void MainWindow::updatePainter(){
    float newX, newY;
    float distance;
    bool isBulletDeleted = false;
    //check every bullet colision and update position
    for(iterBullet = bulletList.begin(); iterBullet != bulletList.end();){
        newX = (*iterBullet)->x + (*iterBullet)->speed * sin((*iterBullet)->rotation + M_PI /2);
        newY = (*iterBullet)->y - (*iterBullet)->speed * cos((*iterBullet)->rotation + M_PI / 2);
        (*iterBullet)->setPos(newX, newY);
        isBulletDeleted = false;
        //check if bullet if offscreen
        if(newX == 0 || newX == this->width
            || newY == 0 || newY == this->height){
            iterBullet = bulletList.erase(iterBullet);
            isBulletDeleted = true;
        }
        //check if bullet hit any enemy
        for(iterEnemy = enemyList.begin(); iterEnemy != enemyList.end() && !isBulletDeleted; ){
            distance = sqrt(pow(newX - (*iterEnemy)->x, 2) + pow(newY - (*iterEnemy)->y, 2));

            if(distance <= (float) (*iterEnemy)->size){
                iterEnemy = enemyList.erase(iterEnemy);
                iterBullet = bulletList.erase(iterBullet);
                isBulletDeleted = true;
                this->score += 1;
            }else{
                iterEnemy++;
            }
        }

        if(!isBulletDeleted) iterBullet++;
    }
    //update position of enemy
    for(iterEnemy = enemyList.begin(); iterEnemy != enemyList.end() && !isPlayerDead; iterEnemy++){

        newX = (*iterEnemy)->x - (*iterEnemy)->speed * cos((*iterEnemy)->angle);
        newY = (*iterEnemy)->y - (*iterEnemy)->speed * sin((*iterEnemy)->angle);

        (*iterEnemy)->setPos(newX, newY);

        //check if enemy hit player
        distance = sqrt(pow(newX - player.getX(), 2) + pow(newY - player.getY(), 2));
        if(distance <= (float)player.getSize()){
            isPlayerDead = true;
            playerIsDead();
        }
    }
    if(!isPlayerDead){
        levelCount->setText("Level: " + QString::number(level)
                    + "      Score: " + QString::number(score));
        levelCount->adjustSize();
    }
    update();
}
void MainWindow::generateEnemy(){
    int positionSpawn, i = 0;
    float newX, newY;
    double angle;
    do{
        //randomly generate enemy
        positionSpawn = rand() % spawnLocations;
        angle = (2*M_PI / spawnLocations) * positionSpawn;
        newX = player.getX() + width * cos(angle);
        newY = player.getY() + height * sin(angle);

        Enemy * enemyEntity = new Enemy(newX, newY);

        enemyEntity->setAngle(angle);
        enemyList.push_back(enemyEntity);
    }while(i++ < (level / 5));
}
void MainWindow::levelUp(){
    level++;
}

void MainWindow::FileSaveAsMenu(){
    timer->stop();
    enemyTimer->stop();
    levelUpTimer->stop();

    QString fileName = QFileDialog::getSaveFileName(this, "Save As...", "", "GAME File (*.txt)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly)){
            QMessageBox::information(this, "Unable to open file", file.errorString());
            return;
        }
        QTextStream out(&file);
        out << "gaming by Jakov file" << Qt::endl;
        out << level << Qt::endl;
        out << score << Qt::endl;

    }
}
void MainWindow::FileOpenMenu(){
    timer->stop();
    enemyTimer->stop();
    levelUpTimer->stop();

    QString fileName = QFileDialog::getOpenFileName(this, "Open save...", "", "GAME File (*.txt)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::information(this, "Unable to open file", file.errorString());
            return;
        }
        QTextStream in(&file);
        QString str;
        str = in.readLine();
        if(str == "gaming by Jakov file"){
            in >> level >> Qt::endl;
            in >> score >> Qt::endl;
        }
    }
    bulletList.clear();
    enemyList.clear();
    levelCount->show();
    continueGame();
}

void MainWindow::playerIsDead(){
    isPlayerDead = true;
    isGamePaused = true;
    menu->endMenu();
    enemyList.clear();
    bulletList.clear();
    timer->stop();
    enemyTimer->stop();
    levelUpTimer->stop();
    releaseMouse();
}

void MainWindow::startMenu(){
    levelCount->hide();
    isGamePaused = true;
    isPlayerDead = true;
    menu->showMenu();
    level = 1;
    score = 0;
    enemyList.clear();
    bulletList.clear();
    releaseMouse();
}

void MainWindow::startNewGame(){
    //make everything default value
    levelCount->show();
    isGamePaused = false;
    isPlayerDead = false;
    menu->hideMenu();
    level = 1;
    score = 0;
    enemyList.clear();
    bulletList.clear();
    timer->start();
    enemyTimer->start();
    levelUpTimer->start();
    grabMouse();
}

void MainWindow::continueGame(){
    isGamePaused = false;
    isPlayerDead = false;
    menu->hideMenu();
    timer->start();
    enemyTimer->start();
    levelUpTimer->start();
    grabMouse();
}

void MainWindow::exitGame(){
    timer->stop();
    enemyTimer->stop();
    levelUpTimer->stop();
    enemyList.clear();
    bulletList.clear();
    isPlayerDead = true;
    isGamePaused = true;
    //on start menu exit
    if(menu->button_saveGame->isHidden()
        && menu->button_continue->isHidden())
        QCoreApplication::quit();
    else{
        startMenu();
    }
}

void MainWindow::paintEvent(QPaintEvent *){
    if(isPlayerDead || isGamePaused) return;
    QPainter painter(this);
    player.drawPlayer(painter);
    //draw every bullet
    for(iterBullet = bulletList.begin(); iterBullet != bulletList.end(); iterBullet++){
        (*iterBullet)->drawBullet(painter);
    }
    //draw every enemy
    for(iterEnemy = enemyList.begin(); iterEnemy != enemyList.end(); iterEnemy++){
        (*iterEnemy)->drawEnemy(painter);
    }
}
void MainWindow::mouseMoveEvent(QMouseEvent * event){
    if(isPlayerDead || isGamePaused) return;
    float angle = atan2((float) (event->pos().y() - player.getY()), (float) (event->pos().x() - player.getX()));
    player.setRotation(angle  + (M_PI/2));
}
void MainWindow::mousePressEvent(QMouseEvent *event){

    if(isPlayerDead || isGamePaused) {
        return;
    }
    if(event->button() == Qt::LeftButton){
        float angle = player.rotation - M_PI/2;
        Bullet * projectile = new Bullet(10, angle);
        //make bullet spawn at head of player
        projectile->setPos(player.getX() +  (2 * player.getSize() * sin(player.rotation)), player.getY() - (2 * player.getSize() * cos(player.rotation)));
        bulletList.push_back(projectile);
    }
}

void MainWindow::keyPressEvent(QKeyEvent * event){

    if(event->key() == Qt::Key_Escape){

        //show pause menu
        if(menu->label_title->isHidden()){
            menu->pauseMenu();
            timer->stop();
            enemyTimer->stop();
            levelUpTimer->stop();
            isGamePaused = true;
            releaseMouse();
        }else if(!isGamePaused)
            continueGame();
    }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return a.exec();
}
