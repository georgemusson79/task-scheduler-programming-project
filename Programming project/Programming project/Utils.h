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

	/*
	Moves an element of a vector from one position to the next, modifies the vector in place
	\param v Reference of the vector to be modified
	\param pos Index of the element to be moved
	\param whereTo Index of where the element will be moved to
	*/
	template <typename T>
	void moveItem(std::vector<T>& v, int pos, int whereTo) {
		int first = pos;
		int second = whereTo;
		auto firstIt = v.begin() + pos;
		auto secondIt = v.begin();

		//if whereTo exceeds the bounds of the array set to the end of the array
		if (whereTo < v.size())  secondIt = v.begin() + whereTo;
		else secondIt = v.end();

		if (first < second) {
			for (auto it1 = firstIt; it1 < secondIt; it1++) {
				//if next position is end of array move there then stop
				auto next = std::next(it1);
				if (next != v.end()) std::iter_swap(it1, next);
				else return;
			}
		}

		else {
			for (auto it1 = firstIt; it1 > secondIt; it1--) {
				if (it1 == v.begin()) return;
				auto prev = std::prev(it1);
				std::iter_swap(it1, prev);
			}
		}
	}
	
	std::vector<std::string> split(std::string str,char delimiter = '\n');
	int toInt(char c);
}