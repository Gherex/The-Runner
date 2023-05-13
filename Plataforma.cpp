#include "Plataforma.h"
#include <cstdlib>
#include <ctime>
using namespace std;

Plataforma::Plataforma(){
	Actualizar_plataformas();
	velocidad=-3;
}

void Plataforma::Actualizar_plataformas(){
	arriba_plat = getPosition().y;
	abajo_plat = getPosition().y + getSize().y;
	izquierda_plat = getPosition().x;
	derecha_plat = getPosition().x + getSize().x;
}

void Plataforma::Cargar_tamanio(Vector2f &tamanio_){
	tamanio=tamanio_;
	setSize(tamanio);
}

void Plataforma::Cargar_posicion(Vector2f posicion_){
	posicion=posicion_;
	setPosition(posicion);
}

Vector2f Plataforma::Ver_tamanio(){return tamanio;}
Vector2f Plataforma::Ver_posicion(){return posicion;}
float Plataforma::Ver_velocidad(){return velocidad;}
float Plataforma::Ver_arriba_plataforma(){return arriba_plat;}
float Plataforma::Ver_abajo_plataforma(){return abajo_plat;}
float Plataforma::Ver_izquierda_plataforma(){return izquierda_plat;}
float Plataforma::Ver_derecha_plataforma(){return derecha_plat;}

Plataforma::~Plataforma() {
	
}

