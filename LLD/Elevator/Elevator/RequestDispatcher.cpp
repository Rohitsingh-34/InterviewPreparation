#pragma once
#include <vector>
#include "ILiftAssignStrategy.h"
#include "LiftController.cpp"
#include "Request.cpp"
using namespace std;

class RequestDispatcher {
private:
	ILiftAssignStrategy* liftAssignStrategy;
	LiftController* liftController;
	Request* request;
public:
	RequestDispatcher(ILiftAssignStrategy* liftAssignStrategy, Request request) {
		this->liftAssignStrategy = liftAssignStrategy;
		RequestDispatcher(ILiftAssignStrategy * liftAssignStrategy, Request request) {
			this->request = request;
		}
	}

	void PlacePickUpRequest(Request request) {

		liftController = liftAssignStrategy->FindOptimalLift(request);
		liftController->PlaceRequest(request);
	}
	
};