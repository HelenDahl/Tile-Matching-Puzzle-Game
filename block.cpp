#include "block.h"

Block::Block(int type, int index, QWidget *parent) : QLabel(parent)
{
    if(type == 0)
    {
        m_Free = false;
    }
    else
    {
        m_Free = true;
    }
    m_type = type;
    m_index = index;
    playerIndex = -1;
    isChosen = false;
    m_hint = false;

    if(type == 12)
        m_score = 2*BLOCK_SCORE;
    else
        m_score = BLOCK_SCORE;

    setFixedSize(BLOCK_WIDTH, BLOCK_HEIGHT);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

QRect Block::getBoundary()
{
    return QRect(pos(), size());
}

void Block::paintEvent(QPaintEvent *event)
{
    //block has been eliminated, not drawn
    if(m_Free == false)
    {
        setStyleSheet("border: 2px solid transparent;");
        return;
    }

    //draw blocks
    QLabel::paintEvent(event);

    //the block is chosen, draw the border
    if(isChosen)
    {
        if(playerIndex == 1)
        {
            setStyleSheet(QString("border: %1px solid red;").arg(BLOCK_CHOSEN_BORDER));
        }
        else if(playerIndex == 2)
        {
            setStyleSheet(QString("border: %1px solid blue;").arg(BLOCK_CHOSEN_BORDER));
        }
    }
    else if(m_hint)
    {
        setStyleSheet("border: 2px solid yellow;");
    }
    else
    {
        setStyleSheet("border: 2px solid purple;");
    }

    QPainter painter(this);
    QPixmap blockPixmap(QString(BLOCK_PATH).arg(m_type));
    painter.drawPixmap(0,0,blockPixmap.scaled(BLOCK_WIDTH, BLOCK_HEIGHT));
}
