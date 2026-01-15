#include "rotors.h"

#include <fstream>
#include <iostream>
#include <string>

static int rotors[NUM_ROTORS][26];
static int notches[NUM_ROTORS];
static int reflector[26];

static int lletraAIndex(char c) { return c - 'A'; }

static bool esLiniaUtil(const std::string& s) {
    if (s.empty()) return false;
    size_t i = s.find_first_not_of(" \t\r\n");
    if (i == std::string::npos) return false;
    return s[i] != '#';
}

static bool llegirLiniaUtil(std::ifstream& f, std::string& out) {
    while (std::getline(f, out)) {
        if (!out.empty() && out.back() == '\r') out.pop_back();
        if (esLiniaUtil(out)) return true;
    }
    return false;
}

static bool validar26Majuscules(const std::string& s) {
    if (s.size() != 26) return false;
    for (char c : s) if (c < 'A' || c > 'Z') return false;
    return true;
}

static bool validar1Majuscula(const std::string& s) {
    if (s.size() < 1) return false;
    char c = s[0];
    return (c >= 'A' && c <= 'Z');
}

bool carregarRotor(const char* nomFitxer, int idxRotor) {
    if (idxRotor < 0 || idxRotor >= NUM_ROTORS) {
        std::cout << "Error: idxRotor invalid\n";
        return false;
    }

    std::ifstream f(nomFitxer);
    if (!f.is_open()) {
        std::cout << "Error: no s'ha pogut obrir " << nomFitxer << "\n";
        return false;
    }

    std::string linia;

    // línia 1: permutació
    if (!llegirLiniaUtil(f, linia) || !validar26Majuscules(linia)) {
        std::cout << "Error: permutacio invalida a " << nomFitxer << "\n";
        return false;
    }
    for (int i = 0; i < 26; i++)
        rotors[idxRotor][i] = lletraAIndex(linia[i]);

    // línia 2: notch (opcional, si falta el posem a 'Z')
    if (llegirLiniaUtil(f, linia)) {
        if (!validar1Majuscula(linia)) {
            std::cout << "Error: notch invalid a " << nomFitxer << "\n";
            return false;
        }
        notches[idxRotor] = lletraAIndex(linia[0]);
    }
    else {
        notches[idxRotor] = lletraAIndex('Z');
    }

    return true;
}

bool carregarReflector(const char* nomFitxer) {
    std::ifstream f(nomFitxer);
    if (!f.is_open()) {
        std::cout << "Error: no s'ha pogut obrir " << nomFitxer << "\n";
        return false;
    }

    std::string linia;
    if (!llegirLiniaUtil(f, linia) || !validar26Majuscules(linia)) {
        std::cout << "Error: permutacio de reflector invalida\n";
        return false;
    }
    for (int i = 0; i < 26; i++)
        reflector[i] = lletraAIndex(linia[i]);

    return true;
}

// getters

int getRotor(int rotor, int index) { return rotors[rotor][index]; }
int getRotorNotch(int rotor) { return notches[rotor]; }
int getReflector(int index) { return reflector[index]; }
