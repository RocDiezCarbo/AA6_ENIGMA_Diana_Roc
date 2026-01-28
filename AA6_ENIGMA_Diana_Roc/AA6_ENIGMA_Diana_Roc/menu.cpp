#include <iostream>
#include "menu.h"
#include "enigma.h"

void printMenu() {
    std::cout << "-------------------------------\n";
    std::cout << "            ENIGMA:\n";
    std::cout << "-------------------------------\n";
    std::cout << "1. Xifrar missatge\n";
    std::cout << "2. Desxifrar missatge\n";
    std::cout << "3. Editar rotors\n";
    std::cout << "4. Sortir\n";
    std::cout << "---------------------------------\n";
}

int selectOption() {
    int option;
    std::cout << "Selecciona una opcio: ";
    std::cin >> option;
    return option;
}

void menu() {
    int option;
    do {
        printMenu();
        option = selectOption();

        switch (option) {
        case 1:
            if (llegirIMagatzemarMissatge("Missatge.txt")) {
                xifrarMissatge();
            }
            break;
        case 2: desxifrarMissatge(); break;
        case 3: editarRotor(); break;
        case 4: std::cout << "Sortint...\n"; break;
        default: std::cout << "Opcio invalida.\n"; break;
        }
    } while (option != 4);
}
