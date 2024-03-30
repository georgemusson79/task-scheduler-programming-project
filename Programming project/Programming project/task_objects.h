#pragma once
#include "simple_renderables.h"

class TaskObject : public Draggable {
private:
	Label* taskNameHeading = nullptr;
	//TextField taskName;

	//LabelFixedCharDims filePathHeading;
	//TextField filePath;

	bool isInPlace = false;
	void _create();
public:
	TaskObject(SDL_Renderer* renderer, int x, int y, int w, int h);
	~TaskObject();

	void render() override;
	void update() override;
	bool setPos(Vector2 pos) override;

};