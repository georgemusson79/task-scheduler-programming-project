#pragma once
#include "DateAndTime.h"
#include "simple_renderables.h"
#include <chrono>
#define MAX_PATH          260

class Task {
public:
	std::string taskName;
	std::string time = "";
	std::string frequency;
	std::string fileName;
	std::string extraArgs;
	std::string whenToRun;
	std::string date;

	Task(std::string taskName,
		std::string frequency,
		std::string fileName,
		std::string extraArgs,
		std::string whenToRun,
		std::string time);

	Task(std::string taskName,
		std::string frequency,
		std::string fileName,
		std::string extraArgs,
		std::string whenToRun,
		std::string time,
		std::string date);

	std::vector<std::string> isValid();
	bool execute(bool raiseErrorOnFail);
	

	bool operator==(const Task& other) const {
		return taskName == other.taskName &&
			time == other.time &&
			frequency == other.frequency &&
			fileName == other.fileName &&
			extraArgs == other.extraArgs &&
			whenToRun == other.whenToRun;
	};

	std::string convertToExportableFormat();
	std::time_t timeAndDateTotime_t();
};


//GUI object containing a number of text boxes and drop down menus for inputting the options for when to run
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

	//opens a file explorer window allowing the user to select a file whose path will be put into the filepath textbox
	void setFilePath();

	Task convertToRunnableTask();

	//path will be put into the filepath textbox
	void setFilePathStr(std::string path);

	//fill the extra args textbox with specified args
	void setExtraArgs(std::string args);

	//fill the InputTime box with the specified time as long as it's valid
	void setTime(std::string time);

	//set the frequency drop down menu to the titem as long as it's valid
	void setFrequency(std::string item);
	//set the whenToRun drop down menu to the titem as long as it's valid

	void setWhenToRun(std::string item);

	void render() override;
	void update() override;
	bool setPos(Vector2 pos) override;

	//get if cursor has clicked on this
	bool getHightlighted() {
		return this->highlighted;
	}

};


//gui object containing a list of task objects as well as methods for executing tasks
class TaskList : public Renderable {
protected:
	RenderableRect* biggerbox = nullptr;
	RenderableRect* smallerbox = nullptr;
	//get where a task should be on the screen, takes an array index relative to this->tasks as input
	TaskObject* selectedTask = nullptr;
	int selectedTaskPos = 0;

	Vector2 getTaskPixelPosition(int pos);
	//convert the pixel position of a task into an array index relating to this->tasks
	int getNearestIndexFromYPos(int yPos);
	//get task height in pixels
	int getTaskHeight() {
		return this->smallerbox->getDims().x / this->tasksOnScreen;
	}
	std::string nextTaskName = "Task #1";
	//get what the next task will be called, as more tasks are added they will go up Task #1, Task #2, Task #3 etc..;
	std::string getNameNextTask();
	std::vector<Button*> buttons = {};
	float posFirstTaskToRender = 0;
	int tasksOnScreen;
	std::vector<TaskObject*> tasks = {};

	//move a task into another part of the this->tasks array
	void moveTask(int first, int second);

	//handle keyboard input, if delete is pressed while a task is selected itll be deleted
	void handleKBInput();

	//if there are more tasks than can be rendered to screen the displayed tasks will go up/down onto/off the screen
	void scrollDown();
	void scrollUp();

	//if the tasks have problems, such as no file name, a file path that doesnt exist or no time set this will display a windows error message explaining what happened
	int checkInvalidThenDisplayErr();
public:
	bool raiseErrorOnFail = true; //if true, will raise an error while executeTasks is called if there is a fail
	/* Executes tasks, should only be called as a seperate thread
	* \param tasks: A list of the tasks to be executed
	* \param raiseErrorOnFail: display a windows error message if the task was unable to execute for whatever reason
	* \param pathToTaskFile: reads and writes task information to a file (unused)
	*/
	static void execTasks(std::vector<Task> tasks, bool raiseErrorOnFail, std::string pathToTaskFile);

	//if a task is finished write it to the file under CompletedTasks (unused)
	static bool addCompletedTaskToFile(std::string pathToTaskFile, Task t);


	TaskList(SDL_Renderer* renderer, int x, int y, int w, int h, int tasksOnScreen = 4);
	~TaskList();
	TaskObject* addTask();
	void removeTaskFromEnd();
	//reset the pixel positions of each task according to their position in this->tasks
	void updateTaskPositions();
	void render() override;
	void update() override;
	bool exportCurrentTaskList();
	//converts the tasks to a string of their information, each task is seperated by a newlines
	std::string convertToExportableFormat();
	//open file explorer for the user to select a path then calls importTaskListFromPath
	bool importTaskList();
	//opens the file at specified path and converts the strings into task objects before displaying them in the tasklist
	bool importTaskListFromPath(std::wstring path);
	//checks tasks are valid then calls TaskList::execTasks in a seperate thread
	void executeTasks();
};