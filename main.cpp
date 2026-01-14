#include "mainwindow.h"
#include <QApplication>
#include "glob.h"
#include "filework.h"
#include <QDebug>

// Определяем глобальные переменные
std::vector<Contact> contacts;
QString currentFilename = "phonebook.txt";
Validator validator;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qDebug() << "=== Starting PhoneBook ===";
    qDebug() << "Current filename:" << currentFilename;

    // Пытаемся загрузить контакты
    readFileQt(currentFilename);
    qDebug() << "Loaded contacts:" << contacts.size();

    MainWindow window;
    window.show();

    return app.exec();
}
