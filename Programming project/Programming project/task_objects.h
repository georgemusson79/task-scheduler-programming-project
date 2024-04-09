#pragma once
#include "DateAndTime.h"
#include "simple_renderables.h"
#define MAX_PATH          260

class TaskObject : public Draggable {
private:

	Label* frequencyHeading = nullptr;
	DropDownMenu* frequency = nullptr;

	TimeInputBox* timeInput = nullptr;
	DropDownMenu* whenToDoTask = nullptr;
	Label* doTaskHeading = nullptr;

	Label* taskNameHeading = nullptr;
	TextField* taskName = nullptr;

	Label* filePathHeading = nullptr;
	TextField* filePath = nullptr;
	Button* filePathBrowse = nullptr;

	Label* extraArgsHeading = nullptr;
	TextField* extraArgs = nullptr;
	int charWidth;
	int charHeight;
	int margin; //distance between textboxes
	bool isInPlace = false;
	bool highlighted = false;
	void _create();
public:

	/*
	* Get if a task's parameters are all valid
	* \return Returns an empty vector if all parameters are valid otherwise returns string list of errors
	*/
	std::vector<std::string> checkValidityOfTask();
	std::string getTaskName();
	std::string getExtraArgs();
	std::string getProgramPath();
	std::string getInputtedTime();
	std::string getFrequency();
	std::string getWhenToRun();

	TaskObject(SDL_Renderer* renderer, int x, int y, int w, int h);
	~TaskObject();
	void setName(std::string text);
	bool setDims(int x, int y) override;
	void setFilePath();

	void setFilePathStr(std::string path);
	void setExtraArgs(std::string args);
	void setTime(std::string time);
	void setFrequency(std::string item);
	void setWhenToRun(std::string item);

	void render() override;
	void update() override;
	bool setPos(Vector2 pos) override;
	bool getHightlighted() {
		return this->highlighted;
	}

};


class TaskList : public Renderable {
protected:
	//get where a task should be on the screen, takes an array index relative to this->tasks as input
	TaskObject* selectedTask = nullptr;
	int selectedTaskPos = 0;

	Vector2 getTaskPixelPosition(int pos);
	int getNearestIndexFromYPos(int yPos);
	int getTaskHeight() {
		return this->smallerbox->getDims().x / this->tasksOnScreen;
	}
	std::string nextTaskName = "Task #1";
	std::string getNameNextTask();
	std::vector<Button*> buttons = {};
	float posFirstTaskToRender = 0;
	int tasksOnScreen;
	std::vector<TaskObject*> tasks = {};
	Rectangle* biggerbox = nullptr;
	Rectangle* smallerbox = nullptr;
	void moveTask(int first, int second);
	void handleKBInput();
	void scrollDown();
	void scrollUp();
	int checkInvalidThenDisplayErr();
public:


	TaskList(SDL_Renderer* renderer, int x, int y, int w, int h, int tasksOnScreen = 4);
	~TaskList();
	TaskObject* addTask();
	void removeTaskFromEnd();
	void updateTaskPositions();
	void render() override;
	void update() override;
	bool exportCurrentTaskList();
	std::string convertToExportableFormat();
	bool importTaskList();
};