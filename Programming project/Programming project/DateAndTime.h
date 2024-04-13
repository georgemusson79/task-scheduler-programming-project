#pragma once
#include "simple_renderables.h"
#include <SDL.h>
#include <array>
#include <string>

//a textbox that only supports numerical input, biggest inputtable number can be set
class NumericTextbox : public TextField {
public:
	int maxNumericValue;
	int minNumericValue;
	NumericTextbox(SDL_Renderer* renderer, int x, int y, int w, int h, int numChars, int maxNumericValue, int minNumericValue, std::string* textInput,SDL_Color textColor = SDL_Color(0, 0, 0), SDL_Color bgColor = SDL_Color(255, 255, 255));
	bool setText(std::string text) override;
};


//4 textboxes that allow the input for a time in 24hr clock format
class TimeInputBox : public Renderable {
private:
	enum TimeSlots {
		HOURS_BIG, HOURS_SMALL, MINUTES_BIG, MINUTES_SMALL
	};
	std::array<NumericTextbox*, 4>  timeTextBox;
	Label* seperator;

public:
	bool fillEmptySlots = false;
	//returns time without colon
	std::string getTime();

	/*Generate a box that supports time inputs in 24 hour clock format
	* \param renderer: Renderer to display output to
	* \param x,y,w,h: position, width and height of the object
	* \param textInput: a pointer to a string containing the text to write into the textbox
	* \param seperatorColor: the color of the colon the seperates the hours and minutes
	*/
	TimeInputBox(SDL_Renderer* renderer, std::string* textInput,int x, int y, int w, int h, SDL_Color textColor = SDL_Color(0, 0, 0), SDL_Color bgColor = SDL_Color(255, 255, 255), SDL_Color seperatorColor = SDL_Color(0, 0, 0));
	
	//Takes a 4 digit numerical string and sets the time if it is valid
	void setTime(std::string time);
	//sets pos of the object, returns true on success otherwise returns false
	bool setPos(Vector2 pos) override;
	bool setPos(int x, int y) override {
		return this->setPos({ x,y });
	}
	//set dimensions of the object, returns true on success otherwise returns false
	bool setDims(Vector2 dims) override;
	//renders the object to renderer, called eevery frame
	void render() override;
	//updates the object state and handles user interaction, called every frame
	void update() override;

	~TimeInputBox();
};

