
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

class Singleton {
	string value;
	Singleton() {}
	Singleton(string value) {
		this->value = value;
	}
	static mutex mtx;
public:
	static Singleton* instance;

	static Singleton* GetInstance(string value) {
		lock_guard<mutex> lock(mtx);
		if (instance == nullptr)
			instance = new Singleton(value);
		return instance;
	}

	void printValue() {
		//	    lock_guard<mutex> lock(mtx); without it output is like : thread1 thread1
		cout << value << endl;
	}
};
mutex Singleton::mtx;
Singleton* Singleton::instance = nullptr;

void thread1() {

	Singleton* s1 = Singleton::GetInstance("thread1");

	s1->printValue();
};


void thread2() {

	Singleton* s2 = Singleton::GetInstance("thread2");
	s2->printValue();
};
int main() {

	thread t1(thread1);
	thread t2(thread2);
	t1.join();
	t2.join();

	return 0;
}

/*
possible output with safe threading

thread1
thread1

thread1thread1

-> i means both threads work context switching happened at cout before "<<endl" code piece

like t1 print thread1
like t2 print thread1
then t1 print <<endl and then t2 print <<endl;

here instance for both are same

*/