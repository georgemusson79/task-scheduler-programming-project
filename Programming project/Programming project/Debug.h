#include <SDL.h>
#include <iostream>
namespace Debug {
	inline int frames = 0; //Number of frames since last time fps was outputted to console
	inline int lastFPScheck = SDL_GetTicks(); //Get time since last fps output
	inline int FPSCheckInterval = 5000; //Time in milliseconds between fps outputs to console
	inline bool outputFPS = false;
	
	//Calculates FPS and outputs results every FPSCheckInterval assuming outputFPS is true
	void handleFPS();
}