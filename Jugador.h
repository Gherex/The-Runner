#ifndef JUGADOR_H
#define JUGADOR_H

class Jugador{
public:
	char nombre[10];
	int puntaje;
	Jugador(char *n_, int puntaje_);
	~Jugador();
};

#endif
