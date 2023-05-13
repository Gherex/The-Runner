#include "Fin.h"
#include <fstream>
#include "Jugador.h"
#include "InputBox.h"
#include <sstream>
#include <vector>
#include <SFML/System/String.hpp>
#include <iterator>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iostream>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <ios>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <list>
#include <iomanip>
using namespace std;
using namespace sf;

Fin::Fin(RenderWindow &w){
	ventana_fin=&w;
	
	tff.loadFromFile("Imagenes/fin_juego.png");
	fondo_fin.setTexture(tff); //tff : TEXTURA FONDO FIN
	
	abierto=true;
	
	fuente.loadFromFile("Datos/arial.ttf");
	
	//MUESTRO CONFIRMACION
	confirmacion.setPosition(Vector2f(20,50));
	confirmacion.setColor(Color::Black);
	confirmacion.setFont(fuente);
	confirmacion.setCharacterSize(50);
	confirmacion.setStyle(Text::Bold);
	confirmacion.setString("Enter para guardar puntuacion!");
	
	//MUESTRO NOMBRE INGRESADO
	nombre.setPosition(Vector2f(45,230));
	nombre.setColor(Color::Blue);
	nombre.setFont(fuente);
	nombre.setCharacterSize(30);
	nombre.setStyle(Text::Bold);
	nombre.setString("Nombre: "+nombre_ingresado_str);
	
	//MUESTRO PUNTAJE
	puntos.setPosition(Vector2f(45,300));
	puntos.setColor(Color::Blue);
	puntos.setFont(fuente);
	puntos.setCharacterSize(30);
	puntos.setStyle(Text::Bold);
	puntos.setString("Puntaje: ");
}

void Fin::Bucle_fin(int puntuacion){
	
	abierto=true;
	puntaje=puntuacion;
	
	InputText text_entrada(fuente,30,Color::White);
	text_entrada.setPosition(Vector2f(170,230));
	
	stringstream ss_aux; //LO UTILIZO PARA CONVERTIR LA PUNTUACION EN UN STRING
	ss_aux<<puntaje;
	String aux = "Puntaje:  " + ss_aux.str();
	puntos.setString(aux); // puntos QUEDARIA: "Puntaje: XXXX"
	
	while (abierto){ //BUCLE FIN
		Event e;
		while(ventana_fin->pollEvent(e)) {
			
			if(e.type == Event::Closed) {ventana_fin->close(); exit(0);}
			
			if (e.type==sf::Event::KeyPressed && e.key.code==sf::Keyboard::Return) {
				nombre_ingresado_str=text_entrada.getValue();
				text_entrada.setString(nombre_ingresado_str);
				Pasar_nombre_a_char();//CONVIERTO EL NOMBRE INGRESADO DE STRING A CHAR[10]
				Procesos_fin();	
				abierto=false;
			} else text_entrada.processEvent(e); 
		}
		
	ventana_fin->clear(Color::Black);
	text_entrada.update();
	ventana_fin->draw(fondo_fin);
	ventana_fin->draw(text_entrada);
	ventana_fin->draw(nombre); //MUESTRO "Nombre: AAAAA"
	ventana_fin->draw(puntos); //MUESTRO "Puntaje: XXXX"
	ventana_fin->draw(confirmacion); //MUESTRO "Enter para guardar puntuacion!"
		
	ventana_fin->display();
	}
}	

void Fin::Pasar_nombre_a_char(){
	if (nombre_ingresado_str.size()<10){ //SI EL NOMBRE TIENE MENOS DE 10 LETRAS RELLENO CON ESPACIOS
		for(int i=nombre_ingresado_str.size();i<10;i++){nombre_ingresado[i]=' ';}
	}
	for (int i=0;i<10;i++){nombre_ingresado[i] = nombre_ingresado_str[i];} //SI TIENE MAS DE DIEZ, PASA LAS PRIMERAS 10
}

void Fin::Procesos_fin(){
	Cargar_Puntos(); //PASA LOS DATOS DEL ARCHIVO A MI LISTA
	Jugador nuevo(nombre_ingresado,puntaje); //PASO LOS DATOS DE NOMBRE-PUNTUACION A LA CLASE JUGADOR
	Ordenar_lista(nuevo); //AGREGO EL NUEVO JUGADOR, SI LA LISTA SUPERA LOS 5 JUGADORES, ELIMINA EL MENOR
	Guardar_Puntos(); //PASA LOS DATOS DE LA LISTA AL ARCHIVO BINARIO
}

void Fin::Cargar_Puntos(){ //CARGA LOS PUNTOS ALMACENADOS EN EL ARCHIVO BINARIO EN MI LISTA_DE_JUGADORES DE LA CLASE FIN
	
	fstream archi_binario("Datos/rank.dat",ios::in|ios::binary|ios::ate); //ABRO EL ARCHIVO Y ME POSICIONO AL FINAL DEL MISMO
	
	char nombre_[10];
	int num;
	
	//COMPRUEBO QUE EL ARCHIVO SE ABRIO CORRECTAMENTE
	if (!archi_binario.is_open()) {
		cout<<endl<<"No se pudo abrir el archivo en funcion Cargar_puntos()"<<endl;
	}
	
	//CALCULO LARGO DE FICHERO
	int largo = (archi_binario.tellp() / (sizeof(nombre_) + sizeof(num))); //
	
	//VUELVO AL COMIENZO PARA COMENZAR A LEER DATOS
	archi_binario.seekg(0);
	
	for(int i=0;i<largo;i++) { 
		archi_binario.read((char*)&nombre_, sizeof(nombre_));
		archi_binario.read((char*)&num, sizeof(num));
		Jugador aux(nombre_,num); //PASO DATOS A CLASE JUGADOR
		lista_de_jugadores.push_back(aux); //AGREGO EL JUGADOR A LA LISTA
	}
	archi_binario.close();
}

void Fin::Ordenar_lista(Jugador nuevo){
	//INSERTO NUEVO JUGADOR, (ME QUEDA UNA LISTA DE 6 JUGADORES)
	lista_de_jugadores.push_back(nuevo);
	//BORRO UN JUGADOR DE LA LISTA CUANDO LA MISMA SUPERA LOS 5 JUGADORES
	int c= lista_de_jugadores.size();
	if (c>=5){
		Eliminar_menor_de_lista();//BUSCO EL MENOR Y LO ELIMINO
	}
}

void Fin::Eliminar_menor_de_lista(){ //BUSCO EL MENOR Y LO ELIMINO
	
	list<Jugador>::iterator it, menor;
	
	menor = lista_de_jugadores.begin();
	it = menor;
	++it;
	
	while(it!=lista_de_jugadores.end()){ //RECORRO LA LISTA CON ITERADORES HASTA QUE IT== END()
		if((*it).puntaje<(*menor).puntaje){menor=it;}
		++it;
	}
	lista_de_jugadores.erase(menor);
}

void Fin::Guardar_Puntos(){ //PASO DATOS DE LA LISTA AL ARCHIVO BINARIO
	
	fstream archi_binario("Datos/rank.dat",ios::binary|ios::out|ios::trunc);
	
	//COMPRUEBO QUE EL ARCHIVO SE ABRIO CORRECTAMENTE
	if (!archi_binario.is_open()) {
		cout<<endl<<"No se pudo abrir el archivo en funcion Guardar_Puntos()"<<endl;
	}
	
	list<Jugador>::iterator it;
	list<Jugador>::iterator mayor;
	
	while(!lista_de_jugadores.empty()){ //CORTA EL WHILE CUANDO LA LISTA QUEDA VACIA
		
		mayor = lista_de_jugadores.begin();
		it = mayor;
		++it;
	
		while(it!=lista_de_jugadores.end()){
			if((*it).puntaje>(*mayor).puntaje){mayor=it;}
			++it;
		}
		
		//PASO DATOS DE LA LISTA AL ARCHIVO BINARIO
		archi_binario.write((char*)&(*mayor).nombre, sizeof((*mayor).nombre));
		archi_binario.write((char*)&(*mayor).puntaje, sizeof((*mayor).puntaje));
		
		lista_de_jugadores.erase(mayor); //PASO EL MAYOR Y LO ELIMINO DE LA LISTA
	}
	archi_binario.close();
}

Fin::~Fin() {
}

