#ifndef USERSINGLESETTING_H
#define USERSINGLESETTING_H

#include <QtWidgets>
#include "config.h"
#include "singlegame.h"

class UserSingleSetting: public QWidget
{
    Q_OBJECT

public:
    UserSingleSetting(QWidget *parent = nullptr);

private:
    QLabel* titleLabel;
    QLabel* rowsLabel;
    QLabel* columnsLabel;
    QLabel* timeLimitLabel;
    QSpinBox* input_rows;
    QSpinBox* input_columns;
    QSpinBox* input_timeLimit;
    QPushButton* startButton;
    SingleGame* newSingleGame;

    void startNewSingleGame();
};

#endif // USERSINGLESETTING_H
