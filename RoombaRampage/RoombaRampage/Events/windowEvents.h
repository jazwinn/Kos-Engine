#pragma once
#include "baseEvent.h"
#include <sstream>

class classWindowResizeEvent : public classEvent {
public:
	classWindowResizeEvent(unsigned int givenWidth, unsigned int givenHeight) : lvWidth(givenWidth), lvHeight(givenHeight) {}
	unsigned int funcGetWinWidth() const { return lvWidth; }
	unsigned int funcGetWinHeight() const { return lvHeight; }
	EVENT_CLASS_TYPE(winResized)
	EVENT_CLASS_CAT(eventCatApp)
	std::string funcEventNameString() const override {
		std::stringstream stream;
		stream << "Window resized ,width: " << lvWidth << " Height: " << lvHeight;
		return stream.str();
	}
private:
	unsigned int lvWidth, lvHeight;
};

class classWindowClosedEvent : public classEvent {
public:
	classWindowClosedEvent() = default;
	EVENT_CLASS_TYPE(winClosed)
	EVENT_CLASS_CAT(eventCatApp)
};

class classWindowMovedEvent : public classEvent {
public:
	classWindowMovedEvent(unsigned int givenX, unsigned int givenY) : lvPosX(givenX), lvPosY(givenY) {}

	unsigned int funcGetPosX() const { return lvPosX; }
	unsigned int funcGetPosY() const { return lvPosY; }

	EVENT_CLASS_TYPE(winMoved)
	EVENT_CLASS_CAT(eventCatApp)

	std::string funcEventNameString() const override {
		std::stringstream stream;
		stream << "Window moved , Pos X: " << lvPosX << " Pos Y: " << lvPosY;
		return stream.str();
	}
private:
	unsigned int lvPosX, lvPosY;
};