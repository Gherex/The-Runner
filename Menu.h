#ifndef MENU_H
#define MENU_H
#include "Juego.h"
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

using namespace std;
using namespace sf;

class Menu {
	RenderWindow *ventana_menu;
	Juego juego_;
	Font fuente_texto;
	Text jugar,ranking,instrucciones,salir;
	int opcion;
	Sprite fondo_menu, instruc_menu, rank_menu;
	Texture tfm,tim,trm; //tf: Textura fondo menu, ti: Textura instrucciones menu, tr: Textura ranking menu
	bool menu_abierto, salir_;
	Clock reloj; 
	Time tiempo;
	Music *musica_menu;
	Sound sonido_selector;
	SoundBuffer buffer;
public:
	Menu(RenderWindow &w);
	void Bucle_menu();
	void Dibujar_menu();
	void Procesar_eventos_menu();
	void Ver_Ranking_menu();
	void Ver_Instrucciones_menu();
	void Seleccionar_opcion(Text *t);
	void Desenmarcar_opcion(Text *t);
	~Menu();
};

#endif
