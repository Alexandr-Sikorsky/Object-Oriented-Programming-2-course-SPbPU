#include "ui.h"

#include <iostream>

using namespace std;

void Clear() {
    for (int i = 0; i < 50; i++) {
        cout << endl;
    }
}

//////////////////////////   1. Show all Contacts   ////////////////////////////

void showContacts(const vector<int>& indexes, bool showIndex = false) {

    if (indexes.empty() && showIndex) {
        cout << "There are no contacts to display" << endl;
        cout << "Press Enter to return...";
        cin.get();
        return;
    }

    if (indexes.empty() && !showIndex) {
        cout << "The Phonebook is empty" << endl;
        cout << "Press Enter to return...";
        cin.get();
        return;
    }

    else {
        for (size_t i = 0; i < indexes.size(); i++) {
            const Contact& currentContact = contacts[indexes[i]];


            if (showIndex) {
                cout << "[" << i + 1 << "] ";
            } 
            else {
                cout << "- ";
            }

            cout << "Name: " << currentContact.getSurname() << " " << currentContact.getName() << " ";
            if (!currentContact.getPatronymic().empty()) {
                cout << currentContact.getPatronymic();
            }
            cout << " | Email: " << currentContact.getEmail() << " | Phones: ";

            vector<Phone> phones = currentContact.getPhones();
            for (size_t j = 0; j < phones.size(); j++) {
                cout << phones[j].number << " (" << phones[j].type << ")";
                if (j < phones.size() - 1) cout << ", ";
            }
            
            if(currentContact.getBday() != "") cout << " | BDay: " << currentContact.getBday();
            if(currentContact.getAddress() != "") cout << " | Address: " << currentContact.getAddress();
            cout << endl;
        }

        // cout << "Press Enter to return...";
        // cin.get();
    }

}


void showAllContacts(bool showIndex = false) {
    if (!showIndex){
        Clear();
        cout << "==========================================================" << endl;
        cout << "                     ALL CONTACTS" << endl;
        cout << "==========================================================" << endl;
    }
    cout << endl;
    vector<int> allIndexes;
    for (size_t i = 0; i < contacts.size(); i++) {
        allIndexes.push_back(i);
    }
    showContacts(allIndexes, showIndex);

    if (!showIndex){
        cout << endl << "Press Enter to return...";
        cin.get();
    }
}




string selectPhoneType() {
    while (true)
    {
        cout << "Phone types: 1 - WORK, 2 - HOME, 3 - OFFICE" << endl;
        cout << "Select type (1-3, default 1. Press Enter to skip): ";
        
        string choice;
        getline(cin, choice);
        
        if (choice.empty() || choice == "1") return "WORK";
        if (choice == "2") return "HOME";
        if (choice == "3") return "OFFICE";
        else{
            cout << endl << "Error: Incorrect input. Repeat or press Enter to skip" << endl << endl;
        }
    }
}


bool askYesNo(const string& question) {
    while (true) {
        cout << question << " (y/n): ";
        string answer;
        getline(cin, answer);
        
        if (answer == "y" || answer == "Y") return true;
        if (answer == "n" || answer == "N") return false;
        
        cout << "Please enter 'y' or 'n'." << endl;
    }
}


//////////////////////////////////   MENU   //////////////////////////////////

int getMenuChoice() {
    int choice;
    cout << "Select an action: ";

    while (!(cin >> choice)) {
        cout << "Error: Enter a number" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}


void showMenu() {
    Clear();
    cout << "|====================================================================|" << endl;
    cout << "|     |=======================================================|      |" << endl;
    cout << "|     |                        PHONEBOOK                      |      |" << endl;
    cout << "|     |=======================================================|      |" << endl;
    cout << "|====================================================================|" << endl << endl;
    cout << "1. Show all Contacts" << endl;
    cout << "2. Add new Contact" << endl;
    cout << "3. Edit Contact" << endl;
    cout << "4. Delete Contact" << endl;
    cout << "5. Contact search" << endl;
    cout << "6. Sort Contacts" << endl;
    cout << "0. Exit" << endl << endl;
}

