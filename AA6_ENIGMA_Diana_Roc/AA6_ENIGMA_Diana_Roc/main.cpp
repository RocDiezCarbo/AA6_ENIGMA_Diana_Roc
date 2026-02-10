#include <iostream>
#include "rotors.h"
#include "menu.h"

int main() {
    // Carregar els 3 rotors i el reflector
    if (!carregarRotor("rotor_I.txt", 0) ||
        !carregarRotor("rotor_II.txt", 1) ||
        !carregarRotor("rotor_III.txt", 2) ||
        !carregarReflector("reflector.txt")) {

        std::cout << "ERROR 1: No s'han pogut carregar els rotors/reflector.\n";
        return 1;
    }

    std::cout << "OK: Rotors carregats.\n";
    menu();
    return 0;
}
