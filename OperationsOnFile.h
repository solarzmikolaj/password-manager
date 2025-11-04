//
// Created by Mikołaj Solarz on 20.05.2023.
//

#include <vector>
#include <string>
#include "Record.h"

///FUNCTIONS FOR ENCRYPTING / DECRYPTING
void encryptFile(const std::vector<Record*>& records, const std::string& outputFilename, const std::string& filePassword);
std::vector<Record*> decryptFile(const std::string& inputFilename, const std::string& password);
std::vector<std::string> split(const std::string &content, char separator);

///FUNCTIONS THAT RETURNS DIRECTORY / PATH
std::string getFilePath(int choice);
void listOffilesFromCurrentPath();

///FUNCTIONS THAT HELPS WITH TIMESTAMPS
std::string convertDate(int date);
int linesFromFile(const std::string &inputFilename);
void updateTimeStamp(const std::string &inputFilename);
std::pair<std::string, std::string> encrypted2TimeStamps();

