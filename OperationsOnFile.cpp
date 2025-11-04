#include "OperationsOnFile.h"
#include <vector>
#include <string>
#include "Record.h"
#include "Menus.h"
#include <filesystem>
#include <fstream>
#include <iostream>


/**
 * @brief Funkcja zwraca  Vector z podzielonego std::string
 * @details Funkcja dzieli ciąg znaków std::string, dzięki określonemu separatorowi
 * @param content ciąg słów do oddzielenia
 * @param separator pojedyńczy znak typu char jako separator
 * @return std::vector<std::string> z rozdzielonymi słowami
 */
std::vector<std::string> split(const std::string &content, char separator) {
    std::vector<std::string> words;
    std::stringstream ss(content);

    std::string oneWord;
    while (getline(ss, oneWord, separator)) {
        words.push_back(oneWord);
    }
    return words;
}
/**
 * @brief Funkcja szyfruje std::string podany przez argument
 * @details Funkcja na podstawie dostarczonego std::string filePassword
 * szyfruje podany string i zwraca go
 * @param toEncrypt
 * @param filePassword
 * @return zaszyfrowany std::string toEncrypt
 */
std::string encrypt(const std::string &toEncrypt, const std::string &filePassword) {
    std::string copyOf_toEncrypt = toEncrypt;
    std::string transformedPassword = filePassword;
    char firstLetter = transformedPassword.at(0);
    char lastLetter = transformedPassword.back();
    int sizeOfPassword = transformedPassword.size();

    int shift = firstLetter + lastLetter + sizeOfPassword;

    for (char &c: transformedPassword) {
        c = c ^ shift;
    }

    for (char &c: copyOf_toEncrypt) {
        c = c ^ shift;
    }

    return copyOf_toEncrypt;
}
/**
 * @brief Funkcja tworzy zaszyfrowany plik
 * @details Funkcja zapisuje w pierwszej oraz ostatniej linii pliku zaszyfrowany timestamp,
 * w pozostałych liniach są szyfrowane elementy klasy Record*
 * jedna linia w pliku = jeden rekord
 * @param records baza haseł
 * @param outputFilename plik wyjsciowy
 * @param filePassword  haslo do bazy danych
 */
void encryptFile(const std::vector<Record *> &records,
                 const std::string &outputFilename, const std::string &filePassword) {
    std::pair<std::string, std::string> timestamps = encrypted2TimeStamps();

    std::ofstream file(outputFilename);
    if (file.is_open()) {
        file << timestamps.first << "\n";
        for (auto item: records) {
            std::string currentRecord =
                    item->name + "|" + item->login + "|" + item->password + "|" + item->site + "|" + item->category;
            std::string encryptedData = encrypt(currentRecord, filePassword);
            file << encryptedData << "\n";
        }
        file << timestamps.second;
        file.close();
    } else {
        std::cout << "Blad przy otwieraniu pliku " << outputFilename << std::endl;
    }
}
/**
 * @brief Funkcja rozszyfrowuje plik
 * @details Funkcja rozszyfrowuje plik podanym przez argument hasłem filePassword,
 * zapisuje go do tymczasowego pliku tmp.tmp,
 * pomija pierszą i ostatnią linie w rozszyfrowywaniu,
 * następnie czyta rozszyfrowany tymczasowy plik, na podstawie niego tworzy nowe rekordy
 * i uzupełnia nimi std::vector<Record *> decryptedRecords
 * na końcu usuwa plik tymczasowy
 * @param inputFilename plik wejsciowy
 * @param filePassword haslo do bazy danych
 * @return std::vector<Record *> zawierająca wszystkie wpisy haseł
 */
std::vector<Record *> decryptFile(const std::string &inputFilename, const std::string &filePassword) {
    std::vector<Record *> decryptedRecords = {};
    std::string line;
    int counterLineInFile = 0;
    std::ofstream tmpFile("tmp.tmp");
    std::ifstream inputFile(inputFilename);
    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line))
            counterLineInFile++;
    }
    if (inputFile.is_open()) {
        int currentLine = 1;
        inputFile.clear();
        inputFile.seekg(0, std::ios::beg);
        while (std::getline(inputFile, line)) {
            if (currentLine == 1 || currentLine == counterLineInFile) {
                tmpFile << line << std::endl;
            } else {
                tmpFile << encrypt(line, filePassword) << std::endl;
            }
            currentLine++;
        }
        inputFile.close();
    }
    tmpFile.close();
    std::ifstream decodedFile("tmp.tmp");
    while (std::getline(decodedFile, line)) {
        std::vector<std::string> words = split(line, '|');
        if (words.size() == 5) {
            std::string name = words[0];
            std::string login = words[1];
            std::string password = words[2];
            std::string site = words[3];
            std::string category = words[4];

            decryptedRecords.push_back(new Record(name, login, password, site, category));
        }

    }
    decodedFile.close();
    inputFile.close();
    std::remove("tmp.tmp");
    return decryptedRecords;
}
/**
 * @brief Funkcja robiąca update timestampa
 * @details Funkcja posiłkuje się funkcją encrypted2TimeStamps(),
 * z której odczytuje aktualny czas
 * Zapisuje w pierwszej linii kodu zaszyfrowaną część daty
 * Zapisuje w ostatniej linii kodu drugą zaszyfrowaną część daty
 * @param inputFilename plik wejsciowy
 */
void updateTimeStamp(const std::string &inputFilename) {
    int numberOflines = linesFromFile(inputFilename);
    std::pair<std::string, std::string> timestamps = encrypted2TimeStamps();
    std::fstream file(inputFilename, std::ios::in | std::ios::out);
    if (file.is_open()) {
        std::string line;
        int lineNumber = 1;

        while (std::getline(file, line)) {
            if (lineNumber == 1) {
                file.seekp(0);
                file << timestamps.first << "\n";
                file.seekp(std::ios::end);
            } else if (lineNumber == numberOflines) {
                file.seekp(file.tellg());
                file << timestamps.second;
                file.seekp(std::ios::end);
            }
            lineNumber++;
        }
        file.close();
    } else {
        std::cout << "Nie udało się otworzyć pliku." << std::endl;
    }

}

/**
 * @brief Funkcja wyświetlająca listę plików w bieżącym katalogu
 */
void listOffilesFromCurrentPath() {
    std::filesystem::path folderPath = std::filesystem::current_path();
    for (const auto &entry: std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::cout << entry.path().filename().string() << std::endl;
        }
    }
}

/**
 * @brief Funkcja służąca do wyłuskania bezpośredniej ścieżki do pliku
 * @param choice -> zmienna typu int, używana do switch'a wewnątrz funkcji
 * @return zwraca std::string z ścieżką / nazwą pliku
 */
std::string getFilePath(int choice) {
    std::string filePath;
    std::string fileName;
    switch (choice) {
        case 1:
            listOffilesFromCurrentPath();
            std::cout << "Podaj nazwe pliku: ";
            std::cin >> fileName;
            filePath = "./" + fileName;
            return filePath;
            break;
        case 2:
            std::cout << "Podaj bezposrednia sciezke do pliku: ";
            std::cin >> filePath;
            return filePath;
            break;
        case 3: {
            std::cout << "Wprowadz nazwe dla twojej bazy" << std::endl;
            std::cin >> fileName;
            filePath = fileName;
            std::ofstream file(filePath);
            if (file.is_open()) {
                std::cout << "Plik o nazwie " + fileName + " zostal utworzony" << std::endl;
                std::cout << "Znajduje sie w tym samym katalogu co aplikacja" << std::endl;
                std::cout << "Dodaj do bazy jakikolwiek wpis aby uniknac braku wpisow w bazie" << std::endl;
            } else {
                std::cout << "Nie mozna utworzyc pliku." << std::endl;
            }
            return filePath;
        }

        default:
            std::cout << "Nieprawidlowy wybor." << std::endl;
            chooseFileMenu();
            break;
    }
}

std::string convertDate(int date) {
    std::string convertedData = std::to_string(date);
    if (convertedData.size() == 1) {
        return "0" + convertedData;
    } else
        return convertedData;
}
/**
 * @brief Funkcja zwraca dwa zaszyfrowane stringi w których zakodowana jest data
 * @details Funkcja pobiera datę z systemu odpowiednio ją szyfruje
 * a następnie zwraca w postaci 2 stringów
 *
 * @return std::pair<std::string, std::string> gdzie pierwszy string to
 * sekundy,rok,dzien
 * a drugi string to
 * minuty,godziny,miesiac
 */
std::pair<std::string, std::string> encrypted2TimeStamps() {
    srand(static_cast<unsigned int>(time(nullptr)));
    std::time_t currentTime = std::time(nullptr);
    std::tm *localTime = std::localtime(&currentTime);

    std::string year = convertDate(localTime->tm_year + 1900);
    std::string month = convertDate(localTime->tm_mon + 1);
    std::string day = convertDate(localTime->tm_mday);
    std::string hour = convertDate(localTime->tm_hour);
    std::string minute = convertDate(localTime->tm_min);
    std::string second = convertDate(localTime->tm_sec);
    std::string timestamp1 = "";
    std::string timestamp2 = "";
    for (int i = 1; i <= 40; i++) {
        std::string randNumber = std::to_string(rand() % 10);
        timestamp1 = timestamp1 + randNumber;
    }
    //SWAPING
    timestamp1[6] = second[0];
    timestamp1[7] = second[1];
    timestamp1[14] = year[0];
    timestamp1[15] = year[1];
    timestamp1[16] = year[2];
    timestamp1[17] = year[3];
    timestamp1[20] = day[0];
    timestamp1[21] = day[1];
    for (int i = 1; i <= 40; i++) {
        std::string randNumber = std::to_string(rand() % 10);
        timestamp2 = timestamp2 + randNumber;
    }
    timestamp2[11] = minute[0];
    timestamp2[12] = minute[1];
    timestamp2[16] = hour[0];
    timestamp2[17] = hour[1];
    timestamp2[20] = month[0];
    timestamp2[21] = month[1];

    return std::make_pair(timestamp1, timestamp2);
}
/**
 * @brief Funkcja zwraca ilość linii w pliku
 * @param inputFilename nazwa pliku
 * @return liczbę linii
 */
int linesFromFile(const std::string &inputFilename) {
    int counterLineInFile = 0;
    std::ifstream fileToRead(inputFilename);
    if (fileToRead.is_open()) {
        std::string line;
        while (std::getline(fileToRead, line))
            counterLineInFile++;
    }
    fileToRead.close();
    return counterLineInFile;
}