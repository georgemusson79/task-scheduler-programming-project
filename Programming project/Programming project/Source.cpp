//source.cpp
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <thread>
#include "Main_Functions.h"
#include "Debug.h"
#include "Vector2.h"
#include "simple_renderables.h"
#include "DateAndTime.h"
#include "Cursor.h"
#include "Collision.h"
#include "DateAndTime.h"
#include "task_objects.h"


int main(int argc, char** argv) {
	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	Cursor::setCursor(SDL_SYSTEM_CURSOR_HAND);
	TTF_Init();


	Vector2 scrDims = Main::getDisplayDims();
	Main::window = SDL_CreateWindow("Task Scheduler", (0x1FFF0000u | (0)), SDL_WINDOWPOS_UNDEFINED, scrDims.x * 0.9, scrDims.y * 0.8, SDL_WINDOW_SHOWN);
	SDL_GetWindowSize(Main::window, &WINDOW_WIDTH, &WINDOW_HEIGHT);
	Main::renderer = (Debug::vsync) ? SDL_CreateRenderer(Main::window, -1,  SDL_RENDERER_PRESENTVSYNC) : SDL_CreateRenderer(Main::window, -1, SDL_RENDERER_ACCELERATED);
	int w = WINDOW_WIDTH / 1.5;
	int x = (WINDOW_WIDTH / 2) - (w / 2);

	TaskList* taskList=Renderable::create <TaskList>(Main::renderer, x, 0, w, WINDOW_HEIGHT,5);
	std::vector<std::string> items = { "item1", "item2", "item3" };

	
	SDL_Event e;
	while (Main::running) {
		Main::removeDestroyedObjects();
		Main::handleEvents(e);
		Main::updateRenderables();
		Debug::handleKBInput();
	}
	return 0;
}