#include <SDL.h>
#include <iostream>
#include <unordered_map>

#include "Vector2.h"

namespace Debug {
	inline Vector2 _getPxDistancePointA = { 0,0 }; //used for getPxDistanceAsMsg
	inline bool _gettingDistbetween2Pts = false; //used for getPxDistanceAsMsg

	inline int frames = 0; //Number of frames since last time fps was outputted to console
	inline int lastFPScheck = SDL_GetTicks(); //Get time since last fps output
	inline int FPSCheckInterval = 5000; //Time in milliseconds between fps outputs to console
	inline bool outputFPS = true;
	inline bool vsync = true;
	inline bool debugModeEnabled = false;
	extern const std::unordered_map<SDL_Scancode, void(*)()> keyToDebugFunc; //map of keyboard presses to their respective debug functions to call when clicked
	
	//Calculates FPS and outputs results every FPSCheckInterval assuming outputFPS is true
	void handleFPS();

	//Calls debug functions when their associated hotkey in keyToDebugFunc is pressed
	void handleKBInput();

	/* if user presses f12 with debugging enabled, generate message dialog with Cursor position
	if user presses f12 again get message box with distance between the 2 points */
	void getPxDistanceAsMsg();

	void update();
}