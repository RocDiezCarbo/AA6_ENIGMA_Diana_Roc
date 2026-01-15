#pragma once

const int NUM_ROTORS = 3;

// Carrega un sol rotor des d'un fitxer (per ex. "rotor_I.txt") a l'índex idxRotor (0,1,2)
bool carregarRotor(const char* nomFitxer, int idxRotor);

// Carrega el reflector des d'un fitxer (per ex. "reflector.txt")
bool carregarReflector(const char* nomFitxer);

int getRotor(int rotor, int index);
int getRotorNotch(int rotor);
int getReflector(int index);
