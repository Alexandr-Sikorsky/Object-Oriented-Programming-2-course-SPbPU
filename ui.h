#pragma once
#include "glob.h"

void Clear();

void showContacts(const vector<int>& indexes, bool showIndex);
void showAllContacts(bool showIndex);

string selectPhoneType();

bool askYesNo(const string& question);

int getMenuChoice();
void showMenu();