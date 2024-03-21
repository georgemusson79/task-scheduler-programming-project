#include "renderable.h"

void Label::setText(std::wstring text) {

}

Label::Label(int x, int y, int w, int h, SDL_Color textColor, std::string pathToBg) {
	renderScrDims = { x,y,w,h };
	bg=new Sprite(x, y, w, h, pathToBg);
	this->textColor = textColor;
}

Label::Label(int x, int y, int w, int h, SDL_Color textColor, SDL_Color bgColor) {
	renderScrDims = { x,y,w,h };
	this->setBgColor(bgColor);
	this->textColor = textColor;
	
}

Label::Label(int x, int y, int w, int h, SDL_Color textColor) {
	this->renderScrDims = { x, y, w, h };
	this->textColor = textColor;
}



void Label::render() {
	if (bg!=nullptr) bg->render();
}

void Label::setTextColor(SDL_Color color) {
	this->textColor = color;
}

void Label::setBgColor(SDL_Color color) {
	delete bg;
	bg = new Rectangle(renderScrDims.x, renderScrDims.y, renderScrDims.w, renderScrDims.h, true, color);
}

void Label::updateBgImage(std::string pathToBg) {
	delete bg;
	if(pathToBg!="") bg = new Sprite(renderScrDims.x, renderScrDims.y, renderScrDims.w, renderScrDims.h, pathToBg);
}

Label::~Label() {
	delete bg;
}

void Label::setRenderingDims(int x, int y, int w, int h) {
	renderScrDims = { x,y,w,h };
	if (bg!=nullptr) bg->setRenderingDims(x, y, w, h);
}