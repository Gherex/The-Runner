#include "Menu.h"
#include "Juego.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Audio/Music.hpp>
#include <fstream>
#include <sstream>
#include <ios>
#include <string>

#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <iomanip>
#include <iostream>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Color.hpp>
using namespace std;
using namespace sf;

Menu::Menu(RenderWindow &w) : juego_(w) {//CONSTRUCTOR
	ventana_menu=&w;
	opcion=0; //SE POSICIONA EN EL BOTON JUGAR
	menu_abierto=true; //CONTROL DE LOOP DE MENU
	salir_=false; //SALE DEL LOOP
	
	//FONDO MENU
	tfm.loadFromFile("Imagenes/fondo_menu.png");
	fondo_menu.setTexture(tfm);
	
	//FONDO RANKING
	trm.loadFromFile("Imagenes/ranking.png");
	rank_menu.setTexture(trm);	
	
	//FONDO INSTRUCCIONES
	tim.loadFromFile("Imagenes/instrucciones.png");
	instruc_menu.setTexture(tim);
	
	//CARGO FUENTE DE TEXTO
	fuente_texto.loadFromFile("Datos/arial.ttf");
	
	//ASIGNO FUENTE
	jugar.setFont(fuente_texto);
	ranking.setFont(fuente_texto);
	instrucciones.setFont(fuente_texto);
	salir.setFont(fuente_texto);
	
	//ASIGNO TEXTO
	jugar.setString("Jugar");
	ranking.setString("Ranking");
	instrucciones.setString("Instrucciones");
	salir.setString("Salir");
	
	//ASIGNO POSICION DE TEXTO
	jugar.setPosition(Vector2f(120,120));
	ranking.setPosition(Vector2f(120,230));
	instrucciones.setPosition(Vector2f(120,340));
	salir.setPosition(Vector2f(120,450));
	
	//ASIGNO TAMAÑO
	jugar.setCharacterSize(50);
	ranking.setCharacterSize(50);
	instrucciones.setCharacterSize(50);
	salir.setCharacterSize(50);
	
	//ASIGNO COLOR AL TEXTO
	jugar.setColor(Color::Black);
	ranking.setColor(Color::Black);
	instrucciones.setColor(Color::Black);
	salir.setColor(Color::Black);
	
	//CARGO MUSICA
	musica_menu = new Music; 
	musica_menu->openFromFile("Musica/musica_menu.ogg");
	
	buffer.loadFromFile("Musica/sonido_selector.wav");
	sonido_selector.setBuffer(buffer);
}

void Menu::Bucle_menu(){
	
	musica_menu->play();
	musica_menu->setLoop(true);
	
	while(menu_abierto){ 
		
		Event e;
		while(ventana_menu->pollEvent(e)) {
			if(e.type == Event::Closed) {
				musica_menu->stop();
				delete musica_menu;
				ventana_menu->close();
				exit(0);
			}
		}
		Procesar_eventos_menu();
		if (salir_){ //SI EL JUGADOR PRESIONA EL BOTON SALIR
			return ;
		}
		Dibujar_menu();
	}
}


void Menu::Procesar_eventos_menu(){
	
	Text *text_; //PUNTERO A TEXTO
	tiempo=reloj.getElapsedTime();
	text_=&jugar;//PARA QUE INICIALMENTE EL PUNTERO APUNTE A JUGAR
	Seleccionar_opcion(text_); //SUBRAYA LA PALABRA Y AUMENTA DE TAMAÑO
	
	if(Keyboard::isKeyPressed(Keyboard::Up)&&tiempo.asSeconds()>=0.15){
		opcion-=1;
		sonido_selector.play();
		if (opcion<=0){opcion=0;} //PARA QUE NO SE PASE DE BOTON JUGAR
		reloj.restart();
	}

	if(Keyboard::isKeyPressed(Keyboard::Down)&&tiempo.asSeconds()>=0.15){
		opcion+=1;
		sonido_selector.play();
		if (opcion>=4){opcion=3;} //PARA QUE NO SE PASE DE BOTON SALIR
		reloj.restart();
	}
	
	switch(opcion){
	case 0:
		Seleccionar_opcion(&jugar); //SUBRAYO UNA Y DESMARCO LAS DEMAS
		Desenmarcar_opcion(&ranking);
		Desenmarcar_opcion(&instrucciones);
		Desenmarcar_opcion(&salir);
		break;
	case 1:
		Seleccionar_opcion(&ranking);
		Desenmarcar_opcion(&jugar);
		Desenmarcar_opcion(&instrucciones);
		Desenmarcar_opcion(&salir);
		break;
	case 2:
		Seleccionar_opcion(&instrucciones);
		Desenmarcar_opcion(&ranking);
		Desenmarcar_opcion(&jugar);
		Desenmarcar_opcion(&salir);
		break;
	case 3:
		Seleccionar_opcion(&salir);
		Desenmarcar_opcion(&instrucciones);
		Desenmarcar_opcion(&jugar);
		Desenmarcar_opcion(&ranking);
		break;
	}

	if(Keyboard::isKeyPressed(Keyboard::Return)){ //SI PRESIONA ENTER 
		switch(opcion){
		case 0: 
			musica_menu->stop();
			juego_.Go();
			musica_menu->play();
		case 1:	
			Ver_Ranking_menu();
			break;
		case 2:	
			Ver_Instrucciones_menu();
			break;
		case 3:	
			salir_=true;
			musica_menu->stop();
			ventana_menu->close();
			delete musica_menu;
			break;
			
		}
	}
}

void Menu::Dibujar_menu(){
	ventana_menu->clear();
	ventana_menu->draw(fondo_menu);
	ventana_menu->draw(jugar);
	ventana_menu->draw(ranking);
	ventana_menu->draw(instrucciones);
	ventana_menu->draw(salir);
	ventana_menu->display();
}

void Menu::Ver_Ranking_menu(){
	
	ventana_menu->clear();
	ventana_menu->draw(rank_menu);
	
	fstream archi("Datos/rank.dat",ios::binary|ios::in|ios::out); 
	
	//COMPRUEBO QUE EL ARCHIVO SE ABRIO CORRECTAMENTE
	if (!archi.is_open()) {
		cout<<endl<<"No se pudo abrir el archivo en funcion Cargar_puntos()"<<endl;
	}
	
	Text text_rank;
	text_rank.setFont(fuente_texto);
	text_rank.setColor(Color::Black);
	text_rank.setStyle(Text::Bold); //NEGRITA
	
	char nombre[10];
	stringstream *ss_aux; //STRINGSTREAM AUXILIAR UTILIZADO PARA ALMACENAR LOS PUNTOS DE CADA JUGADOR
	string puntaje, jugador, texto_de_ranking;
	int puntos, longitud_rank, posicion_y=180;
	
	texto_de_ranking="TOP 5";
	text_rank.setString(texto_de_ranking);
	text_rank.setPosition(Vector2f(330,50));
	text_rank.setCharacterSize(50);
	ventana_menu->draw(text_rank);//DIBUJA TITULO EN PANTALLA
	
	texto_de_ranking="Pulsa Backspace para volver al menu.";
	text_rank.setString(texto_de_ranking);
	text_rank.setPosition(Vector2f(140,110));
	text_rank.setCharacterSize(25);
	ventana_menu->draw(text_rank);
	
	texto_de_ranking="Pulsa F11 para reiniciar el rank.";
	text_rank.setString(texto_de_ranking);
	text_rank.setPosition(Vector2f(150,500));
	ventana_menu->draw(text_rank);
	
	text_rank.setColor(Color::White);
	texto_de_ranking="Nombres:";
	text_rank.setString(texto_de_ranking);
	text_rank.setPosition(Vector2f(230,160));
	ventana_menu->draw(text_rank);
	
	texto_de_ranking="Puntajes:";
	text_rank.setString(texto_de_ranking);
	text_rank.setPosition(Vector2f(480,160));
	ventana_menu->draw(text_rank);
	
	//OBTENGO LONGITUD DEL ARCHIVO
	archi.seekg(0,ios::end); //ME PARO AL FINAL
	longitud_rank=(archi.tellg()/(sizeof(nombre)+sizeof(puntos))); //CALCULO LONGITUD DEL ARCHIVO
	archi.seekg(0,ios::beg); //ME PARO AL COMIENZO
	
	//CAMBIO DE COLOR Y TAMAÑO PARA NOMBRES Y PUNTAJES QUE CONTIENE EL ARCHIVO BINARIO
	text_rank.setColor(Color::Blue);
	text_rank.setCharacterSize(24);
	
	for(int i=0;i<longitud_rank;i++){
		
		ss_aux = new stringstream;
		
		archi.read((char*)&nombre, sizeof(nombre)); //LEO EL NOMBRE UBICADO EN EL ARCHIVO BINARIO
		archi.read((char*)&puntos, sizeof(puntos)); //LEO EL PUNTAJE UBICADO EN EL ARCHIVO BINARIO
		if(!puntos==0){
			*ss_aux<<puntos;
			puntaje = ss_aux->str(); //CONVIERTO EL PUNTAJE EN UN STRING
			jugador = nombre;
			
			posicion_y+=50; //ESPACIO ENTRE JUGADORES MOSTRADOS
			
			//MUESTRO LOS NOMBRES
			text_rank.setString(jugador);
			text_rank.setPosition(Vector2f(230,posicion_y));
			ventana_menu->draw(text_rank);
			
			//MUESTRO LOS PUNTAJES
			text_rank.setString(puntaje);
			text_rank.setPosition(Vector2f(480,posicion_y));
			ventana_menu->draw(text_rank);
			delete ss_aux;
		}
	}
	ventana_menu->display();
	
	while(!Keyboard::isKeyPressed(Keyboard::BackSpace)){
		Event e;
		while(ventana_menu->pollEvent(e)) {
			if(e.type == Event::Closed){ventana_menu->close();}
		}
		if(Keyboard::isKeyPressed(Keyboard::F11)){
			char nom_nuevo[10]="         "; //NOMBRE VACIO
			int score_nuevo=0; //PUNTUACION CERO
			archi.seekg(0,ios::beg); //ME PARO AL COMIENZO
			
			for(int i=0;i<5;i++){
				
				archi.write((char*)&nom_nuevo, sizeof(nom_nuevo));
				archi.write((char*)&score_nuevo, sizeof(score_nuevo));
			}
			break;
		}
	}
	archi.close();
}
	
void Menu::Ver_Instrucciones_menu(){
	
	ventana_menu->clear();
	ventana_menu->draw(instruc_menu);
	ventana_menu->display();
	
	while(!Keyboard::isKeyPressed(Keyboard::BackSpace)){
		Event e;
		while(ventana_menu->pollEvent(e)) {
			if(e.type == Event::Closed){ventana_menu->close();}
		}
	}
}

void Menu::Seleccionar_opcion(Text *t){
	t->setStyle(Text::Underlined); //SUBRAYAR PALABRA
	t->setCharacterSize(60);
}

void Menu::Desenmarcar_opcion(Text *t){
	t->setStyle(Text::Bold);
	t->setCharacterSize(50);
}

Menu::~Menu() {
	
}

