#include "const.h"
#include "enigma.h"
#include "rotors.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

// Converteix una lletra majúscula a índex 0-25
static int lletraAIndex(char c) {
    return c - 'A';
}

// Converteix un índex 0..25 a lletra majúscula
static char indexALletra(int idx) {
    return 'A' + idx;
}

// Llegeix el fitxer "Missatge.txt" i prepara el text 
static bool llegirMissatge(std::string& text) {
    std::ifstream f("Missatge.txt");
    if (!f.is_open()) {
        std::cout << "[ERROR] Missatge.txt: no s'ha pogut obrir\n";
        return false;
    }

    std::string linia;
    text = "";
    while (std::getline(f, linia)) {
        for (char c : linia) {
            // Convertir a majúscula si és lletra
            if (std::isalpha(c)) {
                text += std::toupper(c);
            }
            // Ignorar tot el demés 
        }
    }
    f.close();

    if (text.empty()) {
        std::cout << "[ERROR] Missatge.txt: Fitxer buit\n";
        return false;
    }

    return true;
}


static bool demanarConfiguracioInicial(int* posicions) {
    std::string input;
    std::cout << "Introdueix les 3 lletres de finestra: ";
    std::cin >> input;

    if (input.length() != 3) {
        std::cout << "[ERROR] Cal exactament 3 lletres\n";
        return false;
    }

    for (int i = 0; i < 3; i++) {
        if (input[i] < 'A' || input[i] > 'Z') {
            std::cout << "[ERROR] Les finestres han de ser majúscules\n";
            return false;
        }
        posicions[i] = lletraAIndex(input[i]);
    }

    return true;
}

// Xifra una sola lletra passant pels 3 rotors i el reflector
static char xifraLletra(char lletra, int* posicions) {
    int idx = lletraAIndex(lletra);

    // Passa pels 3 rotors cap endavant
    for (int i = 0; i < NUM_ROTORS; i++) {
        idx = (idx + posicions[i]) % 26;
        idx = getRotor(i, idx);
        idx = (idx - posicions[i] + 26) % 26;
    }

    // Passa pel reflector
    idx = getReflector(idx);

    // Torna pels 3 rotors cap enrere 
    for (int i = NUM_ROTORS - 1; i >= 0; i--) {
        idx = (idx + posicions[i]) % 26;
        // Busquem la posició inversa
        for (int j = 0; j < 26; j++) {
            if (getRotor(i, j) == idx) {
                idx = j;
                break;
            }
        }
        idx = (idx - posicions[i] + 26) % 26;
    }

    return indexALletra(idx);
}

// Desxifra una sola lletra sense avançar rotors, només retrocedeix
static char desxifraLletra(char lletra, int* posicions) {
    int idx = lletraAIndex(lletra);

    // Passa pel reflector
    idx = getReflector(idx);

    // Torna pels 3 rotors cap enrere 
    for (int i = NUM_ROTORS - 1; i >= 0; i--) {
        idx = (idx + posicions[i]) % 26;
        // Busquem la posició inversa
        for (int j = 0; j < 26; j++) {
            if (getRotor(i, j) == idx) {
                idx = j;
                break;
            }
        }
        idx = (idx - posicions[i] + 26) % 26;
    }

    // Passa pels rotors cap endavant 
    for (int i = 0; i < NUM_ROTORS; i++) {
        idx = (idx + posicions[i]) % 26;
        idx = getRotor(i, idx);
        idx = (idx - posicions[i] + 26) % 26;
    }

    return indexALletra(idx);
}

bool xifrarMissatge() {
    std::string missatge;
    if (!llegirMissatge(missatge)) {
        return false;
    }

    int posicions[NUM_ROTORS];
    if (!demanarConfiguracioInicial(posicions)) {
        return false;
    }

    std::ofstream out("Xifrat.txt");
    if (!out.is_open()) {
        std::cout << "[ERROR] A xifrat.txt: no s'ha pogut crear\n";
        return false;
    }

    int comptador = 0;
    int totalLletres = missatge.length();

    for (char c : missatge) {
        avancaRotors(posicions);
        char xifrada = xifraLletra(c, posicions);
        out << xifrada;

        comptador++;
        if (comptador % 5 == 0 && comptador < totalLletres) {
            out << " ";
        }
    }

    out.close();

    int grups = (totalLletres + 4) / 5;
    std::cout << "Missatge xifrat a \"Xifrat.txt\" (" << totalLletres<< " lletres, " << grups << " grups de 5)\n";

    return true;
}

bool desxifrarMissatge() {
    std::ifstream in("Xifrat.txt");
    if (!in.is_open()) {
        std::cout << "[ERROR] Xifrat.txt: no s'ha pogut obrir\n";
        return false;
    }

    std::string xifrat;
    char c;
    while (in.get(c)) {
        if (c >= 'A' && c <= 'Z') {
            xifrat += c;
        }
    }
    in.close();

    if (xifrat.empty()) {
        std::cout << "[ERROR] Xifrat.txt: fitxer buit\n";
        return false;
    }

    int posicions[NUM_ROTORS];
    if (!demanarConfiguracioInicial(posicions)) {
        return false;
    }

    std::ofstream out("desxifrat.txt");
    if (!out.is_open()) {
        std::cout << "[ERROR] desxifrat.txt: no s'ha pogut crear\n";
        return false;
    }

    for (char c : xifrat) {
        avancaRotors(posicions);
        char desxifrada = desxifraLletra(c, posicions);
        out << desxifrada;
    }

    out.close();

    std::cout << "Missatge desxifrat a \"desxifrat.txt\" (" << xifrat.length() << " lletres)\n";

    return true;
}

void editarRotor() {
    int numRotor;
    std::cout << "Quin rotor vols editar? (1-3): ";
    std::cin >> numRotor;

    if (numRotor < 1 || numRotor > 3) {
        std::cout << "[ERROR] Rotor invalid\n";
        return;
    }

    int idx = numRotor - 1;
    std::string novaPermutacio;
    std::cout << "Introdueix la nova permutacio (26 majuscules A-Z): ";
    std::cin >> novaPermutacio;

    if (novaPermutacio.length() != 26) {
        std::cout << "[ERROR] Permutacio incorrecta, calen 26 lletres\n";
        return;
    }

    for (char c : novaPermutacio) {
        if (c < 'A' || c > 'Z') {
            std::cout << "[ERROR] Permutacio incorrecta, calen 26 lletres uniques A-Z\n";
            return;
        }
    }

    // Guardar a fitxer
    std::string nomFitxer;
    if (numRotor == 1) 
        nomFitxer = "rotor_I.txt";
    else if (numRotor == 2) 
        nomFitxer = "rotor_II.txt";
    else 
        nomFitxer = "rotor_III.txt";

    std::ofstream out(nomFitxer);
    if (!out.is_open()) {
        std::cout << "[ERROR] No s'ha pogut guardar el rotor\n";
        return;
    }

    out << novaPermutacio << "\n";
    out.close();

    std::cout << "Rotor " << numRotor << " guardat a \"" << nomFitxer << "\"\n";
}