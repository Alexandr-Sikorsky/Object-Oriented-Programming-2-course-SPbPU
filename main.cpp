#include "mainwindow.h"
#include <QApplication>
#include "glob.h"
#include "filework.h"
#include <QDebug>
#include <QSysInfo>


// Определяем глобальные переменные
std::vector<Contact> contacts;
QString currentFilename = "phonebook.txt";
Validator validator;

int main(int argc, char *argv[])
{
    // Драйверы
    QCoreApplication::addLibraryPath("C:/Program Files/PostgreSQL/18/bin");
    QCoreApplication::addLibraryPath("C:/Qt/Qt5.14.2/5.14.2/mingw73_64/plugins");
    QCoreApplication::addLibraryPath("C:/Qt/Qt5.14.2/5.14.2/mingw73_64/bin");
    QCoreApplication::addLibraryPath(".");
    QCoreApplication::addLibraryPath("./sqldrivers");

    qDebug() << "Пути к плагинам:" << QCoreApplication::libraryPaths();
    qDebug() << "Доступные SQL драйверы:" << QSqlDatabase::drivers();
    qDebug() << "Qt build architecture:" << QSysInfo::buildCpuArchitecture();
    qDebug() << "Qt version:" << QT_VERSION_STR;
    qputenv("QT_PLUGIN_PATH", "C:/Qt/5.14.2/mingw73_64/plugins");
    qputenv("PATH", "C:/Program Files/PostgreSQL/18/bin;" + qgetenv("PATH"));
    QApplication app(argc, argv);

    qDebug() << "=== Starting PhoneBook ===";
    qDebug() << "Current filename:" << currentFilename;
    readFileQt(currentFilename);
    qDebug() << "Loaded contacts:" << contacts.size();

    MainWindow window;
    window.show();

    return app.exec();
}
