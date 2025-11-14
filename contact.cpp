#include "contact.h"
using namespace std; 


Contact::Contact(const string& n, const string& s, const string& e, const vector <Phone>& ph, const string& p, const string& b, const string& a): 
    name(n), surname(s), email(e), phones (ph), patronymic(p), bday(b), address(a){}

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