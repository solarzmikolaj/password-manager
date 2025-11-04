#include "Record.h"
#include <vector>
#include <set>

///MENUS
std::string chooseFileMenu();
void mainMenu(std:: vector<Record *> &records, std::set<std::string> &categories,
              const std::string &directoryOfAFile, const std::string & filePassword);

void passwordManagerMenu(int choice, std::vector<Record *> &records, std::set<std::string> &categories,
                         const std::string &direcotryOfAFile,const std::string &filePassword);

///FUNCTIONS FOR A FILES
void listOffilesFromCurrentPath();
std::string getFilePath(int choice);
std::vector<std::string> split(const std::string& content, char separator);

///FUNCTIONS ON DATABASES
std::set<std::string> createSetOfCategories(std::vector<Record*> records);
