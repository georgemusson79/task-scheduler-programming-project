#pragma once
#include "simple_renderables.h"

class TaskObject : public Draggable {
private:
	Label* taskNameHeading = nullptr;
	TextField* taskName = nullptr;

	Label* filePathHeading = nullptr;
	TextField* filePath = nullptr;
	Button* filePathBrowse = nullptr;

	Label* extraArgsHeading = nullptr;
	TextField* extraArgs = nullptr;

	bool isInPlace = false;
	void _create();
public:
	TaskObject(SDL_Renderer* renderer, int x, int y, int w, int h);
	~TaskObject();

	static void staticSetFilePath(TaskObject* object);
	void setFilePath(std::string data);
	void render() override;
	void update() override;
	bool setPos(Vector2 pos) override;

};