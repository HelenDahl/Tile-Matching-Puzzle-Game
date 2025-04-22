#ifndef DOUBLEGAMEOVERMENU_H
#define DOUBLEGAMEOVERMENU_H

#include <QtWidgets>
#include "config.h"
#include "doublegame.h"

class DoubleGame;

class DoubleGameOverMenu : public QWidget
{
    Q_OBJECT
public:
    DoubleGameOverMenu(int score1, int score2, QWidget *parent = nullptr);

    //GameOver pic
    QPixmap m_pixmap;

    //show the game result
    QLabel* playerScore1;
    QLabel* playerScore2;
    QLabel* winLabel;

    QPushButton* quitButton;
    void clickQuitButton();

    void paintEvent(QPaintEvent *);

signals:
    void closeTheWindow();
};

#endif // DOUBLEGAMEOVERMENU_H
