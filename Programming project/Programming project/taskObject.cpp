#include "task_objects.h"
#include <Windows.h>


bool TaskObject::setDims(int x, int y) {
	this->renderScrDims.w = x;
	this->renderScrDims.h = y;
	this->charWidth = this->renderScrDims.w / 70;
	this->charHeight = this->renderScrDims.h / 5;
	this->margin = charWidth;
	int headingWidth;
	this->bg->setDims(x, y);


	this->taskName->setDims({ this->charWidth * 20, this->charHeight });
	headingWidth = this->taskNameHeading->getText().size() * charWidth;
	this->taskNameHeading->setDims(headingWidth, charHeight);

	this->filePath->setDims({ this->charWidth * 20, this->charHeight });
	headingWidth = this->filePathHeading->getText().size() * charWidth;
	this->filePathHeading->setDims({ headingWidth, charHeight });
	this->filePathBrowse->setDims({ charWidth * 8, charHeight * 1.2 });

	this->whenToDoTask->setDims({ charWidth * 20, charHeight });
	this->timeInput->setDims({ charWidth * 10,charHeight });
	this->doTaskHeading->setDims(charWidth * this->doTaskHeading->getText().length(), charHeight);



	this->setPos(this->getPos()); //set pos to set first elements into position to calculate extra args width and position

	int extraArgsX = this->filePathBrowse->getPos().x + this->filePathBrowse->getDims().x - this->renderScrDims.x;
	int extraArgsWidth = this->renderScrDims.w - extraArgsX - (2 * margin);
	int extraArgsCharCount = extraArgsWidth / this->charWidth;
	this->extraArgs->setCharactersPerLine(extraArgsCharCount);
	this->extraArgs->setDims({ extraArgsWidth, this->charHeight });

	int frequencyWidth = this->renderScrDims.w - (this->frequency->getPos().x - this->renderScrDims.x) - margin;
	this->frequency->setDims({ frequencyWidth,charHeight });
	this->frequencyHeading->setDims({ this->frequencyHeading->getText().length() * charWidth, charHeight });

	this->setPos(this->getPos());
	return true;

}

TaskObject::TaskObject(SDL_Renderer* renderer, int x, int y, int w, int h) : Draggable(renderer, x, y, w, h, SDL_Color(191, 191, 191), SDL_Color(110, 110, 110), nullptr , ONLY_Y) {
	this->charWidth = (float)w / 70;
	this->charHeight = h / 5;
	this->margin = charWidth; 
	SDL_Color textColor= { 0,0,0 };
	SDL_Color textBoxBg = { 255, 255, 255 };

	//this would be an array if i had more time

	this->taskName = new TextField(renderer, x, y + (int)(h * 0.5), charWidth * 20, charHeight, textColor,textBoxBg,20);
	this->taskNameHeading = Label::createBasicLabel(renderer, "Task Name:", 0, 0, charWidth, charHeight, textColor);

	this->filePath= new TextField(renderer, taskName->getPos().x+taskName->getDims().y + margin, taskName->getPos().y, charWidth * 20, charHeight, textColor, textBoxBg, 20);
	this->filePath->setMaxAllowedChars(MAX_PATH);
	this->filePathHeading = Label::createBasicLabel(renderer, "Path to file:", 0, 0, charWidth, charHeight, textColor);
	this->filePathBrowse = new Button(renderer, x, y, charWidth * 8, charHeight*1.2, "browse.png", &TaskObject::setFilePath,SDL_FLIP_NONE,this);



	this->extraArgs = new TextField(renderer,x , y, charWidth*20, charHeight, textColor, textBoxBg, 20);
	this->extraArgsHeading = Label::createBasicLabel(renderer, "Extra arguments:", 0, 0, charWidth, charHeight, textColor);

	this->whenToDoTask = new DropDownMenu(renderer, x, y, charWidth * 20, charHeight, { "Immediately", "At set time" });
	this->doTaskHeading = Label::createBasicLabel(renderer, "When To Do: ", 0, 0, charWidth, charHeight, textColor);
	this->timeInput = new TimeInputBox(renderer, 0, 0, charWidth*5, charHeight, SDL_Color{ 0,0,0 }, SDL_Color(255, 255, 255),SDL_Color(255,255,255));

	this->frequencyHeading = Label::createBasicLabel(renderer, "Repeat: ", 0, 0, charWidth, charHeight, textColor);
	this->frequency = new DropDownMenu(renderer, 0, 0, charWidth * 20, charHeight, { "Only once","Daily","Weekly","Monthly","Yearly" });

	this->setPos({ x, y });
	this->setDims( w,h );
}
void TaskObject::render() {
	//this would be a for loop if i had more time
	Draggable::render();
	this->taskNameHeading->render();
	this->taskName->render();

	this->filePath->render();
	this->filePathHeading->render();
	this->filePathBrowse->render();

	this->extraArgs->render();
	this->extraArgsHeading->render();

	this->whenToDoTask->render();
	this->doTaskHeading->render();
	this->frequencyHeading->render();
	this->frequency->render();
	if (this->whenToDoTask->getSelectedItem()=="At set time") this->timeInput->render();

}
void TaskObject::update() {
	//this would be a for loop if i had more time
	this->taskName->update();
	this->filePath->update();
	this->filePathBrowse->update();
	this->extraArgs->update();
	this->whenToDoTask->update();
	this->frequency->update();
	if (this->whenToDoTask->getSelectedItem() == "At set time") this->timeInput->update(); //only display the time input if the user wants to run the task at a specific time
	Draggable::update();
}



bool TaskObject::setPos(Vector2 pos) {
	if (Draggable::setPos(pos)) {
		Vector2 newpos = this->getPos();
		Vector2 center = this->getCenter();
		Vector2 dims = this->getDims();

		int headingRenderY = (float)newpos.y + (dims.y * 0.05);
		int contentRenderingY = headingRenderY + charHeight;
		int renderX = this->getPos().x+margin;

		//this would be a for loop if i had more time
		this->taskNameHeading->setPos(renderX, headingRenderY);
		this->taskName->setPos(renderX, contentRenderingY);
		renderX += this->taskName->getDims().x + margin;

		this->filePath->setPos(renderX, taskName->getPos().y);
		this->filePathHeading->setPos(renderX, headingRenderY);
		renderX += this->filePath->getDims().x;
		this->filePathBrowse->setPos(renderX, contentRenderingY);
		renderX += this->filePathBrowse->getDims().x + margin;

		this->extraArgs->setPos(renderX, contentRenderingY);
		this->extraArgsHeading->setPos(this->extraArgs->getPos().x, headingRenderY);
		renderX += this->extraArgs->getDims().x + margin;

		
		renderX = this->renderScrDims.x+margin;
		contentRenderingY += charHeight * 2;
		this->doTaskHeading->setPos(renderX, contentRenderingY);
		renderX += this->doTaskHeading->getDims().x + margin;
		this->whenToDoTask->setPos(renderX, contentRenderingY);
		renderX += this->whenToDoTask->getDims().x + margin;
		this->timeInput->setPos(renderX, contentRenderingY);
		renderX += this->timeInput->getDims().x + margin;
		
		this->frequencyHeading->setPos(renderX, contentRenderingY);
		renderX += this->frequencyHeading->getDims().x + margin;
		this->frequency->setPos(renderX, contentRenderingY);
		
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

	delete this->whenToDoTask;
	delete this->timeInput;
	delete this->doTaskHeading;

	delete this->frequency;
	delete this->frequencyHeading;

}



void TaskObject::setFilePath() {
	std::wstring data = Main::openFileExplorerLoad();

	this->filePath->setText(std::string(data.begin(),data.end()));
	this->filePath->setPosFirstCharToRender(0);
}

void TaskObject::setName(std::string text) {
	this->taskName->setText(text);
}