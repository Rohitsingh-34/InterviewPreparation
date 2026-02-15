#include "Lift.cpp"
#include "Request.cpp"
#include "IButton.h"
#include "LiftController.cpp"
#include <iostream>
#pragma once
using namespace std;

class FloorButton:public IButton {
private:
	bool _isPressed;
	LiftController* liftController;
	int destinationFloor;
public:
    FloorButton(int floor, LiftController* liftController) {
		this->destinationFloor = floor;
		this->liftController = liftController;
		this->_isPressed = false;
	}

	void press(Floor* floor) override {
		this->_isPressed = true;
		cout << "in floor button"<<endl;
	}

	bool isPressed() override {
		return this->_isPressed;
	}

	void reset() override {
		this->_isPressed = false;
	}
};