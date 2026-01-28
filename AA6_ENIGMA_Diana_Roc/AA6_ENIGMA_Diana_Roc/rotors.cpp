#include "rotors.h"

#include <fstream>
#include <iostream>
#include <string>

// Matriu de cablejats dels rotors: NUM_ROTORS rotors, cadascun amb 26 posicions (A..Z)
static int rotors[NUM_ROTORS][26];
// Posició del notch (lletra on fa avançar el següent rotor) per a cada rotor
static int notches[NUM_ROTORS];
// Cablejat del reflector: 26 posicions (A..Z)
static int reflector[26];

// Converteix una lletra majúscula ('A'..'Z') en un índex 0..25
static int lletraAIndex(char c) { return c - 'A'; }

// Indica si una línia del fitxer és "útil": no buida, no només espais i no comença per '#'
static bool esLiniaUtil(const std::string& s) {
    if (s.empty()) return false;
    size_t i = s.find_first_not_of(" \t\r\n");
    if (i == std::string::npos) return false;
    return s[i] != '#';
}

// Llegeix del fitxer la següent línia "útil" (no buida ni comentari)
static bool llegirLiniaUtil(std::ifstream& f, std::string& out) {
    while (std::getline(f, out)) {
        if (!out.empty() && out.back() == '\r') out.pop_back(); // Windows CRLF
        if (esLiniaUtil(out)) return true;
    }
    return false;
}

// Valida que sigui una permutació exacta de 26 lletres majúscules A..Z, sense repeticions
static bool validarPermutacioAZ(const std::string& s) {
    if (s.size() != 26) return false;

    bool vist[26] = { false };
    for (char c : s) {
        if (c < 'A' || c > 'Z') return false;
        int k = c - 'A';
        if (vist[k]) return false;
        vist[k] = true;
    }
    return true;
}

// Comprova que la línia té com a mínim un caràcter i que el primer és una majúscula
static bool validar1Majuscula(const std::string& s) {
    if (s.empty()) return false;
    char c = s[0];
    return (c >= 'A' && c <= 'Z');
}

// Carrega un sol rotor des del fitxer 'nomFitxer' a l'índex 'idxRotor'
bool carregarRotor(const char* nomFitxer, int idxRotor) {
    if (idxRotor < 0 || idxRotor >= NUM_ROTORS) {
        std::cout << "[ERROR] idxRotor invalid\n";
        return false;
    }

    std::ifstream f(nomFitxer);
    if (!f.is_open()) {
        std::cout << "[ERROR] " << nomFitxer << ": no s'ha pogut obrir\n";
        return false;
    }

    std::string linia;

    // Línia 1: cablejat (permutació de 26 lletres úniques)
    if (!llegirLiniaUtil(f, linia) || !validarPermutacioAZ(linia)) {
        std::cout << "[ERROR] " << nomFitxer
            << ": permutacio incorrecta — calen 26 lletres úniques A–Z\n";
        return false;
    }

    for (int i = 0; i < 26; i++)
        rotors[idxRotor][i] = lletraAIndex(linia[i]);

    // Línia 2 (opcional): notch
    if (llegirLiniaUtil(f, linia)) {
        if (!validar1Majuscula(linia)) {
            std::cout << "[ERROR] " << nomFitxer << ": notch invalid\n";
            return false;
        }
        notches[idxRotor] = lletraAIndex(linia[0]);
    }
    else {
        notches[idxRotor] = lletraAIndex('Z'); // per defecte
    }

    return true;
}

// Carrega el reflector des del fitxer 'nomFitxer'
bool carregarReflector(const char* nomFitxer) {
    std::ifstream f(nomFitxer);
    if (!f.is_open()) {
        std::cout << "[ERROR] " << nomFitxer << ": no s'ha pogut obrir\n";
        return false;
    }

    std::string linia;

    // Reflector: també ha de ser una permutació de 26 lletres úniques
    if (!llegirLiniaUtil(f, linia) || !validarPermutacioAZ(linia)) {
        std::cout << "[ERROR] " << nomFitxer
            << ": permutacio incorrecta — calen 26 lletres úniques A–Z\n";
        return false;
    }

    for (int i = 0; i < 26; i++)
        reflector[i] = lletraAIndex(linia[i]);

    return true;
}

// Retorna el cablejat del rotor 'rotor' a la posició 'index'
int getRotor(int rotor, int index) { return rotors[rotor][index]; }

// Retorna la posició del notch del rotor 'rotor'
int getRotorNotch(int rotor) { return notches[rotor]; }

// Retorna el cablejat del reflector a la posició 'index'
int getReflector(int index) { return reflector[index]; }

// Avança rotors (mode notch)
void avancaRotors(int* posicions) {
    // Avança rotor 0 sempre
    posicions[0] = (posicions[0] + 1) % 26;

    // Si rotor 0 arriba al notch, avança rotor 1
    if (posicions[0] == getRotorNotch(0)) {
        posicions[1] = (posicions[1] + 1) % 26;

        // Si rotor 1 arriba al notch, avança rotor 2
        if (posicions[1] == getRotorNotch(1)) {
            posicions[2] = (posicions[2] + 1) % 26;
        }
    }
}
