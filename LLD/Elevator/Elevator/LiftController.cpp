#pragma once
#include <vector>
#include "Lift.cpp"
#include "Request.cpp"
using namespace std;

class LiftController {
private:
	vector<Lift*> lifts;
	int index;
public:
	LiftController(int idx=0) {
		this->index = idx;
	}

	void SetLifts(vector<Lift*>& newLifts) {
		this->lifts = newLifts;
	}

	void moveLift() {
		for (auto lift : lifts) {
			if (lift) {
				lift->move();
			} else {
				cout << "Warning: null lift pointer in LiftManagerInstance::moveLift\n";
			}
		}
	}
    
	void PlaceRequest(Request* req) {
		if (!req) return;
		Lift* pickedLift = this->FindMinimumWaitLift(req);
		if (!pickedLift) {
			cout << "Error: no available lift to place request " << req->floor->number << endl;
			return;
		}
		pickedLift->addRequest(req);
		cout << "request - " << req->floor->number << " : added to for lift - " << pickedLift->getLiftNumber() << endl;
	}

	Lift* FindMinimumWaitLift(Request* req) {
		if (lifts.empty()) return nullptr;
		// ensure index is within bounds
		if (index < 0 || (size_t)index >= lifts.size()) index = 0;
		Lift* liftpickup = lifts[index];
		index = (index + 1) % static_cast<int>(lifts.size());
		return liftpickup;
	}
};