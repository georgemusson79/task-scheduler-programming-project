#include "renderable.h"
#include "font.h"
#include <SDL_ttf.h>

void Label::setText(std::string text) {
	SDL_DestroyTexture(textTexture);
	rawText = text;
	if (text == "") {
		textTexture = nullptr;
		return;
	}
	
	int w;
	int h;
	TTF_SizeText(font, text.c_str(), &w, &h);
	int pixelsPerLine = (w / text.size()) * this->charsPerLine;

	SDL_Surface* s = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), this->textColor, pixelsPerLine);
	if (s == nullptr) throw "Unable to load texture";
	this->textTexture = SDL_CreateTextureFromSurface(Main::renderer, s);
	SDL_FreeSurface(s);
}

void Label::setFont(std::string fontPath) {
	TTF_CloseFont(this->font);
	if (fontPath == "") {
		SDL_RWops* data=SDL_RWFromConstMem(defaultFont, sizeof(defaultFont));
		//std::cout << sizeof(defaultFont) << "\n";
		this->font = TTF_OpenFontRW(data, 1, 90);
		if (this->font == NULL) std::cout << TTF_GetError() << "\n";
	}
	else {
		this->font = TTF_OpenFont(fontPath.c_str(), 90);
	}
	if (this->font == NULL) std::cerr << TTF_GetError() << "\n";

}

Label::Label(int x, int y, int w, int h, SDL_Color textColor, std::string pathToBg, int charsPerLine, std::string pathToFont) {
	renderScrDims = { x,y,w,h };
	bg=new Sprite(x, y, w, h, pathToBg);
	this->textColor = textColor;
	this->setCharactersPerLine(charsPerLine);
	this->setFont(pathToFont);
}

Label::Label(int x, int y, int w, int h, SDL_Color textColor, SDL_Color bgColor,int charsPerLine, std::string pathToFont) {
	renderScrDims = { x,y,w,h };
	this->setBgColor(bgColor);
	this->textColor = textColor;
	this->setCharactersPerLine(charsPerLine);
	this->setFont(pathToFont);
	
}

Label::Label(int x, int y, int w, int h, SDL_Color textColor, int charsPerLine, std::string pathToFont) {
	this->renderScrDims = { x, y, w, h };
	this->textColor = textColor;
	this->charsPerLine = 0;
	this->setFont(pathToFont);
}



void Label::render() {
	if (bg!=nullptr) bg->render();
	if (this->textTexture != nullptr) SDL_RenderCopy(Main::renderer, textTexture, NULL, &this->renderScrDims);
}

void Label::setCharactersPerLine(int chars) {
	this->charsPerLine = chars;
	if (this->textTexture != nullptr) this->setText(this->rawText);
}

void Label::setTextColor(SDL_Color color) {
	this->textColor = color;
	this->setText(this->rawText);
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
	SDL_DestroyTexture(this->textTexture);
	TTF_CloseFont(this->font);
}

void Label::setRenderingDims(int x, int y, int w, int h) {
	renderScrDims = { x,y,w,h };
	if (bg!=nullptr) bg->setRenderingDims(x, y, w, h);
}
