#pragma once

#include <iostream>
#include <SDL_image.h>
#include <functional>
#include <algorithm>
#include <limits>
#include <SDL_ttf.h>

#include "Main_Functions.h"
#include "Vector2.h"

enum MovementLimitations {
	ANYWHERE, ONLY_X, ONLY_Y
};

class Renderable {
protected:
	bool markForDeletion = false;
	bool focused = false;
	int renderPriority = 0;
	SDL_Renderer* renderer;
	SDL_Rect renderScrDims; //dimensions for rendering the object to the screen 

public:
	//Must be called by every class that inherits Renderable. Sets the renderer
	Renderable(SDL_Renderer* renderer) : renderer(renderer) {}
	std::string name;
	bool moveForwardWhenFocused = true;

	//set on screen position
	virtual bool setPos(Vector2 pos);
	virtual bool setPos(int x, int y) {
		return this->setPos({ x, y });
	}
	Vector2 getPos();
	virtual bool setCenter(Vector2 pos);
	virtual bool setCenter(int x, int y) {
		return this->setCenter({ x,y });
	}
	Vector2 getCenter();

	//set on screen width and height
	virtual bool setDims(Vector2 dims);
	virtual  bool setDims(int w, int h) {
		return this->setDims({ w,h });
	}
	Vector2 getDims();


	//combination of setPos and setDims
	virtual bool setRenderingDims(int x, int y, int w, int h);
	SDL_Rect getRenderingDims();

	//Update object, called every frame
	virtual void update() {}

	//Render object to screen
	virtual void render() = 0;

	//Create an object that inherits Renderable and push it to Main::renderables, returning a pointer to the created object
	template <typename RenderableObj, typename... Args>
	static RenderableObj* create(Args...args) {
		static_assert(std::is_constructible_v<RenderableObj, Args...>, "Unable to construct class with current arguments"); //assert that class can be created with args
		static_assert(std::is_base_of_v<Renderable, RenderableObj>, "RenderableObj is not of type Renderable"); //assert that class base is Renderable
		RenderableObj* obj = new RenderableObj(args...); //dynamically allocate object then append to Main::renderables

		Main::renderables.push_back(obj);

		//sort list by render priority, higher means it will render towards the top of the screen
		std::sort(Main::renderables.begin(), Main::renderables.end(), [](Renderable* a, Renderable* b) {
			return a->getRenderPriority() < b->getRenderPriority();
			});

		return obj;
	}

	int getRenderPriority() {
		return renderPriority;
	}

	virtual ~Renderable() {
		tryRemoveFocus();
		std::cout << "renderable destroyed\n";
	}

	bool trySetFocus();
	bool tryRemoveFocus();

	void destroy();
	bool toBeDestroyed();

};

class Sprite : public Renderable {
protected:
	SDL_Texture* img;
	int textureWidth;
	int textureHeight;
	std::string pathToImg;

public:
	SDL_Rect renderImgDims;
	SDL_RendererFlip flip;


	Sprite(SDL_Renderer* renderer, int x, int y, int w, int h, std::string pathToImg, SDL_RendererFlip flip = SDL_FLIP_NONE);
	SDL_Texture* getTexture();
	void render() override;
	void setImg(std::string pathToImg);
	std::string getPathToImg();
	~Sprite();
};



class Button : public Sprite {
private:
	SDL_Texture* img;
	std::function<void()> fn;
	bool alreadyClicked = false;

public:
	template <typename Fn, typename... Args>
	Button(SDL_Renderer* renderer, int x, int y, int w, int h, std::string pathToImg, Fn function = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE, Args... arguments) : Sprite(renderer, x, y, w, h, pathToImg, flip) {
		this->setFunction(function, arguments...);
		this->renderPriority = 100;
	}

	template <typename Fn, typename... Args>
	void setFunction(Fn function, Args... arguments) {
		if (function != NULL) {
			static_assert(std::is_invocable_v<Fn, Args...>, "Function is not callable with provided arguments");
			auto f = std::bind(function, arguments...);
			fn = f;
		}

	}


	void update() override;
	virtual void onHover() {} //runs when the cursor is hovered over the button
	void onClick() {
		this->fn();
	}
};

class Draggable : public Renderable {
protected:
	Vector2 dragPosition = { 0,0 }; //position where the cursor picked up the object
	bool clicked = false;
	SDL_Rect movementBounds; //area the object can be moved inside
	Renderable* bg = nullptr;

public:
	MovementLimitations movementLimits;

	Draggable(SDL_Renderer* renderer, int x, int y, int w, int h, std::string pathToImg, SDL_Rect* movementBounds = nullptr, MovementLimitations movementLimits = ANYWHERE, SDL_RendererFlip flip = SDL_FLIP_NONE);
	Draggable(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color color, SDL_Color borderColor, SDL_Rect* movementBounds = nullptr, MovementLimitations movementLimits = ANYWHERE, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void render() override;
	void update() override;
	~Draggable();

	bool setPos(Vector2 pos) override;
};

class Rectangle : public Renderable {
public:
	bool fill = false;
	SDL_Color colour;
	void render();
	Rectangle(SDL_Renderer* renderer, int x, int y, int w, int h, bool fill, SDL_Color colour = { 255,255,255,255 });
};

//creates a read only block of text on the screen the size of renderingDims
class Label : public Renderable {
protected:
	SDL_Color textColor;
	SDL_Texture* textTexture = nullptr;
	std::string rawText = "";
	Renderable* bg = nullptr;
	std::string fontPath;
	TTF_Font* font = NULL;
	int charsPerLine;
public:

	//create a label with text all on one line but each character is the specified width and height for the generated text
	static Label* createBasicLabel(SDL_Renderer* renderer, std::string text, int x, int y, int charW, int charH, SDL_Color textColor, std::string pathToBg, std::string pathToFont = "");
	static Label* createBasicLabel(SDL_Renderer* renderer, std::string text, int x, int y, int charW, int charH, SDL_Color textColor, std::string pathToFont = "");
	static Label* createBasicLabel(SDL_Renderer* renderer, std::string text, int x, int y, int charW, int charH, SDL_Color textColor, SDL_Color bgColor, std::string pathToFont = "");

	bool setRenderingDims(int x, int y, int w, int h) override;
	virtual bool setText(std::string text);
	virtual void setFont(std::string fontPath, int fontSz = 72);
	Label(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color textColor, std::string pathToBg, int charsPerLine = 0, std::string pathToFont = "");
	Label(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color textColor, SDL_Color bgColor, int charsPerLine = 0, std::string pathToFont = "");
	Label(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color textColor, int charsPerLine = 0, std::string pathToFont = "");
	~Label();
	SDL_Texture* getTextTexture() {
		return this->textTexture;
	}
	std::string getText() {
		return this->rawText;
	}

	bool setPos(int x, int y)  override {
		return this->setPos({ x, y });
	}

	bool setPos(Vector2 pos) override;
	virtual void render() override;
	virtual void setTextColor(SDL_Color color);
	void setBgColor(SDL_Color color);
	void updateBgImage(std::string pathToBg);
	virtual void setCharactersPerLine(int chars);

};

class TextField : public Label {
protected:
	int highlightTxtBegin = -1; //unused
	int highlightTxtEnd = -1; //unused


	int _heldDownKeyPressInterval = 30; //when held down, a functional key will do its action every x milliseconds eg backspace will backspace every 50ms when held down
	int _heldDownKeyPressLast = 0;
	int _timeSinceLastFnKeyPress = 0;


	int maxAllowedCharacters = 500;
	std::string renderedText = "";
	int typingCursorPos = 0;
	SDL_Color cursorColor = { 0,0,0,255 };
	int numCharsToDisplay = 9;
	int posFirstCharToRender = 0;
	std::vector<Uint8> keysPressedBefore = {};
	std::vector<Uint8> keysPressed = {};
	std::string* _textKeysPressed;
	float pxPerCharacter; //the pixel width of each character (all characters will be the same width)

	//called in all textfield constructors
	void _construct(int maxCharsToDisplay, std::string* textKeys);
	void handleFnKeysSinglePress();
	void handleFnKeysHeldDown();
	//update keysPressed with the keys pressed this frame and keysPressedBefore with keys pressed last frame
	void _updateKeysPressed();
	void _moveCursorLeft(int times = 1);
	void _moveCursorRight(int times = 1);
	void _generateTypingCursor();
	void _handleKBInput();
	void _addTextAtCursorPos(std::string text);
	int _calculateRenderedTextSize();

	void _paste();

	//unused
	void _highlightText(int begin, int end);


	//return where the cursor is in the rawtext string
	int _getCursorPosinText();
public:
	TextField(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color textColor, std::string pathToBg, int maxCharsToDisplay, std::string pathToFont = "", std::string* textKeys = &Main::textInputThisFrame);
	TextField(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color textColor, SDL_Color bgColor, int maxCharsToDisplay, std::string pathToFont = "", std::string* textKeys = &Main::textInputThisFrame);
	TextField(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color textColor, int maxCharsToDisplay, std::string pathToFont = "", std::string* textKeys = &Main::textInputThisFrame);
	~TextField();
	void render() override;
	void update() override;


	void setMaxAllowedChars(int chars);
	//max amount of characters that can be shown in the text field at a time
	void setCharactersPerLine(int chars) override;

	//set the position of the first character in rawText to begin rendering from
	void setPosFirstCharToRender(int first);

	bool setText(std::string text) override;
	bool insertText(int position, std::string text);
	bool backspace(int pos);
	bool del(int pos);

	std::string getRenderedText();

};


//similar to label except characters all have the height of heightOfChar
//width is determined by the width of each character in the font which is then scaled to be proportional to heightofChar

class LabelFixedCharDims : public Label {
protected:
	int heightOfChar;
	std::vector<std::string> _splitTextByLine(std::string text);
	std::vector<std::string> _splitText(std::string text, int interval);
public:
	LabelFixedCharDims(SDL_Renderer* renderer, int x, int y, int heightOfChar, std::string text, SDL_Color textColor, std::string pathToBg, int charsPerLine = 0, std::string pathToFont = "");
	LabelFixedCharDims(SDL_Renderer* renderer, int x, int y, int heightOfChar, std::string text, SDL_Color textColor, SDL_Color bgColor, int charsPerLine = 0, std::string pathToFont = "");
	LabelFixedCharDims(SDL_Renderer* renderer, int x, int y, int heightOfChar, std::string text, SDL_Color textColor, int charsPerLine = 0, std::string pathToFont = "");
	bool setText(std::string text) override;
};