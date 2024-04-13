#define _CRT_SECURE_NO_WARNINGS

#include "task_objects.h"
#include "Collision.h"
#include "Utils.h"
#include "Main_Functions.h"

#include <fstream>
#include <sstream>
#include <iterator>
#include <thread>

std::string TaskList::getNameNextTask() {
	return "Task #" + std::to_string(this->tasks.size() + 1);
}

TaskObject* TaskList::addTask() {
	int taskHeight = this->smallerbox->getDims().y / this->tasksOnScreen;
	SDL_Rect dims = { this->smallerbox->getPos().x,this->smallerbox->getPos().y,this->smallerbox->getDims().x,taskHeight};
	TaskObject* t=new TaskObject(renderer, dims.x,dims.y+(taskHeight*this->tasks.size()), dims.w, dims.h);
	t->movementBounds = this->smallerbox->getRenderingDims();
	t->setName(this->getNameNextTask());
	this->tasks.push_back(t);
	this->updateTaskPositions();
	return t;
}

void TaskList::removeTaskFromEnd() {
	if (this->tasks.empty()) return;
	TaskObject* t = this->tasks.back();
	this->tasks.pop_back();
	delete t;
	if (this->posFirstTaskToRender != 0) this->posFirstTaskToRender--;
	this->updateTaskPositions();
}

void TaskList::updateTaskPositions() {
	int h = this->smallerbox->getDims().y / this->tasksOnScreen;
	int x = this->smallerbox->getPos().x;
	int y = this->smallerbox->getPos().y;
	int initialY = y;

	//std::vector<TaskObject*> renderedItems = Utils::getSubArray(this->tasks, this->posFirstTaskToRender, this->tasksOnScreen);
	
	

	int focusedTaskActualPos = -1;
	int focusedTaskArrayPos = -1;
	int maxValue = this->tasksOnScreen;

	std::vector<TaskObject*> values = Utils::getSubArray(this->tasks, posFirstTaskToRender, maxValue);
	for (int i = 0; i < values.size(); i++) if (values[i]->getFocused()) {
		focusedTaskActualPos = i;
		focusedTaskArrayPos = this->getNearestIndexFromYPos(values[i]->getPos().y) - this->posFirstTaskToRender;
	}
	if (focusedTaskActualPos != -1) {
		Utils::moveItem(values, focusedTaskActualPos, focusedTaskArrayPos);
	}


	for (TaskObject* t : values) {
		t->setPos({ x,y });
		y += h;
	}
}

void TaskList::render() {
	this->biggerbox->render();
	this->smallerbox->render();
	std::vector<TaskObject*> renderedItems = Utils::getSubArray(this->tasks, this->posFirstTaskToRender, this->tasksOnScreen);
	int taskNum = 0;
	for (auto& task : renderedItems) {
		bool isInsideBox = Collision::isInsideOf(this->smallerbox->getRenderingDims(), task->getRenderingDims());
		if (isInsideBox) task->render();

		if (task->getFocused() && this->getTaskPixelPosition(taskNum) != task->getPos()) {
			this->selectedTaskPos = this->getNearestIndexFromYPos(task->getPos().y);
			if (this->selectedTaskPos >= tasks.size()) this->selectedTaskPos = tasks.size()-1;
			int indexToPxPos = this->getTaskPixelPosition(this->selectedTaskPos).y;

			SDL_RenderDrawLine(this->renderer, this->biggerbox->getPos().x, indexToPxPos, this->biggerbox->getPos().x+ this->biggerbox->getDims().x, indexToPxPos);
		}
		taskNum++;
	}
	for (int i = 0; i < 5; i++) buttons[i]->render();
	if (this->posFirstTaskToRender > 0) buttons[5]->render();
	if (this->posFirstTaskToRender + this->tasksOnScreen < this->tasks.size()) buttons[6]->render();
	if (Main::tasksExecutingInSeperateThread) this->nextTaskToExecute->render();

}

void TaskList::update() {
	int taskNum=0;
	this->selectedTask = nullptr;
	int selectedTaskNum = this->posFirstTaskToRender;
	int whereToMoveTask = this->posFirstTaskToRender;
	std::vector<TaskObject*> renderedTasks = Utils::getSubArray(this->tasks, this->posFirstTaskToRender, this->tasksOnScreen);
	for (auto task : renderedTasks) {
		
		if (task->getFocused()) {
			this->selectedTask = task;
			whereToMoveTask = this->getNearestIndexFromYPos(task->getPos().y);
			selectedTaskNum = taskNum+ this->posFirstTaskToRender;
			this->updateTaskPositions();
		}
		task->update();
		taskNum++;
	}

	if (selectedTask != nullptr && !selectedTask->getFocused()) this->moveTask(selectedTaskNum, whereToMoveTask);
	this->handleKBInput();
	for (int i = 0; i < 5;i++) buttons[i]->update();
	if (this->posFirstTaskToRender > 0) buttons[5]->update();
	if (this->posFirstTaskToRender+this->tasksOnScreen < this->tasks.size()) buttons[6]->update();

	Renderable::update();
}

int TaskList::getNearestIndexFromYPos(int yPos) {
	int taskHeight = this->getTaskHeight();
	int boxLength = this->smallerbox->getDims().y;
	int boxStart = this->smallerbox->getPos().y;

	int index = 0;
	int closestDistance = std::numeric_limits<int>::max();
	for (int i = this->posFirstTaskToRender; i < this->posFirstTaskToRender+this->tasksOnScreen; i++) {
		int distance = abs(yPos - this->getTaskPixelPosition(i).y);
		if (distance < closestDistance) {
			closestDistance = distance;
			index = i;
		}
	}
	return index;
}

void TaskList::handleKBInput() {
	const Uint8* kbInput=SDL_GetKeyboardState(nullptr);

	//deletes any selected tasks if the user presses the delete key
	if (kbInput[SDL_SCANCODE_DELETE]) {
		for (TaskObject* task : this->tasks) {
			if (task->getHightlighted()) {
				auto it = std::find(this->tasks.begin(), this->tasks.end(), task);
				this->tasks.erase(it);
				delete task;
				this->updateTaskPositions();
				break;
			}
		}
	}

}

Vector2 TaskList::getTaskPixelPosition(int pos) {
	if (pos < 0) return { 999,999 };
	else {
		int taskHeight = this->smallerbox->getDims().y / this->tasksOnScreen;
		int posOnScreen = pos - this->posFirstTaskToRender;
		return { this->smallerbox->getPos().x,this->smallerbox->getPos().y + (taskHeight * posOnScreen) };
	}
}

void TaskList::moveTask(int first, int second) {
	if (second == first) {
		this->updateTaskPositions();
		return;
	}
	else if (second >= this->tasks.size()) {
		this->tasks.push_back(this->tasks[first]);
		this->tasks.erase(this->tasks.begin() + first);
	}
	else {
		auto firstIt= this->tasks.begin() + first;
		auto secondIt = this->tasks.begin() + second;
		Utils::moveItem(this->tasks, first, second);

	}
	this->updateTaskPositions();
}


bool TaskList::setPos(int x, int y) {
	this->renderScrDims.x = x;
	this->renderScrDims.y = y;
	int w = this->renderScrDims.w;
	int h = this->renderScrDims.h;

	int xmargin = w * 0.05;
	int ymargintop = h * 0.15;
	this->tasksOnScreen = tasksOnScreen;

	int buttonWidth = w / 8;
	int buttonHeight = ymargintop / 3;
	int buttonY = y+ymargintop - buttonHeight - (h / 100);
	int gapBetweenButtons = w / 80;
	int buttonX = x + xmargin;

	buttons[0]->setPos(buttonX, buttonY);
	buttonX += buttonWidth + gapBetweenButtons;
	buttons[1]->setPos(buttonX, buttonY);
	buttonX += (buttonWidth * 1.2) + gapBetweenButtons;
	buttons[2]->setPos(buttonX, buttonY);
	buttonX += (buttonWidth * 1.2) + gapBetweenButtons;
	buttons[3]->setPos(buttonX, buttonY);
	buttonX += (buttonWidth * 1.2) + gapBetweenButtons;
	buttons[4]->setPos(buttonX, buttonY);

	this->biggerbox->setPos({ x, y });
	this->smallerbox->setPos({ x + xmargin, y + ymargintop });

	int boxCenter = this->smallerbox->getPos().x + (this->smallerbox->getDims().x / 2);
	int upDownButtonWidth = buttonWidth / 4;
	int upDownButtonPosX = this->smallerbox->getDims().x + this->smallerbox->getPos().x;
	buttons[5]->setPos(upDownButtonPosX, this->smallerbox->getPos().y);
	buttons[6]->setPos(upDownButtonPosX, this->smallerbox->getPos().y + this->smallerbox->getDims().y - buttonHeight / 2);


	for (TaskObject* t : this->tasks) {
		t->movementBounds = { 0,0,9999999,99999999 }; //set the boundaries to a large value to prevent issues with moving
		t->setPos({ this->smallerbox->getPos().x,t->getPos().y });
		t->movementBounds = this->smallerbox->getRenderingDims();
	}

	this->updateTaskPositions();
	return true;

}


TaskList::TaskList(SDL_Renderer* renderer,int x, int y, int w, int h,int tasksOnScreen) : Renderable(renderer) {
	int xmargin = w * 0.05;
	int ymargintop = h * 0.15;
	this->tasksOnScreen = tasksOnScreen;
	this->setRenderingDims(x, y, w, h);

	int buttonWidth = w / 8;
	int buttonHeight = ymargintop / 3;
	int buttonY = ymargintop - buttonHeight-(h/100);
	int gapBetweenButtons = w / 80;

	int buttonX = x + xmargin;


	this->buttons.push_back(new Button(renderer,buttonX, buttonY, buttonWidth, buttonHeight, "add task.png", &TaskList::addTask, SDL_FLIP_NONE, this));
	buttonX += buttonWidth + gapBetweenButtons;
	this->buttons.push_back(new Button(renderer, buttonX, buttonY, buttonWidth*1.2, buttonHeight, "remove task.png", &TaskList::removeTaskFromEnd, SDL_FLIP_NONE, this));
	buttonX += (buttonWidth*1.2) + gapBetweenButtons;
	this->buttons.push_back(new Button(renderer, buttonX, buttonY, buttonWidth*1.2, buttonHeight, "export task.png", &TaskList::exportCurrentTaskList, SDL_FLIP_NONE, this));
	buttonX += (buttonWidth * 1.2) + gapBetweenButtons;
	this->buttons.push_back(new Button(renderer, buttonX, buttonY, buttonWidth * 1.2, buttonHeight, "import task.png", &TaskList::importTaskList, SDL_FLIP_NONE, this));
	buttonX += (buttonWidth * 1.2) + gapBetweenButtons;
	this->buttons.push_back(new Button(renderer, buttonX, buttonY, buttonWidth * 1.2, buttonHeight, "execute task.png", &TaskList::executeTasks, SDL_FLIP_NONE, this));


	this->smallerbox = new RenderableRect(renderer, x + xmargin, y + ymargintop, w - (2 * xmargin), h - ymargintop, true, SDL_Color(150, 150, 150));
	this->biggerbox = new RenderableRect(renderer, x, y, w, h, true, SDL_Color(210, 210, 210));
	

	int boxCenter = this->smallerbox->getPos().x + (this->smallerbox->getDims().x / 2);
	int upDownButtonWidth = buttonWidth / 4;
	int upDownButtonPosX = this->smallerbox->getDims().x + this->smallerbox->getPos().x;
	this->buttons.push_back(new Button(renderer, upDownButtonPosX, this->smallerbox->getPos().y, upDownButtonWidth, buttonHeight/2, "task up button.png", &TaskList::scrollUp, SDL_FLIP_NONE, this));
	this->buttons.push_back(new Button(renderer, upDownButtonPosX, this->smallerbox->getPos().y+this->smallerbox->getDims().y-buttonHeight/2, upDownButtonWidth, buttonHeight / 2, "task up button.png", &TaskList::scrollDown, SDL_FLIP_VERTICAL, this));



	int margin = Main::WINDOW_WIDTH / 20;
	int labelH = this->smallerbox->getDims().y/2;
	int labelW = Main::WINDOW_WIDTH - this->renderScrDims.w - (2 * margin);
	int labelY = this->smallerbox->getPos().y;
	int labelX = this->renderScrDims.w + margin;
	this->nextTaskToExecute = new Label(renderer, labelX, labelY, labelW, labelH, SDL_Color(0, 0, 0, 255), 0);
	this->nextTaskToExecute->drawBorder = true;


}


void TaskList::scrollDown() {
	if (this->posFirstTaskToRender + this->tasksOnScreen < tasks.size()) {
		this->posFirstTaskToRender += 1;
		this->updateTaskPositions();
	}
	
}
void TaskList::scrollUp() {
	if (this->posFirstTaskToRender > 0) {
		this->posFirstTaskToRender -= 1;
		this->updateTaskPositions();
	}
}

TaskList::~TaskList() {
	delete this->biggerbox;
	delete this->smallerbox;
}

bool TaskList::exportCurrentTaskList() {
	//if error and no selected dont export
	if (this->checkInvalidThenDisplayErr() != 6) return false;
	std::wstring path = Main::openFileExplorerSave({ {L".task Files",L"*.task"} });
	if (path == L"") return false;
	std::ofstream file(path);
	file << "Tasks\n";
	file << this->convertToExportableFormat();
	file << "\n";
	file << "EndTasks\n";
	return true;
}

bool TaskList::importTaskListFromPath(std::wstring path) {
	std::string buffer;
	bool isTasksSection = false;
	std::ifstream file(path);

	try {
		while (std::getline(file, buffer)) {

			//determine the start and end of the tasks
			if (buffer == "Tasks") isTasksSection = true;
			if (buffer == "EndTasks") {
				isTasksSection = false;
				return true;
			}


			if (isTasksSection && buffer != "Tasks") {
				std::vector<std::string> split = Utils::split(buffer,"::::");
				if (split.size() == 0) continue; //skip blank lines
				if (split.size() < 5) {
					throw std::exception("Invalid task params");
				}
				else {
					TaskObject* t = this->addTask();
					t->setName(split[0]);
					t->setFilePathStr(split[1]);
					t->setExtraArgs(split[2]);
					t->setFrequency(split[3]);
					t->setWhenToRun(split[4]);
					if (t->getWhenToRun() != "Immediately") t->setTime(split[5]);
				}
			}
		}
	}
	catch (std::exception e) {
		std::string output = "Unable to import tasks\n\nThe following error occured:\n";
		output += e.what();
		Main::windowsErrMessageBoxOk("Errors occured while loading file", output);
		return false;
	}
	return false;
}

bool TaskList::importTaskList() {
	std::wstring path = Main::openFileExplorerLoad({ {L".task Files",L"*.task"} });
	std::ifstream file(path);
	return TaskList::importTaskListFromPath(path);
}

std::string TaskList::convertToExportableFormat() {
	std::stringstream ss;
	for (auto task : this->tasks) {
		ss << task->getTaskName() << "::::" << task->getProgramPath() << "::::" << task->getExtraArgs() << "::::" << task->getFrequency() << "::::" << task->getWhenToRun();
		if (task->getWhenToRun() != "Immediately") ss <<"::::" << task->getInputtedTime();
		ss << "\n";
	}
	return ss.str();
}

int TaskList::checkInvalidThenDisplayErr() {
	bool err = false;
	std::vector<std::string> errs = {};
	if (this->tasks.size() > 0) {
		for (auto& task : this->tasks) {

			std::vector<std::string> res = task->checkValidityOfTask();
			if (res.size() > 0) {
				err = true;
				std::string taskName = task->getTaskName();
				if (taskName == "") taskName = "(No Name)";
				errs.push_back(taskName);
				errs.push_back("\n");
				for (std::string singleError : res) {

					errs.push_back(singleError);
					errs.push_back("\n");
				}
			}
		}
	}
	else {
		errs.push_back("No Tasks Added");
		err = true;
	}

	if (err) {
		std::string output = "Errors were present in the following Tasks:\n";
		for (std::string str : errs) output += str;
		output += "\nContinue?";
		return Main::windowsErrMessageBoxOkCancel("Errors in Tasks", output);
	}
	else return 6;
}

void TaskList::executeTasks() {
	if (Main::tasksExecutingInSeperateThread) {
		Main::windowsErrMessageBoxOk("Unable to start tasks", "A set of tasks is already executing, restart the program or wait for the tasks to finish to execute another task list.");
		return;
	}
	int res = this->checkInvalidThenDisplayErr();
	//res==6 means that the ok button was pressed res==0 means there were no errors
	if (res == 6 || res == 0) {
		std::vector<Task> tasksToRun = {};
		for (auto task : this->tasks) tasksToRun.push_back(task->convertToRunnableTask());
		//this->moveToAnimation({ 0,0 }, 300);
		std::thread t(TaskList::execTasks, tasksToRun, raiseErrorOnFail, "",this);
		t.detach();
	}
}

void TaskList::execTasks(std::vector<Task> tasks, bool raiseErrorOnFail, std::string pathToTaskFile,TaskList* tl) {
	Main::tasksExecutingInSeperateThread = true;
	if (!pathToTaskFile.empty()) {
		std::fstream file(pathToTaskFile, std::ios::in | std::ios::out);
		if (file.fail()) {
			Main::windowsErrMessageBoxOk("Unable to run tasks", "Unable to run tasks: file could not be opened");
			Main::tasksExecutingInSeperateThread = false;
			return;
		}
	}

	for (Task t : tasks) {
		if (t.whenToRun == "Immediately") {
			Utils::DateAndTime now = Utils::getCurrentDateAndTime();
			t.time = now.time;
			t.date = now.date;
		}
	}

	//sort items so that the soonest task is last
	std::sort(tasks.begin(), tasks.end(), [](Task a, Task b) {
		std::time_t aTime = a.timeAndDateTotime_t();
		std::time_t bTime = b.timeAndDateTotime_t();
		return aTime > bTime;
		});



#//execute 

	while (tasks.size() > 0) {

		if (tl != nullptr) {
			std::string taskLabelText = "Tasks to execute:\n";
			for (int i = 0; i < 5; i++) {
				int pos = tasks.size() - 1 - i;
				if (pos >= 0) taskLabelText += tasks[i].taskName + "\n";
				else taskLabelText += "\n";
			}
			tl->nextTaskToExecute->setText(taskLabelText);
		}

		Task t = *(tasks.end() - 1);
		tasks.pop_back();

		t.execute(raiseErrorOnFail);
		if (!pathToTaskFile.empty()) TaskList::addCompletedTaskToFile(pathToTaskFile, t);

		if (t.frequency != "Only once") tasks.push_back(t);

		std::sort(tasks.begin(), tasks.end(), [](Task a, Task b) {
			std::time_t aTime = a.timeAndDateTotime_t();
			std::time_t bTime = b.timeAndDateTotime_t();
			return aTime > bTime;
			});


	}
	Main::tasksExecutingInSeperateThread = false;

}



bool TaskList::addCompletedTaskToFile(std::string pathToTaskFile, Task t) {
	std::fstream file(pathToTaskFile,std::ios::in|std::ios::out);
	if (file.fail()) return false;
	bool startOfCompleteTasksFound = false;
	int pos = -1;
	bool endOfCompleteTasksFound = false;
	bool taskAdded = false;

	std::string fullFile = "";
	std::string line;
	while (std::getline(file, line)) {
		bool addLine = true;
		if (line == "CompletedTasks") startOfCompleteTasksFound = true;
		
		if (line == "EndOfCompletedTasks") {
			 if (!startOfCompleteTasksFound) addLine = false; //delete this line if there is an end but no start
			 else endOfCompleteTasksFound = true;
		}
		
		if (startOfCompleteTasksFound && !taskAdded) {
			fullFile += t.convertToExportableFormat() + "\n";
			taskAdded = true;
		}
		if (addLine) fullFile += line + "\n";
	}
	if (!endOfCompleteTasksFound) fullFile += "EndOfCompletedTasks";
	file.close();
	return true;
}
