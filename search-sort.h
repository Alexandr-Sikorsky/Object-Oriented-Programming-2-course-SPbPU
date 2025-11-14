#pragma once
#include "glob.h"

struct SearchCondition {
    int field;
    string value;
};

int selectSearchField(const vector<bool>& usedFields);
string getSearchValue(int field);
bool checkCondition(const Contact& contact, int field, const string& value);
void findContact(const vector<SearchCondition>& conditions);
void searchContact();

bool compareContacts(const Contact& cA, const Contact& cB, int field, bool ascending);
void sortContacts();