#ifndef FIN_H
#define FIN_H
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>
#include "Jugador.h"
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <string>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <list>
using namespace std;
using namespace sf;

class Fin{
	int puntaje;
	RenderWindow *ventana_fin;
	list<Jugador> lista_de_jugadores;
	Font fuente;
	char nombre_ingresado[10];
	string nombre_ingresado_str;
	Text nombre, puntos, confirmacion;
	Sprite fondo_fin;
	Texture tff; //tff: textura fondo fin
	bool abierto;
public:
	Fin(RenderWindow &w);
	void Cargar_Puntos();
	void Guardar_Puntos();
	void Pasar_nombre_a_char();
	void Ordenar_lista(Jugador aux);
	void Eliminar_menor_de_lista();
	void Procesos_fin();
	void Bucle_fin(int puntuacion);
	~Fin();
};

#endif

