#include "Personaje.h"
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "Plataforma.h"

Personaje::Personaje() {
	
	//CARGO TEXTURA
	tp.loadFromFile("Imagenes/corriendo.png");
	setScale(0.6,0.6);
	setTexture(tp);
	tp.setSmooth(true);
	saltando=false;
	saltar=true;
	tamanio_personaje=Vector2f(187,210);
	origen.x=0;
	origen.y=0;
	tiempo_max=100;
	gravedad=10;
	vivo=true;
}

void Personaje::Activar_caida(bool cayendo){
	caer=cayendo;
}

void Personaje::Actualizar_estado_de_salto(bool saltar_, bool saltando_){
	saltar=saltar_;
	saltando=saltando_;
}

void Personaje::Gravedad_personaje(){
	if(caer){
		move(0,gravedad);
	}
}	

void Personaje::Carga_coordenadas_personaje(){
	arriba = getPosition().y;
	abajo = getPosition().y+tamanio_personaje.y*0.6;
	izquierda = getPosition().x;
	derecha = getPosition().x+tamanio_personaje.x*0.6;
}	

void Personaje::Actualizar_personaje(){
	Carga_coordenadas_personaje();
}

void Personaje::Recortar_personaje(){ //RECORTO EL SPRITE
	setTextureRect(IntRect(origen.x*tamanio_personaje.x,origen.y*tamanio_personaje.y,tamanio_personaje.x,tamanio_personaje.y));
}

void Personaje::Control_de_fotogramas(){ //CONTROL USANDO RELOJ
	tiempo_personaje = reloj_personaje.getElapsedTime();
	if(tiempo_personaje.asMilliseconds() >= tiempo_max){ //tiempo_max : VARIABLE QUE MIDE LA VELOCIDAD DE CAMBIO
		origen.x++;
		if(origen.x>5 && origen.y==0){
		origen.x=0;
		origen.y=1;
		}
		if (origen.x>3 && origen.y==1){
			origen.x=0;
			origen.y=0;
		}
	reloj_personaje.restart();
	}
}

void Personaje::Cambiar_valores_origen(float x_, float y_){
	origen.x=x_;
	origen.y=y_;
}

void Personaje::Cambiar_tiempo_fotograma(int aux){ //TIEMPO ENTRE CADA FOTOGRAMA
	tiempo_max = aux;
}

void Personaje::Calcular_max_altura_salto(float limite){ //SE CALCULA MAX ALTURA PARTIENDO DESDE SU CABEZA
	max_altura_salto = arriba - limite;
}

void Personaje::Cambiar_estado_vivo(bool v){
	vivo = v;
}

void Personaje::Actualizar_saltando(bool s){
	saltando = s;
}

void Personaje::Actualizar_saltar(bool s){
	saltar = s;
}

bool Personaje::Ver_saltando(){return saltando;}
bool Personaje::Ver_saltar(){return saltar;}
float Personaje::Ver_arriba_personaje(){return arriba;}
float Personaje::Ver_abajo_personaje(){return abajo;}
float Personaje::Ver_izquierda_personaje(){return izquierda;}
float Personaje::Ver_derecha_personaje(){return derecha;}
float Personaje::Ver_tamanio_personaje_x(){return tamanio_personaje.x;}
float Personaje::Ver_tamanio_personaje_y(){return tamanio_personaje.y;}
float Personaje::Ver_max_altura_salto(){return max_altura_salto;}
bool Personaje::Esta_Vivo(){return vivo;}

Personaje::~Personaje() {}

