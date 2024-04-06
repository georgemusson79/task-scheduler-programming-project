#include "Utils.h"
namespace Utils {
	std::string toUpper(std::string str) {
		for (char& ch : str) ch=std::toupper(ch);
		return str;
	}
	std::string toLower(std::string str) {
		for (char& ch : str) ch=std::tolower(ch);
		return str;
	}
	SDL_Rect getWindowBounds(SDL_Window* window) {
		int w;
		int h;
		SDL_GetWindowSize(window, &w, &h);
		return  { 0,0,w,h };
	}

}