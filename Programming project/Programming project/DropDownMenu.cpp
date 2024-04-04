#include "simple_renderables.h"
#include "Collision.h"
#include "Cursor.h"
#include <memory>

DropDownMenuItem::DropDownMenuItem(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color color, SDL_Color highlightColor, std::string text, DropDownMenu* parent) : Rectangle(renderer, x, y, w, h, true, color) {
	this->highlightColor = highlightColor;
	this->xmargin = this->renderScrDims.w * 0.2;
	this->ymargin = this->renderScrDims.h * 0.1;
	this->label = new Label(renderer, x+xmargin, y+ymargin, w-(xmargin*2), h-(ymargin*2), SDL_Color(0, 0, 0, 255));
	this->label->setText(text);
	this->defaultColor = color;
	this->parent = parent;
}

std::string DropDownMenuItem::getText() {
	return this->label->getText();
}
void DropDownMenuItem::setText(std::string text) {
	this->label->setText(text);
}


void DropDownMenuItem::render() {
	Rectangle::render();
	this->label->render();
}



bool DropDownMenu::setPos(int x, int y) {
	return this->setPos({ x,y });
}

void DropDownMenuItem::update() {
	Vector2 pos = Cursor::getPos();
	bool cursorCollides = Collision::collidesWith(this->renderScrDims, pos);
	bool parentFocused = (this->parent == nullptr) ? false : this->parent->getFocused();

	bool prevClicked = this->isClicked;
	this->isClicked = Cursor::isLeftClicked();


	if (cursorCollides && (!Cursor::isFocused || Cursor::focusedItem==this || parentFocused) ) {
		this->colour = this->highlightColor;
		Cursor::setCursor(SDL_SYSTEM_CURSOR_HAND);

		if (!prevClicked && this->isClicked) this->isClickTapped = true;
		else this->isClickTapped = false;

	}
	else this->colour = defaultColor;

}

bool DropDownMenuItem::getIsClickedOnThis() {
	return (this->isClickTapped && Collision::collidesWith(this->renderScrDims,Cursor::getPos()));
}

bool DropDownMenuItem::setPos(Vector2 pos) {
	Rectangle::setPos(pos);
	this->label->setPos(pos.x+this->xmargin,pos.y+this->ymargin);
	return true;
}
bool DropDownMenuItem::setDims(Vector2 dims) {
	this->xmargin = this->renderScrDims.w * 0.2;
	this->ymargin = this->renderScrDims.h * 0.1;
	Rectangle::setDims(dims);
	this->label->setDims(dims.x-(2*xmargin),dims.y-(2*ymargin));
	this->setPos(this->getPos());
	return true;
}

DropDownMenuItem::~DropDownMenuItem() {
	delete this->label;
}



bool DropDownMenu::setDims(Vector2 dims) {
	int x = this->getPos().x;
	int y = this->getPos().y;
	this->renderScrDims.w = dims.x;
	this->renderScrDims.h = dims.y;

	for (DropDownMenuItem* item : items) {
		item->setDims({ dims.x, dims.y });
		item->setPos({ x, y });
		y += dims.y;
	}
	return true;
}

bool DropDownMenu::setPos(Vector2 pos) {
	if (this->items.empty()) return false;
	this->renderScrDims.x = pos.x;
	this->renderScrDims.y = pos.y;

	int y = pos.y;
	for (DropDownMenuItem* item : items) {
		item->setPos({ pos.x, y });
		y += this->renderScrDims.h;;
	}
	return true;
}


//dimensions here are the dimensions of each dropMenuItem
DropDownMenu::DropDownMenu(SDL_Renderer* renderer, int x, int y, int w, int h, std::vector<std::string> text, SDL_Color color, SDL_Color highlightColor) : Renderable(renderer) {
	this->renderScrDims = { x,y,w,h };
	int itemY = y;
	for (std::string item : text) {
		this->items.push_back(new DropDownMenuItem(renderer, x, itemY, w, h, color, highlightColor, item,this));
		itemY += h;
	}
}

void DropDownMenu::render() {
	if (items.size() == 0) return;
	
	this->items[0]->render();


	if (this->focused) {
		for (DropDownMenuItem* item : this->items) {
			if (item!=items[0]) item->render();
		}
	}
}

void DropDownMenu::update() {
	if (!this->focused) {
		this->items[0]->update();
		if (this->items[0]->getIsClickedOnThis()) {
			this->trySetFocus();
		}
	}

	else {
		bool cursorIntersects = false;

		for (DropDownMenuItem* item : this->items) {
			item->update();
			if (Collision::collidesWith(item->getRenderingDims(),Cursor::getPos())) cursorIntersects=true;

			if (item->getIsClickedOnThis()) {
				std::string temp = item->getText();
				item->setText(this->items[0]->getText());
				this->items[0]->setText(temp);
				this->tryRemoveFocus();
			}
		}
		if (Cursor::isLeftClicked() && !cursorIntersects) this->tryRemoveFocus();
	}
}

std::string DropDownMenu::getSelectedItem() {
	if (this->items.size() > 0) return this->items[0]->getText();
	else return "";
}

DropDownMenu::~DropDownMenu() {
	for (DropDownMenuItem* item : this->items) delete item;
}