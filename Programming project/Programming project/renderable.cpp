#include "simple_renderables.h"
#include "Cursor.h"
#include "Collision.h"
Vector2 Renderable::getPos() {
	return { this->renderScrDims.x,this->renderScrDims.y };
}

bool Renderable::setPos(Vector2 pos) {
	this->renderScrDims.x = pos.x;
	this->renderScrDims.y = pos.y;
	return true;
}

bool Renderable::setDims(Vector2 dims) {
	this->renderScrDims.w = dims.x;
	this->renderScrDims.h = dims.y;
	return true;
}

Vector2 Renderable::getDims() {
	return { this->renderScrDims.w,this->renderScrDims.h };
}

bool Renderable::trySetFocus() {
	if (!Cursor::isFocused) {
		focused = true;
		Cursor::isFocused = true;
		Cursor::focusedItem = this;
		return true;
	}
	return false;
}

bool Renderable::tryRemoveFocus() {
	if (focused) {
		focused = false;
		Cursor::isFocused = false;
		Cursor::focusedItem = nullptr;
		return true;
	}
	return false;
}

bool Renderable::setRenderingDims(int x, int y, int w, int h) {
	if (this->setPos({ x,y }) && this->setDims({ w,h })) return true;
	return false;
}
SDL_Rect Renderable::getRenderingDims() {
	return this->renderScrDims;
}