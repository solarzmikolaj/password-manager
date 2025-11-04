#include <filesystem>
#include <iostream>
#include "Menus.h"
#include "Record.h"
#include <set>
#include "OperationsOnFile.h"
#include "OperationOnDatabase.h"



using namespace std;

/**
 * @brief Wyświetla główne MENU menadżera
 *
 * @details Wyświetla główne MENU menadżera, dzięki któremu możemy dokonać operacji na hasłach
 * @param records baza haseł
 * @param categories baza kategorii
 * @param directoryOfAFile ścieżka/nazwa pliku
 * @param filePassword hasło do bazy danych/pliku
 */
void mainMenu(vector<Record *> &records, std::set<std::string> &categories,
              const std::string &directoryOfAFile, const std::string &filePassword) {
    int passwordMenuChoice;
    std::cout << endl;
    std::cout << "1.Wyszukaj hasla" << endl;
    std::cout << "2.Posortuj hasla" << endl;
    std::cout << "3.Dodaj haslo" << endl;
    std::cout << "4.Edytuj haslo" << endl;
    std::cout << "5.Dodaj kategorie" << endl;
    std::cout << "6.Usun kategorie" << endl;
    std::cout << "7.Usun haslo / hasla" << endl;
    std::cout << "8.Wyswietl timestamp" << endl;
    std::cout<<std::endl;
    std::cout << "9.Wyjdz z programu" << endl;
    cin >> passwordMenuChoice;
    if (passwordMenuChoice == 9) {
        exit(0);
    }
    passwordManagerMenu(passwordMenuChoice, records, categories, directoryOfAFile, filePassword);
}

/**
 * @brief Ta funkcja zwraca ścieżkę do pliku
 * @return zwraca ścieżkę do pliku
 */
std::string chooseFileMenu() {
    int fileDirectoryMethod;
    std::cout << "1. Wybierz plik z folderu programu" << std::endl;
    std::cout << "2. Podaj bezposrednia sciezke do pliku" << std::endl;
    std::cout << "3. Utworz nowa baze danych" << std::endl;
    std::cin >> fileDirectoryMethod;
    std::string directoryOfAFile = getFilePath(fileDirectoryMethod);
    return directoryOfAFile;
}

/**
 * @brief Wywołanie poszczególnych metod z MENUS.cpp
 * @param choice  parametr używany do wyboru odpowiedniego działania
 * @param records  baza haseł
 * @param categories  baza kategorii
 * @param direcotryOfAFile  ścieżka absolutna/nazwa pliku
 * @param filePassword  hasło do bazy danych/pliku
 */
void passwordManagerMenu(int choice, vector<Record *> &records, std::set<std::string> &categories,
                         const std::string &direcotryOfAFile, const std::string &filePassword) {
    switch (choice) {
        case 1:
            search(records);
            mainMenu(records, categories, direcotryOfAFile, filePassword);
            break;
        case 2:
            sort(records);
            mainMenu(records, categories, direcotryOfAFile, filePassword);
            break;
        case 3:
            add(records, categories, direcotryOfAFile, filePassword);
            mainMenu(records, categories, direcotryOfAFile, filePassword);
            break;
        case 4:
            edit(records, categories, direcotryOfAFile, filePassword);
            mainMenu(records, categories, direcotryOfAFile, filePassword);
            break;
        case 5:
            addCategory(categories);
            mainMenu(records, categories, direcotryOfAFile, filePassword);
            break;
        case 6:
            deleteCategory(categories, records, direcotryOfAFile, filePassword);
            mainMenu(records, categories, direcotryOfAFile, filePassword);
            break;
        case 7 :
            deletePassword(categories, records, direcotryOfAFile, filePassword);
            mainMenu(records, categories, direcotryOfAFile, filePassword);
        case 8:
            showTimeStamp(direcotryOfAFile, filePassword);
            mainMenu(records, categories, direcotryOfAFile, filePassword);
            break;
        default:
            cout << "Nie ma takiej opcji, SPROBOJ JESZCZE RAZ" << endl;
            mainMenu(records, categories, direcotryOfAFile, filePassword);
            break;

    }

}

/**
 * Funkcja zwracająca set kategorii na podstawie vector<Record*>
 * @param records baza haseł
 * @return set storzony na podstawie vectora
 */
std::set<std::string> createSetOfCategories(std::vector<Record *> records) {
    std::set<std::string> categories = {};
    for (auto record: records) {
        categories.insert(record->category);
    }
    return categories;
}






