#include "task_objects.h"
#include <Windows.h>

TaskObject::TaskObject(SDL_Renderer* renderer, int x, int y, int w, int h) : Draggable(renderer, x, y, w, h, SDL_Color(191, 191, 191), SDL_Color(110, 110, 110), nullptr, ONLY_Y) {
	int charWidth = (float)w / 70;
	int charHeight = h / 3.5;
	int margin = charWidth; //distance between textboxes
	std::cout << "char" << charHeight;
	SDL_Color textColor={ 0,0,0 };
	SDL_Color textBoxBg(255, 255, 255);
	
	this->taskName = new TextField(renderer, 0, y + (int)(h * 0.5), charWidth * 20, charHeight, textColor,textBoxBg,20);
	this->taskNameHeading = Label::createBasicLabel(renderer, "Task Name:", 0, 0, charWidth, charHeight, textColor);

	this->filePath= new TextField(renderer, taskName->getPos().x+taskName->getDims().y + margin, taskName->getPos().y, charWidth * 20, charHeight, textColor, textBoxBg, 20);
	this ->filePath->setMaxAllowedChars(MAX_PATH);
	this->filePathHeading = Label::createBasicLabel(renderer, "Path to file:", 0, 0, charWidth, charHeight, textColor);
	this->filePathBrowse = new Button(renderer, this->getPos().x, this->getPos().y, charWidth * 8, charHeight*1.2, "browse.png", TaskObject::staticSetFilePath,SDL_FLIP_NONE,this);
	//Button(renderer, this->getPos().x, this->getPos().y, charWidth * 6, charHeight, "browse.png", setFilePath, SDL_FLIP_NONE, this);

	this->extraArgs = new TextField(renderer, taskName->getPos().x + taskName->getDims().y + margin, taskName->getPos().y, charWidth * 20, charHeight, textColor, textBoxBg, 20);
	this->extraArgsHeading = Label::createBasicLabel(renderer, "Extra arguments:", 0, 0, charWidth, charHeight, textColor);


	
	this->setPos({ x, y });
}
void TaskObject::render() {
	Draggable::render();
	this->taskNameHeading->render();
	this->taskName->render();

	this->filePath->render();
	this->filePathHeading->render();
	this->filePathBrowse->render();

	this->extraArgs->render();
	this->extraArgsHeading->render();

}
void TaskObject::update() {
	this->taskName->update();
	this->filePath->update();
	this->filePathBrowse->update();
	Draggable::update();
}

bool TaskObject::setPos(Vector2 pos) {
	if (Draggable::setPos(pos)) {
		Vector2 newpos = this->getPos();
		Vector2 center = this->getCenter();
		Vector2 dims = this->getDims();
		int headingRenderHeight = newpos.y + (dims.y * 0.2);
		int charWidth = this->getDims().x / 60;
		int charHeight = this->getDims().y / 3.5;
		int margin = charWidth; //distance between textboxes

		this->taskNameHeading->setPos(newpos.x + (dims.x * 0.05), headingRenderHeight);
		this->taskName->setPos(this->taskNameHeading->getPos().x, this->taskNameHeading->getPos().y + (this->taskNameHeading->getDims().y));
		
		this->filePath->setPos(taskName->getPos().x + taskName->getDims().x + margin, taskName->getPos().y);
		this->filePathHeading->setPos(this->filePath->getPos().x, this->filePath->getPos().y - this->filePath->getDims().y);
		this->filePathBrowse->setPos(filePath->getPos().x + filePath->getDims().x, filePath->getPos().y);

		this->extraArgs->setPos(filePathBrowse->getPos().x + filePathBrowse->getDims().x + margin, filePathBrowse->getPos().y);
		this->extraArgsHeading->setPos(this->extraArgs->getPos().x, this->extraArgs->getPos().y - this->extraArgs->getDims().y);
		


		return true;
	}
	return false;
}

TaskObject::~TaskObject() {
	delete this->taskNameHeading;
	delete this->taskName;

	delete this->filePath;
	delete this->filePathHeading;
	delete this->filePathBrowse;

	delete this->extraArgs;
	delete this->extraArgsHeading;

}

void TaskObject::staticSetFilePath(TaskObject* object) {
	std::wstring data = Main::openFileExplorerLoad();
	if (data!=L"") object->setFilePath(std::string(data.begin(), data.end()));
}

void TaskObject::setFilePath(std::string data) {
	this->filePath->setText(data);
}