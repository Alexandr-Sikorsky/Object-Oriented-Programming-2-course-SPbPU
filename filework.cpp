#include "filework.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>

using namespace std;


void readFile(const string& currentFilename) {
    ifstream file(currentFilename);
    if (!file.is_open()) {
        cout << "File " << currentFilename << " not found" << endl;
        return;
    }
    
    contacts.clear();
    string line;
    
    while (getline(file, line)){
        if (line.empty()) continue;

        stringstream ss(line);
        string n, s, e, ph, p, d, a;
        
        getline(ss, n, '|');
        getline(ss, s, '|');
        getline(ss, e, '|');
        getline(ss, ph, '|');
        getline(ss, p, '|');
        getline(ss, d, '|');
        getline(ss, a, '|');

        vector<Phone> phones;
        stringstream phonesStream(ph); //create stream from str ph
        string phoneEntry;
        while (getline(phonesStream, phoneEntry, ';')) { //is working while the data is being read successfully
            if (!phoneEntry.empty()) { //protect from ;;
                size_t colonPos = phoneEntry.find(':');
                if (colonPos != string::npos) { //npos ~ not found
                    string number = phoneEntry.substr(0, colonPos);
                    string type = phoneEntry.substr(colonPos + 1);
                    phones.push_back(Phone(number, type));
                } else {
                    //if no type - old
                    phones.push_back(Phone(phoneEntry, "WORK"));
                }
            }
        }

        contacts.push_back(Contact(n, s, e, phones, p, d, a));
    }
    
    file.close();
    cout << "Contacts loaded from " << currentFilename << endl;
}


void saveToFile() {
    // Преобразуем QString в std::string
    std::string filename = currentFilename.toStdString();
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cout << "Error: Cannot save to file " << filename << std::endl;
        return;
    }

    for (const Contact& contact : contacts) {
        file << contact.getName() << "|"
             << contact.getSurname() << "|"
             << contact.getEmail() << "|";

        std::vector<Phone> phones = contact.getPhones();
        for (size_t j = 0; j < phones.size(); j++) {
            file << phones[j].number << ":" << phones[j].type;
            if (j < phones.size() - 1) file << ";";
        }

        file << "|"
             << contact.getPatronymic() << "|"
             << contact.getBday() << "|"
             << contact.getAddress() << std::endl;
    }

    file.close();
}

void readFileQt(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Error", "Cannot open file: " + filename);
        return;
    }

    contacts.clear();
    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.isEmpty()) continue;

        QStringList parts = line.split('|');
        if (parts.size() < 7) continue;

        string n = parts[0].toStdString();
        string s = parts[1].toStdString();
        string e = parts[2].toStdString();
        QString ph = parts[3];
        string p = parts[4].toStdString();
        string d = parts[5].toStdString();
        string a = parts[6].toStdString();

        vector<Phone> phones;
        QStringList phoneEntries = ph.split(';', Qt::SkipEmptyParts);
        for (const QString& phoneEntry : phoneEntries) {
            QStringList phoneParts = phoneEntry.split(':');
            if (phoneParts.size() == 2) {
                phones.push_back(Phone(phoneParts[0].toStdString(),
                                      phoneParts[1].toStdString()));
            } else if (phoneParts.size() == 1) {
                phones.push_back(Phone(phoneParts[0].toStdString(), "WORK"));
            }
        }

        contacts.push_back(Contact(n, s, e, phones, p, d, a));
    }

    file.close();
}

void saveToFileQt(const QString& filename)
{
    qDebug() << "=== saveToFileQt called ===";
    qDebug() << "Filename:" << filename;
    qDebug() << "Number of contacts:" << contacts.size();

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "ERROR: Cannot open file for writing:" << filename;
        QMessageBox::warning(nullptr, "Error",
            "Cannot save to file:\n" + filename + "\n\n" +
            "Error: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    int savedCount = 0;

    for (const Contact& contact : contacts) {
        out << contact.getNameQt() << "|"
            << contact.getSurnameQt() << "|"
            << contact.getEmailQt() << "|";

        // Телефоны
        vector<Phone> phones = contact.getPhones();
        for (size_t j = 0; j < phones.size(); ++j) {
            out << QString::fromStdString(phones[j].number) << ":"
                << QString::fromStdString(phones[j].type);
            if (j < phones.size() - 1) out << ";";
        }

        out << "|"
            << contact.getPatronymicQt() << "|"
            << contact.getBdayQt() << "|"
            << contact.getAddressQt() << "\n";

        savedCount++;
        qDebug() << "Saved contact:" << contact.getSurnameQt() << contact.getNameQt();
    }

    file.close();
    qDebug() << "Successfully saved" << savedCount << "contacts to" << filename;
}
