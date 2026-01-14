#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>
#include <QVector>
#include "contact.h"

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    bool connectToDatabase(const QString& dbName, const QString& user,
                          const QString& password, const QString& host = "localhost");

    bool saveContact(const Contact& contact, int* newId = nullptr);
    bool updateContact(int id, const Contact& contact);
    bool deleteContact(int id);
    QVector<Contact> loadAllContacts();

    QString lastError() const { return m_lastError; }
    bool isConnected() const { return m_db.isOpen(); }

private:
    QSqlDatabase m_db;
    QString m_lastError;

    bool createTables();
};

#endif // DATABASEMANAGER_H
