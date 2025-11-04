//
// Created by Mikolaj Solarz on 19.05.2023.
//
#pragma once
#include <string>
#include <ostream>
#include <iostream>
/**
 * @class Record
 * @brief Tworzy obiekt, który zawiera podstawowe
 * @details w swoim konstruktorze posiada pola specyficzne dla parametrów każdego hasła
 * POLA KLASY RECORD:
 * name -> string który przechowuje nazwę wpisu,
 * login -> string, który przechowuje login do hasła,
 * password -> string, który przechowuje hasło,
 * site -> string, który przechowuje strone internetowa,
 * category -> string, który przechowuje kategorie hasła,
 *
 * KLASA POSIADA 2 Stream Output Operator'y
 * Oba mają za zadanie wyświelić obiekt klasy Record w konsoli
 * Jeden przyjmuje Obiekt klasy Record przekazany przez referencje
 * Drugi przyjmuje wskaźnik do obiektu klasy Record
 */
using std::string;
class Record {
public:
    string name;
    string login;
    string password;
    string site;
    string category;

    Record(std::string name,std::string login,std::string password,std::string site, std::string category)
            : name(name),login(login),password(password),site(site),category(category){

    };
    Record(Record &otherRecord)
            : name(otherRecord.name), login(otherRecord.login),password(otherRecord.password),site(otherRecord.site),category(otherRecord.category){
    };

    friend std::ostream &operator<<(std::ostream &os, const Record &record) {
        std::string site;
        if(record.site == " "){
            site = "---";
        }else{
            site = record.site;
        };
        os <<record.name << " | " << record.login << " | " << record.password << " | "<<site+" | "
           << record.category<<std::endl;
        return os;
    }
    friend std::ostream &operator<<(std::ostream &os, const Record *record) {
        std::string site;
        if(record->site == " "){
            site = "---";
        }else{
            site = record->site;
        };
        os <<record->name << " | " << record->login << " | " << record->password << " | "<<site<<" | "
           << record->category<<std::endl;
        return os;
    }

};

