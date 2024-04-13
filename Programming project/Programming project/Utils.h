#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <SDL.h>
#include <vector>
#include <iterator>
#include <algorithm>
#include <chrono>
#include "Vector2.h"




//A set of utility functions designed to be useable outside of this program
namespace Utils {
	//date and time as strings
	struct DateAndTime {
		std::string time;
		std::string date;
	};

	/*
* Creates an vector where the first element is the first position and the last is the first position + amount
* \param array: array to get a part of
* \param first: first position of the array, if less than 0 start at 0
* \param amount: number of elements to get, if greater than array.size(), becomes array.size()
* \return returns a new array of the same type as the input array, if first is greater than array.size return an empty array
*/
	template<typename T>
	std::vector<T> getSubArray(std::vector<T>& array, int first, int amount) {
		std::vector<T> res = {};
		if (first < 0) first = 0;
		if (amount <= 0) return {};
		if (amount + first > array.size()) amount = array.size();
		if (first > array.size()) return {};

		
		for (int i = first; i < first+amount; i++) {
			res.push_back(array[i]);

		}
		return res;
	}
	//Makes a string uppercase
	std::string toUpper(std::string str);
	//Makes a string lower case
	std::string toLower(std::string str);
	//Get x, y, width and height of a window as an SDL_Rect
	SDL_Rect getWindowBounds(SDL_Window* window);
	//get width and height of an SDL_Texture
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
	

	/*
	* Splits a string into a vector
	* \param str: the string to be split
	* \param delimiter: the character to split at
	* \return Returns a vector of strings
	*/
	std::vector<std::string> split(std::string str,std::string delimiter = "\n");
	int toInt(char c);
	Utils::DateAndTime timeToDateTime(std::tm time);
	std::tm DateTimeToTm(Utils::DateAndTime time);
	Utils::DateAndTime getCurrentDateAndTime();
	std::wstring stringTowstring(std::string str);
	template <typename T>
	std::vector<T> convertCArrayToVector(T arr[], int numElements) {
		std::vector<T> v = {};
		for (int i = 0; i < numElements; i++) v.push_back(arr[i]);
		return v;
	}

	std::string eraseCharFromString(std::string str, char ch);

	//return the previous color and set the new color for drawing for the renderer
	SDL_Color setRenderDrawColor(SDL_Renderer* renderer, SDL_Color color);
}