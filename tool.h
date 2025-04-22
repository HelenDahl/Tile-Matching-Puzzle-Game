#ifndef TOOL_H
#define TOOL_H

#include <QtWidgets>
#include "config.h"

class Tool
{
public:
    Tool(int type, int gameType);

    //if the tool is available，false - not available
    bool m_Free;
    int m_type;
    int m_gameType;

    QPixmap m_pixmap;

    int m_X;
    int m_Y;
    QRect m_Rect;
};

#endif // TOOL_H
