#include "search-sort.h"
#include "ui.h"
#include "filework.h"

#include <iostream>

using namespace std;

/////////////////////////////   3. Contact search   /////////////////////////////

int selectSearchField(const vector<bool>& usedFields) {
    while (true) {
        cout << "Available search fields:" << endl;
        if (!usedFields[1]) cout << "1. Name" << endl;
        if (!usedFields[2]) cout << "2. Surname" << endl;
        if (!usedFields[3]) cout << "3. Phone number" << endl;
        if (!usedFields[4]) cout << "4. Email" << endl;
        if (!usedFields[5]) cout << "5. Patronymic" << endl;
        if (!usedFields[6]) cout << "6. Birthdate" << endl;
        if (!usedFields[7]) cout << "7. Address" << endl;
        cout << "0. Finish adding fields" << endl << endl;

        cout << "Select search field: ";
        string choice;
        cin >> choice;
        cin.ignore();

        // Проверяем что ввод - число
        bool isNumber = true;
        for (char c : choice) {
            if (!isdigit(c)) {
                isNumber = false;
                break;
            }
        }

        if (!isNumber) {
            cout << "Error: Please enter a number!" << endl;
            continue;
        }

        int field = stoi(choice);

        if (field == 0) {
            return 0;
        }

        if (field < 1 || field > 7) {
            cout << "Error: Please select a number from 0 to 7!" << endl;
            continue;
        }

        if (usedFields[field]) {
            cout << "Error: This field has already been used for search!" << endl;
            continue;
        }

        return field;
    }
}


string getSearchValue(int field) {
    string fieldName;
    switch (field) {
        case 1: 
            fieldName = "name"; 
            break;
        case 2: 
            fieldName = "surname"; 
            break;
        case 3: 
            fieldName = "phone number"; 
            break;
        case 4: 
            fieldName = "email"; 
            break;
        case 5: 
            fieldName = "patronymic"; 
            break;
        case 6: 
            fieldName = "birthdate (dd.mm.yyyy)"; 
            break;
        case 7: 
            fieldName = "address"; 
            break;
    }
    
    string value;
    while (true) {
        cout << "Enter " << fieldName << " to search for: ";
        getline(cin, value);
        
        if (value.empty()) {
            cout << "Error: Search query cannot be empty!" << endl;
            continue;
        }
        
        break;
    }
    
    return value;
}


bool checkCondition(const Contact& contact, int field, const string& value) {
    switch (field) {
        case 1: 
            return contact.getName() == value;
        case 2: 
            return contact.getSurname() == value;
        case 3: 
            for (const Phone& phone : contact.getPhones()) {
                if (phone.number == value) {
                    return true;
                }
            }
            return false;
        case 4: 
            return contact.getEmail() == value;
        case 5: 
            return contact.getPatronymic() == value;
        case 6: 
            return contact.getBday() == value;
        case 7: 
            return contact.getAddress() == value;
        default: 
            return false;
    }
}


void findContact(const vector<SearchCondition>& conditions){
    vector<int> result;

    for (size_t i = 0; i < contacts.size(); i++) {
        const Contact& curContact = contacts[i];
        bool satisfiesAllConditions = true;

        for (const SearchCondition& condition : conditions) {
            if (!checkCondition(curContact, condition.field, condition.value)) {
                satisfiesAllConditions = false;
                break;
            }
        }

        if (satisfiesAllConditions) {
            result.push_back(i);
        }
    }
    if (result.empty()) {
        cout << "No contacts found matching your criteria." << endl << endl;
    } 
    else 
    {
        cout << "Found " << result.size() << " contact(s):" << endl << endl;
        showContacts(result, true);
    }
}


void searchContact() {
    while(true){
        Clear();

        cout << "==========================================================" << endl;
        cout << "                   SEARCHING CONTACT" << endl;
        cout << "==========================================================" << endl << endl;

        if (contacts.empty()) {
            cout << "Contact list is empty" << endl;
            cout << "Press Enter to return..." << endl;
            cin.get();
            return;
        }

        vector<SearchCondition> searchConditions;
        vector<bool> usedFields(8, false); // 0-7, uses 1-7

        while(true){
        cout << "==========================================================" << endl;
        cout << "                ADDING SEARCH FIELD" << endl;
        cout << "==========================================================" << endl << endl;

            if (!searchConditions.empty()) {
                cout << "Current search field:" << endl;
                for (const SearchCondition& condition : searchConditions) 
                {
                    string fieldName;
                    switch (condition.field) 
                    {
                        case 1: 
                            fieldName = "Name"; 
                            break;
                        case 2: 
                            fieldName = "Surname"; 
                            break;
                        case 3: 
                            fieldName = "Phone"; 
                            break;
                        case 4: 
                            fieldName = "Email"; 
                            break;
                        case 5: 
                            fieldName = "Patronymic"; 
                            break;
                        case 6: 
                            fieldName = "Birthdate"; 
                            break;
                        case 7: 
                            fieldName = "Address"; 
                            break;
                    }
                    cout << "  - " << fieldName << ": " << condition.value << endl;
                }
                cout << endl;
            }

            bool hasAvailableFields = false; //check - there are any more available fields
            for (int i = 1; i <= 7; i++) {
                if (!usedFields[i]) 
                {
                    hasAvailableFields = true;
                    break;
                }
            }
                
            if (!hasAvailableFields) 
            { //no? - nu go otsuda
                cout << "All search fields have been used." << endl;
                break;
            }

            int selectedField = selectSearchField(usedFields);
            
            if (selectedField == 0) {
                
                if (searchConditions.empty()) 
                { //too early
                    cout << "No search field specified. Returning to menu." << endl;
                    cout << "Press Enter to continue...";
                    cin.get();
                    return;
                }
                break;
            }

            string value = getSearchValue(selectedField);
            
            SearchCondition newCondition;

            newCondition.field = selectedField;
            newCondition.value = value;

            searchConditions.push_back(newCondition);
            usedFields[selectedField] = true;

            if (hasAvailableFields)
            { 
                //another one byte..so.. search field?
                if (!askYesNo("Add another search field?")) 
                {
                    break;
                }
            }
        }

        if (!searchConditions.empty()) {
            Clear();
            cout << "==========================================================" << endl;
            cout << "                     SEARCH RESULTS" << endl;
            cout << "==========================================================" << endl << endl;
            
            findContact(searchConditions);
        }
        
        
        // Another one?
        if (!askYesNo("Perform another search?")) {
            break;
        }

    }
}


/////////////////////////////   6. Sort Contacts   //////////////////////////////

bool compareContacts(const Contact& cA, const Contact& cB, int field, bool ascending) {

    string a_val, b_val;

    switch (field) {
        case 1: 
            a_val = cA.getName(); b_val = cB.getName(); 
            break;
        case 2: 
            a_val = cA.getSurname(); b_val = cB.getSurname(); 
            break;
        case 3: 
            a_val = cA.getEmail(); b_val = cB.getEmail(); 
            break;
        case 4: 
            a_val = cA.getFirstPhone(); b_val = cB.getFirstPhone(); 
            break;
        case 5: 
            a_val = cA.getPatronymic(); b_val = cB.getPatronymic(); 
            break;
        case 6: { // spec for bDay
            const string& dateA = cA.getBday();
            const string& dateB = cB.getBday();
            
            if (dateA.empty() && dateB.empty()) return false;
            if (dateA.empty()) return !ascending; // in end
            if (dateB.empty()) return ascending; 
            
            int yearA = stoi(dateA.substr(6, 4));
            int monthA = stoi(dateA.substr(3, 2));
            int dayA = stoi(dateA.substr(0, 2));
            
            int yearB = stoi(dateB.substr(6, 4));
            int monthB = stoi(dateB.substr(3, 2));
            int dayB = stoi(dateB.substr(0, 2));
            
            int dateValueA = yearA * 10000 + monthA * 100 + dayA; // useful checking format
            int dateValueB = yearB * 10000 + monthB * 100 + dayB;
            
            return ascending ? (dateValueA < dateValueB) : (dateValueA > dateValueB);
        }
        case 7: 
            a_val = cA.getAddress(); b_val = cB.getAddress(); 
            break;
    }
    
    return ascending ? (a_val < b_val) : (a_val > b_val);
}


void sortContacts() {
    while(true){
        Clear();
        cout << "==========================================================" << endl;
        cout << "                    SORTING CONTACTS" << endl;
        cout << "==========================================================" << endl << endl;

        if (contacts.empty()) {
            cout << "Contact list is empty" << endl;
            cout << "Press Enter to return..." << endl;
            cin.get();
            return;
        }

        int field;
        cout << "Select field to sort by:" << endl;
        cout << "1. Name" << endl;
        cout << "2. Surname" << endl;
        cout << "3. Email" << endl;
        cout << "4. Phone numbers" << endl;
        cout << "5. Patronymic" << endl;
        cout << "6. Birthdate" << endl;
        cout << "7. Address" << endl;
        cout << "0. Cancel" << endl << endl;
        while (true){
            cout << "Select field (0-7): ";
            cin >> field;
            cin.ignore();

            if (field == 0) {
                return;
            }
            if (field >= 1 && field <= 7) {
                break;
            }

            cout << "Error: Please select a number from 1 to 7!" << endl;
            continue;
        }
        
        bool ascending;
        cout << endl << "Sort order:" << endl;
        cout << "1. Ascending (A-Z, 0-9)" << endl;
        cout << "2. Descending (Z-A, 9-0)" << endl;
        cout << "0. Cancel" << endl << endl;

        while (true) {
            cout << "Select order (0-2): ";
            int orderChoice;
            cin >> orderChoice;
            cin.ignore();

            if (orderChoice == 0) return;
            if (orderChoice == 1) {
                ascending = true;
                break;
            } else if (orderChoice == 2) {
                ascending = false;
                break;
            }
            
            cout << "Error: Please enter 0, 1 or 2!" << endl;
            continue;
        }

        vector<int> sortedIndexes;
        for (size_t i = 0; i < contacts.size(); i++) {
            sortedIndexes.push_back(i);
        }

        sort(sortedIndexes.begin(), sortedIndexes.end(), [field, ascending](int a, int b) {
            return compareContacts(contacts[a], contacts[b], field, ascending);
        });


        string fieldName;
        switch (field) {
            case 1: 
                fieldName = "Name"; 
                break;
            case 2: 
                fieldName = "Surname"; 
                break;
            case 3: 
                fieldName = "Email"; 
                break;
            case 4: 
                fieldName = "Phone number"; 
                break;
            case 5: 
                fieldName = "Patronymic"; 
                break;
            case 6: 
                fieldName = "Birthdate"; 
                break;
            case 7: 
                fieldName = "Address"; 
                break;
        }

        string orderName = ascending ? "ascending" : "descending";
        cout << endl << "Contacts sorted by " << fieldName << " (" << orderName << ") successfully!" << endl;

        showContacts(sortedIndexes, false);

        if (!askYesNo("Perform another sort?")) {
        break;
        }
    }

}

