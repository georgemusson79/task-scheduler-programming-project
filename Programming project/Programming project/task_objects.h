#pragma once
#include "simple_renderables.h"

class TaskObject : public Draggable {
private:


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
	void _create();
public:
	TaskObject(SDL_Renderer* renderer, int x, int y, int w, int h);
	~TaskObject();
	void setName(std::string text);
	bool setDims(int x, int y) override;
	void setFilePath();
	void render() override;
	void update() override;
	bool setPos(Vector2 pos) override;

};


class TaskList : public Renderable {
protected:
	std::vector<TaskObject*> tasks = {};
	Rectangle* biggerbox = nullptr;
	Rectangle* smallerbox = nullptr;
public:
	TaskList(SDL_Renderer* renderer, int x, int y, int w, int h);
	~TaskList();
	void addTask(std::string taskName);
	void render() override;
	void update() override;
};