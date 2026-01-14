#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "contactdialog.h"
#include "filework.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QStatusBar>
#include <QPushButton>
#include <QTableView>
#include <QDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QDebug>


// CaseInsensitive - регистр не важен

extern std::vector<Contact> contacts;
extern QString currentFilename;
extern Validator validator;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // весь интерфейс пользователя
{
    ui->setupUi(this);

    if (!ui->statusbar) {
        ui->statusbar;
    }

    contactModel = new QStandardItemModel(this); // модель таблицы Qt - представление данных (не сами данные)
    proxyModel = new QSortFilterProxyModel(this); // модель для сортировки и фильтрации (без изменения реального порядка контактов)
    proxyModel->setSourceModel(contactModel);

    qDebug() << "Вызываем setupDatabaseMenu()";
    setupDatabaseMenu();
    qDebug() << "Меню базы данных создано";

    setupTable();
    loadContacts();

    // подключение основных кнопок
    if (ui->addButton) {
        connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::onAddContact);
    }
    if (ui->editButton) {
        connect(ui->editButton, &QPushButton::clicked, this, &MainWindow::onEditContact);
    }
    if (ui->deleteButton) {
        connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteContact);
    }
    if (ui->searchButton) {
        connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::onSearchContact);
    }
    if (ui->clearSearchButton) {
        connect(ui->clearSearchButton, &QPushButton::clicked, this, &MainWindow::onClearSearch);
    }

    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSaveFile);
    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::onLoadFile);
    connect(ui->actionExit, &QAction::triggered, this, &QMainWindow::close);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAbout);
    connect(ui->advancedSearchButton, &QPushButton::clicked,
            this, &MainWindow::onAdvancedSearch);

    setWindowTitle("PhoneBook - " + currentFilename);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupTable()
{
    QStringList headers = {"Surname", "Name", "Patronymic", "Email",
                          "Phones", "Birthday", "Address"};
    contactModel->setHorizontalHeaderLabels(headers);

    ui->tableView->setModel(proxyModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection); // пользователь может выделять только одну строку.
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows); // выделение всей строки
    ui->tableView->verticalHeader()->setVisible(false);

    // Ширина
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setColumnWidth(0, 120); // Surname
    ui->tableView->setColumnWidth(1, 120); // Name
    ui->tableView->setColumnWidth(2, 120); // Patronymic
    ui->tableView->setColumnWidth(3, 180); // Email
    ui->tableView->setColumnWidth(4, 200); // Phones
    ui->tableView->setColumnWidth(5, 120); // Birthday
}

void MainWindow::updateContactTable()
{
    // пересобираем таблицу контактов (после изменения вектора контактов)
    contactModel->removeRows(0, contactModel->rowCount());
    for (size_t i = 0; i < contacts.size(); ++i) {
        const Contact &contact = contacts[i];

        QList<QStandardItem*> row;

        // Surname
        QStandardItem *surnameItem = new QStandardItem(contact.getSurnameQt());
        surnameItem->setData(static_cast<int>(i), Qt::UserRole + 1); // индекс контакта для правильного редактирования - служебные данные. корекктные фил/сорт
        row.append(surnameItem);


        // Name
        QStandardItem *nameItem = new QStandardItem(contact.getNameQt());
        row.append(nameItem);

        // Patronymic
        QStandardItem *patronymicItem = new QStandardItem(contact.getPatronymicQt());
        row.append(patronymicItem);

        // Email
        QStandardItem *emailItem = new QStandardItem(contact.getEmailQt());
        row.append(emailItem);

        // Phones
        QString phonesText;
        auto phones = contact.getPhonesQt();
        for (int j = 0; j < phones.size(); ++j) {
            phonesText += phones[j].first + " (" + phones[j].second + ")";
            if (j < phones.size() - 1) phonesText += ", ";
        }
        QStandardItem *phonesItem = new QStandardItem(phonesText);
        row.append(phonesItem);

        // Birthday - ааа qdate
        QString bday = contact.getBdayQt();
        QStandardItem *bdayItem = new QStandardItem();

        if (!bday.isEmpty()) {
            QDate date = QDate::fromString(bday, "dd.MM.yyyy");

            bdayItem->setData(date, Qt::DisplayRole);
        } else {
            // пустые в конец
            bdayItem->setData(QDate(), Qt::DisplayRole);

        }

        row.append(bdayItem);

        // Address
        QStandardItem *addressItem = new QStandardItem(contact.getAddressQt());
        row.append(addressItem);

        for (auto item : row) {
            item->setEditable(false);
        }

        contactModel->appendRow(row);
    }
    ui->statusbar->showMessage(QString("Total contacts: %1").arg(contacts.size()));
}

void MainWindow::loadContacts()
{
    readFileQt(currentFilename);
    updateContactTable();
}

void MainWindow::saveContacts()
{

    saveToFileQt(currentFilename);
    ui->statusbar->showMessage("Contacts saved to " + currentFilename, 3000);
}

int MainWindow::getSelectedContactIndex() const
{
    QModelIndexList selected = ui->tableView->selectionModel()->selectedRows();
    if (selected.isEmpty())
        return -1;

    QModelIndex proxyIndex = selected.first();
    QModelIndex sourceIndex = proxyModel->mapToSource(proxyIndex); // прокси в исходную модель (повторить)

    return contactModel
        ->item(sourceIndex.row(), 0)
        ->data(Qt::UserRole + 1)
        .toInt();
}


void MainWindow::onAddContact()
{
    ContactDialog dialog(this);
    dialog.setWindowTitle("Add New Contact");

    if (dialog.exec() == QDialog::Accepted) {
        Contact newContact = dialog.getContact();
        contacts.push_back(newContact);
        updateContactTable();
        saveContacts();
        ui->statusbar->showMessage("Contact added successfully", 3000);
    }
}

void MainWindow::onEditContact()
{
    int index = getSelectedContactIndex();
    if (index == -1) {
        QMessageBox::warning(this, "Warning", "Please select a contact to edit.");
        return;
    }

    ContactDialog dialog(this);
    dialog.setWindowTitle("Edit Contact");
    dialog.setContact(contacts[index]);

    if (dialog.exec() == QDialog::Accepted) {
        contacts[index] = dialog.getContact();
        updateContactTable();
        saveContacts();

        ui->statusbar->showMessage("Contact updated successfully", 3000);
    }
}

void MainWindow::onDeleteContact()
{
    int index = getSelectedContactIndex();
    if (index == -1) {
        QMessageBox::warning(this, "Warning", "Please select a contact to delete.");
        return;
    }

    Contact contactToDelete = contacts[index];
    QString contactName = QString("%1 %2").arg(contactToDelete.getSurnameQt()).arg(contactToDelete.getNameQt());

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Delete",
        QString("Are you sure you want to delete contact:\n%1?").arg(contactName),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Удаляем из локального вектора
        bool wasInDatabase = (contactToDelete.getId() != -1);
        int contactId = contactToDelete.getId();

        contacts.erase(contacts.begin() + index);
        updateContactTable();

        // Если контакт был в базе данных - удаляем оттуда
        if (m_isDatabaseMode && m_dbManager.isConnected() && wasInDatabase && contactId != -1) {
            if (m_dbManager.deleteContact(contactId)) {
                ui->statusbar->showMessage(QString("Contact '%1' deleted from database").arg(contactName), 3000);
            } else {
                ui->statusbar->showMessage(QString("Failed to delete contact '%1' from database: %2")
                    .arg(contactName).arg(m_dbManager.lastError()), 5000);

                // Восстанавливаем контакт в векторе при ошибке удаления
                contacts.insert(contacts.begin() + index, contactToDelete);
                updateContactTable();
            }
        }

        saveContacts(); // Сохраняем в файл
        ui->statusbar->showMessage("Contact deleted successfully", 3000);
    }
}

void MainWindow::onClearSearch()
{
    clearAllFilters();
}

void MainWindow::onSearchContact()
{
    QString searchText = ui->searchLineEdit->text().trimmed();

    if (searchText.isEmpty()) {
        clearAllFilters();
        return;
    }

    // прокси - фильтрация
    proxyModel->setFilterKeyColumn(-1); // поиск по всем колонкам (-1)
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    QRegularExpression regex(searchText, QRegularExpression::CaseInsensitiveOption);
    proxyModel->setFilterRegularExpression(regex);

    int matchCount = 0;
    for (int i = 0; i < proxyModel->rowCount(); ++i) {
        if (!ui->tableView->isRowHidden(i)) {
            matchCount++;
        }
    }

    ui->statusbar->showMessage(QString("Found %1 contact(s)").arg(matchCount), 3000);
}

void MainWindow::onSaveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Save Contacts",
        currentFilename,
        "Text Files (*.txt);;All Files (*)");

    if (!fileName.isEmpty()) {
        currentFilename = fileName;
        saveContacts();
        setWindowTitle("PhoneBook - " + currentFilename);
    }
}

void MainWindow::onLoadFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Load Contacts",
        "",
        "Text Files (*.txt);;All Files (*)");

    if (!fileName.isEmpty()) {
        currentFilename = fileName;
        readFileQt(currentFilename);
        updateContactTable();
        setWindowTitle("PhoneBook - " + currentFilename);
        ui->statusbar->showMessage("Contacts loaded from " + currentFilename, 3000);
    }
}

void MainWindow::onAdvancedSearch()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Advanced Search - All Fields");
    dialog.setMinimumWidth(350);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);

    // header
    QLabel *titleLabel = new QLabel("Search in specific fields:", &dialog);
    mainLayout->addWidget(titleLabel);

    QVector<QLineEdit*> fieldEdits;
    QStringList fieldNames = {
        "Name:", "Surname:", "Patronymic:",
        "Email:", "Phone:", "Birthday:", "Address:"
    };

    for (int i = 0; i < fieldNames.size(); ++i) {
        QHBoxLayout *rowLayout = new QHBoxLayout();

        QLabel *label = new QLabel(fieldNames[i], &dialog);
        label->setMinimumWidth(100);

        QLineEdit *edit = new QLineEdit(&dialog);
        edit->setPlaceholderText(QString("Enter text for %1...").arg(fieldNames[i].replace(":", "")));

        rowLayout->addWidget(label);
        rowLayout->addWidget(edit);
        mainLayout->addLayout(rowLayout);

        fieldEdits.append(edit);
    }

    // инструкция для teapot
    QLabel *infoLabel = new QLabel(
        "• Leave field empty to ignore it\n"
        "• All filled fields must match\n"
        "• Search is case-insensitive",
        &dialog
    );
    infoLabel->setWordWrap(true);
    mainLayout->addWidget(infoLabel);

    // Кнопки
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        &dialog
    );
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        bool hasSearch = false;
        for (QLineEdit* edit : fieldEdits) {
            if (!edit->text().trimmed().isEmpty()) {
                hasSearch = true;
                break;
            }
        }

        if (!hasSearch) {
            clearAllFilters();
            return;
        }

        applyAdvancedSearchAllFields(fieldEdits);
    }
}

void MainWindow::applyAdvancedSearchAllFields(const QVector<QLineEdit*>& fieldEdits) // поиском по нескольким полям - через прокси-модель
{
    // все поля в окне - имя: ____ и тд
    QString nameText = fieldEdits[0]->text().trimmed();
    QString surnameText = fieldEdits[1]->text().trimmed();
    QString patronymicText = fieldEdits[2]->text().trimmed();
    QString emailText = fieldEdits[3]->text().trimmed();
    QString phoneText = fieldEdits[4]->text().trimmed();
    QString birthdayText = fieldEdits[5]->text().trimmed();
    QString addressText = fieldEdits[6]->text().trimmed();

    bool hasSearchCriteria = !nameText.isEmpty() || !surnameText.isEmpty() ||
                             !patronymicText.isEmpty() || !emailText.isEmpty() ||
                             !phoneText.isEmpty() || !birthdayText.isEmpty() ||
                             !addressText.isEmpty();

    if (!hasSearchCriteria) {
        clearAllFilters();
        return;
    }

    ui->tableView->setSortingEnabled(false);

    proxyModel->setFilterFixedString("");

    int matchCount = 0;

    for (int row = 0; row < proxyModel->rowCount(); ++row) {
        bool matches = true;

        if (!nameText.isEmpty()) {
            QString name = proxyModel->data(proxyModel->index(row, 1)).toString();
            if (!name.contains(nameText, Qt::CaseInsensitive)) matches = false;
        }

        if (matches && !surnameText.isEmpty()) {
            QString surname = proxyModel->data(proxyModel->index(row, 0)).toString();
            if (!surname.contains(surnameText, Qt::CaseInsensitive)) matches = false;
        }

        if (matches && !patronymicText.isEmpty()) {
            QString patronymic = proxyModel->data(proxyModel->index(row, 2)).toString();
            if (!patronymic.contains(patronymicText, Qt::CaseInsensitive)) matches = false;
        }

        if (matches && !emailText.isEmpty()) {
            QString email = proxyModel->data(proxyModel->index(row, 3)).toString();
            if (!email.contains(emailText, Qt::CaseInsensitive)) matches = false;
        }

        if (matches && !phoneText.isEmpty()) {
            QString phones = proxyModel->data(proxyModel->index(row, 4)).toString();
            if (!phones.contains(phoneText)) matches = false; // Телефон чувствителен к регистру
        }

        if (matches && !birthdayText.isEmpty()) {
            QString birthday = proxyModel->data(proxyModel->index(row, 5)).toString();
            if (birthday != birthdayText) matches = false;
        }

        if (matches && !addressText.isEmpty()) {
            QString address = proxyModel->data(proxyModel->index(row, 6)).toString();
            if (!address.contains(addressText, Qt::CaseInsensitive)) matches = false;
        }

        ui->tableView->setRowHidden(row, !matches);

        if (matches) {
            matchCount++;
        }
    }

    ui->statusbar->showMessage(QString("Found %1 contact(s)").arg(matchCount), 3000);

    // включение сортировки после фильтрации - чтобы не ломалось
    ui->tableView->setSortingEnabled(true);
}

void MainWindow::clearAllFilters()
{
    ui->searchLineEdit->clear();
    proxyModel->setFilterFixedString("");

    for (int i = 0; i < proxyModel->rowCount(); ++i) {
        ui->tableView->setRowHidden(i, false);
    }

    ui->tableView->clearSelection();

    ui->statusbar->showMessage("Showing all contacts", 2000);
    ui->tableView->reset();
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, "About PhoneBook",
        "<h2>PhoneBook Application</h2>"
        "<p>Version 1.0</p>"
        "<p>A simple phone book application with Qt interface.</p>"
        "<p>Features:</p>"
        "<ul>"
        "<li>Add, edit, delete contacts</li>"
        "<li>Search and sort contacts</li>"
        "<li>Save/load to text files</li>"
        "<li>Data validation</li>"
        "</ul>");
}

void MainWindow::setupDatabaseMenu()
{
    QMenu* dbMenu = menuBar()->addMenu("Database");

    // ДОБАВЛЯЕМ ПУНКТ ПОДКЛЮЧЕНИЯ
    QAction* connectAction = dbMenu->addAction("Connect to PostgreSQL");
    connect(connectAction, &QAction::triggered, this, &MainWindow::onConnectToDatabase);

    // Остальные пункты
    QAction* loadDbAction = dbMenu->addAction("Load from DB");
    connect(loadDbAction, &QAction::triggered, this, &MainWindow::onLoadFromDatabase);

    QAction* saveDbAction = dbMenu->addAction("Save to DB");
    connect(saveDbAction, &QAction::triggered, this, &MainWindow::onSaveToDatabase);

    QAction* deleteDbAction = dbMenu->addAction("Delete Selected from DB");
    connect(deleteDbAction, &QAction::triggered, this, &MainWindow::onDeleteFromDatabase);

    // Разделитель
    dbMenu->addSeparator();

    // Кнопка полной очистки базы
    QAction* clearDbAction = dbMenu->addAction("Clear Entire Database");
    connect(clearDbAction, &QAction::triggered, this, [this]() {
        if (QMessageBox::question(this, "Clear Database",
            "This will DELETE ALL CONTACTS from the database!\n"
            "Are you absolutely sure?",
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {

            QSqlQuery query;
            if (query.exec("TRUNCATE TABLE phones CASCADE") &&
                query.exec("TRUNCATE TABLE contacts CASCADE")) {
                ui->statusbar->showMessage("Entire database cleared!", 3000);
            } else {
                ui->statusbar->showMessage("Error clearing database: " + query.lastError().text(), 5000);
            }
        }
    });
}


void MainWindow::onLoadFromDatabase()
{
    if (!m_isDatabaseMode || !m_dbManager.isConnected()) {
        onConnectToDatabase();
        if (!m_isDatabaseMode || !m_dbManager.isConnected()) {
            return;
        }
    }

    try {
        // Спрашиваем подтверждение, если есть локальные контакты
        if (!contacts.empty()) {
            QMessageBox::StandardButton reply = QMessageBox::question(
                this, "Загрузка из базы",
                QString("Все текущие контакты будут заменены данными из базы (%1 контактов).\nПродолжить?")
                    .arg(m_dbManager.loadAllContacts().size()),
                QMessageBox::Yes | QMessageBox::No, QMessageBox::No
            );

            if (reply != QMessageBox::Yes) {
                return;
            }
        }

        QVector<Contact> dbContacts = m_dbManager.loadAllContacts();
        contacts.clear();

        for (const Contact& contact : dbContacts) {
            contacts.push_back(contact);
        }

        updateContactTable();
        ui->statusbar->showMessage(
            QString("Загружено %1 контактов из базы").arg(contacts.size()),
            3000
        );

    } catch (const std::exception& e) {
        ui->statusbar->showMessage("Ошибка загрузки: " + QString::fromStdString(e.what()), 5000);
    }
}

void MainWindow::onSaveToDatabase()
{
    if (!m_isDatabaseMode || !m_dbManager.isConnected()) {
        onConnectToDatabase();
        if (!m_isDatabaseMode || !m_dbManager.isConnected()) {
            return;
        }
    }

    try {
        // Загружаем существующие контакты из базы для сравнения
        QVector<Contact> existingContacts = m_dbManager.loadAllContacts();
        QSet<int> existingIds;

        for (const Contact& contact : existingContacts) {
            existingIds.insert(contact.getId());
        }

        int newCount = 0;
        int updatedCount = 0;
        int deletedCount = 0;
        int errorCount = 0;

        // 1. Сохраняем/обновляем контакты из приложения
        for (auto& contact : contacts) {
            if (contact.getId() == -1 || !existingIds.contains(contact.getId())) {
                // Новый контакт - сохраняем
                int newId;
                if (m_dbManager.saveContact(contact, &newId)) {
                    contact.setId(newId);
                    newCount++;
                } else {
                    errorCount++;
                    qDebug() << "Ошибка сохранения нового контакта:" << m_dbManager.lastError();
                }
            } else {
                // Существующий контакт - обновляем
                if (m_dbManager.updateContact(contact.getId(), contact)) {
                    updatedCount++;
                } else {
                    errorCount++;
                    qDebug() << "Ошибка обновления контакта ID" << contact.getId()
                             << ":" << m_dbManager.lastError();
                }
            }
        }

        // 2. Удаляем контакты, которых нет в приложении, но есть в базе
        for (const Contact& dbContact : existingContacts) {
            bool found = false;
            for (const Contact& appContact : contacts) {
                if (appContact.getId() == dbContact.getId()) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                if (m_dbManager.deleteContact(dbContact.getId())) {
                    deletedCount++;
                } else {
                    errorCount++;
                    qDebug() << "Ошибка удаления контакта ID" << dbContact.getId()
                             << ":" << m_dbManager.lastError();
                }
            }
        }

        QString message = QString("БД синхронизирована: +%1 новых, ~%2 обновлено, -%3 удалено")
            .arg(newCount).arg(updatedCount).arg(deletedCount);

        if (errorCount > 0) {
            message += QString(", %1 ошибок").arg(errorCount);
        }

        ui->statusbar->showMessage(message, 5000);

        // Обновляем локальные данные после синхронизации
        if (newCount > 0 || deletedCount > 0) {
            onLoadFromDatabase();
        }

    } catch (const std::exception& e) {
        ui->statusbar->showMessage("Ошибка синхронизации: " + QString::fromStdString(e.what()), 5000);
    }
}

void MainWindow::onConnectToDatabase()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Подключение к базе данных");

    QFormLayout* layout = new QFormLayout(&dialog);
    QLineEdit* dbEdit = new QLineEdit("postgres", &dialog);
    QLineEdit* userEdit = new QLineEdit("postgres", &dialog);
    QLineEdit* passEdit = new QLineEdit(&dialog);
    passEdit->setEchoMode(QLineEdit::Password);

    layout->addRow("База данных:", dbEdit);
    layout->addRow("Пользователь:", userEdit);
    layout->addRow("Пароль:", passEdit);

    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        if (m_dbManager.connectToDatabase(dbEdit->text(), userEdit->text(), passEdit->text())) {
            ui->statusbar->showMessage("Успешное подключение к PostgreSQL!", 5000);
            m_isDatabaseMode = true;

            // Добавляем визуальный индикатор
            if (!ui->statusbar->findChild<QLabel*>("dbStatus")) {
                QLabel* dbStatus = new QLabel("PostgreSQL", this);
                dbStatus->setObjectName("dbStatus");
                dbStatus->setStyleSheet("color: green; font-weight: bold;");
                ui->statusbar->addWidget(dbStatus);
            }
        } else {
            // ОШИБКА ПОДКЛЮЧЕНИЯ
            QString errorMsg = "Ошибка подключения:\n" + m_dbManager.lastError();
            ui->statusbar->showMessage(errorMsg, 10000);

            // Показываем детальное окно ошибки
            QMessageBox::critical(this, "Ошибка базы данных",
                "Не удалось подключиться к PostgreSQL:\n\n" +
                m_dbManager.lastError() + "\n\n" +
                "Проверьте:\n" +
                "• Сервер PostgreSQL запущен\n" +
                "• Правильные логин/пароль\n" +
                "• Существует база данных");
        }
    }
}

void MainWindow::onDeleteFromDatabase()
{
    if (!m_isDatabaseMode || !m_dbManager.isConnected()) {
        onConnectToDatabase();
        if (!m_isDatabaseMode || !m_dbManager.isConnected()) {
            return;
        }
    }

    int index = getSelectedContactIndex();
    if (index == -1) {
        QMessageBox::warning(this, "Warning", "Please select a contact to delete from database.");
        return;
    }

    Contact contactToDelete = contacts[index];
    if (contactToDelete.getId() == -1) {
        QMessageBox::warning(this, "Warning", "This contact is not in the database yet.\nSave it first before deleting from database.");
        return;
    }

    QString contactName = QString("%1 %2").arg(contactToDelete.getSurnameQt()).arg(contactToDelete.getNameQt());

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Delete from Database",
        QString("This will delete contact '%1' ONLY from the database.\n"
                "The contact will remain in the application.\n"
                "Are you sure?")
            .arg(contactName),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (m_dbManager.deleteContact(contactToDelete.getId())) {
            // Обнуляем ID контакта, чтобы он считался новым при следующем сохранении
            contactToDelete.setId(-1);
            contacts[index] = contactToDelete;
            updateContactTable();

            ui->statusbar->showMessage(QString("Contact '%1' deleted from database only").arg(contactName), 3000);
        } else {
            ui->statusbar->showMessage(QString("Failed to delete contact '%1' from database: %2")
                .arg(contactName).arg(m_dbManager.lastError()), 5000);
        }
    }
}
