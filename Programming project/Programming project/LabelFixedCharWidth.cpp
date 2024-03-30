#include "simple_renderables.h"
#include <string>
#include <sstream>
#include <iostream>

LabelFixedCharDims::LabelFixedCharDims(SDL_Renderer* renderer, int x, int y, int heightOfChar, std::string text, SDL_Color textColor, std::string pathToBg, int charsPerLine, std::string pathToFont) : Label(renderer, x, y, 0, 0, textColor, pathToBg, charsPerLine, pathToFont) {
	this->heightOfChar = heightOfChar;
	this->setText(text);
}

LabelFixedCharDims::LabelFixedCharDims(SDL_Renderer* renderer, int x, int y, int heightOfChar, std::string text, SDL_Color textColor, SDL_Color bgColor, int charsPerLine, std::string pathToFont) : Label(renderer, x, y, 0, 0, textColor, bgColor, charsPerLine, pathToFont) {
	this->heightOfChar = heightOfChar;
	this->setText(text);

}

LabelFixedCharDims::LabelFixedCharDims(SDL_Renderer* renderer, int x, int y, int heightOfChar, std::string text, SDL_Color textColor, int charsPerLine, std::string pathToFont) : Label(renderer, x, y, 0, 0, textColor, charsPerLine, pathToFont) {
	this->heightOfChar = heightOfChar;
	this->setText(text);

}

bool LabelFixedCharDims::setText(std::string text) {
	SDL_DestroyTexture(this->textTexture);
	this->textTexture = nullptr;
	if (text != "") {
		int biggestW = 0;
		int h = 0;
		std::vector<std::string> lines = this->_splitText(text, this->charsPerLine);

		//get biggest line width in pixels and use that as the width of the rendering rectangle
		for (auto line : lines) {
			int w;
			TTF_SizeText(this->font, line.c_str(), &w, &h);
			if (w > biggestW) biggestW = w;
		}

		//scale the size of the text so it has the same height as the inputted character height
		float scaleFactor = (float)this->heightOfChar / h;
		this->renderScrDims.w = biggestW * scaleFactor;


		int lineCount = (this->charsPerLine != 0) ? std::ceil((double)text.length() / this->charsPerLine) : 1; //if charsPerLine is 0 then there is only one line
		//increase height further in case new lines are created with \n
		int extraLineCount = this->_splitTextByLine(text).size() - 1;
		lineCount += extraLineCount;

		int heightOfRectangle = this->heightOfChar * lineCount;
		this->renderScrDims.h = heightOfRectangle;
		return Label::setText(text);
	}
}

std::vector<std::string> LabelFixedCharDims::_splitText(std::string text, int interval) {
	std::vector<std::string> split = {};
	if (interval == 0) split = { text };
	else {
		for (int i = 0; i < text.length(); i += interval) {
			std::string str = text.substr(i, interval);
			split.push_back(str);
		}
	}
	return split;
}

std::vector<std::string> LabelFixedCharDims::_splitTextByLine(std::string text) {
	std::vector<std::string> split = {};
	std::string line;
	std::stringstream str(text);
	while (std::getline(str, line)) split.push_back(line);
	return split;


}