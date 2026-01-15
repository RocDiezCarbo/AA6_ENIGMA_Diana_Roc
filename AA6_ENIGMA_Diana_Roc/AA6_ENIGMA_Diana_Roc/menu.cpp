#include <iostream>

int option;

void printMenu() {
    std::cout << "-------------------------------\n";
	std::cout << "            ENIGMA:" << "\n";
	std::cout << "-------------------------------\n";
    std::cout << "1. Xifrar missatge\n";
    std::cout << "2. Desxifrar missatge\n";
    std::cout << "3. Editar rotors\n";
    std::cout << "4. Sortir\n";
    std::cout << "---------------------------------\n";
}

int llegirOpcio() {
    int opcio;
    std::cout << "Opcio: ";
    std::cin >> opcio;
    return opcio;
}

void menu() {
    printMenu();
    llegirOpcio();
}