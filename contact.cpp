#include "contact.h"
#include <algorithm>

Contact::Contact() : m_id(-1), name(""), surname(""), email(""), patronymic(""), bday(""), address("") {}
Contact::Contact(int id) : m_id(id), name(""), surname(""), email(""),
                           patronymic(""), bday(""), address("") {}
Contact::Contact(const string& n, const string& s, const string& e, const vector <Phone>& ph, const string& p, const string& b, const string& a, int id):
    m_id(id), name(n), surname(s), email(e), phones (ph), patronymic(p), bday(b), address(a){}

Contact::~Contact(){};

//////////////////////////////////   SETTERS   //////////////////////////////////
void Contact::setName(const string& new_name){ name = new_name; }
void Contact::setSurname(const string& new_surname){ surname = new_surname; }
void Contact::setEmail(const string& new_email) { email = new_email; }
void Contact::setPhones(const vector<Phone>& ph) { phones = ph; }
void Contact::addPhone(const string& phone, const string& type) { 
    if (!phone.empty()) phones.push_back(Phone(phone, type)); 
}
void Contact::setPatronymic(const string& added_patronymic) { patronymic = added_patronymic; }
void Contact::setBday(const string& birthDate) { bday = birthDate; }
void Contact::setAddress(const string& added_address) { address = added_address; }

//////////////////////////////////   GETTERS   //////////////////////////////////
string Contact::getName() const { return name; }
string Contact::getSurname() const { return surname; }
string Contact::getEmail() const { return email; }
vector<Phone> Contact::getPhones() const { return phones; }

vector<string> Contact::getPhoneNumbers() const {
    vector<string> numbers;
    for (const Phone& phone : phones) {
        numbers.push_back(phone.number);
    }
    return numbers;
}

string Contact::getFirstPhone() const { 
    return phones.empty() ? "" : phones[0].number; 
}

string Contact::getFirstPhoneNumber() const { 
    if (phones.empty()) return "";
    return phones[0].number + " (" + phones[0].type + ")";
} //*

string Contact::getPatronymic() const { return patronymic; }
string Contact::getBday() const { return bday; }
string Contact::getAddress() const { return address; }

//////////////////////////////////   Qt методы   //////////////////////////////////
QString Contact::getNameQt() const { return QString::fromStdString(name); }
QString Contact::getSurnameQt() const { return QString::fromStdString(surname); }
QString Contact::getEmailQt() const { return QString::fromStdString(email); }
QString Contact::getPatronymicQt() const { return QString::fromStdString(patronymic); }
QString Contact::getBdayQt() const { return QString::fromStdString(bday); }
QString Contact::getAddressQt() const { return QString::fromStdString(address); }

QList<QPair<QString, QString>> Contact::getPhonesQt() const {
    QList<QPair<QString, QString>> result;
    for (const auto& phone : phones) {
        result.append(qMakePair(
            QString::fromStdString(phone.number),
            QString::fromStdString(phone.type)
        ));
    }
    return result;
}

void Contact::setNameQt(const QString& name) { this->name = name.toStdString(); }
void Contact::setSurnameQt(const QString& surname) { this->surname = surname.toStdString(); }
void Contact::setEmailQt(const QString& email) { this->email = email.toStdString(); }
void Contact::setPatronymicQt(const QString& patronymic) { this->patronymic = patronymic.toStdString(); }
void Contact::setBdayQt(const QString& bday) { this->bday = bday.toStdString(); }
void Contact::setAddressQt(const QString& address) { this->address = address.toStdString(); }
void Contact::addPhoneQt(const QString& phone, const QString& type) {
    if (!phone.isEmpty()) {
        phones.push_back(Phone(phone.toStdString(), type.toStdString()));
    }
}

void Contact::removePhone(int index) {
    if (index >= 0 && static_cast<size_t>(index) < phones.size()) {
        phones.erase(phones.begin() + index);
    }
}

void Contact::removePhoneQt(int index) {
    if (index >= 0 && static_cast<size_t>(index) < phones.size()) {
        phones.erase(phones.begin() + index);
    }
}
