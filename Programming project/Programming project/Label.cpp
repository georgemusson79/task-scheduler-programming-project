#include "Main_Functions.h"
#include "simple_renderables.h"
#include <SDL_ttf.h>

bool Label::setText(std::string text) {
	SDL_DestroyTexture(textTexture);
	rawText = text;
	if (text == "") {
		textTexture = nullptr;
		return true;
	}

	int w;
	int h;
	TTF_SizeText(font, text.c_str(), &w, &h);
	int pixelsPerLine = (w / text.size()) * this->charsPerLine;

	this->setFont(this->fontPath, this->renderScrDims.w);
	SDL_Surface* s = TTF_RenderText_Blended_Wrapped(font, text.c_str(), this->textColor, pixelsPerLine);
	if (s == nullptr) throw "Unable to load texture";
	this->textTexture = SDL_CreateTextureFromSurface(renderer, s);
	SDL_FreeSurface(s);
	return true;
}

void Label::setFont(std::string fontPath, int fontSz) {
	TTF_CloseFont(this->font);
	if (fontPath == "") {
		//SDL_RWops* data = SDL_RWFromConstMem(loadedFont.data, loadedFont.szData);
		////std::cout << sizeof(defaultFont) << "\n";
		//this->font = TTF_OpenFontRW(data, 1, 1);
		//if (this->font == NULL) std::cout << TTF_GetError() << "\n";
		this->font = TTF_OpenFont("arial.ttf", fontSz);
		this->fontPath = "arial.ttf";
		if (this->font == nullptr) std::cout << TTF_GetError() << "\n";
	}
	else {
		this->font = TTF_OpenFont(fontPath.c_str(), fontSz);
		this->fontPath = fontPath;
	}

	if (this->font == NULL) std::cerr << TTF_GetError() << "\n";

}

Label::Label(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color textColor, std::string pathToBg, int charsPerLine, std::string pathToFont) : Renderable(renderer) {
	renderScrDims = { x,y,w,h };
	bg = new Sprite(renderer, x, y, w, h, pathToBg);
	this->textColor = textColor;
	this->setCharactersPerLine(charsPerLine);
	this->setFont(pathToFont);
}



Label::Label(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color textColor, SDL_Color bgColor, int charsPerLine, std::string pathToFont) : Renderable(renderer) {
	renderScrDims = { x,y,w,h };
	this->setBgColor(bgColor);
	this->textColor = textColor;
	this->setCharactersPerLine(charsPerLine);
	this->setFont(pathToFont);

}

Label::Label(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color textColor, int charsPerLine, std::string pathToFont) : Renderable(renderer) {
	this->renderScrDims = { x, y, w, h };
	this->textColor = textColor;
	this->setCharactersPerLine(charsPerLine);
	this->setFont(pathToFont);
}



void Label::render() {
	if (bg != nullptr) bg->render();
	if (this->textTexture != nullptr) {
		SDL_RenderCopy(renderer, textTexture, NULL, &this->renderScrDims);
	}
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
	bg = new Rectangle(renderer, renderScrDims.x, renderScrDims.y, renderScrDims.w, renderScrDims.h, true, color);
}

bool Label::setPos(Vector2 pos) {
	this->renderScrDims.x = pos.x;
	this->renderScrDims.y = pos.y;
	if (this->bg != nullptr) this->bg->setPos(pos.x, pos.y);
	return true;
}



void Label::updateBgImage(std::string pathToBg) {
	delete bg;
	if (pathToBg != "") bg = new Sprite(renderer, renderScrDims.x, renderScrDims.y, renderScrDims.w, renderScrDims.h, pathToBg);
}

Label::~Label() {
	delete bg;
	SDL_DestroyTexture(this->textTexture);
	TTF_CloseFont(this->font);
	std::cout << "label destroyed\n";
}

bool Label::setRenderingDims(int x, int y, int w, int h) {
	renderScrDims = { x,y,w,h };
	if (bg != nullptr) return bg->setRenderingDims(x, y, w, h);
	else return false;
}

Label* Label::createBasicLabel(SDL_Renderer* renderer, std::string text, int x, int y, int charW, int charH, SDL_Color textColor, SDL_Color bgColor, std::string pathToFont) {
	int width = text.length() * charW;
	Label* l = new Label(renderer, x, y, width, charH, textColor, bgColor, 0, pathToFont);
	l->setText(text);
	return l;
}

Label* Label::createBasicLabel(SDL_Renderer* renderer, std::string text, int x, int y, int charW, int charH, SDL_Color textColor, std::string pathToBg, std::string pathToFont) {
	int width = text.length() * charW;
	Label* l = new Label(renderer, x, y, width, charH, textColor, pathToBg, 0, pathToFont);
	l->setText(text);
	return l;
}

Label* Label::createBasicLabel(SDL_Renderer* renderer, std::string text, int x, int y, int charW, int charH, SDL_Color textColor, std::string pathToFont) {
	int width = text.length() * charW;
	Label* l = new Label(renderer, x, y, width, charH, textColor, 0, pathToFont);
	l->setText(text);
	return l;
}