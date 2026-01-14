#ifndef CONTACTDIALOG_H
#define CONTACTDIALOG_H

#include <QDialog>
#include "contact.h"
#include "validator.h"

namespace Ui {
class ContactDialog;
}

class ContactDialog : public QDialog // окно для редактирования контакта - валидация + формирование объекта Contact
{
    Q_OBJECT

public:
    explicit ContactDialog(QWidget *parent = nullptr);
    ~ContactDialog();

    Contact getContact() const;
    void setContact(const Contact &contact);

private slots:
    void onAddPhoneClicked();
    void onRemovePhoneClicked();
    void validateForm();

    void accept() override;

private:
    Ui::ContactDialog *ui;
    Contact currentContact;
    Validator validator;

    void setupUi();
    void updatePhoneList();
    void showValidationError(const QString &message);
};

#endif // CONTACTDIALOG_H
