//source.cpp
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <fstream>
#include <thread>
#include <fstream>
#include "Main_Functions.h"
#include "Debug.h"
#include "Vector2.h"
#include "simple_renderables.h"
#include "DateAndTime.h"
#include "Cursor.h"
#include "Collision.h"
#include "DateAndTime.h"
#include "task_objects.h"

int main(int argc, char* argv[]) {


	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	Cursor::setCursor(SDL_SYSTEM_CURSOR_HAND);
	TTF_Init();


	Vector2 scrDims = Main::getDisplayDims();
	Main::window = SDL_CreateWindow("Task Scheduler", (0x1FFF0000u | (0)), SDL_WINDOWPOS_UNDEFINED, scrDims.x * 0.9, scrDims.y * 0.8, SDL_WINDOW_SHOWN);
	SDL_GetWindowSize(Main::window, &Main::WINDOW_WIDTH, &Main::WINDOW_HEIGHT);
	Main::renderer = (Debug::vsync) ? SDL_CreateRenderer(Main::window, -1, SDL_RENDERER_PRESENTVSYNC) : SDL_CreateRenderer(Main::window, -1, SDL_RENDERER_ACCELERATED);
	int w = Main::WINDOW_WIDTH / 1.5;
	int x = (Main::WINDOW_WIDTH / 2) - (w / 2);
	//x = 0;


	TaskList* taskList = Main::create <TaskList>(Main::renderer, 0, 0, w, Main::WINDOW_HEIGHT, 5);

	std::vector<char*> args=Utils::convertCArrayToVector(argv, argc);
	std::vector<std::string> argsstr = {};
	for (char* arg : args) argsstr.push_back(std::string(arg));
	Main::handleProgramArgs(argsstr, taskList);
	

	SDL_Event e;
	while (Main::running) {
		Main::removeDestroyedObjects();
		Main::handleEvents(e);
		Main::updateRenderables();
		if (Debug::debugModeEnabled) Debug::handleKBInput(); //unused outside of development
	}
	return 0;
}
