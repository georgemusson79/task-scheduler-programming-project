#pragma once
#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include "Vector2.h"
#include <functional>
class Renderable {
public:
	SDL_Rect renderScrDims; //dimensions for rendering the object to the screen
	virtual void update() {}
	virtual void render() = 0;
};

class Sprite : public Renderable {
protected:
	SDL_Texture* img;
	int textureWidth;
	int textureHeight;
	std::string pathToImg;

public:
	SDL_RendererFlip flip;
	SDL_Rect renderImgDims;
	Sprite(int x, int y, int w, int h, std::string pathToImg, SDL_RendererFlip flip=SDL_FLIP_NONE);
	~Sprite();
	SDL_Texture* getTexture();
	void render() override;
	void setImg(std::string pathToImg);
	std::string getPathToImg();
};



class Button : public Sprite {
private:
	SDL_Texture* img;
	std::function<void()> fn;
	
public:
	template <typename Fn, typename... Args>
	Button(int x, int y, int w, int h, std::string pathToImg, SDL_RendererFlip flip = SDL_FLIP_NONE, Fn function = NULL, Args... arguments) : Sprite(x,y,w,h,pathToImg,flip) {
		this->setFunction(function, arguments...);
	}
	//void update() override;

	template <typename Fn, typename... Args>
	void setFunction(Fn function, Args... arguments) {
		if (function != NULL) {
			static_assert(std::is_invocable_v<Fn, Args...>, "Function is not callable with provided arguments");
			auto f=std::bind(function, arguments...);
			fn = f;
		}

	}
	void update() override;

	void onClick() {
		this->fn();
	}
};

class Draggable : public Sprite {
public:
	Draggable(int x, int y, int w, int h, std::string pathToImg, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void update() override;
};

class Rectangle : public Renderable {
public:
	bool fill = false;
	SDL_Color colour;
	void render();
	Rectangle(int x, int y, int w, int h, SDL_Color colour = { 255,255,255,255 });
};