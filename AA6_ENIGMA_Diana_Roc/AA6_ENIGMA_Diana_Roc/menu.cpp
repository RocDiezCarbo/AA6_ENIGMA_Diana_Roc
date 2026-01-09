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

int selectOption() {
    std::cout << "Seleccione una opcio: ";
    std::cin >> option;
    
    switch (option)
    {
    case 1:
        std::cout << "1";
        break;
    case 2:
        std::cout << "2";
        break;
    case 3:
        std::cout << "3";
        break;
    case 4:
        std::cout << "4";
        return 0;
    default:
        break;
    }
}

void menu() {
    printMenu();
    selectOption();
}