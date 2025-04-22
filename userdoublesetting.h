#ifndef USERDOUBLESETTING_H
#define USERDOUBLESETTING_H

#include <QtWidgets>
#include "config.h"
#include "doublegame.h"

class UserDoubleSetting: public QWidget
{
    Q_OBJECT

public:
    UserDoubleSetting(QWidget* parent = nullptr);

private:
    QLabel* titleLabel;
    QLabel* rowsLabel;
    QLabel* columnsLabel;
    QLabel* timeLimitLabel;
    QSpinBox* input_rows;
    QSpinBox* input_columns;
    QSpinBox* input_timeLimit;
    QPushButton* startButton;
    DoubleGame* newDoublegame;

    void startNewDoubleGame();
};

#endif // USERDOUBLESETTING_H
