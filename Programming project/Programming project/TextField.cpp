#include "renderable.h"
#include "Cursor.h"
#include "Collision.h"

#include <string>

TextField::TextField(int x, int y, int w, int h, SDL_Color textColor, std::string pathToBg, int maxCharsToDisplay, std::string pathToFont) : Label(x,y,w,h,textColor,pathToBg,0,pathToFont) {
	this->setCharactersPerLine(maxCharsToDisplay);
	this->renderPriority = 10;
	this->name = "textField";
}


TextField::TextField(int x, int y, int w, int h, SDL_Color textColor, SDL_Color bgColor, int maxCharsToDisplay, std::string pathToFont) : Label(x,y, w, h, textColor, bgColor,0,pathToFont) {
	this->setCharactersPerLine(maxCharsToDisplay);
	this->renderPriority = 10;
	this->name = "textField";
}

TextField::TextField(int x, int y, int w, int h, SDL_Color textColor, int maxCharsToDisplay, std::string pathToFont) : Label(x,y,w,h,textColor,0,pathToFont) {
	this->setCharactersPerLine(maxCharsToDisplay);
	this->renderPriority = 10;
	this->name = "textField";

}

TextField::~TextField() {

}

void TextField::setCharactersPerLine(int chars) {
	this->numCharsToDisplay = chars;

	//adjust the pixel width of each character so that the width doesnt change as you add or remove characters
	this->pxPerCharacter = (float)this->renderScrDims.w / chars;
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
		std::string renderedStr = rawText.substr(start, this->numCharsToDisplay);
		int w2;
		int h2;
		TTF_SizeText(this->font, renderedStr.c_str(), &w2, &h2);
		SDL_Rect textToRender = { w,0,w2,h2 };

		SDL_Rect textRenderScrDims = this->renderScrDims;
		textRenderScrDims.w = this->pxPerCharacter*renderedStr.length();

		SDL_RenderCopy(Main::renderer, this->textTexture, &textToRender, &textRenderScrDims);

	}
	if (this->focused) {
		int typingCursorRenderX = (this->typingCursorPos * this->pxPerCharacter)+this->pxPerCharacter;
		Uint8 r, g, b,a;
		SDL_GetRenderDrawColor(Main::renderer, &r, &g, &b,&a);
		SDL_SetRenderDrawColor(Main::renderer, textColor.r, textColor.g, textColor.b, textColor.a);
		SDL_RenderDrawLine(Main::renderer, typingCursorRenderX, this->renderScrDims.y, typingCursorRenderX, this->renderScrDims.y + this->renderScrDims.h);
		SDL_SetRenderDrawColor(Main::renderer, r, g, b, a);
	}
}

void TextField::setPosFirstCharToRender(int first) {
	if (first < 0) first = 0;
	else if (first > rawText.length()) first = rawText.length();
	this->posFirstCharToRender = first;
}

void TextField::update() {
	bool cursorCollides = Collision::collidesWith(this->renderScrDims, Cursor::getPos());
	bool cursorIsClicked = Cursor::isLeftClicked();

	if (cursorCollides) {
		Cursor::setCursor(SDL_SYSTEM_CURSOR_IBEAM);
		if (cursorIsClicked) {
			//if nothing else has been clicked and the user selects the textbox, generate a typing indicator in the nearest available position
			if(this->trySetFocus() || this->focused) this->_generateTypingCursor();
		}
	}

	if (cursorIsClicked && !cursorCollides) this->tryRemoveFocus();
}

void TextField::_generateTypingCursor() { 
	Vector2 cursorPos = Cursor::getPos();
	int cursorxToBox=cursorPos.x - this->renderScrDims.x;
	this->typingCursorPos = std::round(cursorxToBox / this->pxPerCharacter);

}

