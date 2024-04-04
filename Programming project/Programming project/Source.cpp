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
#include "task_objects.h"
int temp() {
	std::cout << "yipee";
	return 3;
}

int main(int argc, char** argv) {
	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	Cursor::setCursor(SDL_SYSTEM_CURSOR_HAND);
	TTF_Init();


	Vector2 scrDims = Main::getDisplayDims();
	Main::window = SDL_CreateWindow("Task Scheduler", (0x1FFF0000u | (0)), SDL_WINDOWPOS_UNDEFINED, scrDims.x * 0.9, scrDims.y * 0.8, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_GetWindowSize(Main::window, &WINDOW_WIDTH, &WINDOW_HEIGHT);
	Main::renderer = (Debug::vsync) ? SDL_CreateRenderer(Main::window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) : SDL_CreateRenderer(Main::window, -1, SDL_RENDERER_ACCELERATED);
	Draggable* d = Renderable::create<Draggable>(Main::renderer, 5, 70, 100, 100, "C:/Screenshot (51).png");
	int w = WINDOW_WIDTH / 2;
	int h = WINDOW_HEIGHT / 8;
	std::cout << w << " " << h << "\n";
	//TaskList* t=Renderable::create <TaskList>(Main::renderer, 0, 0, w, 900);
	//t->addTask("task #1");
	//Renderable::create<LabelFixedCharDims>(Main::renderer, 0, 200, 100, "bruh", SDL_Color(0, 255, 255, 255));
	std::vector<std::string> items = { "item1", "item2", "item3" };
	DropDownMenu* dr=Renderable::create <DropDownMenu>(Main::renderer, 200, 200, 110, 30,items );
	Renderable::create<Button>(Main::renderer, 400*2, 300, 100, 100, "C:/Screenshot (51).png",&DropDownMenu::destroy,SDL_FLIP_NONE,dr);
	TaskObject* t=Renderable::create <TaskObject>(Main::renderer, 0, 0, 500, 100);
	t->setDims(t->getDims().x*2,t->getDims().y);
	TextField* tex=Renderable::create <TextField>(Main::renderer, 0, 100, 400, 60, SDL_Color(0, 0, 0,255),SDL_Color(255,255,255), 12);
	//tex->setDims({ tex->getDims().x * 2, tex->getDims().y });
		
	SDL_Event e;
	while (Main::running) {
		Main::removeDestroyedObjects();
		Main::handleEvents(e);
		Main::updateRenderables();
		Debug::handleKBInput();
	}
	return 0;
}