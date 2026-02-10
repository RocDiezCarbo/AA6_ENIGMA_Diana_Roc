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


std::string normalitzaText(const std::string& input) {
    std::string resultat;
    resultat.reserve(input.size());

    for (unsigned char uc : input) {
        char c = static_cast<char>(uc);

        // Convertim a majúscula ASCII sense dependre de locales
        if (c >= 'a' && c <= 'z') c = static_cast<char>(c - 'a' + 'A');

        // Ens quedem estrictament amb A..Z
        if (c >= 'A' && c <= 'Z') {
            resultat.push_back(c);
        }
    }
    return resultat;
}

bool llegirIMagatzemarMissatge(const std::string& ruta) {
    std::cout << "Introdueix el missatge a xifrar:\n> ";

    std::string entrada;
    // Important: consumim espais i salts de línia pendents (p. ex. després d'un std::cin >> option)
    std::getline(std::cin >> std::ws, entrada);

    std::string net = normalitzaText(entrada);

    std::ofstream f(ruta, std::ios::out | std::ios::trunc);
    if (!f) {
        std::cerr << "[ERROR] No s'ha pogut crear/obrir " << ruta << "\n";
        return false;
    }

    f << net;
    if (!f) {
        std::cerr << "[ERROR] Error escrivint a " << ruta << "\n";
        return false;
    }

    std::cout << "[OK] Missatge guardat a \"" << ruta << "\" ("
        << net.size() << " lletres)\n";
    return true;
}


// Llegeix el fitxer "Missatge.txt" i prepara el text 
static bool llegirMissatge(std::string& text) {
    std::ifstream f("Missatge.txt");
    if (!f.is_open()) {
        std::cout << "[ERROR] Missatge.txt: no s'ha pogut obrir\n";
        return false;
    }

    std::string contingut, linia;
    while (std::getline(f, linia)) {
        contingut += linia;
        contingut += '\n';
    }
    f.close();

    text = normalitzaText(contingut);

    if (text.empty()) {
        std::cout << "[ERROR] Missatge.txt: Fitxer buit o sense lletres A-Z\n";
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
        char desxifrada = xifraLletra(c, posicions);
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
    std::cout << "Introdueix la nova permutacio (26 majuscules A-Z, sense repetir): ";
    std::cin >> novaPermutacio;

    // 1) Validació de mida
    if (novaPermutacio.size() != 26) {
        std::cout << "[ERROR] Permutacio incorrecta: calen 26 lletres\n";
        return;
    }

    // 2) Validació A-Z i unicitat
    bool vist[26] = { false };
    for (char c : novaPermutacio) {
        if (c < 'A' || c > 'Z') {
            std::cout << "[ERROR] Permutacio incorrecta: calen lletres A-Z\n";
            return;
        }
        int k = c - 'A';
        if (vist[k]) {
            std::cout << "[ERROR] Permutacio incorrecta: lletres repetides\n";
            return;
        }
        vist[k] = true;
    }

    // 3) Determinar el fitxer del rotor (mantinc els teus noms actuals)
    std::string nomFitxer;
    if (numRotor == 1) nomFitxer = "rotor_I.txt";
    else if (numRotor == 2) nomFitxer = "rotor_II.txt";
    else nomFitxer = "rotor_III.txt";

    // 4) Conservar el notch actual (2a línia)
    char notchActual = static_cast<char>('A' + getRotorNotch(idx));

    // 5) Guardar el rotor amb 2 línies: cablejat + notch
    std::ofstream out(nomFitxer, std::ios::out | std::ios::trunc);
    if (!out.is_open()) {
        std::cout << "[ERROR] " << nomFitxer << ": no s'ha pogut guardar\n";
        return;
    }

    out << novaPermutacio << "\n" << notchActual << "\n";
    out.close();

    // 6) Recarregar a memòria perquè s'apliqui immediatament
    if (!carregarRotor(nomFitxer.c_str(), idx)) {
        std::cout << "[ERROR] " << nomFitxer
            << ": guardat pero no s'ha pogut recarregar\n";
        return;
    }

    std::cout << "[OK] Rotor " << numRotor << " guardat a \"" << nomFitxer
        << "\" (notch " << notchActual << ")\n";
}
