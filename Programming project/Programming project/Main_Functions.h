#pragma once
#include <SDL.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include "simple_renderables.h"
//Main_functions.h
struct Vector2;
class TaskList;


struct WindowsResource {
	void* data;
	int szData;
};


/*
Contains global variables and utility functions that only work in the context of this program
*/
namespace Main {

	
	inline int WINDOW_WIDTH;
	inline int WINDOW_HEIGHT;


	inline Vector2 posWhereCursorWasDisabled = { 0,0 }; //if cursor is disabled this is to be updated
	void cursorDisable();

	inline bool tasksExecutingInSeperateThread = false;
	inline SDL_Color bgColor = { 255,255,255 }; //background color of the main screen
	inline bool running = true; //if false close the program
	inline std::vector<Renderable*> renderables;
	inline SDL_Window* window;
	inline SDL_Renderer* renderer;
	inline std::string textInputThisFrame = ""; //keyboard presses every frame

	//Get width and height of the screen
	Vector2 getDisplayDims();

	//Handles renderable objects in Main::renderables by displaying them to the screen and updating them
	void updateRenderables();

	//Polls events and handles them accordingly
	void handleEvents(SDL_Event& e);

	//Open File Explorer for loading files
	std::wstring openFileExplorerSave(std::vector<std::pair<std::wstring, std::wstring>> allowedFiles = { {L"All files",L"*.*"} });

	//Open File Explorer for saving files
	std::wstring openFileExplorerLoad(std::vector<std::pair<std::wstring, std::wstring>> allowedFiles = { {L"All files",L"*.*"} }, bool multiselect = false); //creates a file explorer window for loading files

	//clears the screen to bgColor
	void clearScreenDrawBackground();

	//delete any objects in Main::renderables with delete=true
	void removeDestroyedObjects();

	//load a windows resource into memory (unused, does not work)
	WindowsResource loadWindowsResource(int id);
	//generate a windows message with an error symbol and an ok and cancel button
	int windowsErrMessageBoxOkCancel(std::string title, std::string msg);
	//generate a windows message with an error symbol and an ok button
	int windowsErrMessageBoxOk(std::string title, std::string msg);
	//convert windows error code to a message of what it is
	std::string getWindowsErrorMsg(long code);

	/* Takes a 4 char string and converts it to a time_t obj
	* \param strTime: a 4 char string
	* \return a time_t object of the time
	*/
	std::time_t strTimeToTime(std::string strTime);

	/*
	Handles command line arguments
	So far the only commnads are --insert_taskfile followed by a path to a .task file for importing tasks on start up
	The second command is --execute for running the imported tasks on startup
	\param args: a list of arguments as a vector of strings
	\param taskList: a pointer to the main TaskList object
	*/
	void handleProgramArgs(std::vector<std::string> args,TaskList* tasklist);


	/*Create an object that inherits Renderable and push it to Main::renderables
	* \param args: the arguments required for creating an instance of the specified class normally
	* \return a pointer to the created object
	*/
	template <typename RenderableObj, typename... Args>
	RenderableObj* create(Args...args) {
		static_assert(std::is_constructible_v<RenderableObj, Args...>, "Unable to construct class with current arguments"); //assert that class can be created with args
		static_assert(std::is_base_of_v<Renderable, RenderableObj>, "RenderableObj is not of type Renderable"); //assert that class base is Renderable
		RenderableObj* obj = new RenderableObj(args...); //dynamically allocate object then append to Main::renderables
		Main::renderables.push_back(obj);

		//sort list by render priority, higher means it will render towards the top of the screen
		std::sort(Main::renderables.begin(), Main::renderables.end(), [](Renderable* a, Renderable* b) {
			return a->getRenderPriority() < b->getRenderPriority();
			});

		return obj;
	}
}