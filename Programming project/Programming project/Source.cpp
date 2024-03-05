//source.cpp
#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL.h>

#include "Main_Functions.h"
#include "Vector2.h"
#include "renderable.h"

int main() {
	bool running = true;
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	Vector2 scrDims=Main::getDisplayDims();
	Main::window = SDL_CreateWindow("Task Scheduler", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, scrDims.X/2, scrDims.Y/2, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
	Main::renderer = SDL_CreateRenderer(Main::window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	SDL_Event e;
	Main::renderables.push_back(new Sprite(0, 0, 400, 400, "C:/Users/george/Pictures/Screenshots/Screenshot (52).png"));
	while (running) {
		Main::renderRenderables();
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				SDL_Quit();
				running = false;
			}
		}
	}
	
}