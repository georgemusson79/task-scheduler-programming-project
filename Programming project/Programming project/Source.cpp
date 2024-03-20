//source.cpp
#include <iostream>
#include <SDL.h>
#include <thread>
#include "Main_Functions.h"
#include "Debug.h"
#include "Vector2.h"
#include "renderable.h"
#include "Cursor.h"
#include "Collision.h"
int temp() {
	std::cout << "yipee";
	return 3;
}

int main(int argc, char** argv) {

	
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	//Cursor::setCursor(SDL_SYSTEM_CURSOR_HAND);
	std::cout << SDL_GetError() << "\n";
	Vector2 scrDims=Main::getDisplayDims();
	Main::window = SDL_CreateWindow("Task Scheduler", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, scrDims.x/2, scrDims.y/2, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
	Main::renderer = SDL_CreateRenderer(Main::window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	Renderable::create<Draggable>(10, 10, 50, 50, "C:/Screenshot (51).png",ONLY_X);
	SDL_Event e;
	while (Main::running) {
		Cursor::hasCursorChanged = false;
		Main::updateRenderables();
		Main::handleEvents(e);
		Debug::handleFPS();
	}
	return 0;
}