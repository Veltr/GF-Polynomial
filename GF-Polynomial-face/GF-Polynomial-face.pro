QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gfbuildingwindow.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../GF_polynom/gf.h \
    ../GF_polynom/polynomgf.h \
    gfbuildingwindow.h \
    mainwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# liba
#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-GF_polynom-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/release/ -lGF_polynom
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-GF_polynom-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug/ -lGF_polynom
#else:unix: LIBS += -L$$PWD/../build-GF_polynom-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/ -lGF_polynom

#INCLUDEPATH += $$PWD/../build-GF_polynom-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug
#DEPENDPATH += $$PWD/../build-GF_polynom-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug

unix|win32: LIBS += -L$$PWD/../build-GF_polynom-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug/ -lGF_polynom

INCLUDEPATH += $$PWD/../build-GF_polynom-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug
DEPENDPATH += $$PWD/../build-GF_polynom-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug
