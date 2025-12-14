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
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>

// CaseInsensitive - регистр не важен

extern std::vector<Contact> contacts;
extern QString currentFilename;
extern Validator validator;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // весь интерфейс пользователя
{
    ui->setupUi(this);

    contactModel = new QStandardItemModel(this); // модель таблицы Qt - представление данных (не сами данные)
    proxyModel = new QSortFilterProxyModel(this); // модель для сортировки и фильтрации (без изменения реального порядка контактов)
    proxyModel->setSourceModel(contactModel);

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
    statusBar()->showMessage(QString("Total contacts: %1").arg(contacts.size()));
}

void MainWindow::loadContacts()
{
    readFileQt(currentFilename);
    updateContactTable();
}

void MainWindow::saveContacts()
{

    saveToFileQt(currentFilename);
    statusBar()->showMessage("Contacts saved to " + currentFilename, 3000);
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
        statusBar()->showMessage("Contact added successfully", 3000);
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

        statusBar()->showMessage("Contact updated successfully", 3000);
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
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Delete",
        QString("Are you sure you want to delete contact:\n%1 %2?")
            .arg(contactToDelete.getSurnameQt())
            .arg(contactToDelete.getNameQt()),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        contacts.erase(contacts.begin() + index);
        updateContactTable();
        saveContacts();

        statusBar()->showMessage("Contact deleted successfully", 3000);
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

    statusBar()->showMessage(QString("Found %1 contact(s)").arg(matchCount), 3000);
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
        statusBar()->showMessage("Contacts loaded from " + currentFilename, 3000);
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

    statusBar()->showMessage(QString("Found %1 contact(s)").arg(matchCount), 3000);

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

    statusBar()->showMessage("Showing all contacts", 2000);
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
