#pragma once
#include <vector>
#include "Request.cpp"
#include "LiftController.cpp"
#include "IButton.h"
using namespace std;

class DownDirectionButton : public IButton {
private:
	LiftController* liftController;
	int floor;
	bool _isPressed;
public:
	DownDirectionButton(int floor, LiftController* liftController, bool _pressed) {
		this->floor = floor;
		this->liftController = liftController;
		this->_isPressed = _pressed;
	}
	bool isPressed() override {
		return _isPressed;
	}
	void press(Floor* floor) override {
		if (!_isPressed) {
			_isPressed = true;
			liftController->PlaceRequest(new Request(floor));
		}
	}

	void reset() override {
		_isPressed = false;
	}
};

//why ibutton is public extension?