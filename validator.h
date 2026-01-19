#pragma once
#include <string>
#include <regex>
#include <algorithm>
#include <ctime>
#include <QDate>
#include <QRegularExpression>
#include <QString>

using namespace std;

class Validator {
private:
    regex NAME;
    regex EMAIL;
    regex PHONEONE;
    regex PHONETWO;
    regex PHONETHREE;
    regex DATE;

    QRegularExpression qNAME;  // Qt
    QRegularExpression qEMAIL;
    QRegularExpression qPHONEONE;
    QRegularExpression qPHONETWO;
    QRegularExpression qPHONETHREE;
    QRegularExpression qDATE;

    string trim(const string& str) const;
    QString qTrim(const QString& str) const;

public:
    Validator();

//////////////////////////////////   Required fields   //////////////////////////////////
    string validateName(const string& input);
    string validateSurname(const string& input);
    string validateEmailWithoutUsername(const string& input);
    string validateEmail(const string& email, const string& username);

    QString validateNameQt(const QString& input);
    QString validateSurnameQt(const QString& input);
    QString validateEmailWithoutUsernameQt(const QString& input);
    QString validateEmailQt(const QString& input, const QString& username);


//////////////////////////////////   Optional fields   //////////////////////////////////
    string validatePhone(const string& input);
    string validatePatronymic(const string& input);
    string validateBirthday(const string& input);
    string validateAddress(const string& input);

    QString validatePhoneQt(const QString& input);
    QString validatePatronymicQt(const QString& input);
    QString validateBirthdayQt(const QString& input);
    QString validateAddressQt(const QString& input);


};
