#ifndef DOUBLEGAME_H
#define DOUBLEGAME_H

#include <QtWidgets>
#include <ctime>
#include <cstdlib>
#include "config.h"
#include "block.h"
#include "player.h"
#include "doublegameovermenu.h"
#include "tool.h"
#include "singlegame.h"

class DoubleGameOverMenu;
class SingleGame;

class DoubleGame : public QWidget
{
    Q_OBJECT

public:
    DoubleGame(int rows, int columns, int timeLimit, QWidget* parent = nullptr);

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
    void updateScore(int score, int playerIndex);
    void removeChosen();  //clear the selected information on the box

    QPixmap backgroundPixmap;
    QVector<Block *> m_blocks;
    QGridLayout* m_layout;
    Player* m_player1;
    Player* m_player2;

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
    bool m_paused;
    QVector<Tool*> m_tools;
    void pause();
    void gameOver();
    void resumeGame();
    void saveGame();
    void loadGame();
    void quitGame();

    //if player 1 is frozen
    bool freezePlayer1;
    int freezePlayerRecorder1;

    //if player 1 is dizzy
    bool dizzyPlayer1;
    int dizzyPlayerRecorder1;

    //if player 2 is frozen
    bool freezePlayer2;
    int freezePlayerRecorder2;

    //if player 2 is dizzy
    bool dizzyPlayer2;
    int dizzyPlayerRecorder2;

    //utilFunc
    void pickupTool();
    QPoint getRandomEmptyPosition(const QRect& object);

    //collision detection
    bool checkCollision1();
    bool checkCollision2();
    QRect nextPlayerRect1;
    QRect nextPlayerRect2;
    int maxArea1;  //calculate the block 1 with the largest overlapping area with player 1
    int maxAreaIndex1;  //index corresponding to block 1 with the maximum overlapping area
    int maxArea2;  //calculate the block 2 with the largest overlapping area with player 12
    int maxAreaIndex2;  //index corresponding to block 2 with the maximum overlapping area

    //determine if the selected two blocks can be connected; linePoints stores the indices of the points traversed
    bool canBeConnected(int blockIndex1, int blockIndex2, std::vector<std::pair<int, int> >& linePoints);

    //lines to be rendered(indices of the traversed points, the indices of the two blocks, and the remaining display time)
    std::queue<std::tuple<std::vector<std::pair<int, int> >, int, int, int> > m_drawLines;

    //determine if the current block layout has a solution.
    bool isSovable(int& blockIndex1, int& blockIndex2, std::vector<std::pair<int, int> >& linePoints);

    //determine if the block at index blockIndex can is reachable
    bool canBeChosen(int blockIndex);

    //determine whether to give a hint; end the game if unsolvable.
    void giveHint();

    //block rearrangement
    void shuffle();

    //place the tools
    void placeTools();

    //pause menu
    QPixmap m_pauseButton;
    QPixmap m_havePausedButton;
    QPixmap m_hoverPauseButton;
    QRect m_pauseRect;

    //gameover menu
    DoubleGameOverMenu* m_gameOverMenu;
    QWidget* m_pauseOverlay;
    void setPauseLayout();

    //single player Game Menu
    SingleGame* loadedSingleGame;
};

#endif // DOUBLEGAME_H
