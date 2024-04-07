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
	int h = WINDOW_HEIGHT / 8;
	std::cout << w << " " << h << "\n";
	TaskList* t1 = new TaskList(Main::renderer, 0, 0, w, WINDOW_HEIGHT);
	TaskList* t=Renderable::create <TaskList>(Main::renderer, 0, 0, w, WINDOW_HEIGHT,5);
	//Renderable::create<LabelFixedCharDims>(Main::renderer, 0, 200, 100, "bruh", SDL_Color(0, 255, 255, 255));
	std::vector<std::string> items = { "item1", "item2", "item3" };
	//DropDownMenu* dr=Renderable::create <DropDownMenu>(Main::renderer, 200, 200, 110, 30,items );
	//Renderable::create<Button>(Main::renderer, 400*2, 300, 100, 100, "C:/Screenshot (51).png",&DropDownMenu::destroy,SDL_FLIP_NONE,dr);
	//TaskObject* t=Renderable::create <TaskObject>(Main::renderer, 0, 0, 800, 150);
	//t->setDims(t->getDims().x*0.9,t->getDims().y);
	//TextField* tex=Renderable::create <TextField>(Main::renderer, 0, 100, 400, 60, SDL_Color(0, 0, 0,255),SDL_Color(255,255,255), 12,AllowedChars::ONLY_NUMBERS,AllowedCase::ONLY_LOWER);
	//tex->setDims({ tex->getDims().x * 2, tex->getDims().y });
	//TimeInputBox* time=Renderable::create<TimeInputBox>(Main::renderer, 0, 0, 200, 20, SDL_Color(0, 0, 0), SDL_Color(255, 255, 255), SDL_Color(255, 255, 255));
	//time->setDims({time->getDims().x * 7, time->getDims().y * 7});
	
	SDL_Event e;
	while (Main::running) {
		Main::removeDestroyedObjects();
		Main::handleEvents(e);
		Main::updateRenderables();
		Debug::handleKBInput();
	}
	return 0;
}