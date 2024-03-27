#include <Windows.h>
#include <sstream>

#include "Debug.h"
#include "Main_Functions.h"
#include "Cursor.h"
	


const std::unordered_map <SDL_Scancode, void(*)()> Debug::keyToDebugFunc = { {SDL_SCANCODE_F12,&getPxDistanceAsMsg} };

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

void Debug::getPxDistanceAsMsg() {
	if (!Debug::_gettingDistbetween2Pts) {
		Debug::_getPxDistancePointA = Cursor::getPos();

		std::stringstream first;
		first << "First Point Selected, move cursor to second point and press f12 again to get pixel distance.\nFirst point at " << "(" << Debug::_getPxDistancePointA.x << "," << Debug::_getPxDistancePointA.y << ")";
		
		MessageBoxA(NULL,first.str().c_str(), "First Point Selected", MB_OK);
	}

	else {

		Vector2 secondPos = Cursor::getPos();
		std::stringstream second;
		second << "Second point selected at " << "(" << secondPos.x << "," << secondPos.y << ")\n Distance between 2 points is " << Debug::_getPxDistancePointA.distance(secondPos) << " pixels.";

		MessageBoxA(NULL,second.str().c_str(), "Second Point Selected", MB_OK);
	}
	Debug::_gettingDistbetween2Pts = !Debug::_gettingDistbetween2Pts;
}

void Debug::update() {
	Debug::handleFPS();
}

void Debug::handleKBInput() {
	int szKeysPressed;
	const Uint8* keysPressed = SDL_GetKeyboardState(&szKeysPressed);


	//look up each key in keyToDebugFunc to see if they have been pressed, if so call its function
	for (auto pair : Debug::keyToDebugFunc) if (keysPressed[pair.first]) pair.second();
}