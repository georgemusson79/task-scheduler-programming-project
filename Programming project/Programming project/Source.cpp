//source.cpp
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <thread>
#include "Main_Functions.h"
#include "Debug.h"
#include "Vector2.h"
#include "simple_renderables.h"
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
	Draggable* d=Renderable::create<Draggable>(Main::renderer, 50, 70, 100, 100, "C:/Screenshot (51).png");
	Button* b=Renderable::create<Button>(Main::renderer, 100, 100, 50, 50, "C:/Screenshot (24).png", temp);
	TextField* l=Renderable::create<TextField>(Main::renderer,50, 50, 400, 30, SDL_Color(255, 255, 255), SDL_Color(255, 0, 0),40);
	
	Renderable::create<LabelFixedCharDims>(Main::renderer, 0, 200, 50, "obamahamehagih", SDL_Color(255, 255, 255),5);
	SDL_Event e;
	while (Main::running) {

		Main::handleEvents(e);
		Main::updateRenderables();
		Debug::handleKBInput();

		//Debug::handleFPS();
		//std::cout << "frame end\n";
	}
	return 0;
}