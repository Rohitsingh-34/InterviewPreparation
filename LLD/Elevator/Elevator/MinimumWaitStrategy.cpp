#pragma once
#include <LiftController.cpp>

class MinimumWaitStrategy {
private:
	vector<LiftController*> lifts;
public:
	MinimumWaitStrategy(List<LiftController*> lifts) {
		this->lifts = lifts;
	}
	
	LiftController* FindOptimalLift() override {
		return lifts.front();
	}
};