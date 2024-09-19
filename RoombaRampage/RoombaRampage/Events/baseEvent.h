#pragma once
#include <functional>
#include <string>

#define BIT(x) (1<<x)
#define EVENT_CLASS_TYPE(type)	static classEventType funcGetStaticType(){return classEventType::type;}\
								virtual classEventType funcGetEventType() const override {return funcGetStaticType();}\
								virtual const char* funcGetEventName() const override {return #type;}

#define EVENT_CLASS_CAT(category) virtual int funcGetEventCatFlag() const override {return category;}

enum class classEventType {
	none = 0,
	winClosed,winResized,winFocus,winOutFocus,winMoved,
	keyPressed,keyReleased,
	mousePressed,mouseReleased,mouseMoved, mouseScrolled
};

enum eventCategory {
	none = 0,
	eventCatApp			= BIT(0),
	eventCatInput		= BIT(1),
	eventCatKey			= BIT(2),
	eventCatMouse		= BIT(3),
	eventCatMouseKey	= BIT(4)
};

class classEvent {
	friend class classEventDispatch;
public:
	virtual classEventType funcGetEventType() const = 0;
	virtual const char* funcGetEventName() const = 0;
	virtual std::string funcEventNameString() const { return funcGetEventName(); }
	virtual int funcGetEventCatFlag() const = 0;

	inline bool funcIsCat(eventCategory given) { return funcGetEventCatFlag() & given; }
	bool lvIsHandled = false;
};

class classEventDispatch {
private:
	classEvent& lvEvent;
	
public:
	classEventDispatch(classEvent& givenEvent) :lvEvent(givenEvent) {}

	template<typename T, typename U>
	bool funcDispatch(const U& givenFunction) {
		if (lvEvent.funcGetEventType() == T::funcGetStaticType()) {
			lvEvent.lvIsHandled |= givenFunction(static_cast<T&>(lvEvent));
			return true;
		}
		return false;
	}
};

inline std::ostream& operator<<(std::ostream& os, const classEvent& givenEvent) {
	return os << givenEvent.funcEventNameString();
}