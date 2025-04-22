#include "player.h"

Player::Player(int index)
{
    m_index = index;

    if(index == 1)
    {
        m_Pixmap_forward_idle.load(PLAYER_1_FORWARD_IDLE_PATH);
        m_Pixmap_backward_idle.load(PLAYER_1_BACKWARD_IDLE_PATH);
        m_Pixmap_leftward_idle.load(PLAYER_1_LEFTWARD_IDLE_PATH);
        m_Pixmap_rightward_idle.load(PLAYER_1_RIGHTWARD_IDLE_PATH);
        for(int i = 0; i < PLAYER_MAX_PIC; i++)
            m_Pixmap_forward[i].load(QString(PLAYER_1_FORWARD_PATH).arg(i+1));
        for(int i = 0; i < PLAYER_MAX_PIC; i++)
            m_Pixmap_backward[i].load(QString(PLAYER_1_BACKWARD_PATH).arg(i+1));
        for(int i = 0; i < PLAYER_MAX_PIC; i++)
            m_Pixmap_leftward[i].load(QString(PLAYER_1_LEFTWARD_PATH).arg(i+1));
        for(int i = 0; i < PLAYER_MAX_PIC; i++)
            m_Pixmap_rightward[i].load(QString(PLAYER_1_RIGHTWARD_PATH).arg(i+1));
    }
    else if(index == 2)
    {
        m_Pixmap_forward_idle.load(PLAYER_2_FORWARD_IDLE_PATH);
        m_Pixmap_backward_idle.load(PLAYER_2_BACKWARD_IDLE_PATH);
        m_Pixmap_leftward_idle.load(PLAYER_2_LEFTWARD_IDLE_PATH);
        m_Pixmap_rightward_idle.load(PLAYER_2_RIGHTWARD_IDLE_PATH);
        for(int i = 0; i < PLAYER_MAX_PIC; i++)
            m_Pixmap_forward[i].load(QString(PLAYER_2_FORWARD_PATH).arg(i+1));
        for(int i = 0; i < PLAYER_MAX_PIC; i++)
            m_Pixmap_backward[i].load(QString(PLAYER_2_BACKWARD_PATH).arg(i+1));
        for(int i = 0; i < PLAYER_MAX_PIC; i++)
            m_Pixmap_leftward[i].load(QString(PLAYER_2_LEFTWARD_PATH).arg(i+1));
        for(int i = 0; i < PLAYER_MAX_PIC; i++)
            m_Pixmap_rightward[i].load(QString(PLAYER_2_RIGHTWARD_PATH).arg(i+1));
    }

    m_Rotation = 1;
    m_ChosenBlock = -1;
    m_IsMoving = false;
    m_hasStopped = true;
    m_readyToChoose = false;

    m_X = 0;
    m_Y = 0;
    if(index == 1)
    {
        m_Rect.setWidth(PLAYER_1_SIDELENGTH);
        m_Rect.setHeight(PLAYER_1_SIDELENGTH);
        m_pixmap = m_Pixmap_forward_idle.scaled(PLAYER_1_SIDELENGTH, PLAYER_1_SIDELENGTH);
    }
    else if(index == 2)
    {
        m_Rect.setWidth(PLAYER_2_SIDELENGTH);
        m_Rect.setHeight(PLAYER_2_SIDELENGTH);
        m_pixmap = m_Pixmap_forward_idle.scaled(PLAYER_2_SIDELENGTH, PLAYER_2_SIDELENGTH);
    }
    m_Rect.moveTo(m_X, m_Y);

    m_PixmapIndex = 0;
    m_PixmapRecorder = 0;

    m_score = 0;
}

void Player::moveForward()
{
    m_Y += PLAYER_SPEED;
    m_Rotation = 1;
    m_IsMoving = true;
    m_Rect.moveTo(m_X, m_Y);
    m_hasStopped = false;
    m_readyToChoose = false;
}

void Player::moveBackward()
{
    m_Y -= PLAYER_SPEED;
    m_Rotation = 2;
    m_IsMoving = true;
    m_Rect.moveTo(m_X, m_Y);
    m_hasStopped = false;
    m_readyToChoose = false;
}

void Player::moveLeftward()
{
    m_X -= PLAYER_SPEED;
    m_Rotation = 3;
    m_IsMoving = true;
    m_Rect.moveTo(m_X, m_Y);
    m_hasStopped = false;
    m_readyToChoose = false;
}

void Player::moveRightward()
{
    m_X += PLAYER_SPEED;
    m_Rotation = 4;
    m_IsMoving = true;
    m_Rect.moveTo(m_X, m_Y);
    m_hasStopped = false;
    m_readyToChoose = false;
}

void Player::stopMoving()
{
    m_PixmapRecorder = 0;
    m_PixmapIndex = 0;
    m_IsMoving = false;
    updateImage();
    m_hasStopped = true;
}

void Player::faceForward()
{
    m_Rotation = 1;
    if(m_hasStopped)
        m_readyToChoose = true;
}

void Player::faceBackward()
{
    m_Rotation = 2;
    if(m_hasStopped)
        m_readyToChoose = true;
}

void Player::faceLeftward()
{
    m_Rotation = 3;
    if(m_hasStopped)
        m_readyToChoose = true;
}

void Player::faceRightward()
{
    m_Rotation = 4;
    if(m_hasStopped)
        m_readyToChoose = true;
}

void Player::setPosition(int x, int y)
{
    m_X = x;
    m_Y = y;
    m_Rect.moveTo(m_X, m_Y);
}

void Player::setRotation(int dir)
{
    m_Rotation = dir;
}

void Player::setScore(int score)
{
    m_score = score;
}

void Player::updateImage()
{
    if(m_index == 1)
    {
        if(m_IsMoving)
        {
            m_PixmapRecorder++;
            if(m_PixmapRecorder > PLAYER_1_PIXMAPRECORDER_MAX)
            {
                //计数归零
                m_PixmapRecorder = 0;
                //动画图片索引加1循环播放
                m_PixmapIndex++;
                if(m_PixmapIndex >= PLAYER_MAX_PIC)
                    m_PixmapIndex = 0;

                switch(m_Rotation)
                {
                case 1:
                    m_pixmap = m_Pixmap_forward[m_PixmapIndex].scaled(PLAYER_1_SIDELENGTH, PLAYER_1_SIDELENGTH);
                    break;
                case 2:
                    m_pixmap = m_Pixmap_backward[m_PixmapIndex].scaled(PLAYER_1_SIDELENGTH, PLAYER_1_SIDELENGTH);
                    break;
                case 3:
                    m_pixmap = m_Pixmap_leftward[m_PixmapIndex].scaled(PLAYER_1_SIDELENGTH, PLAYER_1_SIDELENGTH);
                    break;
                case 4:
                    m_pixmap = m_Pixmap_rightward[m_PixmapIndex].scaled(PLAYER_1_SIDELENGTH, PLAYER_1_SIDELENGTH);
                    break;
                }
            }
        }
        else
        {
            switch(m_Rotation)
            {
            case 1:
                m_pixmap = m_Pixmap_forward_idle.scaled(PLAYER_1_SIDELENGTH, PLAYER_1_SIDELENGTH);
                break;
            case 2:
                m_pixmap = m_Pixmap_backward_idle.scaled(PLAYER_1_SIDELENGTH, PLAYER_1_SIDELENGTH);
                break;
            case 3:
                m_pixmap = m_Pixmap_leftward_idle.scaled(PLAYER_1_SIDELENGTH, PLAYER_1_SIDELENGTH);
                break;
            case 4:
                m_pixmap = m_Pixmap_rightward_idle.scaled(PLAYER_1_SIDELENGTH, PLAYER_1_SIDELENGTH);
                break;
            }
        }
    }

    if(m_index == 2)
    {
        if(m_IsMoving)
        {
            m_PixmapRecorder++;
            if(m_PixmapRecorder > PLAYER_2_PIXMAPRECORDER_MAX)
            {
                m_PixmapRecorder = 0;
                //animation picture index plus 1 loop play
                m_PixmapIndex++;
                if(m_PixmapIndex >= PLAYER_MAX_PIC)
                    m_PixmapIndex = 0;

                switch(m_Rotation)
                {
                case 1:
                    m_pixmap = m_Pixmap_forward[m_PixmapIndex].scaled(PLAYER_2_SIDELENGTH, PLAYER_2_SIDELENGTH);
                    break;
                case 2:
                    m_pixmap = m_Pixmap_backward[m_PixmapIndex].scaled(PLAYER_2_SIDELENGTH, PLAYER_2_SIDELENGTH);
                    break;
                case 3:
                    m_pixmap = m_Pixmap_leftward[m_PixmapIndex].scaled(PLAYER_2_SIDELENGTH, PLAYER_2_SIDELENGTH);
                    break;
                case 4:
                    m_pixmap = m_Pixmap_rightward[m_PixmapIndex].scaled(PLAYER_2_SIDELENGTH, PLAYER_2_SIDELENGTH);
                    break;
                }
            }
        }
        else
        {
            switch(m_Rotation)
            {
            case 1:
                m_pixmap = m_Pixmap_forward_idle.scaled(PLAYER_2_SIDELENGTH, PLAYER_2_SIDELENGTH);
                break;
            case 2:
                m_pixmap = m_Pixmap_backward_idle.scaled(PLAYER_2_SIDELENGTH, PLAYER_2_SIDELENGTH);
                break;
            case 3:
                m_pixmap = m_Pixmap_leftward_idle.scaled(PLAYER_2_SIDELENGTH, PLAYER_2_SIDELENGTH);
                break;
            case 4:
                m_pixmap = m_Pixmap_rightward_idle.scaled(PLAYER_2_SIDELENGTH, PLAYER_2_SIDELENGTH);
                break;
            }
        }
    }
}


