#include "DateAndTime.h"

std::string TimeInputBox::getTime() {
	std::string time = "";
	for (NumericTextbox* t : timeTextBox) if (t->getText().empty()) t->setText("0"); //fill empty slots with 0s
	for (NumericTextbox* t : timeTextBox) time += t->getText();
	return time;

}

TimeInputBox::TimeInputBox(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color textColor, SDL_Color bgColor, SDL_Color seperatorColor) : Renderable(renderer) {
	for (int i = 0; i < 4; i++) this->timeTextBox[i] = new NumericTextbox(renderer, x, y, 10, 10, 1, 9, 0, textColor, bgColor);
	seperator = Label::createBasicLabel(renderer, ":", x, y, w / 5, h, textColor);
	this->timeTextBox[HOURS_BIG]->maxNumericValue = 2;
	this->timeTextBox[MINUTES_BIG]->maxNumericValue = 5;
	this->setDims({ w,h });
}

bool TimeInputBox::setPos(Vector2 pos) {
	this->renderScrDims.x = pos.x;
	this->renderScrDims.y = pos.y;
	int singleWidth = (float)this->renderScrDims.w / 5;
	int renderX = pos.x;


	timeTextBox[0]->setPos({ renderX,pos.y });
	renderX += singleWidth;
	timeTextBox[1]->setPos({ renderX,pos.y });
	renderX += singleWidth;

	seperator->setPos({ renderX, pos.y });
	renderX += singleWidth;

	timeTextBox[2]->setPos({ renderX,pos.y });
	renderX += singleWidth;
	timeTextBox[3]->setPos({ renderX,pos.y });
	return true;
}

bool TimeInputBox::setDims(Vector2 dims) {
	this->renderScrDims.w = dims.x;
	this->renderScrDims.h = dims.y;
	int singleWidth = dims.x / 5;
	for (NumericTextbox* t : timeTextBox) t->setDims({ singleWidth, dims.y });
	seperator->setDims({ singleWidth,dims.y });
	this->setPos(this->getPos());
	return true;
}

void TimeInputBox::render() {
	for (NumericTextbox* t : this->timeTextBox) t->render();
	this->seperator->render();
}

void TimeInputBox::update() {
	for (NumericTextbox* t : this->timeTextBox) t->update();
	if (this->timeTextBox[this->HOURS_BIG]->getText() == "2") {
		this->timeTextBox[this->HOURS_SMALL]->maxNumericValue = 3;
		if (this->timeTextBox[this->HOURS_SMALL]->getText().size() > 0 && std::stoi(this->timeTextBox[this->HOURS_SMALL]->getText()) > 3) this->timeTextBox[this->HOURS_SMALL]->setText("0");
	}
	else this->timeTextBox[this->HOURS_SMALL]->maxNumericValue = 9;
}

TimeInputBox::~TimeInputBox() {
	for (NumericTextbox* t : this->timeTextBox) delete t;
	delete seperator;
}