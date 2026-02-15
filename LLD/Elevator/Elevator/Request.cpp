#pragma once
#include "RequestType.cpp"
using namespace std;

class Request {
public:
	int floor;
	RequestType requesttype;
	
public:
	Request( int floor, RequestType requesttype){
		this->floor = floor;
		this->requesttype = requesttype;
	}
	
	int getfloor() {
		return floor;
	}
	RequestType getRequestType() {
		return requesttype;
	}
};