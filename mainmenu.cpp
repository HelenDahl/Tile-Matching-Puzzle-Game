#include "mainmenu.h"

MainMenu::MainMenu(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(MENU_WIDTH, MENU_HEIGHT);
    setWindowTitle(MENU_TITLE);
    setWindowIcon(QIcon(":/res/menu_logo2.png"));

    bg.load(MENU_BACKGROUND_PATH);

    QString titleStyle = QString(
        "color: %1;"
        "font-size: %2px;"
        "font-weight: bold;"
        "font-family: 'Times New Roman';"
                             ).arg(MENU_TITLE_COLOR).arg(MENU_TITLE_FONTSIZE);

    gameTitle = new QLabel(tr(MENU_TITLE));
    gameTitle->setStyleSheet(titleStyle);

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
                              ).arg(MENU_BUTTON_COLOR).arg(MENU_BUTTON_FONTSIZE).arg(MENU_BUTTON_HOVER_COLOR).arg(MENU_BUTTON_CLICK_COLOR);

    startSingle = new QPushButton(tr("Start Single Mode Game"));
    startSingle->setCheckable(true);
    startSingle->setFixedSize(QSize(MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT));
    startSingle->setStyleSheet(buttonStyle);

    startDouble = new QPushButton(tr("Start Double Mode Game"));
    startDouble->setCheckable(true);
    startDouble->setFixedSize(QSize(MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT));
    startDouble->setStyleSheet(buttonStyle);

    loadGame = new QPushButton(tr("Load Game"));
    loadGame->setCheckable(true);
    loadGame->setFixedSize(QSize(MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT));
    loadGame->setStyleSheet(buttonStyle);

    quitGame = new QPushButton(tr("Quit Game"));
    quitGame->setCheckable(true);
    quitGame->setFixedSize(QSize(MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT));
    quitGame->setStyleSheet(buttonStyle);

    QVBoxLayout *menuLayout = new QVBoxLayout;
    menuLayout->setSizeConstraint(QLayout::SetFixedSize);
    menuLayout->addWidget(gameTitle);
    menuLayout->addWidget(startSingle);
    menuLayout->addWidget(startDouble);
    menuLayout->addWidget(loadGame);
    menuLayout->addWidget(quitGame);

    menuLayout->setSpacing(MENU_SPACING);
    menuLayout->setContentsMargins(0.5*(MENU_WIDTH-MENU_BUTTON_WIDTH), 0.5*(MENU_HEIGHT-4*MENU_BUTTON_HEIGHT-MENU_TITLE_FONTSIZE-4*MENU_SPACING)+MENU_WIDGETS_OFFSET, 0.5*(MENU_WIDTH-MENU_BUTTON_WIDTH), 0.5*(MENU_HEIGHT-4*MENU_BUTTON_HEIGHT-MENU_TITLE_FONTSIZE-4*MENU_SPACING)-MENU_WIDGETS_OFFSET);
    setLayout(menuLayout);

    connect(startSingle, &QPushButton::clicked, this, &MainMenu::clickSingleButton);
    connect(startDouble, &QPushButton::clicked, this, &MainMenu::clickDoubleButton);
    connect(loadGame, &QPushButton::clicked, this, &MainMenu::clickLoadGame);
    connect(quitGame, &QPushButton::clicked, this, &MainMenu::clickQuitGame);
}


MainMenu::~MainMenu() {}

void MainMenu::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,bg);
}

void MainMenu::clickSingleButton()
{
    close();
    singleSetting = new UserSingleSetting();
    singleSetting->show();
}

void MainMenu::clickDoubleButton()
{
    close();
    doubleSetting = new UserDoubleSetting();
    doubleSetting->show();
}

void MainMenu::clickLoadGame()
{
    QString filePath = QFileDialog::getOpenFileName(this,"保存文件","G:","File(*.txt)");
    if(filePath == "")
        return;

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Could not open file for reading.");
        return;
    }

    QTextStream fin(&file);

    int gameType;
    fin >> gameType;

    if(gameType == 1)
    {
        int in_rows;
        int in_columns;
        int in_timeLimit;
        fin >> in_rows >> in_columns >> in_timeLimit;
        singleGame = new SingleGame(in_rows, in_columns, in_timeLimit);
        fin >> singleGame->m_countDown;

        int playerX;
        int playerY;
        int playerRotation;
        int playerScore;
        fin >> playerX >> playerY >> playerRotation;
        singleGame->m_player->setPosition(playerX, playerY);
        singleGame->m_player->setRotation(playerRotation);
        fin >> playerScore;
        singleGame->m_player->setScore(playerScore);

        singleGame->m_blocks.clear();
        for(int i = 0; i < singleGame->m_rows*singleGame->m_columns; i++)
        {
            bool free;
            QString s_free;
            int type;
            int index;
            bool isChosen;
            QString s_isChosen;
            int playerIndex;
            int score;
            bool hint;
            QString s_hint;
            fin >> s_free >> type >> index >> s_isChosen >> playerIndex >> score >> s_hint;
            free = (s_free == "true");
            isChosen = (s_isChosen == "true");
            hint = (s_hint == "true");
            Block* block = new Block(type, index);
            block->m_Free = free;
            block->isChosen = isChosen;
            block->playerIndex = playerIndex;
            block->m_score = score;
            block->m_hint = hint;
            singleGame->m_blocks.push_back(block);
        }
        singleGame->layBlocks();
        singleGame->initPauseMenu();

        file.close();

        close();
        singleGame->show();
    }

    else if(gameType == 2)
    {
        int in_rows;
        int in_columns;
        int in_timeLimit;
        fin >> in_rows >> in_columns >> in_timeLimit;

        doubleGame = new DoubleGame(in_rows, in_columns, in_timeLimit);
        fin >> doubleGame->m_countDown;

        int playerX;
        int playerY;
        int playerRotaion;
        int playerScore;
        fin >> playerX >> playerY >> playerRotaion;
        doubleGame->m_player1->setPosition(playerX, playerY);
        doubleGame->m_player1->setRotation(playerRotaion);
        fin >> playerX >> playerY >> playerRotaion;
        doubleGame->m_player2->setPosition(playerX, playerY);
        doubleGame->m_player2->setRotation(playerRotaion);

        fin >> playerScore;
        doubleGame->m_player1->setScore(playerScore);
        fin >> playerScore;
        doubleGame->m_player2->setScore(playerScore);

        doubleGame->m_blocks.clear();
        for(int i = 0; i < doubleGame->m_rows*doubleGame->m_columns; i++)
        {
            bool free;
            QString s_free;
            int type;
            int index;
            bool isChosen;
            QString s_isChosen;
            int playerIndex;
            int score;
            bool hint;
            QString s_hint;
            fin >> s_free >> type >> index >> s_isChosen >> playerIndex >> score >> s_hint;
            free = (s_free == "true");
            isChosen = (s_isChosen == "true");
            hint = (s_hint == "true");
            Block* block = new Block(type, index);
            block->m_Free = free;
            block->isChosen = isChosen;
            block->playerIndex = playerIndex;
            block->m_score = score;
            block->m_hint = hint;
            doubleGame->m_blocks.push_back(block);
        }
        doubleGame->layBlocks();
        doubleGame->initPauseMenu();

        file.close();

        close();
        doubleGame->show();
    }
}

void MainMenu::clickQuitGame()
{
    close();
}

