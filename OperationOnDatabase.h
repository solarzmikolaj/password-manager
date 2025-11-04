#include "Record.h"
#include <vector>
#include <set>

///OPTIONS FOR MENU
void add(std::vector<Record *> &records,std::set<std::string>&categories,
         const std::string &directoryOfAFile,const std::string &filePassword);
void search(std::vector<Record *> records);
void sort(std::vector<Record *> records);
void edit(std::vector<Record *> &records,std::set<std::string>&categories, std::string direcoryOfAFile,const std::string& filePassword);
void addCategory(std::set<string>&categories);
void deleteCategory(std::set<string> &categories, std::vector<Record *> &records, std::string direcoryOfAFile,const std::string& filePassword);
void deletePassword(std::set<string> &categories,std::vector<Record *> &records, std::string direcoryOfAFile,const std::string& filePassword);
void showTimeStamp(const std::string &direcoryOfAFile,const std::string &filePassword);

///Printers
template<typename T>
void vectorPrinter(std::vector<T> vec);
template<typename T>
void setPrinter(std::set<T> set);