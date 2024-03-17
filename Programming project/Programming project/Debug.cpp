#include "Debug.h"
#include "Main_Functions.h"

void Debug::handleFPS() {
	if (outputFPS) {
		frames++;
		if (SDL_GetTicks() - lastFPScheck > FPSCheckInterval) {
			std::cout << "{DEBUG] Frames Per Second: " << frames / (FPSCheckInterval / 1000) << "\n"; //calculate and output averge fps in given time
			lastFPScheck = SDL_GetTicks();
			frames = 0;
		}
	}
}

