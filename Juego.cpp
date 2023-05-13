#include "Juego.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Personaje.h"
#include <fstream>
#include "Plataforma.h"
#include <iostream>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/View.hpp>
#include <iomanip>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <cstdlib>
#include <SFML/Window/Keyboard.hpp>
#include "Fin.h"
#include <SFML/System/Time.hpp>
#include <ios>
using namespace std;
using namespace sf;

Juego::Juego(RenderWindow &w) : fin_(w) {
	ventana=&w;
	
	tfj.loadFromFile("Imagenes/fondo_juego.jpg");
	fondo_juego.setTexture(tfj); //tfj : TEXTURA FONDO JUEGO
	
	tfgo.loadFromFile("Imagenes/fondo_game_over.png");
	fondo_game_over.setTexture(tfgo); //tfgo : TEXTURA FONDO GAME OVER
	
	Cargar_plataformas_desde_memoria();
	tp.loadFromFile("Imagenes/textura_madera.jpg");
	for (int i=0;i<2;i++){
		plat_nivel_0[i].setTexture(&tp); //tp : TEXTURA PLATAFORMA
		plat_nivel_1[i].setTexture(&tp);
		plat_nivel_2[i].setTexture(&tp);
	}
	
	//ASIGNO ALTURA DE CADA NIVEL
	nivel_0=(Vector2f(1060,530));
	nivel_1=(Vector2f(1060,360));
	nivel_2=(Vector2f(1060,190));
	
	buffer.loadFromFile("Musica/grito_caida.wav");
	sonido_muerte.setBuffer(buffer);
	
	musica_juego = new Music;
	musica_juego->openFromFile("Musica/musica_juego.ogg");
}

void Juego::Go(){
	
	p.setPosition(100,320);//UBICO AL PERSONAJE
	Cargar_plataformas_iniciales(); //SIEMPRE QUE COMIENZA UN NUEVO JUEGO, COMIENZA CON LAS MISMAS PLATAFORMAS 
	Ubicar_plataformas_iniciales(); 
	
	musica_juego->play();
	musica_juego->setLoop(true);
	reloj_juego.restart();
	
	while(p.Esta_Vivo()) {
		//CREO EVENTO
		Event e;
		//PROCESAMIENTO DE EVENTO
		while(ventana->pollEvent(e)) {
			if(e.type == Event::Closed){
				musica_juego->stop();
				delete musica_juego;
				ventana->close();
				exit(0);
			}
		}
		Actualizar_Juego(); //ACTUALIZA POSICION DE PERSONAJE Y PLATAFORMAS 
		Procesar_Eventos(); //CONTROL DE SALTO, GRAVEDAD, MOVIMIENTOS Y CAIDAS
		Procesar_Colisiones(); //COMPRUEBA SI EL PERSONAJE CAE AL VACIO O ESTA SOBRE PLATAFORMA
		Dibujar_Juego();
	}
	if(!p.Esta_Vivo()) {
		musica_juego->stop();
		Perdio(); //MUESTRA PANTALLA GAME_OVER Y LLAMA A CLASE FIN PARA INGRESAR PUNTUACION
		p.Cambiar_estado_vivo(true); //ACTUALIZO ESTADO DE PERSONAJE. PERMITE PODER VOLVER A JUGAR DESDE EL MENU INICIO
	}
}

void Juego::Cargar_plataformas_desde_memoria(){ //CARGO LOS TAMAÑOS DE CADA PLATAFORMA DESDE UN ARCH. DE TEXTO
	int i=0;
	float tam_x,tam_y;
	fstream archi_plat("Datos/Plataformas.txt",ios::in); //ARCHIVO DE TEXTO CON LOS TAM EN (X,Y) DE CADA PLATAFORMA
	while(archi_plat>>tam_x>>tam_y){
		switch (i){
			case 0:
				plat_larga=(Vector2f(tam_x,tam_y));
			case 1:
				plat_mediana=(Vector2f(tam_x,tam_y));
			case 2:
				plat_corta=(Vector2f(tam_x,tam_y));
				break;
		}
	++i;
	}
	archi_plat.close();
}

void Juego::Asigno_nueva_plataforma(Plataforma &aux){ //ASIGNO NUEVOS TAMAÑOS DE PLATAFORMAS EN FORMA ALEATORIA
	int n = rand()%5;
	switch(n){
	case 0:
		aux.Cargar_tamanio(plat_corta);
		break;
	case 1:
		aux.Cargar_tamanio(plat_mediana);
		break;
	case 2:
		aux.Cargar_tamanio(plat_corta);
		break;
	case 3:
		aux.Cargar_tamanio(plat_mediana);
		break;
	case 4:
		aux.Cargar_tamanio(plat_larga);
		break;
	}	
}

void Juego::Generador_de_plataformas(){ //CUANDO LA PLATAFORMA LLEGA A UN EXTREMO DE LA PANTALLA VUELVE A POSICIONARSE EN EL OTRO EXTREMO
	if (plat_nivel_0[0].Ver_izquierda_plataforma() <= -270){
		Asigno_nueva_plataforma(plat_nivel_0[0]); //ASIGNO NUEVO TAMAÑO A DICHA PLATAFORMA
		plat_nivel_0[0].setPosition(nivel_0);
	}
	if (plat_nivel_0[1].Ver_izquierda_plataforma() <= -270){
		Asigno_nueva_plataforma(plat_nivel_0[1]);
		plat_nivel_0[1].setPosition(nivel_0);
	}
	if (plat_nivel_1[0].Ver_izquierda_plataforma() <= -270){
		Asigno_nueva_plataforma(plat_nivel_1[0]);
		plat_nivel_1[0].setPosition(nivel_1);
	}
	if (plat_nivel_1[1].Ver_izquierda_plataforma() <= -270){
		Asigno_nueva_plataforma(plat_nivel_1[1]);
		plat_nivel_1[1].setPosition(nivel_1);
	}
	if (plat_nivel_2[0].Ver_izquierda_plataforma() <= -270){
		Asigno_nueva_plataforma(plat_nivel_2[0]);
		plat_nivel_2[0].setPosition(nivel_2);
	}
	if (plat_nivel_2[1].Ver_izquierda_plataforma() <= -270){
		Asigno_nueva_plataforma(plat_nivel_2[1]);
		plat_nivel_2[1].setPosition(nivel_2);
	}
}

void Juego::Movimiento_de_plataformas(){ //LAS MUEVE A VELOCIDAD CONSTANTE
	plat_nivel_0[0].move(plat_nivel_0[0].Ver_velocidad(),0);
	plat_nivel_0[1].move(plat_nivel_0[1].Ver_velocidad(),0);
	plat_nivel_1[0].move(plat_nivel_1[0].Ver_velocidad(),0);
	plat_nivel_1[1].move(plat_nivel_1[1].Ver_velocidad(),0);
	plat_nivel_2[0].move(plat_nivel_2[0].Ver_velocidad(),0);
	plat_nivel_2[1].move(plat_nivel_2[1].Ver_velocidad(),0);
}

void Juego::Cargar_plataformas_iniciales(){//CARGO TAMAÑO Y POSICION DE LAS PRIMERAS 6 PLATAFORMAS
	 
	plat_nivel_0[0].Cargar_tamanio(plat_larga);
	plat_nivel_0[1].Cargar_tamanio(plat_mediana);
	plat_nivel_1[0].Cargar_tamanio(plat_mediana);
	plat_nivel_1[1].Cargar_tamanio(plat_corta);
	plat_nivel_2[0].Cargar_tamanio(plat_corta);
	plat_nivel_2[1].Cargar_tamanio(plat_mediana);
	
	plat_nivel_0[0].Cargar_posicion(Vector2f(50,530));
	plat_nivel_0[1].Cargar_posicion(Vector2f(580,530));
	plat_nivel_1[0].Cargar_posicion(Vector2f(350,360));
	plat_nivel_1[1].Cargar_posicion(Vector2f(780,360));
	plat_nivel_2[0].Cargar_posicion(Vector2f(550,190));
	plat_nivel_2[1].Cargar_posicion(Vector2f(970,190));
}

void Juego::Ubicar_plataformas_iniciales(){//UBICO LAS PRIMERAS 6 PLATAFORMAS
	
	plat_nivel_0[0].setPosition(plat_nivel_0[0].Ver_posicion());
	plat_nivel_0[1].setPosition(plat_nivel_0[1].Ver_posicion());
	plat_nivel_1[0].setPosition(plat_nivel_1[0].Ver_posicion());
	plat_nivel_1[1].setPosition(plat_nivel_1[1].Ver_posicion());
	plat_nivel_2[0].setPosition(plat_nivel_2[0].Ver_posicion());
	plat_nivel_2[1].setPosition(plat_nivel_2[1].Ver_posicion());
}


void Juego::Procesar_Eventos(){
	if (!p.Ver_saltando()){
		p.Control_de_fotogramas();
		p.Gravedad_personaje();
	}
	Control_de_salto_personaje();
	Movimiento_de_plataformas();
	Generador_de_plataformas();
}

void Juego::Actualizar_Juego(){
	plat_nivel_0[0].Actualizar_plataformas();
	plat_nivel_0[1].Actualizar_plataformas();
	plat_nivel_1[0].Actualizar_plataformas();
	plat_nivel_1[1].Actualizar_plataformas();
	plat_nivel_2[0].Actualizar_plataformas();
	plat_nivel_2[1].Actualizar_plataformas();
	p.Actualizar_personaje();
}

void Juego::Dibujar_Juego(){
	//LIMPIO PANTALLA
	ventana->clear(Color::Black);
	//DIBUJO EL FONDO
	ventana->draw(fondo_juego);
	//DIBUJO PLATAFORMAS
	ventana->draw(plat_nivel_0[0]);
	ventana->draw(plat_nivel_0[1]);
	ventana->draw(plat_nivel_1[0]);
	ventana->draw(plat_nivel_1[1]);
	ventana->draw(plat_nivel_2[0]);
	ventana->draw(plat_nivel_2[1]);
	//DIBUJO PERSONAJE
	p.Recortar_personaje();	
	ventana->draw(p);
	//MUESTRO EN PANTALLA
	ventana->display();
}

void Juego::Procesar_Colisiones(){
	//SI EL PERSONAJE CAE AL VACIO
	if(p.Ver_arriba_personaje()>600){
		p.Cambiar_estado_vivo(false);
	}
	//SI EL PERSONAJE ESTA SOBRE UNA PLATAFORMA
	Personaje_esta_sobre_plataforma(); //TAMBIEN DEVUELVE UN BOOLIANO
}	

bool Juego::Comprobar_colision_inferior(Plataforma aux){ //COMPRUEBA QUE EL PERSONAJE ESTE ARRIBA (APOYADO) SOBRE PLATAFORMA
	if (p.Ver_abajo_personaje() >= aux.Ver_arriba_plataforma() && p.Ver_abajo_personaje() <= (aux.Ver_arriba_plataforma()+30.0)){
		return true;
	}else{return false;}
}

bool Juego::Personaje_coincide_con_plataforma(Plataforma aux){ //COMPRUEBA QUE COINCIDAN LOS PIES DEL PERSONAJE CON LOS EXTREMOS DE LA PLATAFORMA
	if(p.Ver_abajo_personaje() >= aux.Ver_arriba_plataforma()){
		if(p.Ver_derecha_personaje()>=aux.Ver_izquierda_plataforma() && p.Ver_izquierda_personaje()<=aux.Ver_derecha_plataforma()){
			return true;
		}else{
			return false;
		}
	}
	return false;
}

bool Juego::Personaje_esta_sobre_plataforma(){ //DEVUELVE TRUE SI EL PERSONAJE ESTA APOYADO SOBRE PLATAFORMA Y COINCIDA CON LOS EXTREMOS DE LA MISMA
	int c=0;
	if(Comprobar_colision_inferior(plat_nivel_0[0])&&Personaje_coincide_con_plataforma(plat_nivel_0[0])){
		if(!p.Ver_saltando()){
			p.Activar_caida(false);
			++c;
		}
	}else{
		if(Comprobar_colision_inferior(plat_nivel_0[1])&&Personaje_coincide_con_plataforma(plat_nivel_0[1])){
			if(!p.Ver_saltando()){
				p.Activar_caida(false);
				++c;
			}
		}else{
			if(Comprobar_colision_inferior(plat_nivel_1[0])&&Personaje_coincide_con_plataforma(plat_nivel_1[0])){
				if(!p.Ver_saltando()){
					p.Activar_caida(false);
					++c;
				}
			}else{
				if(Comprobar_colision_inferior(plat_nivel_1[1])&&Personaje_coincide_con_plataforma(plat_nivel_1[1])){
					if(!p.Ver_saltando()){
						p.Activar_caida(false);
						++c;
					}
				}else{
					if(Comprobar_colision_inferior(plat_nivel_2[0])&&Personaje_coincide_con_plataforma(plat_nivel_2[0])){
						if(!p.Ver_saltando()){
							p.Activar_caida(false);
							++c;
						}
					}else{
						if(Comprobar_colision_inferior(plat_nivel_2[1])&&Personaje_coincide_con_plataforma(plat_nivel_2[1])){
							if(!p.Ver_saltando()){
								p.Activar_caida(false);
								++c;
							}
						}else{
							p.Activar_caida(true);
						}
					}
				}
			}
		}	
	}	
	if (c>0){return true;}else{return false;} //EL CONTADOR CUENTA LAS VECES QUE EL PERSONAJE SE ENCUENTRA SOBRE ALGUNA PLATAFORMA, SI C>1, DESACTIVA GRAVEDAD
}

void Juego::Control_de_salto_personaje(){

	if(p.Ver_saltar()){ //COMPRUEBA SI PUEDE SALTAR
		if((Keyboard::isKeyPressed(sf::Keyboard::Space))){
			p.Calcular_max_altura_salto(200);//ALTURA MAXIMA A LA QUE EL SALTO PUEDE LLEGAR, LUEGO SE DETIENE
			p.Actualizar_estado_de_salto(false,true); //(SALTAR, SALTANDO)
			p.Cambiar_valores_origen(3,1);//SPRITE DE SALTO
		}
	}
	
	if(p.Ver_saltando()){
		p.move(0,-8); //VELOCIDAD CON LA QUE SUBE EL PERSONAJE
	}	
	
	if(p.Ver_arriba_personaje() < p.Ver_max_altura_salto()){p.Actualizar_saltando(false);}//COMPRUEBA SI LLEGO A LA ALTURA MAXIMA DE SALTO
	
	if (Personaje_esta_sobre_plataforma()){p.Actualizar_saltar(true);}
}

void Juego::Perdio(){
	sonido_muerte.play();
	Time aux;
	aux = reloj_perdio.restart();;
	ventana->clear();
	ventana->draw(fondo_game_over);
	ventana->display();
	while(aux.asSeconds()<3){ //SIRVE PARA QUE ME MUESTRE LA PANTALLA DE GAME OVER POR UN PAR DE SEGUNDOS
		aux = reloj_perdio.getElapsedTime();
	}
	fin_.Bucle_fin(Pasar_tiempo_juego()); //LLAMA A BUCLE FIN, LA CLASE FIN SE ENCARGA DEL MANEJO DE PUNTOS
	musica_juego->stop();
}

int Juego::Pasar_tiempo_juego(){ //EL TIEMPO DE JUEGO ES EQUIVALENTE A LA PUNTUACION OBTENIDA POR EL JUGADOR
	Time aux;
	aux=reloj_juego.getElapsedTime();
	return (aux.asMilliseconds()/10);
}

Juego::~Juego() {
}


