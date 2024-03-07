#include <SDL.h>
#include <iostream>
namespace Debug {
	int frames = 0; //number of frames since last time fps was outputted to console
	int lastFPScheck = SDL_GetTicks(); //get time since last fps output
	int FPSCheckInterval = 5000; //time in milliseconds between fps outputs to console
	inline bool outputFPS = true;
	void handleFPS() {
		if (outputFPS) {
			frames++;
			if (SDL_GetTicks() - lastFPScheck > FPSCheckInterval) {
				std::cout << "{DEBUG] Frames Per Second: "<< frames / (FPSCheckInterval / 1000) << "\n"; //calculate and output averge fps in given time
				lastFPScheck = SDL_GetTicks();
				frames = 0;
			}
		}
	}
}