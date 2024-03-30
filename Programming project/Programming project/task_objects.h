#pragma once
#include "simple_renderables.h"

class TaskObject : public Draggable {
private:
	bool isInPlace = false;
public:
	TaskObject(int x, int y, int w, int h);
};