#ifndef PLATAFORMA_H
#define PLATAFORMA_H
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
using namespace std;
using namespace sf;

class Plataforma : public RectangleShape{
private:
	float arriba_plat, abajo_plat, izquierda_plat, derecha_plat;
	Vector2f posicion;
	Vector2f tamanio;
	float velocidad;
public:
	Plataforma();
	void Cargar_tamanio(Vector2f &tamanio_);
	void Cargar_posicion(Vector2f posicion_);
	float Ver_velocidad();
	float Ver_arriba_plataforma();
	float Ver_abajo_plataforma();
	float Ver_izquierda_plataforma();
	float Ver_derecha_plataforma();
	Vector2f Ver_tamanio();
	Vector2f Ver_posicion();
	void Actualizar_plataformas();
	~Plataforma();
};

#endif

