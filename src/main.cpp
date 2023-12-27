#include <iostream>

#define TRACKMEM 1
#include "TrackMem.h"

class Account {
public:
	Account(double money = 0.0) : bal(money) {}
	double getBal() { return bal; }
private:
	double bal = 0.0;
};

int main() {
	int* test = NEW(int);
	int* test2 = NEW(int);
	Account* account = NEW(Account);
	Account* account2 = NEW(Account);
	*test = 5;
	
	DELETE(test);
	PrintMemoryUsage();
	PrintAllocatedMem();

	std::cin.get();
}