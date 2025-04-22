#include "singlegame.h"
SingleGame::SingleGame(int rows, int columns, int timeLimit, QWidget *parent) : QWidget(parent), m_rows(rows), m_columns(columns), m_timeLimit(timeLimit)
{
    initScene();

    playGame();
}

void SingleGame::initScene()
{
    srand( (unsigned int)time(NULL) );

    //init the window
    setFixedSize(SINGLE_GAME_WIDTH, SINGLE_GAME_HEIGHT);
    setWindowTitle(GAME_TITLE_SINGLE);
    setWindowIcon(QIcon(SINGLE_GAME_ICON));

    //game logic
    m_Timer.setInterval(GAME_RATE);
    m_countDownTimer.setInterval(1000);
    m_gameOver = false;
    m_hint = false;
    m_hintRemainTime = 0;
    m_hintBlock1 = -1;
    m_hintBlock2 = -1;
    m_flash = false;
    m_flashRemainTime = 0;
    m_tool_Recorder = 0;

    //init tool array
    for(int i = 0; i < TOOLS_MAX_NUM_SINGLE; i++)
    {
        Tool* tool = new Tool(arc4random()%4, 1);
        m_tools.append(tool);
    }

    generateBlocks();

    m_player = new Player(1);
    m_player->m_X = 0;
    m_player->m_Y = 0;

    backgroundPixmap.load(SINGLE_GAME_BG_PATH);
    backgroundPixmap = backgroundPixmap.scaled(SINGLE_GAME_WIDTH, SINGLE_GAME_HEIGHT, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    m_countDown = m_timeLimit;

    maxArea = 0;
    maxAreaIndex = -1;

    nextPlayerRect.moveTo(0,0);
    nextPlayerRect.setWidth(PLAYER_1_SIDELENGTH);
    nextPlayerRect.setHeight(PLAYER_1_SIDELENGTH);

    layBlocks();

    //pause menu
    m_pauseButton.load(GAME_PAUSE_ICON);
    m_havePausedButton.load(GAME_HAVE_PAUSED_ICON);
    m_hoverPauseButton.load(GAME_PAUSE_HOVER_ICON);
    m_pauseRect = QRect(800, 40, 50, 50);
    m_paused = false;

    initPauseMenu();

    //init player pos
    int areaX = (1 - SINGLE_GAME_BOX_WIDTH_RATIO) * 0.5 * SINGLE_GAME_WIDTH;
    int areaY = (1 - SINGLE_GAME_BOX_HEIGHT_RATIO) * 0.75 * SINGLE_GAME_HEIGHT;
    int areaHeight = SINGLE_GAME_BOX_HEIGHT_RATIO * SINGLE_GAME_HEIGHT;
    int blockAreaWidth = m_columns * (BLOCK_WIDTH+2);
    int blockX = (width() - blockAreaWidth) / 2;
    m_player->m_X = areaX + arc4random() % (blockX - areaX - m_player->m_Rect.width() + 1);
    m_player->m_Y = areaY + arc4random() % (areaHeight - m_player->m_Rect.width() +1);
    m_player->m_Rect.moveTo(m_player->m_X, m_player->m_Y);
}

void SingleGame::initPauseMenu()
{
    if(m_pauseOverlay)
        delete m_pauseOverlay;
    m_pauseOverlay = new QWidget(this);
    m_pauseOverlay->setGeometry(0, 0, width(), height());
    m_pauseOverlay->setStyleSheet("background-color: rgba(255, 255, 255, 0.8);");
    m_pauseOverlay->hide();
    setPauseLayout();
}

void SingleGame::playGame()
{
    m_Timer.start();
    m_countDownTimer.start();

    connect(&m_Timer, &QTimer::timeout, [=](){
        if(!m_gameOver && !m_paused)
        {
            updatePlayerPosition();

            m_player->updateImage();

            giveHint();
            updateFlash();
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

void SingleGame::generateBlocks()
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
    if(2 * i < m_rows * m_columns)
        blockTypes.append(0);

    std::shuffle(blockTypes.begin(), blockTypes.end(), *QRandomGenerator::global());

    for(i = 0; i < blockTypes.size(); i++)
    {
        Block* block = new Block(blockTypes[i], i);
        m_blocks.append(block);
    }
}

void SingleGame::layBlocks()
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

void SingleGame::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.drawPixmap(0, 0, backgroundPixmap);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(255, 255, 255, 200));
    painter.drawRect((1 - SINGLE_GAME_BOX_WIDTH_RATIO) * 0.5 * SINGLE_GAME_WIDTH, (1 - SINGLE_GAME_BOX_HEIGHT_RATIO) * 0.75 * SINGLE_GAME_HEIGHT, SINGLE_GAME_BOX_WIDTH_RATIO * SINGLE_GAME_WIDTH, SINGLE_GAME_BOX_HEIGHT_RATIO * SINGLE_GAME_HEIGHT);

    painter.drawPixmap(m_player->m_X, m_player->m_Y, m_player->m_pixmap);

    QFont countDownFont("Arial", SINGLE_GAME_TIMER_FONTSIZE);
    painter.setFont(countDownFont);
    painter.drawText(100, 80, QString("Time Left: %1s").arg(m_countDown));

    QFont scoreFont("Arial", SINGLE_GAME_SCORE_FONTSIZE);
    painter.setFont(scoreFont);
    painter.drawText(500, 80, QString("Score: %1").arg(m_player->m_score));

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
                painter.setBrush(QBrush(QColor(159, 248, 143)));
                break;
            }
            painter.drawEllipse(tool->m_X - (TOOLS_ELIPSIZE-TOOLS_SIZE) / 2, tool->m_Y - (TOOLS_ELIPSIZE-TOOLS_SIZE) / 2, TOOLS_ELIPSIZE, TOOLS_ELIPSIZE);
            painter.drawPixmap(tool->m_X, tool->m_Y, tool->m_pixmap);
        }
    }

    if(!m_drawLines.empty())
    {
        painter.setPen(QPen(Qt::red, 4));
        int lineNum = m_drawLines.size();
        for(int i = 0; i < lineNum; i++)
        {
            auto [lineIndexes, blockIndex1, blockIndex2, remainTime] = m_drawLines.front();
            m_drawLines.pop();

            if(remainTime < 0)
            {
                m_blocks[blockIndex1]->m_Free = false;
                m_blocks[blockIndex2]->m_Free = false;
                continue;
            }

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
                painter.drawLine(zeroX + lastCol * (BLOCK_WIDTH + 2), zeroY + lastRow * (BLOCK_HEIGHT + 2), zeroX + col * (BLOCK_WIDTH + 2), zeroY + row * (BLOCK_HEIGHT + 2));
                lastRow = row;
                lastCol = col;
            }

            m_drawLines.push({lineIndexes, blockIndex1, blockIndex2, remainTime - 1});
        }
    }
}

void SingleGame::updatePlayerPosition()
{
    if(m_gameOver)
        return;

    nextPlayerRect.moveTo(m_player->m_X, m_player->m_Y);

    //if this move does not cause the character to crash into the block or out of the area
    if(!checkCollision())
    {
        if(keysPressed.contains(Qt::Key_W))
            m_player->moveBackward();
        else if(keysPressed.contains(Qt::Key_S))
            m_player->moveForward();
        else if(keysPressed.contains(Qt::Key_A))
            m_player->moveLeftward();
        else if(keysPressed.contains(Qt::Key_D))
            m_player->moveRightward();
        update();
    }
    else if(checkCollision())
    {
        if(keysPressed.contains(Qt::Key_W))
            m_player->faceBackward();
        else if(keysPressed.contains(Qt::Key_S))
            m_player->faceForward();
        else if(keysPressed.contains(Qt::Key_A))
            m_player->faceLeftward();
        else if(keysPressed.contains(Qt::Key_D))
            m_player->faceRightward();
    }
}

void SingleGame::keyPressEvent(QKeyEvent *event)
{
    if(m_gameOver)
        return;

    keysPressed.insert(event->key());
}

void SingleGame::keyReleaseEvent(QKeyEvent *event)
{
    if(m_gameOver)
        return;

    keysPressed.remove(event->key());

    //if the key released is a directional key, the player stop function is called
    switch(event->key())
    {
    case Qt::Key_W:
    case Qt::Key_S:
    case Qt::Key_A:
    case Qt::Key_D:
        m_player->stopMoving();
        break;
    default:
        QWidget::keyReleaseEvent(event);
    }
}

void SingleGame::mousePressEvent(QMouseEvent *event)
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

        //flash
        QRect availableArea((1 - SINGLE_GAME_BOX_WIDTH_RATIO) * 0.5 * SINGLE_GAME_WIDTH, (1 - SINGLE_GAME_BOX_HEIGHT_RATIO) * 0.75 * SINGLE_GAME_HEIGHT, SINGLE_GAME_BOX_WIDTH_RATIO * SINGLE_GAME_WIDTH - m_player->m_Rect.width(), SINGLE_GAME_BOX_HEIGHT_RATIO * SINGLE_GAME_HEIGHT - m_player->m_Rect.height());
        if(m_flash && availableArea.contains(clickPos))
        {
            int blockIndex = -1;
            for(int i = 0; i < m_blocks.size(); i++)
            {
                if(m_blocks[i]->m_Free && m_blocks[i]->getBoundary().contains(clickPos))
                {
                    blockIndex = i;
                    break;
                }
            }

            //clicked a block
            if(blockIndex != -1)
            {
                //if player can reach the block
                int dir = -1;
                if(canBeChosen(blockIndex, dir))
                {
                    //move the player to the block and face it
                    movePlayerToBlock(blockIndex, dir);

                    //if the player didn't choose any block, choose it
                    if(m_player->m_ChosenBlock == -1)
                    {
                        m_player->m_ChosenBlock = blockIndex;
                        m_blocks[blockIndex]->isChosen = true;
                        m_blocks[blockIndex]->playerIndex = 1;
                    }
                    //if the player chose a block, try to match them
                    else
                    {
                        tryToMatch(blockIndex);
                    }
                }
            }

            //click the space
            else
            {
                //check to see if it overlaps the square. If it overlaps, doesn't move
                QRect newPosForPlayer(event->pos().x() - PLAYER_1_SIDELENGTH / 2, event->pos().y() - PLAYER_1_SIDELENGTH / 2, PLAYER_1_SIDELENGTH, PLAYER_1_SIDELENGTH);
                for(auto block : m_blocks)
                {
                    if(block->m_Free && block->getBoundary().intersects(newPosForPlayer))
                        return;
                }
                //check accessibility: click on the peripheral area and it can be reached
                QRect blockArea(m_blocks[0]->getBoundary().x(), m_blocks[0]->getBoundary().y(), m_columns * (BLOCK_WIDTH + 2), m_rows * (BLOCK_HEIGHT + 2));
                if(!blockArea.intersects(newPosForPlayer))
                {
                    m_player->setPosition(event->pos().x() - PLAYER_1_SIDELENGTH / 2, event->pos().y() - PLAYER_1_SIDELENGTH / 2);
                }
                //check accessibility: the area inside the box, check whether it is reachable
                else
                {
                    //try to get the block index of the block neareast to the click point
                    int blockNearestIndex = -1;
                    for(int i = 0; i < m_blocks.size(); i++)
                    {
                        if(m_blocks[i]->getBoundary().intersects(newPosForPlayer))
                        {
                            blockNearestIndex = i;
                            break;
                        }
                    }

                    if(blockNearestIndex != -1)
                    {
                        //depending on whether the block can be touched, the player can reach the position
                        int dir = -1;
                        if(canBeChosen(blockNearestIndex, dir))
                            m_player->setPosition(event->pos().x() - PLAYER_1_SIDELENGTH / 2, event->pos().y() - PLAYER_1_SIDELENGTH / 2);
                    }
                }
            }
        }
    }
}

void SingleGame::updateCountDownTimer()
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

void SingleGame::updateScore(int score)
{
    m_player->m_score += score;
    update();
}

void SingleGame::updateFlash()
{
    if(m_flash)
        m_flashRemainTime--;

    if(m_flashRemainTime < 0)
    {
        m_flash = false;
        m_flashRemainTime = 0;
    }
}

void SingleGame::pause()
{
    m_Timer.stop();
    m_countDownTimer.stop();
    m_pauseOverlay->show();
}

void SingleGame::gameOver()
{
    m_gameOver = true;
    m_countDownTimer.stop();
    m_gameOverMenu = new SingleGameOverMenu(m_player->m_score);
    m_gameOverMenu->show();

    connect(m_gameOverMenu, &SingleGameOverMenu::closeTheWindow, this, &SingleGame::close);
    // close();
}

void SingleGame::removeChosen()
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

bool SingleGame::checkCollision()
{
    //move tool QRect to where the character will be in the next frame
    if(keysPressed.contains(Qt::Key_W))
        nextPlayerRect.moveTo(m_player->m_X, m_player->m_Y - PLAYER_SPEED);
    else if(keysPressed.contains(Qt::Key_S))
        nextPlayerRect.moveTo(m_player->m_X, m_player->m_Y + PLAYER_SPEED);
    else if(keysPressed.contains(Qt::Key_A))
        nextPlayerRect.moveTo(m_player->m_X - PLAYER_SPEED, m_player->m_Y);
    else if(keysPressed.contains(Qt::Key_D))
        nextPlayerRect.moveTo(m_player->m_X + PLAYER_SPEED, m_player->m_Y);

    //determine how the moved tool QRect will collide with something else
    //outside the boundary
    if((nextPlayerRect.x() < (1 - SINGLE_GAME_BOX_WIDTH_RATIO) * 0.5 * SINGLE_GAME_WIDTH) || (nextPlayerRect.x() > (1 - SINGLE_GAME_BOX_WIDTH_RATIO) * 0.5 * SINGLE_GAME_WIDTH + SINGLE_GAME_BOX_WIDTH_RATIO * SINGLE_GAME_WIDTH) || (nextPlayerRect.y() < (1 - SINGLE_GAME_BOX_HEIGHT_RATIO) * 0.75 * SINGLE_GAME_HEIGHT) || (nextPlayerRect.y() > (1 - SINGLE_GAME_BOX_HEIGHT_RATIO) * 0.75 * SINGLE_GAME_HEIGHT + SINGLE_GAME_BOX_HEIGHT_RATIO * SINGLE_GAME_HEIGHT))
        return true;

    //if collide with some blocks
    for(int i = 0; i < m_blocks.size(); i++)
    {
        //collide with available blocks
        if(m_blocks[i]->m_Free && nextPlayerRect.intersects(m_blocks[i]->getBoundary()))
        {
            //if overlapping multiple blocks, take the rectangle with the largest overlap area
            QRect intersection = nextPlayerRect.intersected(m_blocks[i]->getBoundary());

            //calculate the overlap area and take the index of the block with the largest overlap area
            int area = intersection.width() * intersection.height();
            if(area > maxArea)
            {
                maxArea = area;
                maxAreaIndex = i;
            }
        }
    }

    //if there are blocks with the greatest overlap area
    if(maxArea > 0 && maxAreaIndex != -1)
    {
        //the player has stopped and is ready to select blocks
        if(m_player->m_readyToChoose)
        {
            //the player has not selected the block
            if(m_player->m_ChosenBlock == -1)
            {
                //the player select the block
                m_player->m_ChosenBlock = maxAreaIndex;
                //the block is chosen
                m_blocks[maxAreaIndex]->playerIndex = 1;
                m_blocks[maxAreaIndex]->isChosen = true;
            }
            //the player has selected a block, try to match them
            else
            {
                tryToMatch(maxAreaIndex);
            }
        }
        maxArea = 0;
        maxAreaIndex = -1;
        return true;
    }
    return false;
}

bool SingleGame::canBeConnected(int blockIndex1, int blockIndex2, std::vector<std::pair<int, int>> &linePoints)
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
    std::vector<std::vector<int> > visited(m_rows + 2, std::vector<int>(m_columns + 2, 0));

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
            newBends = (lastDir == -1 || lastDir == i) ? bends : (bends+1);
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

void SingleGame::tryToMatch(int blockIndex)
{
    //The character selects another one when a block is already selected, determine whether the two blocks can be connected
    if(m_blocks[m_player->m_ChosenBlock]->m_type == m_blocks[blockIndex]->m_type && m_player->m_ChosenBlock != blockIndex)  //玩家已选择的方块类型和此次选择的方块类型一致
    {
        std::vector<std::pair<int, int> > linePoints;
        if(canBeConnected(m_player->m_ChosenBlock, blockIndex, linePoints) || canBeConnected(blockIndex, m_player->m_ChosenBlock, linePoints))
        {
            m_drawLines.push({linePoints, m_player->m_ChosenBlock, blockIndex, BLOCK_REMAINTIME});
            m_blocks[blockIndex]->playerIndex = 1;
            m_blocks[blockIndex]->isChosen = true;

            m_player->m_ChosenBlock = -1;
            m_player->m_readyToChoose = false;

            updateScore(m_blocks[blockIndex]->m_score);
        }
        else
        {
            m_blocks[m_player->m_ChosenBlock]->isChosen = false;
            m_blocks[m_player->m_ChosenBlock]->playerIndex = -1;
            m_blocks[blockIndex]->isChosen = true;
            m_blocks[blockIndex]->playerIndex = 1;
            m_player->m_ChosenBlock = blockIndex;
        }
    }
    else if(m_blocks[m_player->m_ChosenBlock]->m_type != m_blocks[blockIndex]->m_type)
    {
        m_blocks[m_player->m_ChosenBlock]->isChosen = false;
        m_blocks[m_player->m_ChosenBlock]->playerIndex = -1;
        m_blocks[blockIndex]->isChosen = true;
        m_blocks[blockIndex]->playerIndex = 1;
        m_player->m_ChosenBlock = blockIndex;
    }
}

bool SingleGame::isSovable(int &blockIndex1, int &blockIndex2, std::vector<std::pair<int, int> > &linePoints)
{
    for(int i = 0; i < m_blocks.size(); i++)
    {
        if(!m_blocks[i]->m_Free)
            continue;

        int chooseDir;
        if(!canBeChosen(i, chooseDir))
            continue;

        for(int j = 0; j < m_blocks.size(); j++)
        {
            if(!m_blocks[j]->m_Free)
                continue;

            if(!canBeChosen(j, chooseDir))
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

bool SingleGame::canBeChosen(int blockIndex, int& chooseDir)
{
    int row1 = blockIndex / m_columns;
    int col1 = blockIndex % m_columns;

    const std::pair<int, int> dirs[4] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};

    std::queue<std::tuple<int, int, int> > queue;
    std::vector<std::vector<int> > visited(m_rows + 2, std::vector<int>(m_columns + 2, 0));

    queue.push({row1, col1, -1});
    visited[row1 + 1][col1 + 1] = 1;

    while(!queue.empty())
    {
        auto [currentRow, currentCol, setOffDir] = queue.front();
        queue.pop();

        for(int i = 0; i < 4; i++)
        {
            auto [cr, cc] = dirs[i];
            int nextRow = currentRow + cr;
            int nextCol = currentCol + cc;

            int newDir = setOffDir;

            if(newDir == -1)
                newDir = i + 1;

            if(nextRow >= 0 && nextRow < m_rows && nextCol >= 0 && nextCol < m_columns && m_blocks[nextRow * m_columns + nextCol]->m_Free)
            {
                continue;
            }

            if(visited[nextRow + 1][nextCol + 1] == 1)
            {
                continue;
            }

            if(nextCol <= -1 || nextCol >= m_columns || nextRow <= -1 || nextRow >= m_rows)
            {
                chooseDir = newDir;
                return true;
            }

            visited[nextRow + 1][nextCol + 1] = 1;
            queue.push({nextRow, nextCol, newDir});
        }
    }
    chooseDir = -1;
    return false;
}

void SingleGame::movePlayerToBlock(int blockIndex, int dir)
{
    m_player->setRotation(dir);
    switch(dir)
    {
    case 1://up
        m_player->setPosition(m_blocks[blockIndex]->getBoundary().x() + (BLOCK_WIDTH - PLAYER_1_SIDELENGTH) / 2, m_blocks[blockIndex]->getBoundary().y() - PLAYER_1_SIDELENGTH);
        break;
    case 2://down
        m_player->setPosition(m_blocks[blockIndex]->getBoundary().x() + (BLOCK_WIDTH - PLAYER_1_SIDELENGTH) / 2, m_blocks[blockIndex]->getBoundary().y() + BLOCK_HEIGHT);
        break;
    case 3://right
        m_player->setPosition(m_blocks[blockIndex]->getBoundary().x() + BLOCK_WIDTH, m_blocks[blockIndex]->getBoundary().y() + (BLOCK_HEIGHT - PLAYER_1_SIDELENGTH) / 2);
        break;
    case 4://left
        m_player->setPosition(m_blocks[blockIndex]->getBoundary().x() - PLAYER_1_SIDELENGTH, m_blocks[blockIndex]->getBoundary().y() + (BLOCK_HEIGHT - PLAYER_1_SIDELENGTH) / 2);
        break;
    }
}

QPoint SingleGame::getRandomEmptyPosition(const QRect &object)
{
    QRandomGenerator* generator = QRandomGenerator::global();

    //try 100 times
    for(int i = 0; i < 100; i++)
    {
        int x = (1 - SINGLE_GAME_BOX_WIDTH_RATIO) * 0.5 * SINGLE_GAME_WIDTH + 10 + generator->bounded(SINGLE_GAME_BOX_WIDTH_RATIO * SINGLE_GAME_WIDTH - object.width() - 10);
        int y =  (1 - SINGLE_GAME_BOX_HEIGHT_RATIO) * 0.75 * SINGLE_GAME_HEIGHT + 10 + generator->bounded(SINGLE_GAME_BOX_HEIGHT_RATIO * SINGLE_GAME_HEIGHT - object.height() - 10);

        QRect newObjectRect(x, y, object.width(), object.height());

        QRect blockArea(m_blocks[0]->getBoundary().x(), m_blocks[0]->getBoundary().y(), m_columns * (BLOCK_WIDTH + 2), m_rows * (BLOCK_HEIGHT + 2));

        bool overlap = false;
        if(newObjectRect.intersects(blockArea))
            overlap = true;
        if(newObjectRect.intersects(m_player->m_Rect))
            overlap = true;

        for(auto tool : m_tools)
        {
            if(newObjectRect.intersects(tool->m_Rect))
                overlap = true;
        }

        if(!overlap)
            return QPoint(x, y);
    }
    return QPoint(-1, -1);
}

void SingleGame::pickupTool()
{
    for(auto tool : m_tools)
    {
        if(tool->m_Free && m_player->m_Rect.intersects(tool->m_Rect))
        {
            switch(tool->m_type)
            {
            case 0:
                m_countDown += 30;
                break;
            case 1://hint
                m_hint = true;
                m_hintRemainTime = 10000 / GAME_RATE;
                break;
            case 2://shuffle
                shuffle();
                break;
            case 3://flash
                m_flash = true;
                m_flashRemainTime = 5000 / GAME_RATE;
                break;
            }

            tool->m_Free = false;
        }
    }
}

void SingleGame::giveHint()
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

void SingleGame::shuffle()
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

    int areaX = (1 - SINGLE_GAME_BOX_WIDTH_RATIO) * 0.5 * SINGLE_GAME_WIDTH;
    int areaY = (1 - SINGLE_GAME_BOX_HEIGHT_RATIO) * 0.75 * SINGLE_GAME_HEIGHT;
    int areaHeight = SINGLE_GAME_BOX_HEIGHT_RATIO * SINGLE_GAME_HEIGHT;
    int blockAreaWidth = m_columns * (BLOCK_WIDTH + 2);
    int blockX = (width() - blockAreaWidth) / 2;
    m_player->m_X = areaX + arc4random() % (blockX - areaX - m_player->m_Rect.width() + 1);
    m_player->m_Y = areaY + arc4random() % (areaHeight - m_player->m_Rect.width() +1);
    m_player->m_Rect.moveTo(m_player->m_X, m_player->m_Y);

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

void SingleGame::placeTools()
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
            toolToPlace->m_Rect.moveTo(toolToPlace->m_X, toolToPlace->m_Y);
            toolToPlace->m_Free = true;
        }
    }
}

void SingleGame::setPauseLayout()
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

    connect(resumeButton, &QPushButton::clicked, this, &SingleGame::resumeGame);
    connect(saveButton, &QPushButton::clicked, this, &SingleGame::saveGame);
    connect(loadButton, &QPushButton::clicked, this, &SingleGame::loadGame);
    connect(quitButton, &QPushButton::clicked, this, &SingleGame::quitGame);

    QVBoxLayout *overlayLayout = new QVBoxLayout(m_pauseOverlay);
    overlayLayout->addWidget(pauseMenuTitle);
    overlayLayout->addWidget(resumeButton);
    overlayLayout->addWidget(saveButton);
    overlayLayout->addWidget(loadButton);
    overlayLayout->addWidget(quitButton);

    overlayLayout->setAlignment(Qt::AlignCenter);
    m_pauseOverlay->setLayout(overlayLayout);
}

void SingleGame::resumeGame()
{
    m_pauseOverlay->hide();
    m_paused = false;
    m_Timer.start();
    m_countDownTimer.start();
}

void SingleGame::saveGame()
{
    QString filePath;
    QString fileName = QString("QLink file-Single ")+QDateTime::currentDateTime().toString("yyyy-MM-dd");

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
    fout << '1' << '\n';
    //the number of rows, columns and time limits of the square grid
    fout << m_rows << ' ' << m_columns << ' ' << m_timeLimit << '\n';
    //remain time
    fout << m_countDown << '\n';
    //player position and rotatation
    fout << m_player->m_X << ' ' << m_player->m_Y << ' ' << m_player->m_Rotation << '\n';
    //player score
    fout << m_player->m_score << '\n';
    //blocks map
    removeChosen();
    for(auto block : m_blocks)
    {
        fout << (block->m_Free? "true":"false") << ' ' << block->m_type << ' ' << block->m_index << ' ' << (block->isChosen?"true":"false") << ' ' << block->playerIndex << ' ' << block->m_score << ' ' << (block->m_hint?"true":"false") << '\n';
    }

    file.close();
}

void SingleGame::loadGame()
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

    //load single game
    if(gameType == 1)
    {
        fin >> m_rows >> m_columns >> m_timeLimit;
        fin >> m_countDown;

        int playerX;
        int playerY;
        int playerRotation;
        int playerScore;
        fin >> playerX >> playerY >> playerRotation;
        fin >> playerScore;
        m_player->setPosition(playerX, playerY);
        m_player->setRotation(playerRotation);
        m_player->setScore(playerScore);

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
    else if(gameType == 2)
    {
        int in_rows;
        int in_columns;
        int in_timeLimit;
        fin >> in_rows >> in_columns >> in_timeLimit;

        loadedDoubleGame = new DoubleGame(in_rows, in_columns, in_timeLimit);
        fin >> loadedDoubleGame->m_countDown;

        int playerX;
        int playerY;
        int playerRotation;
        int playerScore;
        fin >> playerX >> playerY >> playerRotation;
        loadedDoubleGame->m_player1->setPosition(playerX, playerY);
        loadedDoubleGame->m_player1->setRotation(playerRotation);
        fin >> playerX >> playerY >> playerRotation;
        loadedDoubleGame->m_player2->setPosition(playerX, playerY);
        loadedDoubleGame->m_player2->setRotation(playerRotation);
        fin >> playerScore;
        loadedDoubleGame->m_player1->setScore(playerScore);
        fin >> playerScore;
        loadedDoubleGame->m_player2->setScore(playerScore);

        loadedDoubleGame->m_blocks.clear();
        for(int i = 0; i < loadedDoubleGame->m_rows * loadedDoubleGame->m_columns; i++)
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
            loadedDoubleGame->m_blocks.push_back(block);
        }
        loadedDoubleGame->layBlocks();
        loadedDoubleGame->removeChosen();
        loadedDoubleGame->initPauseMenu();

        file.close();

        close();
        loadedDoubleGame->show();
    }
}

void SingleGame::quitGame()
{
    close();
}





