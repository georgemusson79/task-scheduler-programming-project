#include "renderable.h"
#include "Main_Functions.h"
Sprite::Sprite(int x, int y, int w, int h, std::string pathToImg,SDL_RendererFlip flip) : flip(flip) {
	this->renderScrDims = { x,y,w,h };
	this->setImg(pathToImg);
}

void Sprite::setImg(std::string pathToImg) {
	this->img = IMG_LoadTexture(Main::renderer, pathToImg.c_str());
	if (this->img==NULL) std::cerr << "Unable to load image: " << IMG_GetError() << "\n";
	if (SDL_QueryTexture(this->img, NULL, NULL, &this->textureWidth, &this->textureHeight)!=0) std::cerr << "Unable to query texture: " << SDL_GetError() << "\n";
	this->renderImgDims = { 0,0,this->textureWidth,this->textureHeight };
	this->pathToImg = pathToImg;
}

void Sprite::render() {
	SDL_RenderCopyEx(Main::renderer, img, &renderImgDims, &renderScrDims, 0, NULL, flip);
	
}

std::string Sprite::getPathToImg() {
	return this->pathToImg;
}

Sprite::~Sprite() {
	SDL_DestroyTexture(this->img);
	std::cout<<"sprite destroyed";
}

SDL_Texture* Sprite::getTexture() {
	return this->img;
}