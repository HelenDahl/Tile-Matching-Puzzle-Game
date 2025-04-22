#include "doublegameovermenu.h"

DoubleGameOverMenu::DoubleGameOverMenu(int score1, int score2, QWidget* parent)
{
    setWindowTitle(GAMEOVER_TITLE);
    m_pixmap.load(GAMEOVER_LOGO_PATH);
    m_pixmap = m_pixmap.scaled(GAMEOVER_LOGO_WIDTH, GAMEOVER_LOGO_HEIGHT);
    setFixedSize(GAMEOVER_WIDTH, GAMEOVER_HEIGHT);

    QString labelStyle = QString(
                             "color: block;"
                             "font-size: %1px;"
                             "font-weight: bold;"
                             "font-family: 'Arial';"
                             ).arg(GAMEOVER_LABEL_FONTSIZE);

    QString scoreString = QString("PLAYER 1 GOT %1 POINTS!").arg(score1);
    playerScore1 = new QLabel(scoreString);
    playerScore1->setStyleSheet(labelStyle);

    scoreString = QString("PLAYER 2 GOT %1 POINTS!").arg(score2);
    playerScore2 = new QLabel(scoreString);
    playerScore2->setStyleSheet(labelStyle);

    if(score1 == score2)
        winLabel = new QLabel("YOU ARE TIED!");
    else
    {
        int winIndex = (score1 > score2) ? 1 : 2;
        winLabel = new QLabel(QString("PLAYER %1 WIN!").arg(winIndex));
    }
    winLabel->setStyleSheet(labelStyle);

    QString buttonStyle = QString(
                              "QPushButton {"
                              "    color: white;"
                              "    background-color: %1;"
                              "    border: 2px solid white;"
                              "    font-size: %2px;"
                              "    font-family: 'Arial';"
                              "    padding: 5px;"
                              "}"
                              "QPushButton:hover {"
                              "    background-color: %3;"
                              "}"
                              "QPushButton:pressed {"
                              "    background-color: %4;"
                              "}"
                              ).arg(GAMEOVER_BUTTON_COLOR).arg(GAMEOVER_BUTTON_FONTSIZE).arg(GAMEOVER_BUTTON_HOVERCOLOR).arg(GAMEOVER_BUTTON_CLICKCOLOR);

    quitButton = new QPushButton(tr("Quit"));
    quitButton->setCheckable(true);
    quitButton->setFixedSize(QSize(GAMEOVER_BUTTON_WIDTH, GAMEOVER_BUTTON_HEIGHT));
    quitButton->setStyleSheet(buttonStyle);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setSizeConstraint(QLayout::SetFixedSize);
    layout->addWidget(playerScore1);
    layout->addWidget(playerScore2);
    layout->addWidget(winLabel);
    layout->addWidget(quitButton);
    layout->setSpacing(GAMEOVER_LAYOUT_SPACING);
    layout->setContentsMargins((width() - GAMEOVER_BUTTON_WIDTH)/2, height()/2, (width() - GAMEOVER_BUTTON_WIDTH)/2, height()/2 - GAMEOVER_LABEL_FONTSIZE - GAMEOVER_BUTTON_HEIGHT - GAMEOVER_LAYOUT_SPACING * 3);
    setLayout(layout);

    connect(quitButton, &QPushButton::clicked, this, &DoubleGameOverMenu::clickQuitButton);
}

void DoubleGameOverMenu::clickQuitButton()
{
    emit closeTheWindow();
    close();
}


void DoubleGameOverMenu::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap((GAMEOVER_WIDTH-GAMEOVER_LOGO_WIDTH)/2, 0.08*GAMEOVER_HEIGHT, m_pixmap);
}


