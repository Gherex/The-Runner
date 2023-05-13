#include <SFML/Graphics/RenderWindow.hpp>
#include "Menu.h"
#include <cstdlib>
#include <iostream>
#include <ctime>
using namespace std;

int main(int argc, char *argv[]){
	
	srand(time(0));
	//CREO LA VENTANA
	RenderWindow *w;
	w = new RenderWindow (VideoMode(800,600,32),"The Runner");
	w->setFramerateLimit(60);
	
	//INSTANCIA DE MENU
	Menu *m;

	m = new Menu(*w);
	m->Bucle_menu(); //DENTRO DE LA CLASE MENU SE ENCUENTRA UNA INSTANCIA DE CLASE JUEGO
	
	delete m;
    delete w;
	return 0;
}

