#include "simple_renderables.h"
#include "Cursor.h"
#include "Collision.h"
#include "Main_Functions.h"
#include "Utils.h"

#include <string>

void TextField::_construct(int maxCharsToDisplay, std::string* textKeys, AllowedChars allowedChars, AllowedCase allowedCase) {
	this->setCharactersPerLine(maxCharsToDisplay);
	this->renderPriority = 10;
	this->name = "textField";
	this->_textKeysPressed = textKeys;
	this->allowedCase = allowedCase;
	this->allowedChars = allowedChars;
}

TextField::TextField(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color textColor, std::string pathToBg, int maxCharsToDisplay, AllowedChars allowedChars, AllowedCase allowedCase, std::string pathToFont, std::string* textKeys) : Label(renderer, x, y, w, h, textColor, pathToBg, 0, pathToFont) {
	this->_construct(maxCharsToDisplay, textKeys, allowedChars, allowedCase);
}

bool TextField::setDims(Vector2 dims) {
	Label::setDims(dims);
	this->setCharactersPerLine(this->numCharsToDisplay);
	return true;
}

TextField::TextField(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color textColor, SDL_Color bgColor, int maxCharsToDisplay, AllowedChars allowedChars, AllowedCase allowedCase,std::string pathToFont, std::string* textKeys) : Label(renderer, x, y, w, h, textColor, bgColor, 0, pathToFont) {
	this->_construct(maxCharsToDisplay, textKeys,allowedChars,allowedCase);
}

TextField::TextField(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color textColor, int maxCharsToDisplay, AllowedChars allowedChars, AllowedCase allowedCase, std::string pathToFont, std::string* textKeys) : Label(renderer, x, y, w, h, textColor, 0, pathToFont) {
	this->_construct(maxCharsToDisplay, textKeys, allowedChars, allowedCase);
}

TextField::~TextField() {

}

void TextField::setCharactersPerLine(int chars) {
	this->numCharsToDisplay = chars;

	//adjust the pixel width of each character so that the width doesnt change as you add or remove characters
	this->pxPerCharacter = this->renderScrDims.w / chars;
}

void TextField::setMaxAllowedChars(int chars) {
	if (chars < 0) chars = 0;
	this->maxAllowedCharacters = chars;
	if (this->rawText.length() > chars) this->setText(rawText.substr(0, chars));
}

void TextField::_updateKeysPressed() {
	this->keysPressedBefore = keysPressed;
	int szKeys;
	const Uint8* keys = SDL_GetKeyboardState(&szKeys);
	keysPressed = std::vector(keys, keys + szKeys);
}

void TextField::_paste() {
	std::string text = SDL_GetClipboardText();
	this->_addTextAtCursorPos(text);
}


void TextField::_highlightText(int begin, int end) {

	this->highlightTxtBegin = std::min(begin, end);
	this->highlightTxtEnd = std::max(begin, end);

	if (this->highlightTxtEnd > this->rawText.length()) this->highlightTxtEnd = this->rawText.length();
	if (this->highlightTxtBegin < 0) this->highlightTxtBegin = 0;
}


void TextField::render() {
	if (bg != nullptr) bg->render();
	if (this->textTexture != nullptr) {
		SDL_Rect renderedCharDims = this->renderScrDims;
		renderedCharDims.w = this->renderedText.size() * this->pxPerCharacter;
		SDL_RenderCopy(renderer, this->textTexture, NULL, &renderedCharDims);
	}
	if (this->focused) {
		//get where the typing cursor should be based on where it is in the string of rendered characters
		int startOfTextbox = this->renderScrDims.x;
		int typingCursorRenderX = startOfTextbox + (this->typingCursorPos * this->pxPerCharacter);

		//draw the cursor to the screen
		Uint8 r, g, b, a;
		SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
		SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);
		SDL_RenderDrawLine(renderer, typingCursorRenderX, this->renderScrDims.y, typingCursorRenderX, this->renderScrDims.y + this->renderScrDims.h);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
	}
}

void TextField::setPosFirstCharToRender(int first) {
	if (first < 0) first = 0;
	//else if (first >= rawText.length()-this->numCharsToDisplay) first = rawText.length()-this->numCharsToDisplay;
	else if (first > (int)rawText.length() - this->numCharsToDisplay && first >= this->posFirstCharToRender) first = rawText.length() - this->numCharsToDisplay;
	else this->posFirstCharToRender = first;
	this->_updateRenderedText();
}

void TextField::update() {
	bool cursorCollides = Collision::collidesWith(this->renderScrDims, Cursor::getPos());

	bool oldCursorIsClicked = this->cursorIsClicked;
	this->cursorIsClicked = Cursor::isLeftClicked();
	bool cursorIsTapped = (this->cursorIsClicked && !oldCursorIsClicked);

	if (cursorCollides) {
		Cursor::setCursor(SDL_SYSTEM_CURSOR_IBEAM);
		if (cursorIsTapped) {
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
	int cursorxRelativeToBox = cursorPos.x - this->renderScrDims.x;


	this->typingCursorPos = std::round(cursorxRelativeToBox / this->pxPerCharacter);
	if (this->typingCursorPos > this->renderedText.size()) this->typingCursorPos = this->renderedText.size();
}

void TextField::setCursorPos(int pos) {
	if (pos < 0) pos = 0;
	else if (pos > this->maxAllowedCharacters) pos = maxAllowedCharacters;
	this->typingCursorPos = pos;
}

void TextField::_moveCursorLeft(int times) {
	if (this->typingCursorPos > 0) this->typingCursorPos -= times;
	else {
		this->setPosFirstCharToRender(this->posFirstCharToRender - times);
	}

}
void TextField::_moveCursorRight(int times) {
	if (this->typingCursorPos + times <= this->renderedText.size()) this->typingCursorPos += times;
	else if (this->renderedText.size() == 0) return;
	else {
		this->setPosFirstCharToRender(this->posFirstCharToRender + times);
		this->typingCursorPos = this->renderedText.size();
	}
}

void TextField::_handleKBInput() {
	this->_updateKeysPressed();
	bool pressOnce = true;


	const SDL_Scancode functionalKeys[] = { SDL_SCANCODE_LEFT ,SDL_SCANCODE_RIGHT, SDL_SCANCODE_BACKSPACE,SDL_SCANCODE_DELETE };
	for (auto key : functionalKeys) {
		if (this->keysPressed[key] && !this->keysPressedBefore[key]) this->_timeSinceLastFnKeyPress = SDL_GetTicks();
		else if (this->keysPressed[key] && this->keysPressedBefore[key] && SDL_GetTicks() - this->_timeSinceLastFnKeyPress > 400) pressOnce = false;
	}

	if (pressOnce) this->handleFnKeysSinglePress();
	else this->handleFnKeysHeldDown();

	if (*this->_textKeysPressed != "") this->_addTextAtCursorPos(*this->_textKeysPressed);

	//paste text if user presses ctrl + v
	if (this->keysPressed[SDL_SCANCODE_LCTRL] && this->keysPressed[SDL_SCANCODE_V] && (!this->keysPressedBefore[SDL_SCANCODE_LCTRL] || !this->keysPressedBefore[SDL_SCANCODE_V])) this->_paste();
}

bool TextField::setText(std::string text) {
	std::string moddedtext = "";
	if (text.size() <= this->maxAllowedCharacters) {
		for (char ch : text) {
			switch (this->allowedChars) {
				case AllowedChars::ONLY_NUMBERS:
					if (std::isdigit(ch)) moddedtext += ch;
					break;
				case AllowedChars::ONLY_LETTERS:
					if (std::isalpha(ch)) moddedtext += ch;
					break;
				case AllowedChars::ONLY_PUNCTUATION:
					if (std::ispunct(ch)) moddedtext += ch;
					break;
				default:
					moddedtext.push_back(ch);
					break;
			}
		}

		switch (this->allowedCase) {
			case (AllowedCase::ONLY_UPPER):
				moddedtext = Utils::toUpper(moddedtext);
				break;
			case (AllowedCase::ONLY_LOWER):
				moddedtext = Utils::toLower(moddedtext);
				break;
		}

		this->rawText = moddedtext;
		this->setPosFirstCharToRender(this->posFirstCharToRender);
	}

	else return false;

}
bool TextField::insertText(int position, std::string text) {
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
	if (this->posFirstCharToRender > this->rawText.length()) return "";
	return rawText.substr(this->posFirstCharToRender, this->numCharsToDisplay);
}


int TextField::_calculateRenderedTextSize() {
	int rawTextSz = this->rawText.length();
	/*normally the number of characters to display is the amount generated however if
	if the user is at the end of the text in the textbox there will be less characters so the function
	will work out the actual amount*/
	if (this->numCharsToDisplay + this->posFirstCharToRender > rawTextSz) {
		int charsRendered = rawTextSz - this->posFirstCharToRender;
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
	newtext.erase(pos, 1);
	return this->setText(newtext);
}

int TextField::_getCursorPosinText() {
	return this->posFirstCharToRender + this->typingCursorPos;
}

void TextField::_updateRenderedText() {
	std::string temp = this->rawText;
	this->renderedText = temp.substr(this->posFirstCharToRender, this->numCharsToDisplay);
	Label::setText(this->renderedText);
	this->rawText = temp;
}

void TextField::handleFnKeysSinglePress() {
	if (this->keysPressed[SDL_SCANCODE_LEFT] && !this->keysPressedBefore[SDL_SCANCODE_LEFT]) this->_moveCursorLeft();
	else if (this->keysPressed[SDL_SCANCODE_RIGHT] && !this->keysPressedBefore[SDL_SCANCODE_RIGHT]) this->_moveCursorRight();
	else if (this->keysPressed[SDL_SCANCODE_BACKSPACE] && !this->keysPressedBefore[SDL_SCANCODE_BACKSPACE]) this->backspace(this->posFirstCharToRender + this->typingCursorPos);
	else if (this->keysPressed[SDL_SCANCODE_DELETE] && !this->keysPressedBefore[SDL_SCANCODE_DELETE]) {
		int pos = this->posFirstCharToRender + this->typingCursorPos;
		if (pos > this->rawText.length()) this->backspace(pos - 1);
		else this->del(pos);
	}
}


void TextField::handleFnKeysHeldDown() {
	if (SDL_GetTicks() - this->_heldDownKeyPressLast < this->_heldDownKeyPressInterval) return;

	if (this->keysPressed[SDL_SCANCODE_LEFT]) this->_moveCursorLeft();
	else if (this->keysPressed[SDL_SCANCODE_RIGHT]) this->_moveCursorRight();
	else if (this->keysPressed[SDL_SCANCODE_BACKSPACE]) this->backspace(this->posFirstCharToRender + this->typingCursorPos);
	else if (this->keysPressed[SDL_SCANCODE_DELETE]) {
		int pos = this->posFirstCharToRender + this->typingCursorPos;
		if (pos > this->rawText.length()) this->backspace(pos - 1);
		else this->del(pos);
	}

	this->_heldDownKeyPressLast = SDL_GetTicks();

}