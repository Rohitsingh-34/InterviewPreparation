#include <iostream>
using namespace std;
#include "Display.cpp"
#include "Direction.cpp"
#include "FloorButton.cpp"
#include "LiftController.cpp"
#include "Floor.cpp"
#include "UpDirectionButton.cpp"
#include "DownDirectionButton.cpp"
#include "RequestDispatcher.cpp"
#include <thread>
#include <chrono>
#include <unordered_map>
#include <atomic>
#include <string>
#include <vector>
#pragma once
void delay(int seconds)
{
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

int main()
{
    int NoOfFloors = 8;
    int LiftCount =  1;

	Display* display = new Display(1);
    cout << "Hello from Elevator" <<display->getCurrentFloor()<< endl;
	cout << "direction: " << ToString(display->getLiftDirection()) << endl;

    

	IButton* button = new FloorButton(2, LiftController);

	cout << "button : "<<button->isPressed()<<endl;

    LiftManagerInstance* liftmangobj = new LiftManagerInstance();

    unordered_map<int,Floor*> floors;
    vector<Lift*> lifts;
    vector<Display*> displays;

    for (int liftid = 1; liftid <= LiftCount; liftid++) {
        vector<IButton*> buttons;
        for (int floor = 1; floor <= NoOfFloors; floor++) {
            buttons.push_back(new FloorButton(floor, liftmangobj));
        }
        Display* display = new Display(1);
        displays.push_back(display);
        Lift* lift = new Lift(liftid, buttons, display);
        lifts.push_back(lift);
    }
	cout << "Total lifts created: " << lifts.size() << endl;
    liftmangobj->SetLifts(lifts);

    for (int i = 1; i <= NoOfFloors; i++) {
        IButton* upbtn = new UpDirectionButton(i, liftmangobj, false);
        IButton* downbtn = new DownDirectionButton(i, liftmangobj, false);
        Floor* floor = new Floor(i, displays, upbtn, downbtn);
        floors[i] = floor;
    }

    ElevatorService* svc = new ElevatorService( liftmangobj);

    // Main input loop: ask user for destination floor and direction.
    cout << "Elevator input loop started. Floors 1.." << NoOfFloors << endl;
    cout << "Enter 'q' at floor prompt to quit." << endl;

    string line;
    while (true) {
        {
            cout << "\nEnter destination floor (1.." << NoOfFloors << ") or 'q' to quit: ";
        }
        if (!std::getline(cin, line)) {
            // EOF or error
            break;
        }
        if (line == "q" || line == "Q") {
            break;
        }
        // Trim whitespace
        auto trim = [](string s) {
            size_t a = s.find_first_not_of(" \t\r\n");
            if (a == string::npos) return string();
            size_t b = s.find_last_not_of(" \t\r\n");
            return s.substr(a, b - a + 1);
            };
        line = trim(line);
        if (line.empty()) {
            continue;
        }
        int inputfloor = 0;
        try {
            inputfloor = stoi(line);
        }
        catch (...) {
            cout << "Invalid floor input. Please enter a number." << endl;
            continue;
        }
        if (inputfloor < 1 || inputfloor > NoOfFloors) {
            cout << "Floor out of range. Valid floors: 1.." << NoOfFloors << endl;
            continue;
        }

        cout << "Enter direction ('up' or 'down'): ";
        if (!std::getline(cin, line)) {
            break;
        }
        string direction = trim(line);
        // to lowercase
        for (auto& c : direction) c = tolower(c);

        auto it = floors.find(inputfloor);
        if (it == floors.end()) {
            cout << "Internal error: floor object not found." << endl;
            continue;
        }
        Floor* targetFloor = it->second;

        if (direction == "up") {
            svc->PlaceUpRequest(targetFloor);
            {
                cout << "Placed UP request for floor " << inputfloor << endl;
            }
        }
        else if (direction == "down") {
            svc->PlaceDownRequest(targetFloor);
            {
                cout << "Placed DOWN request for floor " << inputfloor << endl;
            }
        }
        else {
            cout << "Invalid direction. Please enter 'up' or 'down'." << endl;
        }

    }
    

    while (true) {
        delay(2);
        liftmangobj->moveLift();
    }

    cout << "Exiting Elevator program." << endl;
    return 0;

}
