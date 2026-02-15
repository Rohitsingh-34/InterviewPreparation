#pragma once
#include "Display.cpp"
#include <vector>
#include "IButton.h"
#include "Request.cpp"
#include <queue>
#include "LiftStatus.cpp"
#include <mutex>
using namespace std;

class Lift {
private:
	int liftNumber;
	int currFloor;
	LiftStatus status;
	vector<IButton*> buttons;
	Display* display;
	queue<Request*> requests;
	std::mutex req_mutex;
public:	
	Lift(int number, vector<IButton*> buttons, Display* display, int currfloor=1, LiftStatus status = LiftStatus:: IDLE ) {
		this->liftNumber = number;
		this->currFloor = currfloor;
		this->status = status;
		this->buttons = buttons;
		this->display = display;
	}
	int getLiftNumber() {
		return this->liftNumber;
	}
	void move() {
			if (requests.empty()) {
				status = LiftStatus::IDLE;
				if (display) {
					display->currFloor = currFloor;
					display->direction = Direction::IDLE;
					display->ShowDisplay();
				}
				return;
			}
			Request* req = requests.front();

		if (req->floor->number == currFloor) {
			display->ShowDisplay();
			this->OpenLift();
			{
				if (!requests.empty()) { // double-check before pop
					requests.pop();
				}
			}
		}
		else if (req->floor->number > currFloor) {
			status = LiftStatus::MOVING_UP;
			display->currFloor = ++currFloor;
			display->direction = Direction::UP;
			display->ShowDisplay();
		}
		else {
			status = LiftStatus::MOVING_DOWN;
			display->currFloor = --currFloor;
			display->direction = Direction::DOWN;
			display->ShowDisplay();
		}
	}
	void updateStatus() {
		cout << "updating status in lift class" << endl;
	}

	void addRequest(Request* req) {
		requests.push(req);
	}
	
	void OpenLift() {
		cout << endl<< "Lift1 Opened at Floor: " << currFloor << endl;

		cout << "Lift closed at Floor: " << currFloor << endl << endl;
	}
};

/*LIFT : 1
current floor: 1 & direction: UP
LIFT : 1
current floor: 2 & direction: UP
Lift Opened at Floor: 3

Lift closed at Floor: 3
LIFT : 1
current floor: 2 & direction: UP
LIFT : 1
current floor: 3 & direction: UP
LIFT : 1
current floor: 4 & direction: UP
LIFT : 1
current floor: 5 & direction: UP
Lift Opened at Floor: 6

Lift closed at Floor: 6
LIFT : 1
current floor: 5 & direction: UP
LIFT : 1
current floor: 6 & direction: UP*/