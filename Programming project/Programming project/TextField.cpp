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

void TextField::_updateKeysPressed() {
	this->keysPressedBefore = keysPressed;
	int szKeys;
	const Uint8* keys = SDL_GetKeyboardState(&szKeys);
	keysPressed=std::vector(keys, keys + szKeys);
	this->textKeysPressed = Main::textInputThisFrame;
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
		this->renderedText = rawText.substr(start, this->numCharsToDisplay);
		int w2;
		int h2;
		TTF_SizeText(this->font, this->renderedText.c_str(), &w2, &h2);
		SDL_Rect textToRender = { w,0,w2,h2 };

		SDL_Rect textRenderScrDims = this->renderScrDims;
		textRenderScrDims.w = this->pxPerCharacter*this->renderedText.length();

		SDL_RenderCopy(Main::renderer, this->textTexture, &textToRender, &textRenderScrDims);

	}
	if (this->focused) {
		//get where the typing cursor should be based on where it is in the string of rendered characters
		int startOfTextbox = this->renderScrDims.x;
		int typingCursorRenderX = startOfTextbox + (this->typingCursorPos * this->pxPerCharacter);

		//draw the cursor to the screen
		Uint8 r, g, b,a;
		SDL_GetRenderDrawColor(Main::renderer, &r, &g, &b,&a);
		SDL_SetRenderDrawColor(Main::renderer, textColor.r, textColor.g, textColor.b, textColor.a);
		SDL_RenderDrawLine(Main::renderer, typingCursorRenderX, this->renderScrDims.y, typingCursorRenderX, this->renderScrDims.y + this->renderScrDims.h);
		SDL_SetRenderDrawColor(Main::renderer, r, g, b, a);
	}
}

void TextField::setPosFirstCharToRender(int first) {
	if (first < 0) first = 0;
	//else if (first >= rawText.length()-this->numCharsToDisplay) first = rawText.length()-this->numCharsToDisplay;
	else if (first > (int)rawText.length() - this->numCharsToDisplay && first > this->posFirstCharToRender) return;
	this->posFirstCharToRender = first;
	this->renderedText = rawText.substr(this->posFirstCharToRender, this->numCharsToDisplay);
}

void TextField::update() {
	bool cursorCollides = Collision::collidesWith(this->renderScrDims, Cursor::getPos());
	bool cursorIsClicked = Cursor::isLeftClicked();

	if (cursorCollides) {
		Cursor::setCursor(SDL_SYSTEM_CURSOR_IBEAM);
		if (cursorIsClicked) {
			//if nothing else has been clicked and the user selects the textbox, generate a typing indicator in the nearest available position
			if (this->trySetFocus() || this->focused) this->_generateTypingCursor();
		}
	}

	if (this->focused) {
		this->_handleKBInput();
	}

	if (cursorIsClicked && !cursorCollides) this->tryRemoveFocus();
}

void TextField::_generateTypingCursor() { 
	Vector2 cursorPos = Cursor::getPos();
	int cursorxRelativeToBox=cursorPos.x - this->renderScrDims.x;


	this->typingCursorPos = std::round(cursorxRelativeToBox / this->pxPerCharacter);
	if (this->typingCursorPos > this->renderedText.size()) this->typingCursorPos = this->renderedText.size();
}



void TextField::_moveCursorLeft(int times) {
	if (this->typingCursorPos > 0) this->typingCursorPos-=times;
	else {
		this->setPosFirstCharToRender(this->posFirstCharToRender - times);
	}
	
}
void TextField::_moveCursorRight(int times) {
	if (this->typingCursorPos+times <= this->renderedText.size()) this->typingCursorPos+=times;
	else if (this->renderedText.size() == 0) return;
	else {
		this->setPosFirstCharToRender(this->posFirstCharToRender + times);
		this->typingCursorPos = this->renderedText.size();
	}
}

void TextField::_handleKBInput() {
	this->_updateKeysPressed();
	if (this->keysPressed[SDL_SCANCODE_LEFT] && !this->keysPressedBefore[SDL_SCANCODE_LEFT]) this->_moveCursorLeft();
	else if (this->keysPressed[SDL_SCANCODE_RIGHT] && !this->keysPressedBefore[SDL_SCANCODE_RIGHT]) this->_moveCursorRight();
	else if (this->keysPressed[SDL_SCANCODE_BACKSPACE] && !this->keysPressedBefore[SDL_SCANCODE_BACKSPACE]) this->backspace(this->posFirstCharToRender + this->typingCursorPos);
	else if (this->keysPressed[SDL_SCANCODE_DELETE] && !this->keysPressedBefore[SDL_SCANCODE_DELETE]) {
		int pos = this->posFirstCharToRender + this->typingCursorPos;
		if (pos > this->rawText.length()) this->backspace(pos - 1);
		else this->del(pos);
	}

	if (this->textKeysPressed != "") {
		this->_addTextAtCursorPos(this->textKeysPressed);
	}
}

bool TextField::setText(std::string text) {
	if (text.size() < this->maxAllowedCharacters) {
		Label::setText(text);
		this->renderedText = this->getRenderedText();
	}

	else return false;

}
bool TextField::insertText(int position, std::string text) {
	//std::string prev = rawText.substr(0, position);
	//std::string after = rawText.substr(position, rawText.size());
	//prev.append(text);
	//prev.append(after);
	if (position > rawText.size()) position = rawText.size();
	if (position < 0) position = 0;

	std::string newtext = rawText;
	newtext.insert(position, text);
	return this->setText(newtext);

}

void TextField::_addTextAtCursorPos(std::string text) {
	if (this->insertText(this->_getCursorPosinText(), text)) {
		this->_moveCursorRight(text.size());
	}
}

std::string TextField::getRenderedText() {
	if (this->posFirstCharToRender> this->rawText.length()) return "";
	return rawText.substr(this->posFirstCharToRender, this->numCharsToDisplay);
}

std::string TextField::getText() {
	return this->rawText;
}

int TextField::_calculateRenderedTextSize() {
	int rawTextSz = this->rawText.length();
	/*normally the number of characters to display is the amount generated however if
	if the user is at the end of the text in the textbox there will be less characters so the function
	will work out the actual amount*/
	if (this->numCharsToDisplay+this->posFirstCharToRender > rawTextSz) {
		int charsRendered=rawTextSz - this->posFirstCharToRender;
		return charsRendered;
	}
}

bool TextField::backspace(int pos) {
	bool deleted = this->del(pos - 1);
	if (deleted) {
		if (this->posFirstCharToRender != 0) this->setPosFirstCharToRender(this->posFirstCharToRender - 1);
		else  this->_moveCursorLeft();
		return true;
	}
	else return false;
}

bool TextField::del(int pos) {
	if (rawText.size() == 0) return false;
	if (pos < 0) return false;
	if (pos > rawText.size()) return false;
	std::string newtext = rawText;
	newtext.erase(pos,1);
	return this->setText(newtext);
}

int TextField::_getCursorPosinText() {
	return this->posFirstCharToRender + this->typingCursorPos;
}