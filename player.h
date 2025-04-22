#ifndef PLAYER_H
#define PLAYER_H

#include <QtWidgets>
#include "config.h"

class Player
{
public:
    Player(int index);

    //move the player pos
    void moveForward();
    void moveBackward();
    void moveLeftward();
    void moveRightward();
    void stopMoving();

    //change player orientation
    void faceForward();
    void faceBackward();
    void faceLeftward();
    void faceRightward();

    void setPosition(int x, int y);
    void setRotation(int dir);
    void setScore(int score);

    QPixmap m_Pixmap_forward_idle;
    QPixmap m_Pixmap_backward_idle;
    QPixmap m_Pixmap_leftward_idle;
    QPixmap m_Pixmap_rightward_idle;
    QPixmap m_Pixmap_forward[PLAYER_MAX_PIC];
    QPixmap m_Pixmap_backward[PLAYER_MAX_PIC];
    QPixmap m_Pixmap_leftward[PLAYER_MAX_PIC];
    QPixmap m_Pixmap_rightward[PLAYER_MAX_PIC];
    QPixmap m_pixmap;

    //player index
    int m_index;

    //1-forward，2-backward，3-left，4-right
    int m_Rotation;
    //player coordinates
    int m_X;
    int m_Y;
    QRect m_Rect;

    //The index of the block selected by the player
    int m_ChosenBlock;  //didn't choose any block : -1

    bool m_IsMoving;
    bool m_hasStopped;
    bool m_readyToChoose;

    //Index of the currently played image
    int m_PixmapIndex;
    //Time interval timer for animation rotation
    int m_PixmapRecorder;
    void updateImage();

    int m_score;
};

#endif // PLAYER_H
