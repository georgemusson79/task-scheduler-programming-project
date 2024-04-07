#pragma once
#include <iostream>
#include <SDL.h>
#include <vector>
#include <iterator>
#include <algorithm>
#include "Vector2.h"
namespace Utils {

	template<typename T>
	std::vector<T> getSubArray(std::vector<T>& array, int first, int amount) {
		std::vector<T> res = {};
		if (first < 0) first = 0;
		if (amount <= 0) return {};
		if (amount + first > array.size()) amount = array.size();

		
		for (int i = first; i < first+amount; i++) {
			res.push_back(array[i]);

		}
		return res;
	}

	std::string toUpper(std::string str);
	std::string toLower(std::string str);
	SDL_Rect getWindowBounds(SDL_Window* window);
	Vector2 getTextureDims(SDL_Texture* t);

	template <typename T>
	void moveItem(std::vector<T>& v, int pos, int whereTo) {
		auto firstIt = v.begin() + pos;
		auto secondIt = v.begin() + whereTo;
		int first = pos;
		int second = whereTo;
		if (first < second) {
			for (auto it1 = firstIt; it1 < secondIt; it1++) {
				auto next = std::next(it1);
				std::iter_swap(it1, next);
			}
		}

		else {
			for (auto it1 = firstIt; it1 > secondIt; it1--) {
				auto prev = std::prev(it1);
				std::iter_swap(it1, prev);
			}
		}
	}

}