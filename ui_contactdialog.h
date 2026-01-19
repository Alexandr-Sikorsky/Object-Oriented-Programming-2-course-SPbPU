/********************************************************************************
** Form generated from reading UI file 'contactdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTACTDIALOG_H
#define UI_CONTACTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ContactDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *nameEdit;
    QLabel *label_2;
    QLineEdit *surnameEdit;
    QLabel *label_3;
    QLineEdit *patronymicEdit;
    QLabel *label_4;
    QLineEdit *emailEdit;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QListWidget *phoneList;
    QHBoxLayout *horizontalLayout;
    QLineEdit *phoneEdit;
    QComboBox *phoneTypeCombo;
    QPushButton *addPhoneButton;
    QPushButton *removePhoneButton;
    QGroupBox *groupBox_3;
    QFormLayout *formLayout_2;
    QLabel *label_5;
    QDateEdit *birthdayEdit;
    QLabel *label_6;
    QLineEdit *addressEdit;
    QLabel *statusLabel;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ContactDialog)
    {
        if (ContactDialog->objectName().isEmpty())
            ContactDialog->setObjectName(QString::fromUtf8("ContactDialog"));
        ContactDialog->resize(500, 500);
        verticalLayout = new QVBoxLayout(ContactDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(ContactDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        formLayout = new QFormLayout(groupBox);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        nameEdit = new QLineEdit(groupBox);
        nameEdit->setObjectName(QString::fromUtf8("nameEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, nameEdit);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        surnameEdit = new QLineEdit(groupBox);
        surnameEdit->setObjectName(QString::fromUtf8("surnameEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, surnameEdit);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        patronymicEdit = new QLineEdit(groupBox);
        patronymicEdit->setObjectName(QString::fromUtf8("patronymicEdit"));

        formLayout->setWidget(2, QFormLayout::FieldRole, patronymicEdit);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        emailEdit = new QLineEdit(groupBox);
        emailEdit->setObjectName(QString::fromUtf8("emailEdit"));

        formLayout->setWidget(3, QFormLayout::FieldRole, emailEdit);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(ContactDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        phoneList = new QListWidget(groupBox_2);
        phoneList->setObjectName(QString::fromUtf8("phoneList"));

        verticalLayout_2->addWidget(phoneList);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        phoneEdit = new QLineEdit(groupBox_2);
        phoneEdit->setObjectName(QString::fromUtf8("phoneEdit"));

        horizontalLayout->addWidget(phoneEdit);

        phoneTypeCombo = new QComboBox(groupBox_2);
        phoneTypeCombo->setObjectName(QString::fromUtf8("phoneTypeCombo"));

        horizontalLayout->addWidget(phoneTypeCombo);

        addPhoneButton = new QPushButton(groupBox_2);
        addPhoneButton->setObjectName(QString::fromUtf8("addPhoneButton"));

        horizontalLayout->addWidget(addPhoneButton);

        removePhoneButton = new QPushButton(groupBox_2);
        removePhoneButton->setObjectName(QString::fromUtf8("removePhoneButton"));

        horizontalLayout->addWidget(removePhoneButton);


        verticalLayout_2->addLayout(horizontalLayout);


        verticalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(ContactDialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        formLayout_2 = new QFormLayout(groupBox_3);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_5);

        birthdayEdit = new QDateEdit(groupBox_3);
        birthdayEdit->setObjectName(QString::fromUtf8("birthdayEdit"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, birthdayEdit);

        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_6);

        addressEdit = new QLineEdit(groupBox_3);
        addressEdit->setObjectName(QString::fromUtf8("addressEdit"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, addressEdit);


        verticalLayout->addWidget(groupBox_3);

        statusLabel = new QLabel(ContactDialog);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));

        verticalLayout->addWidget(statusLabel);

        buttonBox = new QDialogButtonBox(ContactDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ContactDialog);

        QMetaObject::connectSlotsByName(ContactDialog);
    } // setupUi

    void retranslateUi(QDialog *ContactDialog)
    {
        ContactDialog->setWindowTitle(QCoreApplication::translate("ContactDialog", "Contact", nullptr));
        groupBox->setTitle(QCoreApplication::translate("ContactDialog", "Basic Information", nullptr));
        label->setText(QCoreApplication::translate("ContactDialog", "Name:", nullptr));
        label_2->setText(QCoreApplication::translate("ContactDialog", "Surname:", nullptr));
        label_3->setText(QCoreApplication::translate("ContactDialog", "Patronymic:", nullptr));
        label_4->setText(QCoreApplication::translate("ContactDialog", "Email:", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("ContactDialog", "Phones", nullptr));
        addPhoneButton->setText(QCoreApplication::translate("ContactDialog", "Add", nullptr));
        removePhoneButton->setText(QCoreApplication::translate("ContactDialog", "Remove", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("ContactDialog", "Additional Information", nullptr));
        label_5->setText(QCoreApplication::translate("ContactDialog", "Birthday:", nullptr));
        birthdayEdit->setDisplayFormat(QCoreApplication::translate("ContactDialog", "dd.MM.yyyy", nullptr));
        label_6->setText(QCoreApplication::translate("ContactDialog", "Address:", nullptr));
        statusLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ContactDialog: public Ui_ContactDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTACTDIALOG_H
