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
    if (s.empty()) return false;  // línia completament buida
    // Cerca el primer caràcter que no sigui espai/tab/salt de línia
    size_t i = s.find_first_not_of(" \t\r\n");
    if (i == std::string::npos) return false;  // només hi ha blancs
    return s[i] != '#';  // si comença per '#', la considerem comentari
}

// Llegeix del fitxer la següent línia "útil" (no buida ni comentari)
static bool llegirLiniaUtil(std::ifstream& f, std::string& out) {
    while (std::getline(f, out)) {
        // Elimina '\r' final en cas de fitxers amb format Windows
        if (!out.empty() && out.back() == '\r') out.pop_back();
        // Retorna la primera línia que sigui útil
        if (esLiniaUtil(out)) return true;
    }
    // No s'ha trobat cap línia útil abans d'arribar al final
    return false;
}

// Comprova que una línia té EXACTAMENT 26 majúscules (A..Z)
// S'utilitza per validar el cablejat de rotors i reflector
static bool validar26Majuscules(const std::string& s) {
    if (s.size() != 26) return false;
    for (char c : s) if (c < 'A' || c > 'Z') return false;
    return true;
}

// Comprova que la línia té com a mínim un caràcter i que el primer és una majúscula
// S'utilitza per validar la lletra del notch
static bool validar1Majuscula(const std::string& s) {
    if (s.size() < 1) return false;
    char c = s[0];
    return (c >= 'A' && c <= 'Z');
}

// Carrega un sol rotor des del fitxer 'nomFitxer' a l'índex 'idxRotor'
bool carregarRotor(const char* nomFitxer, int idxRotor) {
    // Comprovació de rang de l'índex (0..NUM_ROTORS-1)
    if (idxRotor < 0 || idxRotor >= NUM_ROTORS) {
        std::cout << "Error: idxRotor invalid\n";
        return false;
    }

    // Obrim el fitxer del rotor
    std::ifstream f(nomFitxer);
    if (!f.is_open()) {
        std::cout << "Error: no s'ha pogut obrir " << nomFitxer << "\n";
        return false;
    }

    std::string linia;

    // Ha de ser una permutació de 26 majúscules
    if (!llegirLiniaUtil(f, linia) || !validar26Majuscules(linia)) {
        std::cout << "Error: permutacio invalida a " << nomFitxer << "\n";
        return false;
    }
    // Guardem cada lletra com a índex 0..25 al vector del rotor
    for (int i = 0; i < 26; i++)
        rotors[idxRotor][i] = lletraAIndex(linia[i]);

    // ---- línia 2: notch del rotor (opcional) ----
    // Intentem llegir una segona línia útil
    if (llegirLiniaUtil(f, linia)) {
        // Si hi ha línia, ha de començar per una majúscula
        if (!validar1Majuscula(linia)) {
            std::cout << "Error: notch invalid a " << nomFitxer << "\n";
            return false;
        }
        // Guardem la lletra del notch com a índex 0..25
        notches[idxRotor] = lletraAIndex(linia[0]);
    }
    else {
        // Si no hi ha segona línia, posem notch per defecte a 'Z'
        notches[idxRotor] = lletraAIndex('Z');
    }

    return true;  // càrrega correcta
}

// Carrega el reflector des del fitxer 'nomFitxer'
bool carregarReflector(const char* nomFitxer) {
    std::ifstream f(nomFitxer);
    if (!f.is_open()) {
        std::cout << "Error: no s'ha pogut obrir " << nomFitxer << "\n";
        return false;
    }

    std::string linia;
    // Només cal una línia útil amb 26 majúscules (la permutació del reflector)
    if (!llegirLiniaUtil(f, linia) || !validar26Majuscules(linia)) {
        std::cout << "Error: permutacio de reflector invalida\n";
        return false;
    }
    // Guardem la permutació al vector 'reflector'
    for (int i = 0; i < 26; i++)
        reflector[i] = lletraAIndex(linia[i]);

    return true;  // reflector carregat correctament
}


// Retorna el cablejat del rotor 'rotor' a la posició 'index'
int getRotor(int rotor, int index) { return rotors[rotor][index]; }

// Retorna la posició del notch del rotor 'rotor'
int getRotorNotch(int rotor) { return notches[rotor]; }

// Retorna el cablejat del reflector a la posició 'index'
int getReflector(int index) { return reflector[index]; }
