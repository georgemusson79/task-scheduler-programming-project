#pragma once
#include "simple_renderables.h"
#include <SDL.h>
#include <array>
#include <string>

class NumericTextbox : public TextField {
public:
	int maxNumericValue;
	int minNumericValue;
	NumericTextbox(SDL_Renderer* renderer, int x, int y, int w, int h, int numChars, int maxNumericValue, int minNumericValue, SDL_Color textColor = SDL_Color(0, 0, 0), SDL_Color bgColor = SDL_Color(255, 255, 255));
	bool setText(std::string text) override;
};



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
	TimeInputBox(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color textColor = SDL_Color(0, 0, 0), SDL_Color bgColor = SDL_Color(255, 255, 255), SDL_Color seperatorColor = SDL_Color(0, 0, 0));
	void setTime(std::string time);
	bool setPos(Vector2 pos) override;
	bool setPos(int x, int y) override {
		return this->setPos({ x,y });
	}

	bool setDims(Vector2 dims) override;

	void render() override;

	void update() override;

	~TimeInputBox();
};

