QT += core gui widgets
QT += sql

CONFIG += c++17

SOURCES += \
    databasemanager.cpp \
    main.cpp \
    mainwindow.cpp \
    contactdialog.cpp \
    contact.cpp \
    validator.cpp \
    filework.cpp

HEADERS += \
    databasemanager.h \
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

DISTFILES +=
