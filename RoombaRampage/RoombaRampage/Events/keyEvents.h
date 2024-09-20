#pragma once
#include <sstream>
#include "baseEvent.h"
#include "../Input/keycodes.h"

class classKeyEvent : public classEvent {
public:
	keyCode funcGetKeyCode() const { return lvKeyCode; }
	EVENT_CLASS_CAT(eventCatKey | eventCatInput);

protected:
	classKeyEvent(const keyCode givenKey) :lvKeyCode(givenKey) {}
	keyCode lvKeyCode;
};

class classKeyPressedEvent : public classKeyEvent {
public:
	classKeyPressedEvent(const keyCode givenKey, bool repeating = false) :classKeyEvent(givenKey), lvRepeated(repeating) {}
	bool funcIsRepeating() const { return lvRepeated; }
	EVENT_CLASS_TYPE(keyPressed)

	std::string funcEventNameString() const override {
		std::stringstream stream;
		stream << "Key pressed: " << lvKeyCode << " (repeating? = " << lvRepeated << ")";
		return stream.str();
	}
private:
	bool lvRepeated;
};

class classKeyReleasedEvent : public classKeyEvent {
public:
	classKeyReleasedEvent(const keyCode givenKey) : classKeyEvent(givenKey) {}
	EVENT_CLASS_TYPE(keyReleased)
	std::string funcEventNameString() const override {
		std::stringstream stream;
		stream << "Key released: " << lvKeyCode;
		return stream.str();
	}

};

