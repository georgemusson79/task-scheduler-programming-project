#include "renderable.h"
#include <string>

TextField::TextField(int x, int y, int w, int h, SDL_Color textColor, std::string pathToBg,std::string pathToFont) : Label(x,y,w,h,textColor,pathToBg,0,pathToFont) {

}


TextField::TextField(int x, int y, int w, int h, SDL_Color textColor, SDL_Color bgColor, std::string pathToFont) : Label(x,y, w, h, textColor, bgColor,0,pathToFont) {

}

TextField::TextField(int x, int y, int w, int h, SDL_Color textColor, std::string pathToFont) : Label(x,y,w,h,textColor,0,pathToFont) {

}

TextField::~TextField() {

}

void TextField::setCharactersPerLine(int chars) {
	this->numCharsToDisplay = chars;
}

void TextField::updateKeysPressed() {
	this->keysPressedBefore = keysPressed;

	int szKeys = 0;
	const Uint8* keys = SDL_GetKeyboardState(&szKeys);
	keysPressed=std::vector(keys, keys + szKeys);
}

void TextField::render() {
	if (bg != nullptr) bg->render();
	if (this->textTexture != nullptr) {

		int start = this->posFirstCharToRender;
		//get a string of the characters up to the ones that will be rendered on the screen and their pixel width
		std::string prev = rawText.substr(0, start);
		int w;
		int h;
		TTF_SizeText(this->font, prev.c_str(), &w, &h);

		//get a string of the characters that will be rendered on the screen and their pixel width
		std::string renderedstr = rawText.substr(start, this->numCharsToDisplay);
		int w2;
		int h2;
		TTF_SizeText(this->font, renderedstr.c_str(), &w2, &h2);

		SDL_Rect textToRender = { w,0,w2,h2 };
		SDL_RenderCopy(Main::renderer, this->textTexture, &textToRender, &this->renderScrDims);

	}
}

void TextField::setPosFirstCharToRender(int first) {
	if (first < 0) first = 0;
	else if (first > rawText.length()) first = rawText.length();
	this->posFirstCharToRender = first;
}

void TextField::update() {
	this->numCharsToDisplay = 3;
	this->setPosFirstCharToRender(1);
}