#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Observer{
public:
	virtual void notify(string message) = 0;
};

class Observer1: public Observer {
public:
	void notify(string message) override{
		cout<<"notified to observer1 with message - "<<message<<endl;
	}
};

class Observer2: public Observer {
public:
	void notify(string message) override{
		cout<<"notified to observer2 with message - "<<message<<endl;
	}
};

//preffered vector over map on delete is O(N)
//notify → happens thousands of times
// add/remove → happens rarely

class Subject{
    vector<Observer*> obsrs;
public:
    void addObserver(Observer* obsr){
        obsrs.push_back(obsr);
    }
    void removeObserver(Observer* obsr){
        auto it = find(obsrs.begin(), obsrs.end(), obsr);
        if(it != obsrs.end())
            obsrs.erase(it);
    }
	void notifyObserver(string message){
		for(auto o : obsrs)
    		o->notify(message);
	}
};


int main(){
	Subject* sub = new Subject();
	sub->addObserver(new Observer1());
	sub->addObserver(new Observer2());
	sub->notifyObserver("subject notification triggered");
	return 0;
}
