#include "doublegame.h"

DoubleGame::DoubleGame(int rows, int columns, int timeLimit, QWidget *parent) : QWidget(parent), m_rows(rows), m_columns(columns), m_timeLimit(timeLimit)
{
    initScene();

    playGame();
}

void DoubleGame::initScene()
{
    srand( (unsigned int)time(NULL) );

    //init the window
    setFixedSize(DOUBLE_GAME_WIDTH, DOUBLE_GAME_HEIGHT);
    setWindowTitle(GAME_TITLE_DOUBLE);
    setWindowIcon(QIcon(DOUBLE_GAME_ICON));

    //game logic
    m_Timer.setInterval(GAME_RATE);
    m_countDownTimer.setInterval(1000);
    m_gameOver = false;
    m_hint = false;
    m_hintRemainTime = 0;
    m_hintBlock1 = -1;
    m_hintBlock2 = -1;
    freezePlayer1 = false;
    freezePlayer2 = false;
    freezePlayerRecorder1 = 0;
    freezePlayerRecorder2 = 0;
    dizzyPlayer1 = false;
    dizzyPlayer2 = false;
    dizzyPlayerRecorder1 = 0;
    dizzyPlayerRecorder2 = 0;
    m_tool_Recorder = 0;

    //init tool array
    for(int i = 0; i < TOOLS_MAX_NUM_DOUBLE; i++)
    {
        Tool* tool = new Tool(arc4random() % 5, 2);
        m_tools.append(tool);
    }

    generateBlocks();

    m_player1 = new Player(1);
    m_player1->m_X = 0;
    m_player1->m_Y = 0;
    m_player2 = new Player(2);
    m_player2->m_X = 0;
    m_player2->m_Y = 0;

    backgroundPixmap.load(DOUBLE_GAME_BG_PATH);
    backgroundPixmap = backgroundPixmap.scaled(DOUBLE_GAME_WIDTH, DOUBLE_GAME_HEIGHT, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    m_countDown = m_timeLimit;

    maxArea1 = 0;
    maxAreaIndex1 = -1;
    maxArea2 = 0;
    maxAreaIndex2 = -1;

    nextPlayerRect1.moveTo(0,0);
    nextPlayerRect1.setWidth(PLAYER_1_SIDELENGTH);
    nextPlayerRect1.setHeight(PLAYER_1_SIDELENGTH);
    nextPlayerRect2.moveTo(0,0);
    nextPlayerRect2.setWidth(PLAYER_2_SIDELENGTH);
    nextPlayerRect2.setHeight(PLAYER_2_SIDELENGTH);

    layBlocks();

    //init pause menu
    m_pauseButton.load(GAME_PAUSE_ICON);
    m_havePausedButton.load(GAME_HAVE_PAUSED_ICON);
    m_hoverPauseButton.load(GAME_PAUSE_HOVER_ICON);
    m_pauseRect = QRect(800, 40, 50, 50);
    m_paused = false;

    initPauseMenu();

    //init player pos
    int areaX = (1 - DOUBLE_GAME_BOX_WIDTH_RATIO) * 0.5 * DOUBLE_GAME_WIDTH;
    int areaY = (1 - DOUBLE_GAME_BOX_HEIGHT_RATIO) * 0.75 * DOUBLE_GAME_HEIGHT;
    int areaHeight = DOUBLE_GAME_BOX_HEIGHT_RATIO * DOUBLE_GAME_HEIGHT;
    int blockAreaWidth = m_columns * (BLOCK_WIDTH + 2);
    int blockX = (width() - blockAreaWidth) / 2;
    m_player1->m_X = areaX + arc4random() % (blockX - areaX - m_player1->m_Rect.width() + 1);
    m_player1->m_Y = areaY + arc4random() % (areaHeight - m_player1->m_Rect.width() + 1);
    m_player1->m_Rect.moveTo(m_player1->m_X, m_player1->m_Y);
    m_player2->m_X = width() / 2+blockAreaWidth / 2 + arc4random()%(blockX - areaX - m_player2->m_Rect.width() + 1);
    m_player2->m_Y = areaY + arc4random() % (areaHeight - m_player2->m_Rect.width() + 1);
    m_player2->m_Rect.moveTo(m_player2->m_X, m_player2->m_Y);
}

void DoubleGame::initPauseMenu()
{
    m_pauseOverlay = new QWidget(this);
    m_pauseOverlay->setGeometry(0, 0, width(), height());
    m_pauseOverlay->setStyleSheet("background-color: rgba(255, 255, 255, 0.8);");
    m_pauseOverlay->hide();
    setPauseLayout();
}

void DoubleGame::playGame()
{
    m_Timer.start();
    m_countDownTimer.start();

    connect(&m_Timer, &QTimer::timeout, [=](){
        if(!m_gameOver && !m_paused)
        {
            m_player1->updateImage();
            m_player2->updateImage();

            updatePlayerPosition();

            giveHint();
            placeTools();
            pickupTool();

            update();
        }
    });

    connect(&m_countDownTimer, &QTimer::timeout, [=](){
        if(!m_paused && !m_paused)
        {
            updateCountDownTimer();
        }
    });
}

void DoubleGame::generateBlocks()
{
    QVector<int> blockTypes;

    int blockType = 1;
    int i;
    for(i = 0; i < m_rows * m_columns / 2; i++)
    {
        //all types of blocks are gone, so let's start with the first one
        if(blockType > MAX_BLOCK_TYPES)
            blockType = 1;

        //add in pairs
        blockTypes.append(blockType);
        blockTypes.append(blockType);

        blockType++;
    }

    //when m_rows and m_columns are both odd, one square is missing
    if(2*i < m_rows * m_columns)
        blockTypes.append(0);

    std::shuffle(blockTypes.begin(), blockTypes.end(), *QRandomGenerator::global());

    for(i = 0; i < blockTypes.size(); i++)
    {
        Block* block = new Block(blockTypes[i], i);
        m_blocks.append(block);
    }
}

void DoubleGame::layBlocks()
{
    if(m_layout)
    {
        QLayoutItem* item;
        while((item = m_layout->takeAt(0)) != nullptr)
        {
            QWidget *widgetInLayout = item->widget();
            if(widgetInLayout)
            {
                widgetInLayout->hide();
                delete widgetInLayout;
            }
            delete item;
        }
        delete m_layout;
    }

    m_layout = new QGridLayout(this);
    m_layout->setSpacing(2);
    int count = 0;
    for (int row = 0; row < m_rows; ++row) {
        for (int col = 0; col < m_columns; ++col) {
            m_layout->addWidget(m_blocks[count], row, col);
            count++;
        }
    }

    m_layout->setAlignment(Qt::AlignCenter);
    setLayout(m_layout);
}

void DoubleGame::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.drawPixmap(0,0,backgroundPixmap);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(255, 255, 255, 200));
    painter.drawRect((1 - DOUBLE_GAME_BOX_WIDTH_RATIO) * 0.5 * DOUBLE_GAME_WIDTH, (1 - DOUBLE_GAME_BOX_HEIGHT_RATIO) * 0.75 * DOUBLE_GAME_HEIGHT, DOUBLE_GAME_BOX_WIDTH_RATIO * DOUBLE_GAME_WIDTH, DOUBLE_GAME_BOX_HEIGHT_RATIO * DOUBLE_GAME_HEIGHT);

    painter.drawPixmap(m_player1->m_X, m_player1->m_Y, m_player1->m_pixmap);
    painter.drawPixmap(m_player2->m_X, m_player2->m_Y, m_player2->m_pixmap);

    QFont countDownFont("Arial", DOUBLE_GAME_TIMER_FONTSIZE);
    painter.setFont(countDownFont);
    painter.drawText(DOUBLE_GAME_TIME_X, DOUBLE_GAME_TIME_Y, QString("Time Left: %1s").arg(m_countDown));

    QFont scoreFont("Arial", DOUBLE_GAME_SCORE_FONTSIZE);
    painter.setFont(scoreFont);
    painter.drawText(DOUBLE_GAME_PLAYER_SCORE1_X, DOUBLE_GAME_PLAYER_SCORE1_Y, QString("Player1 Score: %1").arg(m_player1->m_score));
    painter.drawText(DOUBLE_GAME_PLAYER_SCORE2_X, DOUBLE_GAME_PLAYER_SCORE2_Y, QString("Player2 Score: %1").arg(m_player2->m_score));

    if(!m_paused)
        painter.drawPixmap(m_pauseRect, m_pauseButton);
    else
        painter.drawPixmap(m_pauseRect, m_havePausedButton);

    for(auto tool : m_tools)
    {
        if(tool->m_Free)
        {
            switch(tool->m_type)
            {
            case 0:
                painter.setBrush(QBrush(QColor(236, 231, 53)));
                break;
            case 1:
                painter.setBrush(QBrush(QColor(53, 191, 237)));
                break;
            case 2:
                painter.setBrush(QBrush(QColor(248, 144, 216)));
                break;
            case 3:
                painter.setBrush(QBrush(QColor(213, 146, 121)));
                break;
            case 4:
                painter.setBrush(QBrush(QColor(122, 236, 254)));
                break;
            }
            painter.drawEllipse(tool->m_X - (TOOLS_ELIPSIZE-TOOLS_SIZE) / 2, tool->m_Y - (TOOLS_ELIPSIZE-TOOLS_SIZE) / 2, TOOLS_ELIPSIZE, TOOLS_ELIPSIZE);
            painter.drawPixmap(tool->m_X, tool->m_Y, tool->m_pixmap);
        }
    }

    if(!m_drawLines.empty())
    {
        // painter.setPen(QPen(Qt::red,4));
        int lineNum = m_drawLines.size();
        for(int i = 0; i < lineNum; i++)
        {
            //take out the lines to draw
            auto [lineIndexes, blockIndex1, blockIndex2, remainTime] = m_drawLines.front();
            m_drawLines.pop();

            if(remainTime < 0)
            {
                m_blocks[blockIndex1]->m_Free = false;
                m_blocks[blockIndex2]->m_Free = false;
                continue;
            }

            int drawPlayerIndex = m_blocks[blockIndex1]->playerIndex;
            if(drawPlayerIndex == 1)
                painter.setPen(QPen(Qt::red,4));
            else if(drawPlayerIndex == 2)
                painter.setPen(QPen(Qt::blue, 4));

            QRect zeroBlock = m_blocks[0]->getBoundary();
            int zeroX = zeroBlock.x() + BLOCK_WIDTH / 2;
            int zeroY = zeroBlock.y() + BLOCK_HEIGHT / 2;
            int lastRow = -2;
            int lastCol = -2;
            for(auto [row, col] : lineIndexes)
            {
                if(lastRow == -2 && lastCol == -2)
                {
                    lastRow = row;
                    lastCol = col;
                    continue;
                }
                painter.drawLine(zeroX+lastCol * (BLOCK_WIDTH+2), zeroY + lastRow * (BLOCK_HEIGHT + 2), zeroX + col * (BLOCK_WIDTH + 2), zeroY + row * (BLOCK_HEIGHT + 2));
                lastRow = row;
                lastCol = col;
            }

            m_drawLines.push({lineIndexes, blockIndex1, blockIndex2, remainTime - 1});
        }
    }
}

void DoubleGame::keyPressEvent(QKeyEvent *event)
{
    if(m_gameOver)
        return;

    keysPressed.insert(event->key());
}

void DoubleGame::updatePlayerPosition()
{
    if(m_gameOver)
        return;

    if(freezePlayer1)
        freezePlayerRecorder1--;
    if(freezePlayer2)
        freezePlayerRecorder2--;
    if(dizzyPlayer1)
        dizzyPlayerRecorder1--;
    if(dizzyPlayer2)
        dizzyPlayerRecorder2--;

    if(!freezePlayer1)
    {
        if(!dizzyPlayer1)
        {
            nextPlayerRect1.moveTo(m_player1->m_X, m_player1->m_Y);
            if(!checkCollision1())
            {
                if(keysPressed.contains(Qt::Key_W))
                    m_player1->moveBackward();
                else if(keysPressed.contains(Qt::Key_S))
                    m_player1->moveForward();
                else if(keysPressed.contains(Qt::Key_A))
                    m_player1->moveLeftward();
                else if(keysPressed.contains(Qt::Key_D))
                    m_player1->moveRightward();
            }
            else if(checkCollision1())
            {
                if(keysPressed.contains(Qt::Key_W))
                    m_player1->faceBackward();
                else if(keysPressed.contains(Qt::Key_S))
                    m_player1->faceForward();
                else if(keysPressed.contains(Qt::Key_A))
                    m_player1->faceLeftward();
                else if(keysPressed.contains(Qt::Key_D))
                    m_player1->faceRightward();
            }
        }
        else
        {
            nextPlayerRect1.moveTo(m_player1->m_X, m_player1->m_Y);
            if(!checkCollision1())
            {
                if(keysPressed.contains(Qt::Key_W))
                    m_player1->moveForward();
                else if(keysPressed.contains(Qt::Key_S))
                    m_player1->moveBackward();
                else if(keysPressed.contains(Qt::Key_A))
                    m_player1->moveRightward();
                else if(keysPressed.contains(Qt::Key_D))
                    m_player1->moveLeftward();
            }
            else if(checkCollision1())
            {
                if(keysPressed.contains(Qt::Key_W))
                    m_player1->faceForward();
                else if(keysPressed.contains(Qt::Key_S))
                    m_player1->faceBackward();
                else if(keysPressed.contains(Qt::Key_A))
                    m_player1->faceRightward();
                else if(keysPressed.contains(Qt::Key_D))
                    m_player1->faceLeftward();
            }
        }
    }

    if(!freezePlayer2)
    {
        if(!dizzyPlayer2)
        {
            nextPlayerRect2.moveTo(m_player2->m_X, m_player2->m_Y);
            if(!checkCollision2())
            {
                if(keysPressed.contains(Qt::Key_Up))
                    m_player2->moveBackward();
                else if(keysPressed.contains(Qt::Key_Down))
                    m_player2->moveForward();
                else if(keysPressed.contains(Qt::Key_Left))
                    m_player2->moveLeftward();
                else if(keysPressed.contains(Qt::Key_Right))
                    m_player2->moveRightward();
            }
            else if(checkCollision2())
            {
                if(keysPressed.contains(Qt::Key_Up))
                    m_player2->faceBackward();
                else if(keysPressed.contains(Qt::Key_Down))
                    m_player2->faceForward();
                else if(keysPressed.contains(Qt::Key_Left))
                    m_player2->faceLeftward();
                else if(keysPressed.contains(Qt::Key_Right))
                    m_player2->faceRightward();
            }
        }

        else
        {
            nextPlayerRect2.moveTo(m_player2->m_X, m_player2->m_Y);
            if(!checkCollision2())
            {
                if(keysPressed.contains(Qt::Key_Up))
                    m_player2->moveForward();
                else if(keysPressed.contains(Qt::Key_Down))
                    m_player2->moveBackward();
                else if(keysPressed.contains(Qt::Key_Left))
                    m_player2->moveRightward();
                else if(keysPressed.contains(Qt::Key_Right))
                    m_player2->moveLeftward();
            }
            else if(checkCollision2())
            {
                if(keysPressed.contains(Qt::Key_Up))
                    m_player2->faceForward();
                else if(keysPressed.contains(Qt::Key_Down))
                    m_player2->faceBackward();
                else if(keysPressed.contains(Qt::Key_Left))
                    m_player2->faceRightward();
                else if(keysPressed.contains(Qt::Key_Right))
                    m_player2->faceLeftward();
            }
        }
    }

    if(freezePlayerRecorder1 < 0)
    {
        freezePlayer1 = false;
        freezePlayerRecorder1 = 0;
    }
    if(freezePlayerRecorder2 < 0)
    {
        freezePlayer2 = false;
        freezePlayerRecorder2 = 0;
    }
    if(dizzyPlayerRecorder1 < 0)
    {
        dizzyPlayer1 = false;
        dizzyPlayerRecorder1 = 0;
    }
    if(dizzyPlayerRecorder2 < 0)
    {
        dizzyPlayer2 = false;
        dizzyPlayerRecorder2 = 0;
    }
}

void DoubleGame::keyReleaseEvent(QKeyEvent *event)
{
    if(m_gameOver)
        return;

    keysPressed.remove(event->key());

    if(m_gameOver)
        return;

    //if the key released is a directional key, the player stop function is called
    switch(event->key())
    {
    case Qt::Key_W:
    case Qt::Key_S:
    case Qt::Key_A:
    case Qt::Key_D:
        m_player1->stopMoving();
        break;
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_Left:
    case Qt::Key_Right:
        m_player2->stopMoving();
        break;
    default:
        QWidget::keyReleaseEvent(event);
    }
}

void DoubleGame::mousePressEvent(QMouseEvent *event)
{
    if(m_gameOver)
        return;

    if(event->button() == Qt::LeftButton)
    {
        QPoint clickPos = event->pos();
        if(m_pauseRect.contains(clickPos))
        {
            m_paused = true;
            update();
            pause();
        }
    }
}

void DoubleGame::updateCountDownTimer()
{
    if(m_countDown > 0)
    {
        m_countDown--;
        update();
    }
    else
    {
        gameOver();
    }
}

void DoubleGame::updateScore(int score, int playerIndex)
{
    if(playerIndex == 1)
        m_player1->m_score += score;
    else if(playerIndex == 2)
        m_player2->m_score += score;
    update();
}

void DoubleGame::pause()
{
    m_Timer.stop();
    m_countDownTimer.stop();
    m_pauseOverlay->show();
}

void DoubleGame::gameOver()
{
    m_gameOver = true;
    m_countDownTimer.stop();
    m_gameOverMenu = new DoubleGameOverMenu(m_player1->m_score, m_player2->m_score);
    m_gameOverMenu->show();

    connect(m_gameOverMenu, &DoubleGameOverMenu::closeTheWindow, this, &DoubleGame::close);
    // close();
}

void DoubleGame::removeChosen()
{
    for(int i = 0; i < m_blocks.size(); i++)
    {
        if(m_blocks[i]->m_Free && m_blocks[i]->isChosen)
        {
            m_blocks[i]->isChosen = false;
            m_blocks[i]->playerIndex = -1;
        }
        if(m_blocks[i]->m_Free && m_blocks[i]->m_hint)
        {
            m_blocks[i]->m_hint = false;
        }
    }
}

bool DoubleGame::checkCollision1()
{
    if(!dizzyPlayer1)
    {
        if(keysPressed.contains(Qt::Key_W))
            nextPlayerRect1.moveTo(m_player1->m_X, m_player1->m_Y - PLAYER_SPEED);
        else if(keysPressed.contains(Qt::Key_S))
            nextPlayerRect1.moveTo(m_player1->m_X, m_player1->m_Y + PLAYER_SPEED);
        else if(keysPressed.contains(Qt::Key_A))
            nextPlayerRect1.moveTo(m_player1->m_X - PLAYER_SPEED, m_player1->m_Y);
        else if(keysPressed.contains(Qt::Key_D))
            nextPlayerRect1.moveTo(m_player1->m_X + PLAYER_SPEED, m_player1->m_Y);
    }
    else
    {
        if(keysPressed.contains(Qt::Key_W))
            nextPlayerRect1.moveTo(m_player1->m_X, m_player1->m_Y + PLAYER_SPEED);
        else if(keysPressed.contains(Qt::Key_S))
            nextPlayerRect1.moveTo(m_player1->m_X, m_player1->m_Y - PLAYER_SPEED);
        else if(keysPressed.contains(Qt::Key_A))
            nextPlayerRect1.moveTo(m_player1->m_X + PLAYER_SPEED, m_player1->m_Y);
        else if(keysPressed.contains(Qt::Key_D))
            nextPlayerRect1.moveTo(m_player1->m_X - PLAYER_SPEED, m_player1->m_Y);
    }

    if((nextPlayerRect1.x() < (1-DOUBLE_GAME_BOX_WIDTH_RATIO) * 0.5 * DOUBLE_GAME_WIDTH) || (nextPlayerRect1.x() > (1-DOUBLE_GAME_BOX_WIDTH_RATIO) * 0.5 * DOUBLE_GAME_WIDTH + DOUBLE_GAME_BOX_WIDTH_RATIO * DOUBLE_GAME_WIDTH) || (nextPlayerRect1.y() < (1-DOUBLE_GAME_BOX_HEIGHT_RATIO) * 0.75 * DOUBLE_GAME_HEIGHT) || (nextPlayerRect1.y() > (1 - DOUBLE_GAME_BOX_HEIGHT_RATIO) * 0.75 * DOUBLE_GAME_HEIGHT + DOUBLE_GAME_BOX_HEIGHT_RATIO * DOUBLE_GAME_HEIGHT))
        return true;

    for(int i = 0; i < m_blocks.size(); i++)
    {
        if(m_blocks[i]->m_Free && nextPlayerRect1.intersects(m_blocks[i]->getBoundary()))
        {
            QRect intersection = nextPlayerRect1.intersected(m_blocks[i]->getBoundary());
            int area = intersection.width() * intersection.height();
            if(area > maxArea1)
            {
                maxArea1 = area;
                maxAreaIndex1 = i;
            }
        }
    }

    if(maxArea1 > 0 && maxAreaIndex1 != -1)
    {
        //the player has stopped and is ready to select blocks
        if(m_player1->m_readyToChoose)
        {
            //the player has not selected the block, the block has not been selected
            if(m_player1->m_ChosenBlock == -1 && m_blocks[maxAreaIndex1]->isChosen == false)
            {
                //the player select the block
                m_player1->m_ChosenBlock = maxAreaIndex1;
                //the block is chosen
                m_blocks[maxAreaIndex1]->playerIndex = 1;
                m_blocks[maxAreaIndex1]->isChosen = true;
            }
            //the player has selected a block, try to match them
            else if(m_blocks[maxAreaIndex1]->isChosen == false && m_blocks[m_player1->m_ChosenBlock]->m_type == m_blocks[maxAreaIndex1]->m_type && m_player1->m_ChosenBlock != maxAreaIndex1)  //玩家已选择的方块类型和此次选择的方块类型一致
            {
                std::vector<std::pair<int, int> > linePoints;
                //determine whether it can be connected by a route within two bends
                if(canBeConnected(m_player1->m_ChosenBlock, maxAreaIndex1, linePoints) || canBeConnected(maxAreaIndex1, m_player1->m_ChosenBlock, linePoints))
                {
                    //pass it to the drawing function and eliminate the two squares
                    m_drawLines.push({linePoints, m_player1->m_ChosenBlock, maxAreaIndex1, BLOCK_REMAINTIME});
                    m_blocks[maxAreaIndex1]->playerIndex = 1;
                    m_blocks[maxAreaIndex1]->isChosen = true;

                    //player1 back to the init state
                    m_player1->m_ChosenBlock = -1;
                    m_player1->m_readyToChoose = false;

                    //update score
                    updateScore(m_blocks[maxAreaIndex1]->m_score, 1);
                }
                else
                {
                    //unable to connect, deselect the last block and select this block
                    m_blocks[m_player1->m_ChosenBlock]->isChosen = false;
                    m_blocks[m_player1->m_ChosenBlock]->playerIndex = -1;
                    m_blocks[maxAreaIndex1]->isChosen = true;
                    m_blocks[maxAreaIndex1]->playerIndex = 1;
                    m_player1->m_ChosenBlock = maxAreaIndex1;
                }
            }
            else if(m_blocks[maxAreaIndex1]->isChosen == false && m_blocks[m_player1->m_ChosenBlock]->m_type != m_blocks[maxAreaIndex1]->m_type)
            {
                //the type does not match, deselect the last block and select this block
                m_blocks[m_player1->m_ChosenBlock]->isChosen = false;
                m_blocks[m_player1->m_ChosenBlock]->playerIndex = -1;
                m_blocks[maxAreaIndex1]->isChosen = true;
                m_blocks[maxAreaIndex1]->playerIndex = 1;
                m_player1->m_ChosenBlock = maxAreaIndex1;
            }
        }
        maxArea1 = 0;
        maxAreaIndex1 = -1;
        return true;
    }
    return false;
}

bool DoubleGame::checkCollision2()
{
    if(!dizzyPlayer2)
    {
        if(keysPressed.contains(Qt::Key_Up))
            nextPlayerRect2.moveTo(m_player2->m_X, m_player2->m_Y - PLAYER_SPEED);
        else if(keysPressed.contains(Qt::Key_Down))
            nextPlayerRect2.moveTo(m_player2->m_X, m_player2->m_Y + PLAYER_SPEED);
        else if(keysPressed.contains(Qt::Key_Left))
            nextPlayerRect2.moveTo(m_player2->m_X - PLAYER_SPEED, m_player2->m_Y);
        else if(keysPressed.contains(Qt::Key_Right))
            nextPlayerRect2.moveTo(m_player2->m_X + PLAYER_SPEED, m_player2->m_Y);
    }
    else
    {
        if(keysPressed.contains(Qt::Key_Up))
            nextPlayerRect2.moveTo(m_player2->m_X, m_player2->m_Y + PLAYER_SPEED);
        else if(keysPressed.contains(Qt::Key_Down))
            nextPlayerRect2.moveTo(m_player2->m_X, m_player2->m_Y - PLAYER_SPEED);
        else if(keysPressed.contains(Qt::Key_Left))
            nextPlayerRect2.moveTo(m_player2->m_X + PLAYER_SPEED, m_player2->m_Y);
        else if(keysPressed.contains(Qt::Key_Right))
            nextPlayerRect2.moveTo(m_player2->m_X - PLAYER_SPEED, m_player2->m_Y);
    }

    if((nextPlayerRect2.x() < (1-DOUBLE_GAME_BOX_WIDTH_RATIO) * 0.5 * DOUBLE_GAME_WIDTH) || (nextPlayerRect2.x() > (1 - DOUBLE_GAME_BOX_WIDTH_RATIO) * 0.5 * DOUBLE_GAME_WIDTH + DOUBLE_GAME_BOX_WIDTH_RATIO * DOUBLE_GAME_WIDTH) || (nextPlayerRect2.y() < (1-DOUBLE_GAME_BOX_HEIGHT_RATIO) * 0.75 * DOUBLE_GAME_HEIGHT) || (nextPlayerRect2.y() > (1 - DOUBLE_GAME_BOX_HEIGHT_RATIO) * 0.75 * DOUBLE_GAME_HEIGHT + DOUBLE_GAME_BOX_HEIGHT_RATIO * DOUBLE_GAME_HEIGHT))
        return true;

    for(int i = 0; i < m_blocks.size(); i++)
    {
        if(m_blocks[i]->m_Free && nextPlayerRect2.intersects(m_blocks[i]->getBoundary()))
        {
            QRect intersection = nextPlayerRect2.intersected(m_blocks[i]->getBoundary());
            int area = intersection.width() * intersection.height();
            if(area > maxArea2)
            {
                maxArea2 = area;
                maxAreaIndex2 = i;
            }
        }
    }

    if(maxArea2 > 0 && maxAreaIndex2 != -1)
    {
        if(m_player2->m_readyToChoose)
        {
            if(m_player2->m_ChosenBlock == -1 && m_blocks[maxAreaIndex2]->isChosen == false)
            {
                m_player2->m_ChosenBlock = maxAreaIndex2;
                m_blocks[maxAreaIndex2]->playerIndex = 2;
                m_blocks[maxAreaIndex2]->isChosen = true;
            }
            else if(m_blocks[maxAreaIndex2]->isChosen == false && m_blocks[m_player2->m_ChosenBlock]->m_type == m_blocks[maxAreaIndex2]->m_type && m_player2->m_ChosenBlock != maxAreaIndex2)  //玩家已选择的方块类型和此次选择的方块类型一致
            {
                std::vector<std::pair<int, int> > linePoints;
                if(canBeConnected(m_player2->m_ChosenBlock, maxAreaIndex2, linePoints) || canBeConnected(maxAreaIndex2, m_player2->m_ChosenBlock, linePoints))
                {
                    m_drawLines.push({linePoints, m_player2->m_ChosenBlock, maxAreaIndex2, BLOCK_REMAINTIME});
                    m_blocks[maxAreaIndex2]->playerIndex = 2;
                    m_blocks[maxAreaIndex2]->isChosen = true;

                    m_player2->m_ChosenBlock = -1;
                    m_player2->m_readyToChoose = false;

                    updateScore(m_blocks[maxAreaIndex2]->m_score, 2);
                }
                else
                {
                    m_blocks[m_player2->m_ChosenBlock]->isChosen = false;
                    m_blocks[m_player2->m_ChosenBlock]->playerIndex = -1;
                    m_blocks[maxAreaIndex2]->isChosen = true;
                    m_blocks[maxAreaIndex2]->playerIndex = 2;
                    m_player2->m_ChosenBlock = maxAreaIndex2;
                }
            }
            else if(m_blocks[maxAreaIndex2]->isChosen == false && m_blocks[m_player2->m_ChosenBlock]->m_type != m_blocks[maxAreaIndex2]->m_type)
            {
                m_blocks[m_player2->m_ChosenBlock]->isChosen = false;
                m_blocks[m_player2->m_ChosenBlock]->playerIndex = -1;
                m_blocks[maxAreaIndex2]->isChosen = true;
                m_blocks[maxAreaIndex2]->playerIndex = 2;
                m_player2->m_ChosenBlock = maxAreaIndex2;
            }
        }
        maxArea2 = 0;
        maxAreaIndex2 = -1;
        return true;
    }
    return false;
}

bool DoubleGame::canBeConnected(int blockIndex1, int blockIndex2, std::vector<std::pair<int, int>> &linePoints)
{
    if(!m_blocks[blockIndex1]->m_Free || !m_blocks[blockIndex2]->m_Free)
        return false;

    int row1 = blockIndex1 / m_columns;
    int col1 = blockIndex1 % m_columns;
    int row2 = blockIndex2 / m_columns;
    int col2 = blockIndex2 % m_columns;

    //define four directions: up, down, left, and right
    const std::pair<int, int> dirs[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    //BFS queue, store the current block of the row, column, bend times, last forward direction (0,1,2,3 represent up, down, left, right; -1 Indicates that the path is not advanced.), path Indicates the index (row, column) of the path point.
    std::queue<std::tuple<int, int, int, int, std::vector<std::pair<int, int> > > > queue;
    //whether the block of an index (row, column) has been visited, 1 indicates that it has been visited, 0 indicates that it has not been visited
    std::vector<std::vector<int> > visited(m_rows+2, std::vector<int>(m_columns+2, 0));

    //initialize the BFS queue
    queue.push({row1, col1, 0, -1, {{std::pair<int, int>(row1, col1)}} });
    visited[row1 + 1][col1 + 1] = 1;

    while(!queue.empty())
    {
        auto [currentRow, currentCol, bends, lastDir, path] = queue.front();
        queue.pop();

        std::vector<std::pair<int, int> > newPath;
        int newBends;

        for(int i = 0; i < 4; i++)
        {
            auto [cr, cc] = dirs[i];
            int nextRow = currentRow + cr;
            int nextCol = currentCol + cc;
            newBends = (lastDir == -1 || lastDir == i) ? bends : (bends + 1);
            newPath = path;
            newPath.push_back({nextRow, nextCol});

            //reach four corners
            if((nextRow == -1 && nextCol == -1) || (nextRow == m_rows && nextCol == -1) || (nextRow == -1 && nextCol == m_columns) || (nextRow == m_rows && nextCol == m_columns))
                continue;

            //out of bounds
            if(nextCol < -1 || nextCol > m_columns || nextRow < -1 || nextRow > m_rows)
            {
                continue;
            }

            //the number of bending is greater than 2
            if(newBends > 2)
            {
                continue;
            }

            //find target block
            if(nextRow == row2 && nextCol == col2)
            {
                linePoints = newPath;
                return true;
            }

            //normal block
            if(nextRow >= 0 && nextRow < m_rows && nextCol >= 0 && nextCol < m_columns && m_blocks[nextRow * m_columns + nextCol]->m_Free)
            {
                continue;
            }

            //the block is visited
            if(visited[nextRow + 1][nextCol + 1] == 1)
            {
                continue;
            }

            //add to the queue
            visited[nextRow + 1][nextCol + 1] = 1;
            queue.push({nextRow, nextCol, newBends, i, newPath});
        }
    }
    return false;
}

bool DoubleGame::isSovable(int &blockIndex1, int &blockIndex2, std::vector<std::pair<int, int> > &linePoints)
{
    //detect if the map is solvable
    for(int i = 0; i < m_blocks.size(); i++)
    {
        if(!m_blocks[i]->m_Free)
            continue;

        if(!canBeChosen(i))
            continue;

        for(int j = 0; j < m_blocks.size(); j++)
        {
            if(!m_blocks[j]->m_Free)
                continue;

            if(!canBeChosen(j))
                continue;

            if(i == j)
                continue;

            if(m_blocks[i]->m_type != m_blocks[j]->m_type)
                continue;

            std::vector<std::pair<int, int> > lineP;
            if(canBeConnected(i, j, lineP))
            {
                blockIndex1 = i;
                blockIndex2 = j;
                linePoints = lineP;
                return true;
            }
        }
    }
    return false;
}

bool DoubleGame::canBeChosen(int blockIndex)
{
    int row1 = blockIndex / m_columns;
    int col1 = blockIndex % m_columns;

    //four directions
    const std::pair<int, int> dirs[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    //BFS queue, which stores rows and columns for the current block
    std::queue<std::tuple<int, int> > queue;
    //whether the block of an index (row, column) has been visited, 1 indicates that it has been visited, 0 indicates that it has not been visited
    std::vector<std::vector<int> > visited(m_rows + 2, std::vector<int>(m_columns + 2, 0));

    //init BFS queue
    queue.push({row1, col1});
    visited[row1 + 1][col1 + 1] = 1;

    while(!queue.empty())
    {
        auto [currentRow, currentCol] = queue.front();
        queue.pop();

        for(int i = 0; i < 4; i++)
        {
            auto [cr, cc] = dirs[i];
            int nextRow = currentRow + cr;
            int nextCol = currentCol + cc;

            if(nextRow >= 0 && nextRow < m_rows && nextCol >= 0 && nextCol < m_columns && m_blocks[nextRow*m_columns+nextCol]->m_Free)
            {
                continue;
            }

            if(visited[nextRow+1][nextCol+1] == 1)
            {
                continue;
            }

            if(nextCol <= -1 || nextCol >= m_columns || nextRow <= -1 || nextRow >= m_rows)
            {
                return true;
            }

            visited[nextRow+1][nextCol+1] = 1;
            queue.push({nextRow, nextCol});
        }
    }
    return false;
}

QPoint DoubleGame::getRandomEmptyPosition(const QRect &object)
{
    QRandomGenerator* generator = QRandomGenerator::global();

    //try 100 times
    for(int i = 0; i < 100; i++)
    {
        int x = (1 - DOUBLE_GAME_BOX_WIDTH_RATIO) * 0.5 * DOUBLE_GAME_WIDTH + 10 + generator->bounded(DOUBLE_GAME_BOX_WIDTH_RATIO * DOUBLE_GAME_WIDTH - object.width()-10);
        int y =  (1 - DOUBLE_GAME_BOX_HEIGHT_RATIO) * 0.75 * DOUBLE_GAME_HEIGHT + 10 + generator->bounded(DOUBLE_GAME_BOX_HEIGHT_RATIO * DOUBLE_GAME_HEIGHT - object.height()-10);

        QRect newObjectRect(x, y, object.width(), object.height());

        QRect blockArea(m_blocks[0]->getBoundary().x(), m_blocks[0]->getBoundary().y(), m_columns * (BLOCK_WIDTH + 2), m_rows * (BLOCK_HEIGHT + 2));
        //check for overlap with blocks or characters
        bool overlap = false;
        if(newObjectRect.intersects(blockArea))
            overlap = true;
        if(newObjectRect.intersects(m_player1->m_Rect))
            overlap = true;
        if(newObjectRect.intersects(m_player2->m_Rect))
            overlap = true;
        //check for overlap with other tools
        for(auto tool : m_tools)
        {
            if(newObjectRect.intersects(tool->m_Rect))
                overlap = true;
        }

        //no overlap, return
        if(!overlap)
            return QPoint(x, y);
    }

    return QPoint(-1, -1);
}

void DoubleGame::pickupTool()
{
    for(auto tool : m_tools)
    {
        if(tool->m_Free && m_player1->m_Rect.intersects(tool->m_Rect))
        {
            switch(tool->m_type)
            {
            case 0:
                m_countDown += 30;
                break;
            case 1://hint
                m_hint = true;
                m_hintRemainTime = 10000/GAME_RATE;
                break;
            case 2://shuffle
                shuffle();
                break;
            case 3://dizzy
                dizzyPlayer2 = true;
                dizzyPlayerRecorder2 = 10000/GAME_RATE;
                break;
            case 4://freeze
                freezePlayer2 = true;
                freezePlayerRecorder2 = 3000/GAME_RATE;
                break;
            }
            tool->m_Free = false;
        }

        if(tool->m_Free && m_player2->m_Rect.intersects(tool->m_Rect))
        {
            switch(tool->m_type)
            {
            case 0:
                m_countDown += 30;
                break;
            case 1://hint
                m_hint = true;
                m_hintRemainTime = 10000/GAME_RATE;
                break;
            case 2://shuffle
                shuffle();
                break;
            case 3://dizzy
                dizzyPlayer1 = true;
                dizzyPlayerRecorder1 = 10000/GAME_RATE;
                break;
            case 4://freeze
                freezePlayer1 = true;
                freezePlayerRecorder1 = 3000/GAME_RATE;
                break;
            }
            tool->m_Free = false;
        }
    }
}

void DoubleGame::giveHint()
{
    int blockIndex1;
    int blockIndex2;
    std::vector<std::pair<int, int> > lineP;

    if(!isSovable(blockIndex1, blockIndex2, lineP))
    {
        gameOver();
    }

    else if(m_hint && m_hintRemainTime > 0)
    {
        //did not give a hint before, give a hint
        if(m_hintBlock1 == -1 && m_hintBlock2 == -1)
        {
            m_hintBlock1 = blockIndex1;
            m_hintBlock2 = blockIndex2;

            m_blocks[blockIndex1]->m_hint = true;
            m_blocks[blockIndex2]->m_hint = true;
        }

        //the current prompt box has not been eliminated, do not update the hint info
        if(!m_blocks[m_hintBlock1]->isChosen && !m_blocks[m_hintBlock2]->isChosen)
        {
            m_hintRemainTime--;
            return;
        }

        //a block in the current prompt box is deleted, update the hint info
        if(!m_blocks[m_hintBlock1]->m_Free || !m_blocks[m_hintBlock2]->m_Free)
        {
            m_blocks[m_hintBlock1]->m_hint = false;
            m_blocks[m_hintBlock2]->m_hint = false;

            m_hintBlock1 = blockIndex1;
            m_hintBlock2 = blockIndex2;

            m_blocks[blockIndex1]->m_hint = true;
            m_blocks[blockIndex2]->m_hint = true;
        }

        //the current prompt box has been eliminated, update the hint info
        if(m_blocks[m_hintBlock1]->isChosen && m_blocks[m_hintBlock2]->isChosen)
        {
            m_hintBlock1 = blockIndex1;
            m_hintBlock2 = blockIndex2;

            m_blocks[blockIndex1]->m_hint = true;
            m_blocks[blockIndex2]->m_hint = true;
        }
        m_hintRemainTime--;
    }

    //the prompt time is up, cancel the prompt
    else if(m_hint && m_hintRemainTime <= 0)
    {
        if(m_blocks[m_hintBlock1]->m_hint)
            m_blocks[m_hintBlock1]->m_hint = false;
        if(m_blocks[m_hintBlock2]->m_hint)
            m_blocks[m_hintBlock2]->m_hint = false;
        m_hintBlock1 = -1;
        m_hintBlock2 = -1;
        m_hint = false;
    }
}

void DoubleGame::shuffle()
{
    QVector<Block*> tempBlocks;
    for(auto block : m_blocks)
    {
        Block* tempBlock = block;
        tempBlocks.append(tempBlock);
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(tempBlocks.begin(), tempBlocks.end(), g);

    int size = m_blocks.size();
    m_blocks.clear();

    for(int i = 0; i < size; i++)
    {
        Block* newBlock = new Block(tempBlocks[i]->m_type, i);
        newBlock->m_Free = tempBlocks[i]->m_Free;
        newBlock->isChosen = tempBlocks[i]->isChosen;
        newBlock->playerIndex = tempBlocks[i]->playerIndex;
        newBlock->m_score = tempBlocks[i]->m_score;
        newBlock->m_hint = tempBlocks[i]->m_hint;
        m_blocks.push_back(newBlock);
    }
    layBlocks();
    removeChosen();
    initPauseMenu();
    m_hintBlock1 = -1;
    m_hintBlock2 = -1;

    //players
    int areaX = (1 - DOUBLE_GAME_BOX_WIDTH_RATIO) * 0.5 * DOUBLE_GAME_WIDTH;
    int areaY = (1 - DOUBLE_GAME_BOX_HEIGHT_RATIO) * 0.75 * DOUBLE_GAME_HEIGHT;
    int areaHeight = DOUBLE_GAME_BOX_HEIGHT_RATIO * DOUBLE_GAME_HEIGHT;
    int blockAreaWidth = m_columns * (BLOCK_WIDTH + 2);
    int blockX = (width() - blockAreaWidth) / 2;
    m_player1->m_X = areaX + arc4random() % (blockX - areaX - m_player1->m_Rect.width() + 1);
    m_player1->m_Y = areaY + arc4random() % (areaHeight - m_player1->m_Rect.width() + 1);
    m_player1->m_Rect.moveTo(m_player1->m_X, m_player1->m_Y);
    m_player2->m_X = width() / 2+blockAreaWidth / 2 + arc4random() % (blockX - areaX - m_player2->m_Rect.width() + 1);
    m_player2->m_Y = areaY + arc4random()%(areaHeight - m_player2->m_Rect.width() +1);
    m_player2->m_Rect.moveTo(m_player2->m_X, m_player2->m_Y);

    //tools
    for(auto tool : m_tools)
    {
        if(tool->m_Free)
        {
            QPoint toolPos = getRandomEmptyPosition(tool->m_Rect);
            tool->m_X = toolPos.x();
            tool->m_Y = toolPos.y();
            tool->m_Rect.moveTo(tool->m_X, tool->m_Y);
        }
    }
}

void DoubleGame::placeTools()
{
    m_tool_Recorder++;
    if(m_tool_Recorder >= TOOLS_INTERVAL)
    {
        m_tool_Recorder = 0;

        QVector<Tool*> availableTools;
        for(auto tool : m_tools)
        {
            if(!tool->m_Free)
                availableTools.append(tool);
        }

        if(availableTools.empty())
            return;

        int randomIndex = QRandomGenerator::global()->bounded(availableTools.size());
        Tool *toolToPlace = availableTools[randomIndex];

        QPoint place = getRandomEmptyPosition(toolToPlace->m_Rect);
        if(place != QPoint(-1, -1))
        {
            toolToPlace->m_X = place.x();
            toolToPlace->m_Y = place.y();
            toolToPlace->m_Free = true;
            toolToPlace->m_Rect.moveTo(toolToPlace->m_X, toolToPlace->m_Y);
        }
    }
}

void DoubleGame::setPauseLayout()
{
    QString titleStyle = QString(
                             "color: %1;"
                             "font-size: %2px;"
                             "font-weight: bold;"
                             "font-family: 'Arial';"
                             ).arg(GAMEPAUSE_TITLE_COLOR).arg(GAMEPAUSE_TITLE_FONTSIZE);

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
                              ).arg(GAMEPAUSE_BUTTON_COLOR).arg(GAMEPAUSE_BUTTON_FONTSIZE).arg(GAMEPAUSE_BUTTON_HOVER_COLOR).arg(GAMEPAUSE_BUTTON_CLICK_COLOR);

    QLabel* pauseMenuTitle = new QLabel(tr(GAMEPAUSE_TITLE));
    pauseMenuTitle->setStyleSheet(titleStyle);

    QPushButton* resumeButton = new QPushButton("Resume", m_pauseOverlay);
    resumeButton->setFixedSize(GAMEPAUSE_BUTTON_WIDTH, GAMEPAUSE_BUTTON_HEIGHT);
    resumeButton->setStyleSheet(buttonStyle);
    QPushButton* saveButton = new QPushButton("Save", m_pauseOverlay);
    saveButton->setFixedSize(GAMEPAUSE_BUTTON_WIDTH, GAMEPAUSE_BUTTON_HEIGHT);
    saveButton->setStyleSheet(buttonStyle);
    QPushButton* loadButton = new QPushButton("Load", m_pauseOverlay);
    loadButton->setFixedSize(GAMEPAUSE_BUTTON_WIDTH, GAMEPAUSE_BUTTON_HEIGHT);
    loadButton->setStyleSheet(buttonStyle);
    QPushButton* quitButton = new QPushButton("Quit", m_pauseOverlay);
    quitButton->setFixedSize(GAMEPAUSE_BUTTON_WIDTH, GAMEPAUSE_BUTTON_HEIGHT);
    quitButton->setStyleSheet(buttonStyle);

    connect(resumeButton, &QPushButton::clicked, this, &DoubleGame::resumeGame);
    connect(saveButton, &QPushButton::clicked, this, &DoubleGame::saveGame);
    connect(loadButton, &QPushButton::clicked, this, &DoubleGame::loadGame);
    connect(quitButton, &QPushButton::clicked, this, &DoubleGame::quitGame);

    QVBoxLayout *overlayLayout = new QVBoxLayout(m_pauseOverlay);
    overlayLayout->addWidget(pauseMenuTitle);
    overlayLayout->addWidget(resumeButton);
    overlayLayout->addWidget(saveButton);
    overlayLayout->addWidget(loadButton);
    overlayLayout->addWidget(quitButton);

    overlayLayout->setAlignment(Qt::AlignCenter);
    m_pauseOverlay->setLayout(overlayLayout);
}

void DoubleGame::resumeGame()
{
    m_pauseOverlay->hide();
    m_paused = false;
    m_Timer.start();
    m_countDownTimer.start();
}

void DoubleGame::saveGame()
{
    QString filePath;
    QString fileName = QString("QLink file-Double ")+QDateTime::currentDateTime().toString("yyyy-MM-dd");

    filePath = QFileDialog::getSaveFileName(this,"保存文件",fileName,"Text File(*.txt)");

    if(filePath == "")
        return;

    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning("Could not open file for writing.");
        return;
    }

    QTextStream fout(&file);
    //game type
    fout << '2' << '\n';
    //the number of rows, columns and time limits of the square grid
    fout << m_rows << ' ' << m_columns << ' ' << m_timeLimit << '\n';
    //remain time
    fout << m_countDown << '\n';
    //player1, position and rotatation
    fout << m_player1->m_X << ' ' << m_player1->m_Y << ' ' << m_player1->m_Rotation << '\n';
    //player2, position and rotatation
    fout << m_player2->m_X << ' ' << m_player2->m_Y << ' ' << m_player2->m_Rotation << '\n';
    //player1, score
    fout << m_player1->m_score << '\n';
    //player2, score
    fout << m_player2->m_score << '\n';
    //blocks map
    removeChosen();
    for(auto block : m_blocks)
    {
        fout << (block->m_Free? "true":"false") << ' ' << block->m_type << ' ' << block->m_index << ' ' << (block->isChosen?"true":"false") << ' ' << block->playerIndex << ' ' << block->m_score << ' ' << (block->m_hint?"true":"false") << '\n';
    }
    file.close();
}

void DoubleGame::loadGame()
{
    QString filePath = QFileDialog::getOpenFileName(this,"保存文件","G:","File(*.txt)");
    if(filePath == "")
        return;

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("Could not open file for reading.");
        return;
    }

    QTextStream fin(&file);

    int gameType;
    fin >> gameType;

    //load double game
    if(gameType == 2)
    {
        fin >> m_rows >> m_columns >> m_timeLimit;
        fin >> m_countDown;

        int playerX;
        int playerY;
        int playerRotation;
        int playerScore;
        fin >> playerX >> playerY >> playerRotation;
        m_player1->setPosition(playerX, playerY);
        m_player1->setRotation(playerRotation);
        fin >> playerX >> playerY >> playerRotation;
        m_player2->setPosition(playerX, playerY);
        m_player2->setRotation(playerRotation);

        fin >> playerScore;
        m_player1->setScore(playerScore);
        fin >> playerScore;
        m_player2->setScore(playerScore);

        m_blocks.clear();
        for(int i = 0; i < m_rows * m_columns; i++) {
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
            m_blocks.push_back(block);
        }
        m_pauseOverlay->hide();
        m_paused = false;
        m_Timer.start();
        m_countDownTimer.start();

        layBlocks();
        removeChosen();
        initPauseMenu();
        file.close();
    }
    //load single game
    else if(gameType == 1)
    {
        int in_rows;
        int in_columns;
        int in_timeLimit;
        fin >> in_rows >> in_columns >> in_timeLimit;

        loadedSingleGame = new SingleGame(in_rows, in_columns, in_timeLimit);
        fin >> loadedSingleGame->m_countDown;

        int playerX;
        int playerY;
        int playerRotation;
        int playerScore;
        fin >> playerX >> playerY >> playerRotation;
        loadedSingleGame->m_player->setPosition(playerX, playerY);
        loadedSingleGame->m_player->setRotation(playerRotation);

        fin >> playerScore;
        loadedSingleGame->m_player->setScore(playerScore);

        loadedSingleGame->m_blocks.clear();
        for(int i = 0; i < loadedSingleGame->m_rows * loadedSingleGame->m_columns; i++)
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
            loadedSingleGame->m_blocks.push_back(block);
        }
        loadedSingleGame->layBlocks();
        loadedSingleGame->removeChosen();
        loadedSingleGame->initPauseMenu();

        file.close();

        close();
        loadedSingleGame->show();
    }
}

void DoubleGame::quitGame()
{
    close();
}




