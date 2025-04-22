#ifndef BLOCK_H
#define BLOCK_H

#include <QtWidgets>
#include "config.h"

class Block : public QLabel
{
    Q_OBJECT

public:
    Block(int type, int index, QWidget *parent=nullptr);

    QRect getBoundary();

    bool m_Free;  //if the block is available (true: available, visible)
    int m_type;  //block type (for pairing)
    int m_index;  //block in the current map index (for restoring maps)
    bool isChosen;  //if the block is chosen
    int playerIndex;  //which player chose the block
    int m_score;  //how many points is contained in the block
    bool m_hint;  //if highlight the block to give a hint

protected:
    void paintEvent(QPaintEvent *);
};

#endif // BLOCK_H
