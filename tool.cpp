#include "tool.h"

Tool::Tool(int type, int gameType)
{
    m_Free = false;
    m_type = type;
    m_gameType = gameType;
    m_X = 0;
    m_Y = 0;

    m_Rect.setWidth(TOOLS_ELIPSIZE);
    m_Rect.setHeight(TOOLS_ELIPSIZE);
    m_Rect.moveTo(m_X, m_Y);

    switch(type)
    {
    case 0:
        m_pixmap.load(EXTENDTIME_ICON);
        break;
    case 1://hint
        m_pixmap.load(HINT_ICON);
        break;
    case 2://shuffle
        m_pixmap.load(SHUFFLE_ICON);
        break;
    case 3://single: flash，double: dizzy
        if(m_gameType == 1)
            m_pixmap.load(FLASH_ICON);
        else if(m_gameType == 2)
            m_pixmap.load(DIZZY_ICON);
        break;
    case 4://double: freeze
        m_pixmap.load(FREEZE_ICON);
    }
    m_pixmap = m_pixmap.scaled(TOOLS_SIZE, TOOLS_SIZE);
}
