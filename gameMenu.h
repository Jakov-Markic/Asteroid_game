#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class gameMenu : public QVBoxLayout{
public:
    QVBoxLayout * titleScreen;
    QLabel * label_title;
    QPushButton * button_continue;
    QPushButton * button_startNewGame;
    QPushButton * button_loadGame;
    QPushButton * button_saveGame;
    QPushButton * button_exitGame;
    QFont titleFont;
    QFont menuFont;

    gameMenu(QWidget*);
    void hideMenu();
    void showMenu();
    void pauseMenu();
    void endMenu();
};

gameMenu::gameMenu(QWidget* position){
    titleScreen = new QVBoxLayout (position);

    label_title = new QLabel;
    button_continue = new QPushButton;
    button_startNewGame = new QPushButton;
    button_loadGame = new QPushButton;
    button_saveGame = new QPushButton;
    button_exitGame = new QPushButton;

    label_title->setText("Astro defender 238");
    label_title->setAlignment(Qt::AlignHCenter);

    titleFont = label_title->font();
    titleFont.setPointSize(32);
    titleFont.setBold(true);
    label_title->setFont(titleFont);

    menuFont = button_startNewGame->font();
    menuFont.setPointSize(20);


    button_continue->setText("Continue");
    button_continue->setFont(menuFont);
    button_continue->setStyleSheet(
        "QLabel { color: white;"
        "background:transparent;"
        "}"
        "QLabel:hover { color: grey;}");

    button_startNewGame->setText("New Game");
    button_startNewGame->setFont(menuFont);
    button_startNewGame->setStyleSheet(
        "QLabel { color: white;"
        "background:transparent;"
        "}"
        "QLabel:hover { color: grey;}");

    button_loadGame->setText("Load");
    button_loadGame->setFont(menuFont);
    button_loadGame->setStyleSheet(
        "QLabel { color: white;"
        "background:transparent;"
        "}"
        "QLabel:hover { color: grey;}");

    button_saveGame->setText("Save");
    button_saveGame->setFont(menuFont);
    button_saveGame->setStyleSheet(
        "QLabel { color: white;"
        "background:transparent;"
        "}"
        "QLabel:hover { color: grey;}");

    button_exitGame->setText("Exit");
    button_exitGame->setFont(menuFont);
    button_exitGame->setStyleSheet(
        "QLabel { color: white;"
        "background:transparent;"
        "}"
        "QLabel:hover { color: grey;}");

    titleScreen->addWidget(label_title);
    titleScreen->addWidget(button_continue);
    titleScreen->addWidget(button_startNewGame);
    titleScreen->addWidget(button_loadGame);
    titleScreen->addWidget(button_saveGame);
    titleScreen->addWidget(button_exitGame);

    button_continue->hide();
    button_saveGame->hide();
}

void gameMenu::hideMenu(){
    for(int i = 0; i < titleScreen->count(); i++){
        titleScreen->itemAt(i)->widget()->hide();
    }
}
void gameMenu::showMenu(){
    for(int i = 0; i < titleScreen->count(); i++){
        titleScreen->itemAt(i)->widget()->show();
    }
    label_title->setText("Astro defender 238");
    button_saveGame->hide();
    button_continue->hide();
}
void gameMenu::pauseMenu(){
    for(int i = 0; i < titleScreen->count(); i++){
        titleScreen->itemAt(i)->widget()->show();
    }
    label_title->setText("Game Paused");
}

void gameMenu::endMenu(){
    for(int i = 0; i < titleScreen->count(); i++){
        titleScreen->itemAt(i)->widget()->show();
    }
    label_title->setText("Game Over");
    button_continue->hide();
}

#endif // GAMEMENU_H
