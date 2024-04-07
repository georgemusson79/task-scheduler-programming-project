#pragma once
#include <iostream>
#include <SDL.h>
#include <vector>
#include "Vector2.h"
namespace Utils {

	template<typename T>
	std::vector<T> getSubArray(std::vector<T>& array, int first, int amount) {
		std::vector<T> res = {};
		if (first < 0) first = 0;
		if (amount <= 0) return {};
		if (amount + first > array.size()) amount = array.size() - first;

		for (int i = first; i < amount; i++) res.push_back(array[i]);
		return res;
	}

	std::string toUpper(std::string str);
	std::string toLower(std::string str);
	SDL_Rect getWindowBounds(SDL_Window* window);
	Vector2 getTextureDims(SDL_Texture* t);

}