#include "validator.h"
#include <iostream>
#include <regex>
#include <QRegularExpressionValidator>
#include <QDate>
#include <QDebug>

using namespace std;

Validator::Validator():
    NAME(R"(^[a-zA-Z][a-zA-Z\d -]*[a-zA-Z\d]$)"), // name + surname + patronymic
    EMAIL(R"(^[a-zA-Z0-9]+@[a-zA-Z0-9]+\.[a-zA-Z0-9]+$)"), // email
    PHONEONE(R"(^(\+7|8)\d{10}$)"),
    PHONETWO(R"(^(\+7|8)\(\d{3}\)\d{7}$)"), // phones
    PHONETHREE(R"(^(\+7|8)\(\d{3}\)\d{3}-\d{2}-\d{2}$)"),
    DATE(R"(^\d{2}\.\d{2}\.\d{4}$)"), // bday

    // Qt
    qNAME(QRegularExpression("^[a-zA-Z][a-zA-Z\\d -]*[a-zA-Z\\d]$")),
    qEMAIL(QRegularExpression("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$")),
    qPHONEONE(QRegularExpression("^(\\+7|8)\\d{10}$")),
    qPHONETWO(QRegularExpression("^(\\+7|8)\\(\\d{3}\\)\\d{7}$")),
    qPHONETHREE(QRegularExpression("^(\\+7|8)\\(\\d{3}\\)\\d{3}-\\d{2}-\\d{2}$")),
    qDATE(QRegularExpression("^\\d{2}\\.\\d{2}\\.\\d{4}$"))
{
    // Проверка что регулярные выражения валидны
    if (!qNAME.isValid()) qDebug() << "Invalid qNAME regex:" << qNAME.errorString();
    if (!qEMAIL.isValid()) qDebug() << "Invalid qEMAIL regex:" << qEMAIL.errorString();
    if (!qPHONEONE.isValid()) qDebug() << "Invalid qPHONEONE regex:" << qPHONEONE.errorString();
    if (!qPHONETWO.isValid()) qDebug() << "Invalid qPHONETWO regex:" << qPHONETWO.errorString();
    if (!qPHONETHREE.isValid()) qDebug() << "Invalid qPHONETHREE regex:" << qPHONETHREE.errorString();
    if (!qDATE.isValid()) qDebug() << "Invalid qDATE regex:" << qDATE.errorString();
}

string Validator::trim(const string& str) const{
    size_t start = str.find_first_not_of(" \t");
    if (start == string::npos) return "";
    size_t end = str.find_last_not_of(" \t");
    return str.substr(start, end - start + 1);
}

QString Validator::qTrim(const QString& str) const
{
    return str.trimmed();
}

int daysInMonth(int month, int year) {
    static const int days[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 2) {
        bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        return leap ? 29 : 28;
    }
    return days[month];
}

//////////////////////////////////   Required fields   //////////////////////////////////

string Validator::validateName(const string& input) {
    string text = trim(input);
    if (text.empty()) {
        cout << "Error: Name is required!" << endl;
        return "";
    }
    if (!regex_match(text, NAME)) {
        cout << "Error: Name must start with a letter, can contain letters, digits, hyphen (not in end)!" << endl;
        return "";
    }
    return text;
}


string Validator::validateSurname(const string& input) {
    string text = trim(input);
    if (text.empty()) {
        cout << "Error: Surname is required!" << endl;
        return "";
    }
    if (!regex_match(text, NAME)) {
        cout << "Error: Surname must start with a letter, can contain letters, digits, hyphen (not in end)!" << endl;
        return "";
    }
    return text;
}

string Validator::validateEmailWithoutUsername(const string& input) {
    string text = trim(input);
    if (text.empty()) {
        cout << "Error: Email is required!" << endl;
        return "";
    }
    if (!regex_match(text, EMAIL)) {
        cout << "Error: Invalid email format. Example: user@example.com" << endl;
        return "";
    }
    return text;
}

string Validator::validateEmail(const string& input, const string& username) {
    string validated_email = validateEmailWithoutUsername(input);
    if (validated_email.empty()) {
        return "";
    }

    if (!username.empty()) {

        string cleanUsername = username;
        cleanUsername.erase(remove(cleanUsername.begin(), cleanUsername.end(), ' '), cleanUsername.end());

        size_t at_pos = validated_email.find('@');
        if (at_pos != string::npos) {
            string email_username = validated_email.substr(0, at_pos);
            if (email_username.find(cleanUsername) == string::npos) {
                cout << "Error: Email must contain the username (without spaces): " << cleanUsername << endl;
                return "";
            }
        }
    }

    return validated_email;
}


//////////////////////////////////   Optional fields   //////////////////////////////////

string Validator::validatePhone(const string& input) {
    string text = trim(input);
    if (text.empty()) return "";
    if (!regex_match(text, PHONEONE) && !regex_match(text, PHONETWO) && !regex_match(text, PHONETHREE)) {
        cout << "Error: Invalid phone format" << endl <<
                "Examples: +79991234567, 8(999)1234567, +7(812)123-45-67" << endl;
        return "";
    }

    string etext; //to universal format -> 8XXXXXXXXXX
    for (char c : text) {
        if (isdigit(c)){
            etext += c;
        }
    }
    if (etext[0] == '7'){
        etext[0] = '8';
    }

    return etext;

}

string Validator::validatePatronymic(const string& input) {
    string text = trim(input);
    if (text.empty()) return "";
    if (!regex_match(text, NAME)) {
        cout << "Error: Surname must start with a letter, can contain letters, digits, hyphen (not in end)!" << endl;
        return "";
    }
    return text;
}

string Validator::validateBirthday(const string& input) {
    string text = trim(input);
    if (text.empty()) return "";

    if (!regex_match(text, DATE)) {
        cout << "Error: Date must be in format DD.MM.YYYY" << endl;
        return "";
    }

    int day = stoi(text.substr(0, 2));
    int month = stoi(text.substr(3, 2));
    int year = stoi(text.substr(6, 4));

    if (year < 1900 || year > 2025) {
        cout << "Error: Year must be between 1900 and 2025" << endl;
        return "";
    }
    if (month < 1 || month > 12) {
        cout << "Error: Month must be from 01 to 12" << endl;
        return "";
    }
    if (day < 1 || day > daysInMonth(month, year)) {
        cout << "Error: Invalid day for this month" << endl;
        return "";
    }

    time_t now = time(0);
    tm* current_time = localtime(&now);
    
    int current_year = current_time->tm_year + 1900;
    int current_month = current_time->tm_mon + 1;
    int current_day = current_time->tm_mday;
    
    if (year > current_year) {
        cout << "Error: Birthdate cannot be in the future" << endl;
        return "";
    }
    if (year == current_year && month > current_month) {
        cout << "Error: Birthdate cannot be in the future" << endl;
        return "";
    }
    if (year == current_year && month == current_month && day > current_day) {
        cout << "Error: Birthdate cannot be in the future" << endl;
        return "";
    }

    return text;
}

string Validator::validateAddress(const string& input) {
    return trim(input);
}

//////////////////////////////////   Qt методы   //////////////////////////////////

QString Validator::validateNameQt(const QString& input) {
    QString text = qTrim(input);
    if (text.isEmpty()) {
        return QString();
    }
    if (!qNAME.match(text).hasMatch()) {
        return QString();
    }
    return text;
}

QString Validator::validateSurnameQt(const QString& input) {
    QString text = qTrim(input);
    if (text.isEmpty()) {
        return QString();
    }
    if (!qNAME.match(text).hasMatch()) {
        return QString();
    }
    return text;
}

QString Validator::validateEmailQt(const QString& input, const QString& username)
{
    QString text = qTrim(input);
    if (text.isEmpty()) {
        return QString();
    }

    // 1. Проверяем базовый формат email
    if (!qEMAIL.match(text).hasMatch()) {
        return QString();
    }

    // 2. Проверяем что email содержит имя (если username задан)
    if (!username.isEmpty()) {
        QString cleanUsername = qTrim(username);
        cleanUsername.remove(' ');

        // Приводим к нижнему регистру для регистронезависимого сравнения
        QString cleanUsernameLower = cleanUsername.toLower();

        int atPos = text.indexOf('@');
        if (atPos != -1) {
            QString emailUsername = text.left(atPos);

            if (!emailUsername.toLower().contains(cleanUsernameLower)) {
                return QString();
            }
        }
    }

    return text;
}

QString Validator::validatePhoneQt(const QString& input) {
    QString text = qTrim(input);
    if (text.isEmpty()) return QString();

    if (!qPHONEONE.match(text).hasMatch() &&
        !qPHONETWO.match(text).hasMatch() &&
        !qPHONETHREE.match(text).hasMatch()) {
        return QString();
    }

    QString etext;
    for (QChar c : text) {
        if (c.isDigit()) {
            etext.append(c);
        }
    }
    if (etext.startsWith('7')) {
        etext[0] = '8';
    }

    return etext;
}

QString Validator::validatePatronymicQt(const QString& input) {
    QString text = qTrim(input);
    if (text.isEmpty()) return QString();
    if (!qNAME.match(text).hasMatch()) {
        return QString();
    }
    return text;
}

QString Validator::validateBirthdayQt(const QString& input) {
    QString text = qTrim(input);
    if (text.isEmpty()) return QString();

    if (!qDATE.match(text).hasMatch()) {
        return QString();
    }

    int day = text.mid(0, 2).toInt();
    int month = text.mid(3, 2).toInt();
    int year = text.mid(6, 4).toInt();

    if (year < 1900 || year > 2025) {
        return QString();
    }
    if (month < 1 || month > 12) {
        return QString();
    }

    // Проверка дней в месяце
    int daysInMonth;
    if (month == 2) {
        bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        daysInMonth = leap ? 29 : 28;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        daysInMonth = 30;
    } else {
        daysInMonth = 31;
    }

    if (day < 1 || day > daysInMonth) {
        return QString();
    }

    // Используем стандартную библиотеку C++ вместо QDate
    // чтобы избежать зависимости от Qt в этой функции
    time_t now = time(0);
    tm* current_time = localtime(&now);

    int current_year = current_time->tm_year + 1900;
    int current_month = current_time->tm_mon + 1;
    int current_day = current_time->tm_mday;

    if (year > current_year) {
        return QString();
    }
    if (year == current_year && month > current_month) {
        return QString();
    }
    if (year == current_year && month == current_month && day > current_day) {
        return QString();
    }

    return text;
}

QString Validator::validateAddressQt(const QString& input) {
    return qTrim(input);
}
