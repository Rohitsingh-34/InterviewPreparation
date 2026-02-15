#pragma once
#include <vector>
#include "Display.cpp"
#include "IButton.h"
using namespace std;

class Floor {
private:
	int floorNumber;
	vector<Display*> displays;
	IButton* UpButton;
	IButton* DownButton;
public:
    Floor(int number, vector<Display*> dspl, IButton* upbtn, IButton* dnbtn) {
		this->floorNumber = number;
		this->displays = dspl;
		this->UpButton = upbtn;
		this->DownButton = dnbtn;
	}

	int getFloorNumber() {
		return floorNumber;
	}

	IButton* getUpButton() {
		return UpButton;
	}
	IButton* getDownButton() {
		return DownButton;
	}

};