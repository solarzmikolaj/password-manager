#include <string>
#include <vector>
#include "PasswordGenerator.h"
#include "Record.h"

/**
 * @brief Funkcja sprawdza czy std::string password zawiera wielkie litery
 * @param password sprawdzane hasło
 * @return zwraca wartość logiczną bool
 */
bool hasUpperCaseLetter(std::string password) {
    int counter = 0;
    for (int i = 0; i < password.size(); ++i) {
        if (isupper(password.at(i))) {
            counter++;
        }
    }
    return counter > 0;
}
/**
 * @brief Funkcja sprawdza czy w std::string password zawiera znaki specjalne
 * @param password sprawdzane hasło
 * @return zwraca wartość logiczną bool
 */
bool hasSpecialSign(std::string password) {
    std::string specialSignsString = "!@#$%^&*()}{;:.<>?[]'";
    int specialdigits = std::count_if(password.begin(), password.end(), [&specialSignsString](char c) -> auto {
        int counter = 0;
        for (int i = 0; i < specialSignsString.size(); ++i) {
            if (c == specialSignsString.at(i)) {
                counter++;
            }
        }
        return counter;
    });
    return specialdigits > 0;
}

/**
 * @brief Generuje hasło o podanych parametrach
 * @param length -> określa długość generowanego hasła
 * @param upperCaseLetters -> określa czy hasło ma zawierać wielkie litery
 * @param specialSigns -> określa czy hasło ma zawierać znaki specjalne
 * @return
 */
std::string passwordGenerator(int length, bool upperCaseLetters, bool specialSigns) {
    srand(static_cast<unsigned int>(time(nullptr)));

    std::string lowerCaseLettersString = "abcdefghijklmnopqrstuvwxyz";
    std::string upperCaseLettersString = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string specialSignsString = "!@#$%^&*()}{;:.<>?[]'";
    std::string passwordPrimaryDatabase = lowerCaseLettersString;

    if (upperCaseLetters) {
        passwordPrimaryDatabase += upperCaseLettersString;
    }
    if (specialSigns) {
        passwordPrimaryDatabase += specialSignsString;
    }

    std::string finalPassword = "";
    for (int i = 0; i < length; ++i) {
        int randomIndex = rand() % passwordPrimaryDatabase.length();
        finalPassword += passwordPrimaryDatabase[randomIndex];
    }

    if (upperCaseLetters && !hasUpperCaseLetter(finalPassword)) {
        int randomIndex = rand() % finalPassword.length();
        finalPassword[randomIndex] = upperCaseLettersString[rand() % upperCaseLettersString.length()];
    }
    if (specialSigns && !hasSpecialSign(finalPassword)) {
        int randomIndex = rand() % finalPassword.length();
        finalPassword[randomIndex] = specialSignsString[rand() % specialSignsString.length()];
    }

    return finalPassword;
}

/**
 * @brief Zwraca wartość logiczną bool w zależności czy hasło jest silne
 * @details Warunki do spełnienia żeby hasło było silne to:
 * 2*wielkie litery , 2*znaki specjalne , długość hasła >=7
 * @param password -> badane hasło
 * @return
 */
bool isItAStrongPassword(const std::string &password) {
    int upperCaseCounter = 0;
    int specialSignsCounter = 0;
    std::vector<char> specialSignsDatabase = {'!', '@', '#', '$', '%', '%', '^', '&', '*', '(', ')', '}', '{', ';', ':',
                                              ',', '.', '<', '>', '?'};

    for (int i = 0; i < password.length(); i++) {
        auto itContent = std::find(specialSignsDatabase.begin(), specialSignsDatabase.end(), password.at(i));
        if (itContent != specialSignsDatabase.end()) {
            specialSignsCounter++;
        }
        if (std::isupper(password.at(i))) {
            upperCaseCounter++;
        }
    }
    return upperCaseCounter >= 2 && specialSignsCounter >= 2 && password.length() >= 7;
}
/**
 * @brief Funkcja zwraca informacje odnośnie hasła
 * @details określa czy jest bezpiecze oraz czy jest unikalne
 * @param password
 * @param records
 * @return
 */
std::string passwordInfo( std::string password, std::vector<Record *> records) {
    bool isStrong = isItAStrongPassword(password);
    bool isUnique = wasUsedBefore(password, records);
    if (isStrong && !isUnique) {
        return "BRAWO! Twoje haslo jest unikalne, oraz bezpieczne!";
    } else if (!isStrong && !isUnique) {
        return "UWAGA! Twoje haslo nie jest bezpieczne, ALE unikalne!";
    } else if (isStrong && isUnique){
        return "UWAGA! Twoje haslo jest bezpieczne, Ale juz sie powtorzylo!";
    } else if (!isStrong && isUnique ){
        return "UWAGA! Twoje haslo NIE jest bezpieczne, ANI unikalne!";
    }
}

/**
 * @brief Zwraca wartość logiczną bool, która określa czy dane hasło wystąpiło wcześniej
 * @param searchedPassword -> badane hasło
 * @param records  -> jako baza haseł
 * @return
 */
bool wasUsedBefore(std::string searchedPassword, std::vector<Record *> records) {
    int useages = std::count_if(records.begin(), records.end(), [&searchedPassword](const Record *rec) -> auto {
        return rec->password == searchedPassword;
    });
    return useages >= 1;
}