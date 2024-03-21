#pragma once
#include <SDL.h>
#include <vector>
#include <iostream>
#include <fstream>
//Main_functions.h
struct Vector2;
class Renderable;


namespace Main {
	inline bool running = true; //if false close the program
	inline std::vector<Renderable*> renderables;
	inline SDL_Window* window;
	inline SDL_Renderer* renderer;

	//Get width and height of the screen
	Vector2 getDisplayDims();

	//Handles renderable objects in Main::renderables by displaying them to the screen and updating them
	void updateRenderables();

	//Polls events and handles them accordingly
	void handleEvents(SDL_Event& e);

	//Open File Explorer for loading files
	std::wstring openFileExplorerSave(std::vector<std::pair<std::wstring, std::wstring>> allowedFiles = { {L"All files",L"*.*"} });
	
	//Open File Explorer for saving files
	std::vector<std::wstring> openFileExplorerLoad(std::vector<std::pair<std::wstring, std::wstring>> allowedFiles = { {L"All files",L"*.*"} }, bool multiselect=false); //creates a file explorer window for loading files

}