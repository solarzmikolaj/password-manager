#include <iostream>
#include "fmt/ranges.h"
#include "Menus.h"
#include "OperationsOnFile.h"

#define SPACER std::cout<<std::endl;

int main() {
    std::string filePassword;
    std::string directoryOfAFile = chooseFileMenu();

    SPACER
    std::cout << "Podaj haslo do pliku " + directoryOfAFile << std::endl;
    std::cin >> filePassword;
    std::vector<Record *> records = decryptFile(directoryOfAFile, filePassword);
    if(records.size()==0){
        updateTimeStamp(directoryOfAFile);
        std::cout<<"UWAGA!! Wprowadzone haslo moze byc nieprawidlowe, lub baza nie zawiera hasel"<<std::endl;
    }
    std::set<std::string> categories = createSetOfCategories(records);
    mainMenu(records, categories, directoryOfAFile, filePassword);

}
