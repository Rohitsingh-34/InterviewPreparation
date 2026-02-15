#pragma once
#include "Direction.cpp"
#include <iostream>
using namespace std;

class Display {
private:
	int liftNumber;
	int currFloor;
	Direction direction;
public:
	Display(int liftnumber, int currfloor = 1, Direction direction = Direction::IDLE) {
		this->liftNumber = liftnumber;
		this->currFloor = currfloor;
		this->direction = direction;
	}
	int getLiftNumber() {
		return liftNumber;
	}
	int getCurrentFloor() {
		return currFloor;
	}
	Direction getLiftDirection() {
		return direction;
	}
	void ShowDisplay() {
		cout << "LIFT : " << liftNumber << endl;
		cout << "current floor: " << currFloor << " & direction: " << ToString(this->direction) << endl;
	}
};