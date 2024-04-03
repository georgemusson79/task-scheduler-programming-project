#include "task_objects.h"
void TaskList::addTask(std::string taskName) {
	SDL_Rect dims = { this->smallerbox->getPos().x,this->smallerbox->getPos().y,this->smallerbox->getDims().x,this->smallerbox->getDims().y / 5 };
	TaskObject* t=new TaskObject(renderer, dims.x,dims.y,dims.w,dims.h);
	t->movementBounds = this->smallerbox->getRenderingDims();
	t->setName(taskName);
	this->tasks.push_back(t);
}
void TaskList::render() {
	this->biggerbox->render();
	this->smallerbox->render();
	for (auto task : this->tasks) task->render();
}

void TaskList::update() {
	for (auto task : this->tasks) task->update();
}

TaskList::TaskList(SDL_Renderer* renderer,int x, int y, int w, int h) : Renderable(renderer) {
	this->setRenderingDims(x, y, w, h);
	this->biggerbox = new Rectangle(renderer, x, y, w, h, true, SDL_Color(210, 210, 210));
	int xmargin = w * 0.05;
	int ymargintop = h * 0.15;
	int ymarginbottom = xmargin;
	this->smallerbox = new Rectangle(renderer, x + xmargin, y + ymargintop, w - (2 * xmargin), h - ymargintop - ymarginbottom, true, SDL_Color(150, 150, 150));
}

TaskList::~TaskList() {
	delete this->biggerbox;
	delete this->smallerbox;
}