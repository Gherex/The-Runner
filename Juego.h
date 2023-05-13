#ifndef JUEGO_H
#define JUEGO_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include "Personaje.h"
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Plataforma.h"
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include "Fin.h"
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>
using namespace sf;
using namespace std;

class Juego{
private:
	RenderWindow *ventana;
	Personaje p;
	Plataforma plat_nivel_0[2], plat_nivel_1[2], plat_nivel_2[2]; 
	Vector2f plat_corta, plat_mediana, plat_larga;
	Vector2f nivel_0, nivel_1, nivel_2;
	Clock reloj_juego, reloj_perdio;
	Time tiempo_juego;
	Texture tfj,tfgo,tp; // tfj: textura fondo juego, tfgo: textura fondo game over, tp: textura plataforma
	Sprite fondo_juego, fondo_game_over; 
	Fin fin_;
	Sound sonido_muerte;
	SoundBuffer buffer;
	Music *musica_juego;
public:
	Juego(RenderWindow &w);
	void Go();
	void Procesar_Eventos();
	void Cargar_plataformas_iniciales();
	void Ubicar_plataformas_iniciales();
	void Cargar_plataformas_desde_memoria();
	void Asigno_nueva_plataforma(Plataforma &aux);
	void Generador_de_plataformas();
	void Movimiento_de_plataformas();
	bool Comprobar_colision_inferior(Plataforma aux);
	bool Personaje_coincide_con_plataforma(Plataforma aux);
	bool Personaje_esta_sobre_plataforma();
	void Dibujar_Juego();
	void Actualizar_Juego();
	void Procesar_Colisiones();
	void Perdio();
	void Control_de_salto_personaje();
	int Pasar_tiempo_juego();
	~Juego(void);
};

#endif

