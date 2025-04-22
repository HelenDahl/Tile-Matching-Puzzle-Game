#include "singlegameovermenu.h"

SingleGameOverMenu::SingleGameOverMenu(int score, QWidget *parent)
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

    QString scoreString = QString("YOU GOT %1 POINTS!").arg(score);
    playerScore = new QLabel(scoreString);
    playerScore->setStyleSheet(labelStyle);

    congratulationsLabel = new QLabel(tr("CONGRATULATIONS!"));
    congratulationsLabel->setStyleSheet(labelStyle);

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
    layout->addWidget(playerScore);
    layout->addWidget(congratulationsLabel);
    layout->addWidget(quitButton);
    layout->setSpacing(GAMEOVER_LAYOUT_SPACING);
    layout->setContentsMargins((width() - GAMEOVER_BUTTON_WIDTH)/2, height()/2, (width() - GAMEOVER_BUTTON_WIDTH)/2, height()/2 - GAMEOVER_LABEL_FONTSIZE - GAMEOVER_BUTTON_HEIGHT - GAMEOVER_LAYOUT_SPACING * 2);
    setLayout(layout);

    connect(quitButton, &QPushButton::clicked, this, &SingleGameOverMenu::clickQuitButton);
}

void SingleGameOverMenu::clickQuitButton()
{
    emit closeTheWindow();
    close();
}

void SingleGameOverMenu::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap((GAMEOVER_WIDTH-GAMEOVER_LOGO_WIDTH)/2, 0.08*GAMEOVER_HEIGHT, m_pixmap);
}


