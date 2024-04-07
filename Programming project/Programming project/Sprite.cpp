#include "simple_renderables.h"
#include "Main_Functions.h"
Sprite::Sprite(SDL_Renderer* renderer, int x, int y, int w, int h, std::string pathToImg,SDL_RendererFlip flip) : Renderable(renderer) {
	this->flip = flip;
	this->renderScrDims = { x,y,w,h };
	this->setImg(pathToImg);
	this->name = "sprite";
}

void Sprite::setImg(std::string pathToImg) {
	this->img = IMG_LoadTexture(renderer, pathToImg.c_str());
	int w;
	int h;
	SDL_QueryTexture(this->img, NULL, NULL, &w, &h);
	this->textureWidth = w;
	this->textureHeight = h;
	this->renderImgDims = { 0,0,this->textureWidth,this->textureHeight };
	this->pathToImg = pathToImg;
}

void Sprite::render() {
	SDL_RenderCopyEx(renderer, img, &renderImgDims, &renderScrDims, 0, NULL, flip);
}

std::string Sprite::getPathToImg() {
	return this->pathToImg;
}

Sprite::~Sprite() {
	SDL_DestroyTexture(this->img);
	//std::cout<<"sprite destroyed";
}

SDL_Texture* Sprite::getTexture() {
	return this->img;
}