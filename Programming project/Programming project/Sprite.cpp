#include "renderable.h"
#include "Main_Functions.h"
Sprite::Sprite(int x, int y, int w, int h, std::string pathToImg, bool flipHorizontal, bool flipVertical) :flipHorizontal(flipHorizontal), flipVertical(flipVertical) {
	this->renderScrDims = { x,y,w,h };
	this->setImg(pathToImg);
}

void Sprite::setImg(std::string pathToImg) {
	this->img = IMG_LoadTexture(Main::renderer, pathToImg.c_str());
	SDL_QueryTexture(this->img, NULL, NULL, &this->textureWidth, &this->textureWidth);
	this->RenderImgDims = { 0,0,this->textureWidth,this->textureHeight };
	this->pathToImg = pathToImg;
}

void Sprite::render() {
	SDL_RenderCopyEx(Main::renderer, this->img, NULL, &renderScrDims,0);
}

std::string Sprite::getPathToImg() {
	return this->pathToImg;
}
