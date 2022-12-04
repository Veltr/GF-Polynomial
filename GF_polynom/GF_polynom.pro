CONFIG -= qt

TEMPLATE = lib
DEFINES += GF_POLYNOM_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gf.cpp \
    polynomgf.cpp \
    polynomgf_old.cpp

HEADERS += \
    GF_polynom_global.h \
    gf.h \
    polynomgf.h \
    polynomgf_old.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
