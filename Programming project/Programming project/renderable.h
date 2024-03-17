#pragma once
#include <iostream>
#include <SDL_image.h>
#include <functional>

#include "Main_Functions.h"
#include "Vector2.h"


class Renderable {
public:
	SDL_Rect renderScrDims; //dimensions for rendering the object to the screen
	//Update object, called every frame
	virtual void update() {}
	//Render object to screen
	virtual void render() = 0;
	//Create an object that inherits Renderable and push it to Main::renderables
	template <typename RenderableObj,typename... Args>
	static void create(Args...args) {
		static_assert(std::is_constructible_v<RenderableObj, Args...>, "Unable to construct class with current arguments"); //assert that class can be created with args
		static_assert(std::is_base_of_v<Renderable, RenderableObj>, "RenderableObj is not of type Renderable"); //assert that class base is Renderable
		RenderableObj* obj = new RenderableObj(args...); //dynamically allocate object then append to Main::renderables
		Main::renderables.push_back(obj);
	}
	virtual ~Renderable() {
		std::cout << "renderable destroyed\n";
	}
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
	SDL_Texture* getTexture();
	void render() override;
	void setImg(std::string pathToImg);
	std::string getPathToImg();
	~Sprite();
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