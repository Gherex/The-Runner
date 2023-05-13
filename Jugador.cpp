#include "Jugador.h"
#include <cstring>
using namespace std;

Jugador::Jugador(char *n_, int puntaje_) {
	strcpy(nombre,n_);
	puntaje=puntaje_;
}

Jugador::~Jugador() {
	
}

