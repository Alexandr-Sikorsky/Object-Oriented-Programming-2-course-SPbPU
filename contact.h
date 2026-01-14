#ifndef CONTACT_H
#define CONTACT_H

#include <string>
#include <vector>
#include <QString>
#include <QList>

using namespace std;


struct Phone {
    string number;
    string type;
    
    Phone(const string& num, const string& t = "WORK") 
    : number(num), type(t) {}
};

class Contact
{
    private:
        int m_id = -1;
        string name;
        string surname;
        string email;
        vector <Phone> phones;
        string patronymic;
        string bday;
        string address;

    public:
        Contact();
        explicit Contact(int id);
        Contact(const string& n, const string& s, const string& e, const vector<Phone>& ph, const string& p, const string& d, const string& a,  int id = -1);

        int getId() const { return m_id; }
        void setId(int id) { m_id = id; }

        void setName(const string& name);
        void setSurname(const string& surname);
        void setEmail(const string& email);
        void setPhones(const vector<Phone>& phones);
        void addPhone(const string& phone, const string& type = "WORK");
        void setPatronymic(const string& patronymic);
        void setBday(const string& bday);
        void setAddress(const string& address);
        void removePhone(int index);

        string getName() const;
        string getSurname() const;
        string getEmail() const;
        vector<Phone> getPhones() const;
        vector<string> getPhoneNumbers() const;
        string getFirstPhone() const;
        string getFirstPhoneNumber() const; //*
        string getPatronymic() const;
        string getBday() const;
        string getAddress() const;

        //// Qt методы ////
        QString getNameQt() const;
        QString getSurnameQt() const;
        QString getEmailQt() const;
        QList<QPair<QString, QString>> getPhonesQt() const;
        QString getPatronymicQt() const;
        QString getBdayQt() const;
        QString getAddressQt() const;

        void setNameQt(const QString& name);
        void setSurnameQt(const QString& surname);
        void setEmailQt(const QString& email);
        void setPatronymicQt(const QString& patronymic);
        void setBdayQt(const QString& bday);
        void setAddressQt(const QString& address);
        void addPhoneQt(const QString& phone, const QString& type = "WORK");
        void removePhoneQt(int index);

        ~Contact();
};

#endif // CONTACT_H
