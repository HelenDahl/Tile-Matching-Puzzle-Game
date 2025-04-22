#ifndef MAINMENU_H
#define MAINMENU_H

#include <QtWidgets>
#include <QPainter>
#include <QIcon>
#include <QPixmap>
#include "config.h"
#include "usersinglesetting.h"
#include "userdoublesetting.h"
#include "singlegame.h"
#include "doublegame.h"
#include "block.h"

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr);
    ~MainMenu();
    void paintEvent(QPaintEvent* event);

private:
    QLabel* gameTitle;
    QPixmap bg;
    QPushButton* startSingle;
    QPushButton* startDouble;
    QPushButton* loadGame;
    QPushButton* quitGame;
    UserSingleSetting* singleSetting;
    UserDoubleSetting* doubleSetting;
    SingleGame* singleGame;
    DoubleGame* doubleGame;

    void clickSingleButton();
    void clickDoubleButton();
    void clickLoadGame();
    void clickQuitGame();
};
#endif // MAINMENU_H
