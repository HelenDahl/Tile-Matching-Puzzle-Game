QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    block.cpp \
    doublegame.cpp \
    doublegameovermenu.cpp \
    main.cpp \
    mainmenu.cpp \
    player.cpp \
    singlegame.cpp \
    singlegameovermenu.cpp \
    tool.cpp \
    userdoublesetting.cpp \
    usersinglesetting.cpp

HEADERS += \
    block.h \
    config.h \
    doublegame.h \
    doublegameovermenu.h \
    mainmenu.h \
    player.h \
    singlegame.h \
    singlegameovermenu.h \
    tool.h \
    userdoublesetting.h \
    usersinglesetting.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
