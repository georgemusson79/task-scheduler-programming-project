//source.cpp
#include <iostream>
#include <SDL.h>
#include <thread>
#include "Main_Functions.h"
#include "Vector2.h"
#include "renderable.h"

int temp(std::string str) {
	std::cout << "yipee";
	return 3;
}

int main(int argc, char** argv) {
	bool running = true;
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	Vector2 scrDims=Main::getDisplayDims();
	Main::window = SDL_CreateWindow("Task Scheduler", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, scrDims.X/2, scrDims.Y/2, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
	Main::renderer = SDL_CreateRenderer(Main::window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	SDL_Event e;
	Button b(0, 0, 400, 400, "C:/Users/george/Pictures/Screenshots/Screenshot (52).png", SDL_FLIP_NONE, temp,std::string("ahh"));
	Main::renderables.push_back(&b);
	while (running) {
		Main::updateRenderables();
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				SDL_Quit();
				running = false;
			}
		}
	}
	return 0;
}