#ifndef SINGLEGAME_H
#define SINGLEGAME_H

#include <QtWidgets>
#include <ctime>
#include <cstdlib>
#include "config.h"
#include "block.h"
#include "player.h"
#include "singlegameovermenu.h"
#include "tool.h"
#include "doublegame.h"

class SingleGameOverMenu;
class DoubleGame;

class SingleGame : public QWidget
{
    Q_OBJECT

public:
    SingleGame(int rows, int columns, int timeLimit, QWidget* parent = nullptr);

    //initialization
    void initScene();
    void initPauseMenu();
    void generateBlocks();
    void layBlocks();

    void playGame();

    //update
    void paintEvent(QPaintEvent *);
    QSet<int> keysPressed;
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);

    void updatePlayerPosition();
    void updateCountDownTimer();
    void updateScore(int score);
    void updateFlash();
    void removeChosen(); //clear the selected information on the box

    QPixmap backgroundPixmap;
    QVector<Block *> m_blocks;
    QGridLayout* m_layout;
    Player* m_player;

    //game logic
    int m_rows;
    int m_columns;
    int m_timeLimit;
    QTimer m_Timer;
    QTimer m_countDownTimer;
    int m_countDown;
    bool m_gameOver;
    bool m_hint;
    int m_hintBlock1;
    int m_hintBlock2;
    int m_hintRemainTime;
    int m_tool_Recorder;
    bool m_flash;
    int m_flashRemainTime;
    bool m_paused;
    QVector<Tool*> m_tools;
    void pause();
    void gameOver();
    void giveHint();
    void shuffle();
    void placeTools();
    void resumeGame();
    void saveGame();
    void loadGame();
    void quitGame();

    //utilFunc
    QPoint getRandomEmptyPosition(const QRect& object);
    void pickupTool();

    //collision detection
    bool checkCollision();
    QRect nextPlayerRect;  //the position of the player in the next frame is used to determine in advance whether there will be a collision
    int maxArea;  //calculate the square with the largest overlap area with the player
    int maxAreaIndex;  //maximum overlap area square corresponding to the index

    //determines whether the selected two squares can be connected. `linePoints` store the index of the points that pass through them
    bool canBeConnected(int blockIndex1, int blockIndex2, std::vector<std::pair<int, int> >& linePoints);

    //The player has chosen one square, and then another
    void tryToMatch(int blockIndex);

    //The line drawn (including the index of the line, the index of the two squares, and the remaining display time)
    std::queue<std::tuple<std::vector<std::pair<int, int> >, int, int, int> > m_drawLines;

    //Determines whether the current block diagram has a solution
    bool isSovable(int& blockIndex1, int& blockIndex2, std::vector<std::pair<int, int> >& linePoints);

    //To determine whether the block of the index blockIndex can be selected by the character(to use in FLASH)
    //return 1/2/3/4 to indicate that the block can be chosen from above/below/right/left
    //Returning -1 means the block can't be chosen
    bool canBeChosen(int blockIndex, int& dir);
    void movePlayerToBlock(int blockIndex, int dir);

    //pause menu
    QPixmap m_pauseButton;
    QPixmap m_havePausedButton;
    QPixmap m_hoverPauseButton;
    QRect m_pauseRect;

    //gameover menu
    SingleGameOverMenu* m_gameOverMenu;
    QWidget* m_pauseOverlay;
    void setPauseLayout();

    //double game
    DoubleGame* loadedDoubleGame;
};

#endif // SINGLEGAME_H
