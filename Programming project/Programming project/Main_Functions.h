#pragma once
//Main_functions.h
class Vector2;
class SDL_Renderer;
class SDL_Window;
namespace Main {
	SDL_Window* window;
	SDL_Renderer* renderer;
	Vector2 getDisplayDims();
}