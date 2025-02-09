TARGET = SecureVault  
ICON = icon.png  

QT += core gui widgets network

CONFIG += c++11

SOURCES += \
    main.cpp \
    sv_main.cpp\
    rojer_encrypt.cpp

HEADERS += \
    login.h \
    sv_main.h\
    rojer_home.h \
    rojer_encrypt.h


RESOURCES += resources.qrc

INCLUDEPATH += /opt/local/include /opt/local/include/openssl

LIBS += -L/opt/local/lib -lssl -lcrypto