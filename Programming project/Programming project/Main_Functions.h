#pragma once
#include "renderable.h"
#include <vector>
//Main_functions.h
class Vector2;
class SDL_Renderer;
class SDL_Window;

namespace Main {
	inline std::vector<Renderable*> renderables;
	inline SDL_Window* window;
	inline SDL_Renderer* renderer;
	Vector2 getDisplayDims();
	void updateRenderables();
}