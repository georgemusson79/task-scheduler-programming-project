#pragma once
#include <iostream>
#include <SDL.h>
namespace Utils {
	std::string toUpper(std::string str);
	std::string toLower(std::string str);
	SDL_Rect getWindowBounds(SDL_Window* window);
}