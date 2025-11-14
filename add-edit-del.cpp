#include "add-edit-del.h"
#include "ui.h"
#include "filework.h"

#include <iostream>

using namespace std;

/////////////////////////////   2. Add new Contact   /////////////////////////////

void addContact() {
    Clear();
    cout << "==========================================================" << endl;
    cout << "                     ADDING CONTACT" << endl;
    cout << "==========================================================" << endl << endl;
    
    string n, s, p, email, date, adr;
    vector<Phone> phones;

    do {
        cout << "Name*: ";
        getline(cin, n);
        n = validator.validateName(n);
    } while (n.empty());

    do {
        cout << "Surname*: ";
        getline(cin, s);
        s = validator.validateSurname(s);
    } while (s.empty());
    
    do {
        cout << "Email*: ";
        getline(cin, email);
        email = validator.validateEmail(email, n);
    } while (email.empty());
    
    
    string phone;
    do {
        cout << "Phone number 1*: ";
        getline(cin, phone);
        phone = validator.validatePhone(phone);
        if (phone.empty()) {
            cout << "Error: At least one phone number is required!" << endl;
        }
    } while (phone.empty());


    string phoneType = selectPhoneType();
    phones.push_back(Phone(phone, phoneType));
    
    // Optional phone numbers

    for (int i = 2; i <= 100; i++) {
        cout << "Phone number " << i << " (Enter to skip): ";
        getline(cin, phone);
        
        if (phone.empty()) {
            break;
        }
        
        string validatedPhone = validator.validatePhone(phone);
        if (validatedPhone.empty()) {
            cout << "Error: Invalid phone format! Please enter again or press Enter to skip." << endl;
            i--; // return to back i
            continue;
        }
        
        string phoneType = selectPhoneType();
        phones.push_back(Phone(validatedPhone, phoneType)); 
        cout << "Phone number " << i << " (" << phoneType << ") added successfully!" << endl;
    }
    
    
    do {
        cout << "Patronymic: ";
        getline(cin, p);
        if (p.empty()) {
            break; // maybe empty
        }
        p = validator.validatePatronymic(p);
        // If p is empty after validation, it means there was an error, we continue the cycle.
    } while (p.empty());

    do {
        cout << "Birthdate (dd.mm.yyyy): ";
        getline(cin, date);
        if (date.empty()) {
            break;
        }
        date = validator.validateBirthday(date);
    } while (date.empty());
    
    cout << "Address: ";
    getline(cin, adr);
    adr = validator.validateAddress(adr);
    
    // Contact creating
    Contact newContact(n, s, email, phones, p, date, adr);
    contacts.push_back(newContact);
    saveToFile();
    cout << "Contact successfully added!" << endl;
    cout << "Press Enter to return..." << endl;
    cin.get();
}

//////////////////////////////   3. Edit Contact   //////////////////////////////

void editPhones(Contact& contact) {
    while (true) {
        Clear();
        cout << "==========================================================" << endl;
        cout << "                     EDITING PHONES" << endl;
        cout << "==========================================================" << endl << endl;

        
        vector<Phone> phones = contact.getPhones();
        
        cout << "Current phone numbers:" << endl;
        if (phones.empty()) {
            cout << " No phone numbers" << endl;
        } 
        else {
            for (size_t i = 0; i < phones.size(); i++) {
                cout << "  " << i + 1 << ") " << phones[i].number << " (" << phones[i].type << ")" << endl;
            }
        }
        
        cout << endl << "Options:" << endl;
        cout << "1. Add new phone number" << endl;
        cout << "2. Edit phone number" << endl;
        cout << "3. Delete phone number" << endl;
        cout << "0. Back to contact editing" << endl << endl;
        
        string choice;
        cout << "Select option: ";
        cin >> choice;
        cin.ignore();
        
        if (choice == "0") { //Back to contact editing
            break;
        }

        else if (choice == "1") { //Add new phone number
            string newPhone;
            cout << "Enter new phone number: ";
            getline(cin, newPhone);
            
            string validatedPhone = validator.validatePhone(newPhone);
            if (!validatedPhone.empty()) {
                string phoneType = selectPhoneType();
                contact.addPhone(validatedPhone, phoneType);
                cout << "Phone number added successfully!" << endl;
            } 
            // else {
            //     cout << "Error: Invalid phone format!" << endl;
            // }
        }

        else if (choice == "2") { //Edit phone number
            if (phones.empty()) {
                cout << "No phone numbers to edit!" << endl;
            }
            else 
            {   
                for (size_t i = 0; i < phones.size(); i++) {
                cout << "  " << i + 1 << ") " << phones[i].number << " (" << phones[i].type << ")" << endl;
            }
                cout << "Enter phone number to edit (1-" << phones.size() << "): ";
                string indexStr;
                getline(cin, indexStr);
                
                try {

                    int phoneIndex = stoi(indexStr) - 1;

                    if (phoneIndex >= 0 && phoneIndex < phones.size()) 
                    {
                        cout << "Enter new phone number: ";
                        string newPhone;
                        getline(cin, newPhone);
                        
                        string validatedPhone = validator.validatePhone(newPhone);
                        if (!validatedPhone.empty()) {
                            string phoneType = selectPhoneType();
                            phones[phoneIndex].number = validatedPhone;
                            phones[phoneIndex].type = phoneType;
                            contact.setPhones(phones);
                            cout << "Phone number updated successfully!" << endl;
                        } 
                        // else {
                        //     cout << "Error: Invalid phone format!" << endl;
                        // }
                    } 
                    else {
                        cout << "Invalid phone number index!" << endl;
                    }
                } 
                catch (...) {
                    cout << "Invalid input!" << endl;
                }
            }
        }

        else if (choice == "3") { //Delete phone number
            if (phones.empty()) {
                cout << "No phone numbers to delete!" << endl;
            } 

            else if (phones.size() == 1) {
                cout << "Error: Cannot delete the last phone number!" << endl;
            } 

            else 
            {
                for (size_t i = 0; i < phones.size(); i++) {
                cout << "  " << i + 1 << ") " << phones[i].number << " (" << phones[i].type << ")" << endl;
            }
                cout << "Enter phone number to delete (1-" << phones.size() << "): ";
                string indexStr;
                getline(cin, indexStr);
                
                try {
                    int phoneIndex = stoi(indexStr) - 1;
                    if (phoneIndex >= 0 && phoneIndex < phones.size()) {
                        phones.erase(phones.begin() + phoneIndex);
                        contact.setPhones(phones);
                        cout << "Phone number deleted successfully!" << endl;
                    } 
                    else {
                        cout << "Invalid phone number index!" << endl;
                    }
                } catch (...) {
                    cout << "Invalid input!" << endl;
                }
            }
        }
        else {
            cout << "Invalid option!" << endl;
        }
        
        cout << "Press Enter to continue...";
        cin.get();
    }
}


void editSelectedContact(int index){
    Contact& contact = contacts[index - 1];
    Clear();
    while (true) {
        
        cout << "==========================================================" << endl;
        cout << "                     EDITING CONTACT" << endl;
        cout << "==========================================================" << endl << endl;
        cout << "1. Name: " << contact.getName() << endl;
        cout << "2. Surname: " << contact.getSurname() << endl;
        cout << "3. Patronymic: " << contact.getPatronymic() << endl;
        cout << "4. Email: " << contact.getEmail() << endl;
        cout << "5. Birthdate: " << contact.getBday() << endl;
        cout << "6. Address: " << contact.getAddress() << endl;
        cout << "7. Phone numbers: " << contact.getFirstPhoneNumber(); 
        if((contact.getPhones().size()-1)>0)
        {
            cout << " and " << (contact.getPhones().size()-1) << " other... ";
        }
        cout << endl << "0. Finish editing" << endl << endl;
        
        string choice;
        cout << "Select the field to edit: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        bool NumbOnly = false;
        for (char curContact : choice) {
            if(!isdigit(curContact)){
                cout << "Enter a number from 0 to 7" << endl;
                NumbOnly = true;
                break;
            }
        }

        if (NumbOnly) {
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            continue;
        }

        int n_field = stoi(choice); 
        
        if (n_field > 7 || n_field < 0){
            cout << "Invalid field selection!" << endl;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            continue;
        }

        if (n_field == 0) {
            saveToFile();
            cout << "Editing completed" << endl << endl;
            return;
        }

        if (n_field == 7) {
            editPhones(contact);
            continue; // -> separate editing section
        }
        
        string new_val;
        cout << "Enter new value: ";
        getline(cin, new_val);

        switch (n_field) {

            case 1: { // Name
                string vName = validator.validateName(new_val);
                if (!vName.empty()) {
                    contact.setName(vName);
                    cout << "Name successfully changed" << endl;
                } 
                // else {
                //     cout << "Error: Invalid name format!" << endl;
                // }
                break;
            }

            case 2: { // Surname
                string vSurname = validator.validateSurname(new_val);
                if (!vSurname.empty()) {
                    contact.setSurname(vSurname);
                    cout << "Surname successfully changed" << endl;
                } 
                // else {
                //     cout << "Error: Invalid surname format!" << endl;
                // }
                break;
            }

            case 3: { // Patronymic
                if (new_val.empty()) {
                    contact.setPatronymic("");
                    cout << "Patronymic successfully cleared" << endl;
                } 
                else {
                    string vPatronymic = validator.validatePatronymic(new_val);
                    if (!vPatronymic.empty()) {
                        contact.setPatronymic(vPatronymic);
                        cout << "Patronymic successfully changed" << endl;
                    } 
                    // else {
                    //     cout << "Error: Invalid patronymic format!" << endl;
                    // }
                }
                break;
            }

            case 4: { // Email
                string vEmail = validator.validateEmail(new_val, contact.getName());
                if (!vEmail.empty()) {
                    contact.setEmail(vEmail);
                    cout << "Email successfully changed" << endl;
                } 
                // else {
                //     cout << "Error: Invalid email format!" << endl;
                // }
                break;
            }

            case 5: { // Birthdate
                if (new_val.empty()) {
                    contact.setBday("");
                    cout << "Birthdate successfully cleared" << endl;
                } 
                else {
                    string vBirthday = validator.validateBirthday(new_val);
                    if (!vBirthday.empty()) {
                        contact.setBday(vBirthday);
                        cout << "Birthdate successfully changed" << endl;
                    } 
                    // else {
                    //     cout << "Error: Invalid birthdate format!" << endl;
                    // }
                }
                break;
            }

            case 6: { // Address
                string vAddress = validator.validateAddress(new_val);
                contact.setAddress(vAddress);
                cout << "Address successfully changed" << endl;
                break;
            }

            default:
                break;
        }

        
        cout << "Press Enter to continue...";
        cin.get();
    }
}


void editContact() {
    Clear();

    if (contacts.empty()) {
        cout << "Contact list is empty" << endl;
        cout << "Press Enter to return..." << endl;
        cin.get();
        return;
    }

    while(true){
        Clear();
        cout << "==========================================================" << endl;
        cout << "                     CONTACTS LIST" << endl;
        cout << "==========================================================" << endl << endl;

        showAllContacts(true);

        cout << endl;
        string choice;
        int Flag = 0;

        cout << "Select contact to edit (enter 0 to exit): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == "0"){
            saveToFile();
            return;
        }

        for (char curContact : choice) {
            if(!isdigit(curContact)){
                cout << "Enter a number from 1 to " << contacts.size() << endl;
                Flag = 1;
                break;
            }
        }

        if (Flag == 1){
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            continue;
        }

        int curContact = stoi(choice);
        if (curContact < 1 || curContact > contacts.size()){
            cout << "Enter a number from 1 to " << contacts.size() << endl;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            continue;
        }

        editSelectedContact(curContact);
    };
        
    cout << "Press Enter to return..." << endl;
    cin.get();
}

/////////////////////////////   4. Delete Contact   /////////////////////////////

void deleteContact() {
    Clear();

    cout << "==========================================================" << endl;
    cout << "                     DELETING CONTACT" << endl;
    cout << "==========================================================" << endl << endl;

    if (contacts.empty()) {
        cout << "Contact list is empty" << endl;
        cout << "Press Enter to return..." << endl;
        cin.get();
        return;
    }

    while(true){
        Clear();
        cout << "==========================================================" << endl;
        cout << "                 SELECT CONTACT TO DELETE" << endl;
        cout << "==========================================================" << endl << endl;

        showAllContacts(true);

        cout << endl;
        cout << "Enter contact number to delete (0 to cancel): ";

        string choice;
        int curContact;
        int CorFlag = 0;

        cin >> choice;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (choice == "0"){
            return;
        }

        for (char curContact : choice) {
            if(!isdigit(curContact)){
                cout << "Please enter a number from 1 to " << contacts.size() << endl;
                CorFlag = 1;
                break;
            }
        }

        if (CorFlag){
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            continue;
        }

        curContact = stoi(choice);
        if (curContact < 1 || curContact > contacts.size()){
            cout << "Please enter a number from 1 to " << contacts.size() << endl;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            continue;
        }

        Contact& contactToDelete = contacts[curContact - 1];

        cout << endl;
        cout << "==========================================================" << endl;
        cout << "CONFIRM DELETION" << endl;
        cout << "Are you sure you want to delete contact:" << endl;
        cout << "-> " << contactToDelete.getSurname() << " " << contactToDelete.getName() << endl;
        cout << endl;
        cout << "This action cannot be undone!" << endl;
        cout << "==========================================================" << endl << endl;

        if (askYesNo("Enter 'y' to confirm or 'n' to cancel")) {
            contacts.erase(contacts.begin() + curContact - 1);
            cout << "Contact successfully deleted!" << endl;
            saveToFile();
        } 
        else {
            cout << "Deletion cancelled." << endl;
        }

        cout << "Press Enter to continue...";
        cin.get();
        return;
    }
}

