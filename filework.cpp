#include "filework.h"

#include <iostream>
#include <fstream>
#include <sstream>

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
    ofstream file(currentFilename);
    if (!file.is_open()) {
        cout << "Error: Cannot save to file " << currentFilename << endl;
        return;
    }
    
    for (const Contact& contact : contacts) {
        file << contact.getName() << "|"
             << contact.getSurname() << "|"
             << contact.getEmail() << "|";

        vector<Phone> phones = contact.getPhones();
        for (size_t j = 0; j < phones.size(); j++) {
            file << phones[j].number << ":" << phones[j].type;
            if (j < phones.size() - 1) file << ";";
        }

        file << "|"
             << contact.getPatronymic() << "|"
             << contact.getBday() << "|"
             << contact.getAddress() << endl;
    }
    
    file.close();
}