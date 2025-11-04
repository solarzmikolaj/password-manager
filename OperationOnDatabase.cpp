#include <vector>
#include "OperationOnDatabase.h"
#include "Record.h"
#include <filesystem>
#include <iostream>
#include <set>
#include <fstream>
#include "PasswordGenerator.h"
#include "OperationsOnFile.h"
#include "Menus.h"
#include <regex>

using std::cin, std::cout, std::endl, std::vector;

/**
 * @brief Głównym zadaniem funkcji jest dodawanie hasła do bazy danych
 * @details po wywołaniu funkcji, użytkownik jest zmuszony do określenia:
 * NAZWY,LOGINU,HASŁA,KATEGORII
 * następnie funkcja zapisuje hasło do pliku oraz do bazy vector<Record*>
 * @param records  -> baza haseł
 * @param categories -> baza kategorii
 * @param directoryOfAFile -> ścieżka/nazwa pliku
 * @param filePassword -> hasło do bazy danych/pliku
 */
void add(std::vector<Record *> &records, std::set<std::string> &categories, const std::string &directoryOfAFile,
         const std::string &filePassword) {
    cin.ignore();
    std::string name;
    std::string login;
    std::string password;
    std::string site;
    std::string category;
    int index;
    int choice;
    int sitechoice;
    std::vector<string> vectorOfSet = {};
    std::cout << "Wprowadz NAZWE dla hasla: " << endl;
    getline(std::cin, name);
    int usages = std::count_if(records.begin(), records.end(), [name](const Record *rec) -> auto {
        return rec->name == name;
    });
    if (usages > 1) {
        cout << "Ta nazwa dla hasla zostala juz uzyta. Sprobuj jeszcze raz!" << endl;
        add(records, categories, directoryOfAFile, filePassword);
    }
    std::cout << "Wprowadz LOGIN dla hasla: " << endl;
    cin >> login;
    std::cout << "Czy zaproponowac ci mocne haslo? [1.TAK / 2.NIE]" << endl;
    cin >> choice;
    switch (choice) {
        case 1: {
            int length;
            int upperCaseChoice;
            int specialDigitsChoice;

            bool upperCaseBool = false;
            bool specialDigitsBool = false;

            std::cout << "Wprowadz dlugosc jaka ma miec haslo: " << endl;
            cin >> length;
            cin.ignore();
            std::cout << "Czy haslo ma miec wielkie litery? [1.TAK / 2.NIE]" << endl;
            cin >> upperCaseChoice;
            switch (upperCaseChoice) {
                case 1: {
                    upperCaseBool = true;
                    break;
                }
                case 2: {
                    upperCaseBool = false;
                    break;
                }
                default:
                    std::cout << "Zly wybor" << endl;
                    add(records, categories, directoryOfAFile, filePassword);
                    break;
            }
            std::cout << "Czy haslo ma miec znaki specjalne? [1.TAK / 2.NIE]" << endl;
            cin >> specialDigitsChoice;
            switch (specialDigitsChoice) {
                case 1: {
                    specialDigitsBool = true;
                    break;
                }
                case 2: {
                    specialDigitsBool = false;
                    break;
                }
                default:
                    std::cout << "Zly wybor" << endl;
                    add(records, categories, directoryOfAFile, filePassword);
                    break;

            }
            password = passwordGenerator(length, upperCaseBool, specialDigitsBool);
            std::cout << "Wygenerowane haslo to: " + password << endl;
            break;
        }
        case 2: {
            cout << "Wprowadz swoje haslo: " << endl;
            cin >> password;
            cout << passwordInfo(password, records) << endl;
            std::cout << endl;
            break;
        }
    }
    std::cout << "Czy chcesz dodac do wpisu SERWIS/STRONE WWW? [1.TAK / 2.NIE]" << endl;
    cin >> sitechoice;
    switch (sitechoice) {
        case 1:
            std::cout << "Wprowadz SERWIS/STRONE WWW" << endl;
            cin >> site;
            break;
        case 2:
            site = " ";
    }
    cin.ignore();
    if (categories.size() == 0) {
        std::cout << "UWAGA! Utworz najpierw kategorie! Nie mam gdzie dodac HASLA!" << endl;

    } else {
        setPrinter(categories);
        std::cout << "Wybierz jedna z powyzszych kategorii: " << endl;
        for (auto current: categories) {
            vectorOfSet.push_back(current);
        }
        std::cout << "Jesli na liscie nie ma docelowej kategorii, mozesz ja utworzyc." << "\n"
                  << "Aby to zrobic nacisnij 0 !" << endl;

        cin >> index;
        index--;
        if ((index) > vectorOfSet.size() - 1 && index < 0 && index != -1) {
            std::cout << "Wprowadziles zle kategorie, sprobuj utworzyc haslo jeszcze raz" << endl;
            add(records, categories, directoryOfAFile, filePassword);
        } else if (index == -1) {
            addCategory(categories);
        } else {
            category = vectorOfSet.at(index);
        }
        if (name != "" && login != "" && password != "" && category != "") {
            Record *record = new Record(name, login, password, site, category);
            records.push_back(record);
            cout << "Haslo zostalo pomyslnie zapisane!" << endl;
        }

        encryptFile(records, directoryOfAFile, filePassword);
    }

}

/**
 * @brief Głównym zadaniem funkcji jest wyszukanie, sprecyzowanego hasła.
 * @details Funkcja może szukać w 3 kryteriach:
 * NAZWA,KATEGORIA,LOGIN,WWW
 * @param records  baza haseł
 */
void search(std::vector<Record *> records) {
    int choice;
    string filterContent;
    std::cout << "1. Wyszukaj po nazwie" << endl;
    std::cout << "2. Wyszukaj po kategorii" << endl;
    std::cout << "3. Wyszukaj po loginie" << endl;
    std::cout << "4. Wyszukaj po stronie WWW"<<endl;
    std::vector<Record *> resultVector = {};
    cin >> choice;
    std::cout << "Wpisz teraz filtrowana fraze: ";
    std::cin.ignore();
    getline(std::cin, filterContent);
    switch (choice) {
        case 1:
            std::ranges::copy_if(records, std::back_inserter(resultVector),
                                 [&filterContent](const Record *rec1) -> auto {
                                     return rec1->name == filterContent;
                                 });
            break;
        case 2:
            std::ranges::copy_if(records, std::back_inserter(resultVector),
                                 [&filterContent](const Record *rec1) -> auto {
                                     return rec1->category == filterContent;
                                 });
            break;
        case 3:
            std::ranges::copy_if(records, std::back_inserter(resultVector),
                                 [&filterContent](const Record *rec1) -> auto {
                                     return rec1->login == filterContent;
                                 });
            break;
        case 4:
            std::ranges::copy_if(records, std::back_inserter(resultVector),
                                 [&filterContent](const Record *rec1) -> auto {
                                     return rec1->site == filterContent;
                                 });
            break;
        default:
            std::cout << "Zly Wybor";
            break;
    }
    vectorPrinter(resultVector);
    if (resultVector.empty()) {
        std::cout << "Brak wynikow!" << endl;
    }

}

/**
 * @brief Głównym zadaniem funkcji jest sortowanie istniejącej już bazy danych.
 * @details
 * Opcje sortowanie jakie oferuje nam funkcja to sortowanie:
 * po nazwie
 * po kategorii
 * po loginie
 * po długości hasła
 * najpierw po kategorii następnie po nazwie
 * najpierw po stronie WWW nastepnie po kategorii
 *
 * na samym końcu prezentuje nam w konsoli wyniki sortowania
 * @param records -> baza haseł
 */
void sort(std::vector<Record *> records) {
    int choice;
    std::vector<Record *> resultVector = records;
    std::cout << "POSORTUJ" << std::endl;
    std::cout << "1. po nazwie" << std::endl;
    std::cout << "2. po kategorii" << std::endl;
    std::cout << "3. po loginie" << std::endl;
    std::cout << "4. po stronie WWW" << std::endl;
    std::cout << "5. po dlugosci hasla" << endl;
    std::cout << "6. Najpierw po kategorii nastepnie po nazwie" << std::endl;
    std::cout << "7. Najpierw po stronie WWW nastepnie po kategorii" << endl;
    std::cin >> choice;
    std::cin.ignore();

    switch (choice) {
        case 1:
            int asc_dsc_choice_name;
            std::cout << "[1.ROSNACO / 2.MALEJACO]" << endl;
            cin >> asc_dsc_choice_name;
            switch (asc_dsc_choice_name) {
                case 1:
                    std::ranges::sort(resultVector, [](const Record *rec1, const Record *rec2) {
                        return rec1->name < rec2->name;
                    });
                    break;
                case 2:
                    std::ranges::sort(resultVector, [](const Record *rec1, const Record *rec2) {
                        return rec1->name > rec2->name;
                    });
                    break;
                default:
                    std::cout << "Zly wybor" << endl;
                    sort(records);
                    break;
            }
            break;
        case 2:
            int asc_dsc_choice_category;
            std::cout << "[1.ROSNACO / 2.MALEJACO]" << endl;
            cin >> asc_dsc_choice_category;
            switch (asc_dsc_choice_category) {
                case 1:
                    std::ranges::sort(resultVector, [](const Record *rec1, const Record *rec2) {
                        return rec1->category < rec2->category;
                    });
                    break;
                case 2:
                    std::ranges::sort(resultVector, [](const Record *rec1, const Record *rec2) {
                        return rec1->category > rec2->category;
                    });
                    break;
                default:
                    std::cout << "Zly wybor" << endl;
                    sort(records);
            }
            break;
        case 3:
            int asc_dsc_choice_login;
            std::cout << "[1.ROSNACO / 2.MALEJACO]" << endl;
            cin >> asc_dsc_choice_login;
            switch (asc_dsc_choice_login) {
                case 1:
                    std::ranges::sort(resultVector, [](const Record *rec1, const Record *rec2) {
                        return rec1->login < rec2->login;
                    });
                    break;
                case 2:
                    std::ranges::sort(resultVector, [](const Record *rec1, const Record *rec2) {
                        return rec1->login > rec2->login;
                    });
                    break;
                default:
                    std::cout << "Zly wybor" << endl;
                    sort(records);
                    break;
            }
            break;
        case 4:
            int asc_dsc_choice_site;
            std::cout << "[1.ROSNACO / 2.MALEJACO]" << endl;
            cin >> asc_dsc_choice_site;
            switch (asc_dsc_choice_site) {
                case 1:
                    std::ranges::sort(resultVector, [](const Record *rec1, const Record *rec2) {
                        return rec1->site < rec2->site;
                    });
                    break;
                case 2:
                    std::ranges::sort(resultVector, [](const Record *rec1, const Record *rec2) {
                        return rec1->site > rec2->site;
                    });
                    break;
                default:
                    std::cout << "Zly wybor" << endl;
                    sort(records);
                    break;
            }
            break;
        case 5:
            int asc_dsc_choice_password_length;
            std::cout << "[1.ROSNACO / 2.MALEJACO]" << endl;
            cin >> asc_dsc_choice_password_length;
            switch (asc_dsc_choice_password_length) {
                case 1:
                    std::ranges::sort(resultVector, [](const Record *rec1, const Record *rec2) {
                        return rec1->password.length() < rec2->password.length();
                    });
                    break;
                case 2:
                    std::ranges::sort(resultVector, [](const Record *rec1, const Record *rec2) {
                        return rec1->password.length() > rec2->password.length();
                    });
                    break;
                default:
                    std::cout << "Zly wybor" << endl;
                    sort(records);
                    break;
            }
            break;
        case 6:
            int asc_dsc_choice_first_category_second_name;
            std::cout << "[1.ROSNACO / 2.MALEJACO]" << endl;
            cin >> asc_dsc_choice_first_category_second_name;
            switch (asc_dsc_choice_first_category_second_name) {
                case 1:
                    std::ranges::sort(resultVector, [](const Record *rec1, const Record *rec2) {
                        if (rec1->category != rec2->category) {
                            return rec1->category < rec2->category;
                        } else {
                            return rec1->name < rec2->name;
                        }
                    });
                    break;
                case 2:
                    std::ranges::sort(resultVector, [](const Record *rec1, const Record *rec2) {
                        if (rec1->category != rec2->category) {
                            return rec1->category > rec2->category;
                        } else {
                            return rec1->name > rec2->name;
                        }
                    });
                    break;

            }
            break;
        case 7:
            int asc_dsc_choice_first_site_second_category;
            std::cout << "[1.ROSNACO / 2.MALEJACO]" << endl;
            cin >> asc_dsc_choice_first_site_second_category;
            switch (asc_dsc_choice_first_site_second_category) {
                case 1:
                    std::ranges::sort(resultVector, [](const Record *rec1, const Record *rec2) {
                        if (rec1->site != rec2->site) {
                            return rec1->site < rec2->site;
                        } else {
                            return rec1->category < rec2->category;
                        }
                    });
                    break;
                case 2:
                    std::ranges::sort(resultVector, [](const Record *rec1, const Record *rec2) {
                        if (rec1->site != rec2->site) {
                            return rec1->site > rec2->site;
                        } else {
                            return rec1->category > rec2->category;
                        }
                    });
                    break;
            }
            break;
    }
    if (resultVector.size() == 0) {
        cout << "Brak wpisow w bazie!" << endl;
        cout << "Dodaj hasla" << endl;
    } else {
        vectorPrinter(resultVector);
    }
    std::cout << std::endl;
}

/**
 * @brief Głównym zadaniem funkcji jest edycja istniejącego już w bazie danych wpisu.
 * @details dzięki funkcji możemy edytować:
 * NAZWE,LOGIN,HASLO,KATEGORIE,WWW.
 *
 * na samym końcu funkcja zapisuje zmiany do pliku oraz uaktualnia baze haseł
 * @param records baza haseł
 * @param categories baza kategorii
 * @param direcoryOfAFile  ścieżka/nazwa pliku
 * @param filePassword hasło do bazy danych/pliku
 */
void edit(std::vector<Record *> &records, std::set<std::string> &categories, std::string direcoryOfAFile,
          const std::string &filePassword) {
    int choice;
    int changeChoice;
    vectorPrinter(records);
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "Ktore z powyzszych hasel chcesz edytowac wpisz cyfre: " << endl;
    cin >> choice;
    choice--;
    std::cout << "Twoje wybrane haslo to: " << endl;
    std::cout << records.at(choice) << endl;
    std::cout << "Co chcesz w hasle zmienic?" << endl;
    std::cout << "1.Nazwe" << endl;
    std::cout << "2.Login" << endl;
    std::cout << "3.Haslo" << endl;
    std::cout << "4.Kategorie" << endl;
    std::cout << "5.Strone WWW" << endl;
    cin >> changeChoice;

    switch (changeChoice) {
        case 1: {
            std::string newName;
            std::cout << "Zmieniam nazwe. Wpisz ja ponizej" << endl;
            cin.ignore();
            std::getline(std::cin, newName);
            records.at(choice)->name = newName;
            std::cout << "Zmienilem dane, teraz twoj wpis wyglada tak" << endl;
            std::cout << records.at(choice) << endl;
            break;
        }
        case 2: {
            std::string newLogin;
            std::cout << "Zmieniam login. Wpisz go ponizej" << endl;
            cin >> newLogin;
            records.at(choice)->login = newLogin;
            std::cout << "Zmienilem dane, teraz twoj wpis wyglada tak" << endl;
            std::cout << records.at(choice) << endl;
            break;
        }
        case 3: {
            std::cout << "Zmieniam haslo. Wpisz je ponizej" << endl;
            std::string newPassword;
            cin >> newPassword;
            records.at(choice)->password = newPassword;
            std::cout << "Zmienilem dane, teraz twoj wpis wyglada tak" << endl;
            std::cout << records.at(choice) << endl;
            break;
        }
        case 4: {
            int categoryChoice;
            vector<std::string> vectorOfCategories = {};
            std::cout << "Zmieniam kategorie." << endl;
            setPrinter(categories);
            for (auto item: categories) {
                vectorOfCategories.push_back(item);
            }
            std::cout << "Wybierz z listy cyfre" << endl;
            cin >> categoryChoice;
            categoryChoice--;
            records.at(choice)->category = vectorOfCategories.at(categoryChoice);
            std::cout << "Zmienilem dane, teraz twoj wpis wyglada tak" << endl;
            std::cout << records.at(choice) << endl;
            break;
        }
        case 5: {
            std::cout << "Zmieniam STRONE. Wpisz je ponizej" << endl;
            std::string newSite;
            cin >> newSite;
            records.at(choice)->site = newSite;
            std::cout << "Zmienilem dane, teraz twoj wpis wyglada tak" << endl;
            std::cout << records.at(choice) << endl;
            break;
        }
        default:
            break;
    }
    encryptFile(records, direcoryOfAFile, filePassword);
}

/**
 * @brief Głównym zadaniem funkcji jest dodanie do bazy kategorii nowej kategorii.
 * @details Funkcja sprawdza także czy kategoria jest unikalna/czy się nie powtarza
 * @param categories ->->baza kategorii
 */
void addCategory(std::set<string> &categories) {
    std::cout << "Wpisz nazwe kategorii: " << endl;
    std::string newCategory;
    cin.ignore();
    getline(std::cin, newCategory);

    if (categories.contains(newCategory)) {
        std::cout << "Podana kategoria juz istnieje!" << endl;
        std::cout << endl;
    } else {
        categories.insert(newCategory);
        cout << "Kategoria " + newCategory + " zostala dodana" << endl;
        std::cout << endl;
    }
}

/**
 * @brief Głównym zadaniem funkcji jest usuwanie kategorii wraz z hasłami które w tej kategorii się znajdują.
 * @details Funkcja dba aby użytkownik nie usunął dużej ilości haseł jednym kliknięciem.
 * Przed wymazaniem kategorii wyświetla komunikat o ilości możliwych do usunięcia haseł.
 * Wymagane jest dodatkowe potwierdzenie użytkownika [1.TAK / 2.NIE]
 * Po usunięciu haseł funkcja aktualizuje baze danych oraz zapisuje zmiany do pliku
 * @param records ->baza haseł
 * @param categories ->baza kategorii
 * @param direcoryOfAFile -> ścieżka/nazwa pliku
 * @param filePassword ->hasło do bazy danych/pliku
 */
void deleteCategory(std::set<string> &categories, std::vector<Record *> &records, std::string direcoryOfAFile,
                    const std::string &filePassword) {
    std::cout << "Ktora kategorie mam usunac?" << endl;
    std::vector<string> vectorOfCategories = {};
    int counter = 1;
    int choice;
    int choiceToDeleteCategory;
    for (auto item: categories) {
        std::cout << counter << "." << item << endl;
        counter++;
    }
    cin >> choice;
    choice--;
    for (auto item: categories) {
        vectorOfCategories.push_back(item);
    }
    std::string categoryToDelete = vectorOfCategories.at(choice);
    auto specificPasswordsCounter = std::count_if(records.begin(), records.end(),
                                                  [&categoryToDelete](Record *rec1) -> auto {
                                                      return rec1->category == categoryToDelete;
                                                  });
    std::cout << "UWAGA USUWASZ KATEGORIE " << vectorOfCategories.at(choice) << " KTORA ZAWIERA "
              << specificPasswordsCounter << " HASEL" << endl;
    std::cout << "CZY NAPEWNO CHCESZ USUNAC TE KATEGORIE? [1.TAK / 2.NIE]" << endl;
    cin >> choiceToDeleteCategory;
    switch (choiceToDeleteCategory) {
        case 1:
            if (choice >= 0 && choice < categories.size()) {
                auto it = categories.begin();
                std::advance(it, choice);
                categories.erase(it);
            }
            std::erase_if(records, [&categoryToDelete](Record *rec1) -> auto {
                return rec1->category == categoryToDelete;
            });
            encryptFile(records, direcoryOfAFile, filePassword);
            std::cout << "Kategoria " << categoryToDelete << " zostala usunieta wraz z " << specificPasswordsCounter
                      << " haslami" << endl;
            break;
        case 2:
            std::cout << "Kategoria " << categoryToDelete << " nie zostala usunieta" << endl;
            break;
        default:
            std::cout << "Zly wybor, sproboj jeszcze raz" << endl;
            deleteCategory(categories, records, direcoryOfAFile, filePassword);
            break;
    }
    //vectorPrinter(records);
}

/**
 * @brief Funkcja wyświetla użytkownikowi listę haseł, które są możliwe do usunięcia.
 * @details Użytkownik podczas pojedyńczego usunięcia wpisuje index hasła w konsole.
 * Podczas masowego usuwania użytkownik jest zobligowany do wpisania index'ów po przecinku.
 * Pod koniec funkcja aktualizuje baze danych oraz plik
 * @param records ->baza haseł
 * @param categories ->baza kategorii
 * @param direcoryOfAFile -> ścieżka/nazwa pliku
 * @param filePassword ->hasło do bazy danych/pliku
 */
void deletePassword(std::set<string> &categories, std::vector<Record *> &records, std::string direcoryOfAFile,
                    const std::string &filePassword) {
    if (records.size() == 0) {
        cout << "Brak wpisow w bazie!,nie mozesz usunac hasel ktorych nie ma" << endl;

    } else {
        std::cout << std::endl;
        vectorPrinter(records);
        std::string indexesToDelete;
        int choice;
        std::cout << "Ktore z powyzszych hasel chcesz usunac?" << endl;
        std::cout << "UWAGA! jesli chcesz usunac pare hasel wypisuj index'y po przecinku!" << endl;
        std::cout << "Jesli usuwasz 1 rekord wpisz w konsole np: 5, jesli usuwasz wiecej wpisz: 7,9,8,2" << endl;
        cin >> indexesToDelete;
        vector<string> indexes = split(indexesToDelete, ',');
        std::cout << "Uwaga usuwasz nastepujace rekordy :" << endl;
        for (int i = 0; i < indexes.size(); ++i) {
            int index = std::stoi(indexes.at(i)) - 1;
            std::cout << records.at(index) << endl;
        }
        std::cout << "Kontynulowac? [1.TAK / 2.NIE]" << endl;
        cin >> choice;
        switch (choice) {
            case 1:
                for (int i = 0; i < indexes.size(); ++i) {
                    int index = std::stoi(indexes.at(i)) - 1;
                    records.erase(records.begin() + index);
                }
                encryptFile(records, direcoryOfAFile, filePassword);
                std::cout << "Haslo zostalo pomyslnie usuniete" << endl;
                break;
            case 2:
                std::cout << "Usuwanie zostalo przerwane" << endl;
                mainMenu(records, categories, direcoryOfAFile, filePassword);
                break;
            default:
                std::cout << "Zly wybor" << endl;

        }
    }
}
/**
 * @brief Głównym zadaniem funkcji jest wyświetlenie timestamp'u
 * @details Funkcja odczytuje z pliku pierwszą oraz ostatnią linie pliku
 * oraz deszyfruje timestamp
 * @param direcoryOfAFile ścieżka/nazwa pliku
 * @param filePassword hasło do bazy danych/pliku
 */
void showTimeStamp(const std::string &direcoryOfAFile, const std::string &filePassword) {
    int counterLineInFile = 1;

    std::vector <std::string> timeStamps = {};
    std::string line;
    std::ifstream inputFile(direcoryOfAFile);
    std::cout<<"Ostatnia edycja / nieudana proba otwarcia pliku zostala przeprowadzona: "<<endl;
    if (inputFile.is_open()) {
        while (std::getline(inputFile, line))
            counterLineInFile++;
    }
    counterLineInFile--;

    if (inputFile.is_open()) {
        int currentLine = 1;
        inputFile.clear();
        inputFile.seekg(0, std::ios::beg);
        while (std::getline(inputFile, line)) {
            if (currentLine == 1 || currentLine == counterLineInFile) {
                timeStamps.push_back(line);
            }
            currentLine++;
        }
        inputFile.close();
    }
    std::string notification = timeStamps.at(0).substr(20,2)+"/"+
            timeStamps.at(1).substr(20,2)+"/"+
            timeStamps.at(0).substr(14,4)+" @ "+
            timeStamps.at(1).substr(16,2)+":"+
            timeStamps.at(1).substr(11,2)+":"+
            timeStamps.at(0).substr(6,2);
    std::cout<<notification<<endl;
}

/**
 * Funkcja mająca za zadanie wyświetlanie w konsoli zawartości set'u
 * @tparam T -> typ set'u
 * @param set -> źródło
 */
template<typename T>
void setPrinter(std::set<T> set) {
    int counter = 1;
    for (T item: set) {
        std::cout << counter << ". " << item << endl;
        counter++;
    }
}

/**
 * Funkcja mająca za zadanie wyświetlanie w konsoli zawartości vector'a
 * @tparam T -> typ vector'a
 * @param set -> źródło
 */
template<typename T>
void vectorPrinter(std::vector<T> vec) {
    std::cout << "   NAZWA | LOGIN | HASLO | WWW | KATEGORIA" << std::endl;
    int counter = 1;
    for (T item: vec) {
        std::cout << counter << ". " << item;
        counter++;
    }
}
