#pragma once
#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include "Vector2.h"
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


class Button : public Renderable {
private:
	SDL_Texture* img;
public:
	Button();
	void update() override;
	void setImg(std::string path);


};