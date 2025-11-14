#pragma once
#include <string>
#include <regex>
#include <algorithm>
#include <ctime>

using namespace std;

class Validator {
private:
    regex NAME;
    regex EMAIL;
    regex PHONEONE;
    regex PHONETWO;
    regex PHONETHREE;
    regex DATE;

    string trim(const string& str) const;

public:
    Validator();

//////////////////////////////////   Required fields   //////////////////////////////////
    string validateName(const string& input);
    string validateSurname(const string& input);
    string validateEmailWithoutUsername(const string& input);
    string validateEmail(const string& email, const string& username);


//////////////////////////////////   Optional fields   //////////////////////////////////
    string validatePhone(const string& input);  // first phone - required
    string validatePatronymic(const string& input);
    string validateBirthday(const string& input);
    string validateAddress(const string& input);
};