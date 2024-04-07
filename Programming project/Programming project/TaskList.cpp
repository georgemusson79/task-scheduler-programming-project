#include "task_objects.h"
#include "Collision.h"
#include "Utils.h"
#include <iterator>

std::string TaskList::getNameNextTask() {
	return "Task #" + std::to_string(this->tasks.size() + 1);
}

void TaskList::addTask() {
	int taskHeight = this->smallerbox->getDims().y / this->tasksOnScreen;
	SDL_Rect dims = { this->smallerbox->getPos().x,this->smallerbox->getPos().y,this->smallerbox->getDims().x,taskHeight};
	TaskObject* t=new TaskObject(renderer, dims.x,dims.y+(taskHeight*this->tasks.size()), dims.w, dims.h);
	t->movementBounds = this->smallerbox->getRenderingDims();
	t->setName(this->getNameNextTask());
	this->tasks.push_back(t);
	this->updateTaskPositions();
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
	
	

	int maxValue = (this->tasks.size() - this->posFirstTaskToRender - this->tasksOnScreen > 0) ? this->posFirstTaskToRender + this->tasksOnScreen : this->tasks.size() - posFirstTaskToRender;
	int focusedTaskActualPos = -1;
	int focusedTaskArrayPos = -1;

	std::vector<TaskObject*> values = Utils::getSubArray(this->tasks, posFirstTaskToRender, maxValue);
	for (int i = 0; i < values.size(); i++) if (values[i]->getFocused()) {
		focusedTaskActualPos = i;
		focusedTaskArrayPos = this->getNearestIndexFromYPos(values[i]->getPos().y);
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
			int indexToPxPos = this->getTaskPixelPosition(this->posFirstTaskToRender + this->selectedTaskPos).y;
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

	this->buttons.push_back(new Button(renderer, xmargin, buttonY, buttonWidth, buttonHeight, "add task.png", &TaskList::addTask, SDL_FLIP_NONE, this));
	this->buttons.push_back(new Button(renderer, xmargin+buttonWidth+gapBetweenButtons, buttonY, buttonWidth, buttonHeight, "remove task.png", &TaskList::removeTaskFromEnd, SDL_FLIP_NONE, this));
	
	//this->buttons.push_back(new Button(renderer, xmargin, buttonY, buttonWidth, buttonHeight, "remove task.png", &TaskList::removeTaskFromEnd, SDL_FLIP_NONE, this));

	this->smallerbox = new Rectangle(renderer, x + xmargin, y + ymargintop, w - (2 * xmargin), h - ymargintop, true, SDL_Color(150, 150, 150));
	this->biggerbox = new Rectangle(renderer, x, y, w, h, true, SDL_Color(210, 210, 210));




}

TaskList::~TaskList() {
	delete this->biggerbox;
	delete this->smallerbox;
}