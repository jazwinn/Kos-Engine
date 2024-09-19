#pragma once
#include "baseEvent.h"
#include "../mousecodes.h"
#include <sstream>

class classMouseMoveEvent : public classEvent {
public:
	classMouseMoveEvent(const float givenX, const float givenY) : lvMousePosX(givenX), lvMousePosY(givenY) {}
	EVENT_CLASS_TYPE(mouseMoved)
	EVENT_CLASS_CAT(eventCatInput | eventCatMouse)
	float funcGetX() const { return lvMousePosX; }
	float funcGetY() const { return lvMousePosY; }
	std::string funcEventNameString() const override{
		std::stringstream stream;
		stream << "Mouse moved, Pos x: " << lvMousePosX << " Pos y: " << lvMousePosY;
		return stream.str();
	}
private:
	float lvMousePosX, lvMousePosY;
};

class classMouseScrollEvent : public classEvent {
public:
	classMouseScrollEvent(const float& givenOffsetX, const float givenOffsetY) : lvOffsetX(givenOffsetX), lvOffsetY(givenOffsetY){}
	float funcGetOffsetX() { return lvOffsetX; }
	float funcGetOffsetY() { return lvOffsetY; }
	EVENT_CLASS_TYPE(mouseScrolled)
	EVENT_CLASS_CAT(eventCatInput | eventCatMouse)
	std::string funcEventNameString() const override {
		std::stringstream stream;
		stream << "Mouse scrolled, Offset X: " << lvOffsetX << " Offset Y: " << lvOffsetY;
		return stream.str();
	}
private:
	float lvOffsetX, lvOffsetY;
};

class classMouseButtonEvent : public classEvent {
public:
	mouseCode funcGetMouseButton() const { return lvMouseButton; }
	EVENT_CLASS_CAT(eventCatInput | eventCatMouseKey | eventCatMouse)
protected:
	classMouseButtonEvent(const mouseCode givenMouseButton) : lvMouseButton(givenMouseButton) {}
	mouseCode lvMouseButton;
};

class classMouseClickEvent : public classMouseButtonEvent {
public:
	classMouseClickEvent(const mouseCode givenMouseButton) : classMouseButtonEvent(givenMouseButton) {}
	EVENT_CLASS_TYPE(mousePressed)
	std::string funcEventNameString() const override {
		std::stringstream stream;
		stream << "Mouse pressed: " << lvMouseButton;
		return stream.str();
	}
};

class classMouseReleasedEvent : public classMouseButtonEvent {
public:
	classMouseReleasedEvent(const mouseCode givenMouseButton) : classMouseButtonEvent(givenMouseButton) {}
	EVENT_CLASS_TYPE(mouseReleased)
	std::string funcEventNameString() const override {
		std::stringstream stream;
		stream << "Mouse released: " << lvMouseButton;
		return stream.str();
	}
};