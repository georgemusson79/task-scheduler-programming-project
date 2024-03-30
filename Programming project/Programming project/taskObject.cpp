#include "task_objects.h"

TaskObject::TaskObject(SDL_Renderer* renderer, int x, int y, int w, int h) : Draggable(renderer, x, y, w, h, SDL_Color(191, 191, 191), SDL_Color(110, 110, 110), nullptr, ONLY_Y) {
	int charWidth = w / 60;
	int charHeight = h / 3.5;
	std::cout << "char" << charHeight;
	//this->taskNameHeading = new LabelFixedCharDims(renderer, x, y, charHeight, "Task Name:", SDL_Color(50, 50, 50));
	this->taskNameHeading = Label::createBasicLabel(renderer, "Task Name:", 0, 0, charWidth, charHeight, SDL_Color(255, 255, 255));
	//this->taskNameHeading = new Label(l);
	this->setPos({ x, y });
}
void TaskObject::render() {
	Draggable::render();
	if (this->taskNameHeading != nullptr) {
		this->taskNameHeading->render();
	}

}
void TaskObject::update() {
	Draggable::update();
}

bool TaskObject::setPos(Vector2 pos) {
	if (Draggable::setPos(pos)) {
		Vector2 newpos = this->getPos();
		Vector2 center = this->getCenter();
		Vector2 dims = this->getDims();
		int headingRenderHeight = newpos.y + (dims.y * 0.2);
		this->taskNameHeading->setPos(newpos.x + (dims.x * 0.1), headingRenderHeight);


		return true;
	}
	return false;
}

TaskObject::~TaskObject() {
	delete this->taskNameHeading;
}