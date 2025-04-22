#ifndef SINGLEGAMEOVERMENU_H
#define SINGLEGAMEOVERMENU_H

#include <QtWidgets>
#include "config.h"
#include "singlegame.h"

class SingleGame;

class SingleGameOverMenu : public QWidget
{
    Q_OBJECT
public:
    SingleGameOverMenu(int score, QWidget *parent = nullptr);

    //GameOver pic
    QPixmap m_pixmap;

    //show the score
    QLabel* playerScore;
    QLabel* congratulationsLabel;

    QPushButton* quitButton;
    void clickQuitButton();

    void paintEvent(QPaintEvent *);

signals:
    void closeTheWindow();
};

#endif // SINGLEGAMEOVERMENU_H
