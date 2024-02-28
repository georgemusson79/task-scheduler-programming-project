#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL.h>

#include "Main_Functions.h"
#include "Vector2.h"

int main() {
	bool running = true;
	SDL_Init(SDL_INIT_EVERYTHING);
	Vector2 scrDims=Main::getDisplayDims();
	SDL_Window* w = SDL_CreateWindow("Task Scheduler", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, scrDims.X/2, scrDims.Y/2, SDL_WINDOW_SHOWN);
	SDL_Event e;
	while (running) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				SDL_Quit();
				running = false;
			}
		}
	}
}