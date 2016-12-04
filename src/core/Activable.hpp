#pragma once

namespace Game {

class Activable {
protected:
	bool active;

public:
	explicit Activable(bool active = true) : active(active) {}

	virtual void setActive(bool b) { active = b; }	
	bool isActive() const { return active; }
	/** @return The active state after the change */
	bool toggleActive() { 
		setActive(!active);
		return active;
	}
};

}