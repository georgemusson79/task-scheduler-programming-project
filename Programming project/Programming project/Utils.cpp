#include "Utils.h"
#include <sstream>

namespace Utils {

	int toInt(char c) {
		return c - '0';
	}


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
	Vector2 getTextureDims(SDL_Texture* t) {
		int w;
		int h;
		SDL_QueryTexture(t, NULL, NULL, &w, &h);
		return Vector2( w,h );
	}

	std::vector<std::string> split(std::string str,char delimiter) {
		std::vector<std::string> v = {};
		std::stringstream ss(str);
		std::string buffer;
		while (std::getline(ss, buffer, delimiter)) v.push_back(buffer);
		return v;
	}
	
}