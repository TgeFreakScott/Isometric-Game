
// @authors:  David Scott	 - C00208031
//			  Nikita Mullins - C00
// @date started: 
// @date finished:


// simple game loop for SFML[2.4.2]


#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#pragma comment(lib,"thor-d.lib")
#pragma comment(lib,"sfeMovie-d.lib")
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#pragma comment(lib,"thor.lib")
#pragma comment(lib,"sfeMovie.lib")
#endif 

#pragma comment(lib,"libyaml-cppmdd") 

#define TINYC2_IMPLEMENTATION
#include "Game.h"

// main enrtry point

int main()
{
	Game game;
	game.run();

	return 1;
}