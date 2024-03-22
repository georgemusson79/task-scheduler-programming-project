//source.cpp
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
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
	Cursor::setCursor(SDL_SYSTEM_CURSOR_HAND);
	TTF_Init();

	Vector2 scrDims=Main::getDisplayDims();
	Main::window = SDL_CreateWindow("Task Scheduler", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, scrDims.x/2, scrDims.y/2, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
	Main::renderer = (Debug::vsync) ? SDL_CreateRenderer(Main::window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) : SDL_CreateRenderer(Main::window, -1, SDL_RENDERER_ACCELERATED);
	//for (int i = 0; i < 4; i++) Renderable::create<Button>(10, i*50, 50, 50, "C:/Screenshot (24).png", temp);
	//for (int i=0; i<4; i++) Renderable::create<Draggable>(10, 10, 50, 50, "C:/Screenshot (51).png",ONLY_X);
	Label* l=Renderable::create<Label>(50, 50, 300, 100, SDL_Color(255, 255, 255),SDL_Color(255,0,0),5);
	l->setText("abc123");
	SDL_Event e;
	while (Main::running) {
		Main::updateRenderables();
		Main::handleEvents(e);
		Debug::handleFPS();
	}
	return 0;
}