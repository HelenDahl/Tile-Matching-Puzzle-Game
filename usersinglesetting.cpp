#include "usersinglesetting.h"

UserSingleSetting::UserSingleSetting(QWidget *parent) : QWidget(parent)
{
    setWindowTitle(CONFIG_TITLE);
    setFixedSize(CONFIG_WIDTH, CONFIG_HEIGHT);

    QString titleLabelStyle = QString(
                                  "color: %1;"
                                  "font-size: %2px;"
                                  "font-weight: bold;"
                                  "font-family: 'Times New Roman';"
                                  ).arg(CONFIG_TITLE_LABEL_COLOR).arg(CONFIG_TITLE_LABEL_FONTSIZE);

    titleLabel = new QLabel(tr(MENU_TITLE));
    titleLabel->setStyleSheet(titleLabelStyle);

    // QString inputLabelStyle = QString(
    //                             "color: %1;"
    //                             "font-size: %2px;"
    //                             "font-weight: bold;"
    //                             "font-family: 'Arial';"
    //                             ).arg(CONFIG_INPUT_LABEL_COLOR).arg(CONFIG_INPUT_LABEL_FONTSIZE);

    rowsLabel = new QLabel("Rows: ", this);
    input_rows = new QSpinBox(this);
    input_rows->setRange(MIN_ROWS, MAX_ROWS);
    input_rows->setValue(DEFAULT_ROWS);

    columnsLabel = new QLabel("Columns: ", this);
    input_columns = new QSpinBox(this);
    input_columns->setRange(MIN_COLUMNS, MAX_COLUMNS);
    input_columns->setValue(DEFAULT_COLUMNS);

    timeLimitLabel = new QLabel("Time Limit: ", this);
    input_timeLimit = new QSpinBox(this);
    input_timeLimit->setRange(MIN_TIMELIMIT, MAX_TIMELIMIT);
    input_timeLimit->setValue(DEFAULT_TIMELIMIT);

    startButton = new QPushButton("Enter Game!", this);
    startButton->setEnabled(true);

    QVBoxLayout *settingLayout = new QVBoxLayout(this);
    settingLayout->addWidget(rowsLabel);
    settingLayout->addWidget(input_rows);
    settingLayout->addWidget(columnsLabel);
    settingLayout->addWidget(input_columns);
    settingLayout->addWidget(timeLimitLabel);
    settingLayout->addWidget(input_timeLimit);
    settingLayout->addWidget(startButton);

    setLayout(settingLayout);

    connect(startButton, &QPushButton::clicked, this, &UserSingleSetting::startNewSingleGame);
}

void UserSingleSetting::startNewSingleGame()
{
    newSingleGame = new SingleGame(input_rows->value(), input_columns->value(), input_timeLimit->value());
    newSingleGame->show();
    close();
}

