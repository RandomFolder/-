QT       += core gui
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    edgeeditor.cpp \
    main.cpp \
    mainwindow.cpp \
    nodecreator.cpp \
    nodeeditor.cpp \
    paintpanel.cpp \
    tabwidget.cpp

HEADERS += \
    Graph.h \
    JSONHandler.h \
    TXTHandler.h \
    XMLHandler.h \
    edgeeditor.h \
    mainwindow.h \
    nodecreator.h \
    nodeeditor.h \
    paintpanel.h \
    tabwidget.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
