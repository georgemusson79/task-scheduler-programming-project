#include "task_objects.h"
#include "Collision.h"
#include "Utils.h"

#include <fstream>
#include <sstream>
#include <iterator>

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
			std::cout << this->selectedTaskPos << "\n";
			int indexToPxPos = this->getTaskPixelPosition(this->selectedTaskPos).y;
			SDL_RenderDrawLine(this->renderer, this->biggerbox->getPos().x, indexToPxPos, this->smallerbox->getPos().x+ this->biggerbox->getDims().x, indexToPxPos);
		}
	}
	for (Button*b : this->buttons) b->render();
		

}

void TaskList::update() {
	int taskNum=0;
	this->selectedTask = nullptr;
	int selectedTaskNum = 0;
	int whereToMoveTask = 0;
	for (auto task : this->tasks) {
		
		if (task->getFocused()) {
			this->selectedTask = task;
			whereToMoveTask = this->getNearestIndexFromYPos(task->getPos().y);
			selectedTaskNum = taskNum;
			this->updateTaskPositions();
		}
		task->update();
		taskNum++;
	}

	if (selectedTask != nullptr && !selectedTask->getFocused()) this->moveTask(selectedTaskNum, whereToMoveTask);
	this->handleKBInput();
	for (Button* b : this->buttons) b->update();

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
		//if (first < second) {
		//	for (auto it1 = firstIt; it1 < secondIt; it1++) {
		//		auto next = std::next(it1);
		//		std::iter_swap(it1, next);
		//	}
		//}

		//else {
		//	for (auto it1 = firstIt; it1 > secondIt; it1--) {
		//		auto prev = std::prev(it1);
		//		std::iter_swap(it1,prev);
		//	}
		//}

		for (auto e : this->tasks) std::cout << e->getTaskName() << "\n";
	}
	this->updateTaskPositions();
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


	this->smallerbox = new Rectangle(renderer, x + xmargin, y + ymargintop, w - (2 * xmargin), h - ymargintop, true, SDL_Color(150, 150, 150));
	this->biggerbox = new Rectangle(renderer, x, y, w, h, true, SDL_Color(210, 210, 210));
	

	int boxCenter = this->smallerbox->getPos().x + (this->smallerbox->getDims().x / 2);
	int upDownButtonWidth = buttonWidth / 4;
	int upDownButtonPosX = this->smallerbox->getDims().x + this->smallerbox->getPos().x;
	this->buttons.push_back(new Button(renderer, upDownButtonPosX, this->smallerbox->getPos().y, upDownButtonWidth, buttonHeight/2, "task up button.png", &TaskList::scrollUp, SDL_FLIP_NONE, this));
	this->buttons.push_back(new Button(renderer, upDownButtonPosX, this->smallerbox->getPos().y+this->smallerbox->getDims().y-buttonHeight/2, upDownButtonWidth, buttonHeight / 2, "task up button.png", &TaskList::scrollDown, SDL_FLIP_VERTICAL, this));





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
	std::wstring path = Main::openFileExplorerSave({ {L".task Files",L"*.task"} });
	if (path == L"") return false;
	std::ofstream file(path);
	file << this->convertToExportableFormat();
	return true;
}

bool TaskList::importTaskList() {
	std::wstring path = Main::openFileExplorerLoad({ {L".task Files",L"*.task"} });
	std::ifstream file(path);
	std::string buffer;
	while (std::getline(file, buffer)) {
		std::vector<std::string> split = Utils::split(buffer, ',');
		if (split.size() < 5) {
			std::cout << "invalid size\n";
			return false;
		}
		else {
			TaskObject* t=this->addTask();
			t->setName(split[0]);
			t->setFilePathStr(split[1]);
			t->setExtraArgs(split[2]);
			t->setFrequency(split[3]);
			t->setWhenToRun(split[4]);
			if (t->getWhenToRun() != "Immediately") t->setTime(split[5]);
		}
	}
	return true;
}

std::string TaskList::convertToExportableFormat() {
	std::stringstream ss;
	for (auto task : this->tasks) {
		ss << task->getTaskName() << "," << task->getProgramPath() << "," << task->getExtraArgs() << "," << task->getFrequency() << "," << task->getWhenToRun();
		if (task->getWhenToRun() != "Immediately") ss << task->getInputtedTime();
		ss << "\n";
	}
	return ss.str();
}

