#ifndef PERSONAJE_H
#define PERSONAJE_H
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
using namespace std;
using namespace sf;

class Personaje : public Sprite{
	Texture tp; //tp: textura personaje
	float arriba,abajo,izquierda,derecha; //POSICION DEL PERSONAJE
	bool saltar, saltando, caer;
	Vector2f origen, tamanio_personaje;
	Clock reloj_personaje;
	Time tiempo_personaje;
	int vida, gravedad, tiempo_max;
	float max_altura_salto;
	bool vivo;
public:
	Personaje();
	void Control_salto(bool saltar,bool saltando);
	void Gravedad_personaje();
	void Carga_coordenadas_personaje();
	void Actualizar_personaje();
	void Recortar_personaje();
	void Control_de_fotogramas();
	void Cambiar_tiempo_fotograma(int aux);
	void Activar_caida(bool cayendo);
	float Ver_arriba_personaje();
	float Ver_abajo_personaje();
	float Ver_izquierda_personaje();
	float Ver_derecha_personaje();
	bool Ver_saltar();
	bool Ver_saltando();
	bool Esta_Vivo();
	void Cambiar_estado_vivo(bool v);
	float Ver_tamanio_personaje_x();
	float Ver_tamanio_personaje_y();
	void Actualizar_estado_de_salto(bool saltar_, bool saltando_);
	void Calcular_max_altura_salto(float limite);
	float Ver_max_altura_salto();
	void Cambiar_valores_origen(float x_, float y_);
	void Actualizar_saltando(bool s);
	void Actualizar_saltar(bool s);
	void Actualizar_doble_salto(bool s);
	~Personaje();
};

#endif
