#pragma once
#include "LiftController.cpp"
#include "IButton.h"
using namespace std;

class UpDirectionButton :public IButton {
private:
	LiftController* liftController;
	int floor;
	bool _isPressed;
public:
	UpDirectionButton(int floor, LiftController* liftManagerInstance, bool ispressed=false) {
		this->floor = floor;
		this->liftController = liftManagerInstance;
		this->_isPressed = ispressed;
	}


	bool isPressed() override {
		return _isPressed;
	}
	void press(Floor* floor) override {
		if (!_isPressed) {
			_isPressed = true;
			Request* req = new Request(floor);
			liftController->PlaceRequest(req);
		}
	}

	void reset() override {
		this->_isPressed = false;
	}

};