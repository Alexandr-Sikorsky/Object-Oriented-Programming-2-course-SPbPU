QT += core gui widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    contactdialog.cpp \
    contact.cpp \
    validator.cpp \
    filework.cpp

HEADERS += \
    mainwindow.h \
    contactdialog.h \
    contact.h \
    validator.h \
    filework.h \
    glob.h

FORMS += \
    mainwindow.ui \
    contactdialog.ui

TARGET = PhoneBookQt
