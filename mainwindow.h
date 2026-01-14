#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QLineEdit>
#include "contact.h"
#include "validator.h"
#include "databasemanager.h"

#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddContact();
    void onEditContact();
    void onDeleteContact();
    void onSearchContact();
    void onClearSearch();
    void onSaveFile();
    void onLoadFile();
    void onAbout();
    void onAdvancedSearch();

    void onLoadFromDatabase();
    void onSaveToDatabase();
    void onDeleteFromDatabase();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *contactModel;
    QSortFilterProxyModel *proxyModel;
    QSqlDatabase m_db;
    Validator validator;

    void setupTable();
    void updateContactTable();
    void loadContacts();
    void saveContacts();
    void applySimpleSearch(const QVector<QLineEdit*>& fieldEdits);
    void applyAdvancedSearchAllFields(const QVector<QLineEdit*>& fieldEdits);
    void clearAllFilters();
    int getSelectedContactIndex() const;

    DatabaseManager m_dbManager;
    bool m_isDatabaseMode = false; // Флаг: файл или БД
    
    void setupDatabaseMenu();
    void onConnectToDatabase();
};

#endif // MAINWINDOW_H
