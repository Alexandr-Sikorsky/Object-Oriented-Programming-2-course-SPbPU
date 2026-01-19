#include "databasemanager.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QPluginLoader>

DatabaseManager::DatabaseManager(QObject *parent) 
    : QObject(parent)
{
    qDebug() << "Available SQL drivers:" << QSqlDatabase::drivers();
}

bool DatabaseManager::connectToDatabase(const QString& dbName, const QString& user,
                                       const QString& password, const QString& host)
{
    // Удаляем старое подключение, если существует
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }

    // Создаем новое подключение
    m_db = QSqlDatabase::addDatabase("QPSQL");

    // Устанавливаем параметры подключения
    m_db.setHostName(host);          // "localhost"
    m_db.setDatabaseName(dbName);    // "postgres"
    m_db.setUserName(user);          // "postgres"
    m_db.setPassword(password);      // ваш пароль
    m_db.setPort(5432);              // стандартный порт

    // Пытаемся открыть соединение
    qDebug() << "Попытка подключения к PostgreSQL...";
    qDebug() << "Сервер:" << host;
    qDebug() << "База:" << dbName;
    qDebug() << "Пользователь:" << user;

    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        qDebug() << "ОШИБКА ПОДКЛЮЧЕНИЯ:" << m_lastError;
        qDebug() << "Native error:" << m_db.lastError().nativeErrorCode();
        qDebug() << "Доступные драйверы:" << QSqlDatabase::drivers();
        return false;
    }

    qDebug() << "Успешное подключение к PostgreSQL!";

    // Устанавливаем кодировку для русского языка
    QSqlQuery query(m_db);
    query.exec("SET CLIENT_ENCODING TO 'UTF8'");

    return createTables();
}


bool DatabaseManager::createTables()
{
    QSqlQuery query;
    
    // Таблица контактов
    if (!query.exec(
        "CREATE TABLE IF NOT EXISTS contacts ("
        "id SERIAL PRIMARY KEY,"
        "name VARCHAR(100) NOT NULL,"
        "surname VARCHAR(100) NOT NULL,"
        "email VARCHAR(100) NOT NULL,"
        "patronymic VARCHAR(100),"
        "bday VARCHAR(20)," // Храним как строку для совместимости
        "address TEXT"
        ")"
    )) {
        m_lastError = query.lastError().text();
        return false;
    }
    
    // Таблица телефонов
    if (!query.exec(
        "CREATE TABLE IF NOT EXISTS phones ("
        "id SERIAL PRIMARY KEY,"
        "contact_id INTEGER REFERENCES contacts(id) ON DELETE CASCADE,"
        "number VARCHAR(20) NOT NULL,"
        "type VARCHAR(20) DEFAULT 'WORK'"
        ")"
    )) {
        m_lastError = query.lastError().text();
        return false;
    }
    
    return true;
}

QVector<Contact> DatabaseManager::loadAllContacts()
{
    QVector<Contact> result;
    QSqlQuery query("SELECT * FROM contacts ORDER BY surname, name");
    
    while (query.next()) {
        Contact contact;
        contact.setId(query.value("id").toInt());
        contact.setNameQt(query.value("name").toString());
        contact.setSurnameQt(query.value("surname").toString());
        contact.setEmailQt(query.value("email").toString());
        contact.setPatronymicQt(query.value("patronymic").toString());
        contact.setBdayQt(query.value("bday").toString());
        contact.setAddressQt(query.value("address").toString());
        
        // Загружаем телефоны
        QSqlQuery phoneQuery;
        phoneQuery.prepare("SELECT number, type FROM phones WHERE contact_id = ?");
        phoneQuery.addBindValue(contact.getId());
        
        if (phoneQuery.exec()) {
            while (phoneQuery.next()) {
                contact.addPhoneQt(
                    phoneQuery.value(0).toString(),
                    phoneQuery.value(1).toString()
                );
            }
        }
        
        result.append(contact);
    }
    
    return result;
}

// databasemanager.cpp
bool DatabaseManager::saveContact(const Contact& contact, int* newId)
{
    qDebug() << "Сохранение нового контакта:"
             << contact.getNameQt() << contact.getSurnameQt();

    QSqlQuery query(m_db);
    query.prepare(
        "INSERT INTO contacts (name, surname, email, patronymic, bday, address) "
        "VALUES (?, ?, ?, ?, ?, ?) RETURNING id"
    );

    query.addBindValue(contact.getNameQt());
    query.addBindValue(contact.getSurnameQt());
    query.addBindValue(contact.getEmailQt());
    query.addBindValue(contact.getPatronymicQt());
    query.addBindValue(contact.getBdayQt());
    query.addBindValue(contact.getAddressQt());

    if (!query.exec() || !query.next()) {
        m_lastError = query.lastError().text();
        qDebug() << "Ошибка сохранения контакта:" << m_lastError;
        return false;
    }

    int contactId = query.value(0).toInt();
    if (newId) *newId = contactId;

    // Сохраняем телефоны
    auto phones = contact.getPhonesQt();
    for (const auto& phone : phones) {
        QSqlQuery phoneQuery(m_db);
        phoneQuery.prepare(
            "INSERT INTO phones (contact_id, number, type) VALUES (?, ?, ?)"
        );
        phoneQuery.addBindValue(contactId);
        phoneQuery.addBindValue(phone.first);
        phoneQuery.addBindValue(phone.second);

        if (!phoneQuery.exec()) {
            m_lastError = phoneQuery.lastError().text();
            qDebug() << "Ошибка сохранения телефона:" << m_lastError;
            return false;
        }
    }

    qDebug() << "Новый контакт сохранен с ID:" << contactId;
    return true;
}

bool DatabaseManager::updateContact(int id, const Contact& contact)
{
    qDebug() << "Обновление контакта ID:" << id
             << contact.getNameQt() << contact.getSurnameQt();

    QSqlQuery query(m_db);
    query.prepare(
        "UPDATE contacts SET name = ?, surname = ?, email = ?, patronymic = ?, bday = ?, address = ? "
        "WHERE id = ?"
    );

    query.addBindValue(contact.getNameQt());
    query.addBindValue(contact.getSurnameQt());
    query.addBindValue(contact.getEmailQt());
    query.addBindValue(contact.getPatronymicQt());
    query.addBindValue(contact.getBdayQt());
    query.addBindValue(contact.getAddressQt());
    query.addBindValue(id);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qDebug() << "Ошибка обновления контакта:" << m_lastError;
        return false;
    }

    // Удаляем старые телефоны
    QSqlQuery deleteQuery(m_db);
    deleteQuery.prepare("DELETE FROM phones WHERE contact_id = ?");
    deleteQuery.addBindValue(id);
    if (!deleteQuery.exec()) {
        m_lastError = deleteQuery.lastError().text();
        qDebug() << "Ошибка удаления старых телефонов:" << m_lastError;
        return false;
    }

    // Добавляем новые телефоны
    auto phones = contact.getPhonesQt();
    for (const auto& phone : phones) {
        QSqlQuery phoneQuery(m_db);
        phoneQuery.prepare(
            "INSERT INTO phones (contact_id, number, type) VALUES (?, ?, ?)"
        );
        phoneQuery.addBindValue(id);
        phoneQuery.addBindValue(phone.first);
        phoneQuery.addBindValue(phone.second);

        if (!phoneQuery.exec()) {
            m_lastError = phoneQuery.lastError().text();
            qDebug() << "Ошибка обновления телефона:" << m_lastError;
            return false;
        }
    }

    qDebug() << "Контакт ID" << id << "обновлен успешно";
    return true;
}

bool DatabaseManager::deleteContact(int id)
{
    qDebug() << "Удаление контакта ID:" << id;

    QSqlQuery query(m_db);
    query.prepare("DELETE FROM contacts WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qDebug() << "Ошибка удаления контакта:" << m_lastError;
        return false;
    }

    qDebug() << "Контакт ID" << id << "успешно удален из базы";
    return true;
}
