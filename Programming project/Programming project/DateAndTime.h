#include "simple_renderables.h"
#include <SDL.h>
#include <array>
#include <string>

class NumericTextbox : public TextField {
public:
	int maxNumericValue;
	int minNumericValue;
	NumericTextbox(SDL_Renderer* renderer, int x, int y, int w, int h, int numChars, int maxNumericValue, int minNumericValue, SDL_Color textColor = SDL_Color(0, 0, 0), SDL_Color bgColor = SDL_Color(255, 255, 255)) : TextField(renderer, x, y, w, h, textColor,bgColor, numChars, AllowedChars::ONLY_NUMBERS) {
		this->maxAllowedCharacters = numChars;
		this->maxNumericValue = maxNumericValue;
		this->minNumericValue = minNumericValue;
	}
	bool setText(std::string text) override {
		std::string moddedtext = "";
		if (text.size() <= this->maxAllowedCharacters) {
			for (char ch : text) if (std::isdigit(ch)) moddedtext += ch;

			if (moddedtext.length() != 0) {
				int num = std::stoi(moddedtext);
				if (num > this->maxNumericValue || num<this->minNumericValue) return false;
			}
			this->rawText = moddedtext;
			this->_updateRenderedText();
			return true;
		}
		return false;
	}
};



class TimeInputBox : public Renderable {
private:
	enum TimeSlots {
		HOURS_BIG, HOURS_SMALL, MINUTES_BIG, MINUTES_SMALL
	};
	std::array<NumericTextbox*, 4>  timeTextBox;
	Label* seperator;

public:

	//returns time without colon
	std::string getTime();
	TimeInputBox(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color textColor = SDL_Color(0, 0, 0), SDL_Color bgColor = SDL_Color(255, 255, 255), SDL_Color seperatorColor = SDL_Color(0, 0, 0));

	bool setPos(Vector2 pos) override;

	bool setDims(Vector2 dims) override;

	void render() override;

	void update() override;

	~TimeInputBox();
};