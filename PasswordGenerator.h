//
// Created by Mikołaj Solarz on 20.05.2023.
//

#include "Record.h"

std::string passwordGenerator(int length,bool upperCaseLetters, bool specialSigns);
std::string passwordInfo(std::string password, std::vector<Record *> records);

///PASSWORD CHCEKERS
bool hasUpperCaseLetter(std::string password);
bool hasSpecialSign(std::string password);
bool isItAStrongPassword(const std::string &password);
bool wasUsedBefore(std::string searchedPassword, std::vector<Record *> records);


