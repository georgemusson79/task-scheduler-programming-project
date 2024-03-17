#pragma once
#include "renderable.h"
#include <vector>
#include <iostream>
#include <fstream>
//Main_functions.h
class Vector2;
class SDL_Renderer;
class SDL_Window;


namespace Main {
	inline bool running = true; //if false close the program
	inline std::vector<Renderable*> renderables;
	inline SDL_Window* window;
	inline SDL_Renderer* renderer;
	Vector2 getDisplayDims(); //get width and height of the screen
	void updateRenderables();
	void handleEvents(SDL_Event& e); //polls events and handles them accordingly
	std::wstring openFileExplorerSave(std::vector<std::pair<std::wstring, std::wstring>> allowedFiles = { {L"All files",L"*.*"} });
	std::vector<std::wstring> openFileExplorerLoad(std::vector<std::pair<std::wstring, std::wstring>> allowedFiles = { {L"All files",L"*.*"} }, bool multiselect=false); //creates a file explorer window for loading files
}