#include "renderable.h"
#include "Cursor.h"
Vector2 Renderable::getPosition() {
	return { this->renderScrDims.x,this->renderScrDims.y };
}

void Renderable::setPos(Vector2 pos) {
	this->renderScrDims.x = pos.x;
	this->renderScrDims.y = pos.y;
}

void Renderable::setDims(Vector2 dims) {
	this->renderScrDims.w = dims.x;
	this->renderScrDims.h = dims.y;
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

void Renderable::setRenderingDims(int x, int y, int w, int h) {
	this->renderScrDims = { x,y,w,h };
}
SDL_Rect Renderable::getRenderingDims() {
	return this->renderScrDims;
}