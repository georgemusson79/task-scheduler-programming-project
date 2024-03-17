#include <SDL.h>
#include <iostream>
namespace Debug {
	inline int frames = 0; //number of frames since last time fps was outputted to console
	inline int lastFPScheck = SDL_GetTicks(); //get time since last fps output
	inline int FPSCheckInterval = 5000; //time in milliseconds between fps outputs to console
	inline bool outputFPS = false;
	void handleFPS();
}