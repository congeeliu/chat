#-------------------------------------------------
#
# Project created by QtCreator 2023-05-08T20:21:26
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chat
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    main.cpp \
    src/widget/widget.cpp \
    src/widget/friendlist.cpp \
    src/widget/friendinfo.cpp \
    src/widget/chatbox.cpp \
    src/widget/addfriend.cpp \
    src/widget/deletefriend.cpp \
    src/widget/register.cpp \
    src/utils/sql.cpp \
    src/utils/util.cpp \
    src/utils/rsa.cpp \
    src/utils/qaesencryption.cpp


HEADERS += \
    src/widget/widget.h \
    src/widget/friendlist.h \
    src/widget/friendinfo.h \
    src/widget/chatbox.h \
    src/widget/addfriend.h \
    src/widget/deletefriend.h \
    src/widget/register.h \
    src/utils/sql.h \
    src/utils/util.h \
    src/utils/rsa.h \
    src/utils/qaesencryption.h


FORMS += \
    src/widget/widget.ui \
    src/widget/friendlist.ui \
    src/widget/friendinfo.ui \
    src/widget/chatbox.ui \
    src/widget/addfriend.ui \
    src/widget/deletefriend.ui \
    src/widget/register.ui

#specify ui directory.
UI_DIR = src/ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/external/gmp_x64/include
LIBS += $$PWD/external/gmp_x64/lib/libgmp.a
LIBS += $$PWD/external/gmp_x64/lib/libgmpxx.a

RESOURCES += \
    pic.qrc
