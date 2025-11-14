#include "contact.h"

#include "validator.h" // basic checks

#include "ui.h"
//
//////////////////////////   1. Show all Contacts   ////////////////////////////
//
// Clear
//
// selectPhoneType
//
// askYesNo
//
// getMenuChoice
//
// showMenu

#include "filework.h"
//
// readFile
//
// saveToFile

#include "add-edit-del.h"
//
/////////////////////////////   2. Add new Contact   /////////////////////////////
//
// addContact
//
//////////////////////////////   3. Edit Contact   //////////////////////////////
//
// editPhones
//
// editSelectedContact
//
// editContact
//
/////////////////////////////   4. Delete Contact   /////////////////////////////
//
// deleteContact

#include "search-sort.h"
//
/////////////////////////////   5. Contact search   /////////////////////////////
//
// SearchCondition
//
// selectSearchField
//
// getSearchValue
//
// checkCondition
//
// findContact
//
// searchContact
//
/////////////////////////////   6. Sort Contacts   //////////////////////////////
//
// compareContacts
// 
// sortContacts


#include <iostream>

#include <regex>
#include <vector>
#include <cctype>

#include <fstream>
#include <sstream>


using namespace std;


vector<Contact> contacts;


string currentFilename = "phonebook.txt";


//////////////////////////////////   VALIDATOR   //////////////////////////////////

Validator validator;

//////////////////////////////////////////////////////////////////////////////////p



int main() {
    int choice;
    readFile(currentFilename);
    
    do {
        showMenu();
        choice = getMenuChoice();
        
        switch (choice) {
            case 1:
                showAllContacts(false);
                break;
            case 2:
                addContact();
                break;
            case 3:
                editContact();
                break;
            case 4:
                deleteContact();
                break;
            case 5:
                searchContact();
                break;
            case 6:
                sortContacts();
                break;
            case 0:
                cout << endl << "Goodbye!" << endl << endl;
                break;
            default:
                cout << "Please enter a valid option (0-5)." << endl;
                cout << "Press Enter to continue..." << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                break;
        }
        
    } 
    while (choice != 0);
    
    return 0;
}


