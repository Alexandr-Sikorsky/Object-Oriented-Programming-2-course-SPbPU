#include "contactdialog.h"
#include "ui_contactdialog.h"
#include <QMessageBox>
#include <QListWidgetItem>

ContactDialog::ContactDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ContactDialog)
{
    ui->setupUi(this);
    setupUi();

    // Подключаем сигналы кнопок
    connect(ui->addPhoneButton, &QPushButton::clicked, this, &ContactDialog::onAddPhoneClicked);
    connect(ui->removePhoneButton, &QPushButton::clicked, this, &ContactDialog::onRemovePhoneClicked);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &ContactDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &ContactDialog::reject);


    // Настройка валидаторов
    ui->nameEdit->setPlaceholderText("Enter name (letters only)");
    ui->surnameEdit->setPlaceholderText("Enter surname (letters only)");
    ui->emailEdit->setPlaceholderText("user@example.com");
    ui->phoneEdit->setPlaceholderText("+79991234567 or 8(999)123-45-67");
    ui->patronymicEdit->setPlaceholderText("Optional");
    ui->addressEdit->setPlaceholderText("Optional address");

    // Настройка QDateEdit для даты рождения
    ui->birthdayEdit->setDisplayFormat("dd.MM.yyyy");
    ui->birthdayEdit->setDate(QDate::currentDate());
    ui->birthdayEdit->setCalendarPopup(true);
    ui->birthdayEdit->setMaximumDate(QDate::currentDate());

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

ContactDialog::~ContactDialog()
{
    delete ui;
}

void ContactDialog::setupUi()
{
    ui->phoneTypeCombo->clear();
    ui->phoneTypeCombo->addItems({"WORK", "HOME", "OFFICE"});
    ui->phoneTypeCombo->setCurrentIndex(0);
}

Contact ContactDialog::getContact() const
{
    return currentContact;
}

void ContactDialog::setContact(const Contact &contact)
{
    currentContact = contact;

    // Заполняем поля из контакта
    ui->nameEdit->setText(contact.getNameQt());
    ui->surnameEdit->setText(contact.getSurnameQt());
    ui->patronymicEdit->setText(contact.getPatronymicQt());
    ui->emailEdit->setText(contact.getEmailQt());

    // Обрабатываем дату рождения
    QString bday = contact.getBdayQt();
    if (!bday.isEmpty()) {
        QStringList parts = bday.split('.');
        if (parts.size() == 3) {
            int day = parts[0].toInt();
            int month = parts[1].toInt();
            int year = parts[2].toInt();
            ui->birthdayEdit->setDate(QDate(year, month, day));
        }
    } else {
        ui->birthdayEdit->setDate(QDate::currentDate());
    }

    ui->addressEdit->setText(contact.getAddressQt());
    updatePhoneList();

    validateForm();
}

void ContactDialog::updatePhoneList()
{
    ui->phoneList->clear();
    auto phones = currentContact.getPhonesQt();
    for (const auto &phone : phones) {
        QString itemText = phone.first + " (" + phone.second + ")";
        QListWidgetItem *item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, phone.first); // Сохраняем номер для быстрого доступа
        ui->phoneList->addItem(item);
    }

    // Активируем/деактивируем кнопку удаления
    ui->removePhoneButton->setEnabled(ui->phoneList->count() > 1);
}

void ContactDialog::onAddPhoneClicked()
{
    QString phoneNumber = ui->phoneEdit->text().trimmed();
    if (phoneNumber.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a phone number.");
        return;
    }


    // Упрощенная проверка телефона
    QString validatedPhone;

    // Удаляем все нецифровые символы для проверки
    QString digitsOnly;
    for (QChar c : phoneNumber) {
        if (c.isDigit()) {
            digitsOnly.append(c);
        }
    }


    if (digitsOnly.length() == 11) {
        // Преобразуем к стандартному виду 8XXXXXXXXXX
        if (digitsOnly.startsWith("7")) {
            digitsOnly[0] = '8';
        }
        validatedPhone = digitsOnly;
    } else if (digitsOnly.length() == 10) {
        // Добавляем 8 в начало
        validatedPhone = "8" + digitsOnly;
    } else {
        QMessageBox::warning(this, "Error",
            "Phone number must contain 10 or 11 digits.\n"
            "Examples: +79991234567, 8(999)1234567");
        return;
    }


    QString phoneType = ui->phoneTypeCombo->currentText();
    currentContact.addPhoneQt(validatedPhone, phoneType);
    updatePhoneList();

    ui->phoneEdit->clear();
    ui->phoneEdit->setFocus();

    ui->statusLabel->setText("Phone added: " + validatedPhone);
    ui->statusLabel->setStyleSheet("color: green;");

}

void ContactDialog::onRemovePhoneClicked()
{
    QListWidgetItem *item = ui->phoneList->currentItem();
    if (!item) {
        showValidationError("Please select a phone number to remove.");
        return;
    }

    // Нельзя удалить последний телефон
    if (ui->phoneList->count() <= 1) {
        showValidationError("Cannot delete the last phone number. At least one phone is required.");
        return;
    }

    int row = ui->phoneList->row(item);
    currentContact.removePhoneQt(row);
    updatePhoneList();

    ui->statusLabel->setText("Phone number removed");
    ui->statusLabel->setStyleSheet("color: orange;");
}

void ContactDialog::validateForm()
{
    bool isValid =
        !ui->nameEdit->text().trimmed().isEmpty() &&
        !ui->surnameEdit->text().trimmed().isEmpty() &&
        !ui->emailEdit->text().trimmed().isEmpty() &&
        !currentContact.getPhones().empty();

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(isValid);
}

void ContactDialog::showValidationError(const QString &message)
{
    ui->statusLabel->setText(message);
    ui->statusLabel->setStyleSheet("color: red;");
}

void ContactDialog::accept()
{
    // Переносим данные из UI в объект Contact
    // До этого currentContact - как временный контейнер

    // Проверяем все поля перед принятием
    QString name = ui->nameEdit->text().trimmed();
    QString surname = ui->surnameEdit->text().trimmed();
    QString patronymic = ui->patronymicEdit->text().trimmed();
    QString email = ui->emailEdit->text().trimmed();
    QString address = ui->addressEdit->text().trimmed();
    QDate birthday = ui->birthdayEdit->date();

    // 1. Проверка обязательных полей
    if (name.isEmpty() || surname.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Error",
            "Please fill all required fields: Name, Surname, Email.");
        return;
    }

    // 2. Проверка телефонов
    if (currentContact.getPhones().empty()) {
        QMessageBox::warning(this, "Error",
            "At least one phone number is required.");
        return;
    }

    // 3. Валидация имени
    QString validatedName = validator.validateNameQt(name);
    if (validatedName.isEmpty()) {
        QMessageBox::warning(this, "Error",
            "Invalid name format.\n"
            "• Must start with a letter\n"
            "• Can contain letters, digits, hyphen, space\n"
            "• Cannot end with hyphen\n"
            "Example: John, Anna-Maria, Jean Luc");
        ui->nameEdit->setFocus();
        return;
    }

    // 4. Валидация фамилии
    QString validatedSurname = validator.validateSurnameQt(surname);
    if (validatedSurname.isEmpty()) {
        QMessageBox::warning(this, "Error",
            "Invalid surname format.\n"
            "• Must start with a letter\n"
            "• Can contain letters, digits, hyphen, space\n"
            "• Cannot end with hyphen\n"
            "Example: Smith, O'Brien, van der Berg");
        ui->surnameEdit->setFocus();
        return;
    }

    // 5. Валидация email (САМОЕ ВАЖНОЕ!)
    QString validatedEmail = validator.validateEmailQt(email, name);
    if (validatedEmail.isEmpty()) {
        QMessageBox::warning(this, "Error",
            QString("Invalid email format or email doesn't contain your name.\n\n"
                   "For name '%1', email should be like:\n"
                   "• %2@example.com\n"
                   "• %2.johnson@company.org\n\n"
                   "Basic email format: username@domain.com")
                .arg(name)
                .arg(name.toLower()));
        ui->emailEdit->setFocus();
        ui->emailEdit->selectAll();
        return;
    }

    // 6. Валидация отчества (опционально)
    QString validatedPatronymic = validator.validatePatronymicQt(patronymic);
    if (patronymic.isEmpty()) {
        validatedPatronymic = ""; // Пустое отчество - нормально
    } else if (validatedPatronymic.isEmpty()) {
        QMessageBox::warning(this, "Error", "Invalid patronymic format.");
        ui->patronymicEdit->setFocus();
        return;
    }

    // 7. Валидация даты рождения (если указана)
    QString birthdayStr;
    if (birthday != QDate::currentDate()) {
        QString dateText = birthday.toString("dd.MM.yyyy");
        QString validatedDate = validator.validateBirthdayQt(dateText);
        if (validatedDate.isEmpty()) {
            QMessageBox::warning(this, "Error",
                "Invalid birth date or date is in the future.");
            ui->birthdayEdit->setFocus();
            return;
        }
        birthdayStr = validatedDate;
    }

    // 8. ВСЕ ПРОВЕРКИ ПРОЙДЕНЫ - сохраняем контакт
    currentContact.setNameQt(validatedName);
    currentContact.setSurnameQt(validatedSurname);
    currentContact.setPatronymicQt(validatedPatronymic);
    currentContact.setEmailQt(validatedEmail);
    currentContact.setBdayQt(birthdayStr);
    currentContact.setAddressQt(validator.validateAddressQt(address));


    // 9. Закрываем диалог
    QDialog::accept();
}
