#include <iostream>
#include <unordered_map>
#include <typeinfo>

class AllocData {
public:
	AllocData(size_t line): line(line) {}
	virtual void printData() { std::cout << "[ERROR]\n"; };

protected:
	size_t line = 0;
};

template<typename T>
class AllocType : public AllocData {
public:
	AllocType(T* data, size_t line) : data(data), AllocData(line) {}
	void printData() override {
		std::cout << data << " [" << typeid(*data).name() << "] LINE " << line << '\n';
	}
private:
	T* data;
};

class AllocationMetrics {
public:
	static AllocationMetrics s_AllocationMetrics;

	uint32_t TotalAllocated = 0;
	uint32_t TotalFreed = 0;
	std::unordered_map<void*, AllocData*> allocMap;

	uint32_t CurrentUsage() { return TotalAllocated - TotalFreed; }

private:
};

static AllocationMetrics s_AllocationMetrics;

//Shows how many bytes are still in use.
//Should be used at end of application.
static void PrintMemoryUsage() {
	std::cout << "Memory Usage: " << s_AllocationMetrics.CurrentUsage() << " bytes\n";
}

//Outputs all memory that hasn't been freed.
//Should be used at end of application.
static void PrintAllocatedMem() {
	std::cout << "Memory Still in Use:\n";
	for (auto& [key, val] : s_AllocationMetrics.allocMap) val->printData();
}

#define TRACKMEM 1

#if TRACKMEM
#define NEW(x) _new<x>(sizeof(x), __LINE__)
#define DELETE(x) _delete(x, sizeof(*x))

template<typename T>
T* _new(size_t size, size_t line) {
	s_AllocationMetrics.TotalAllocated += (uint32_t)size;
	T* data = (T*)malloc(size);
	s_AllocationMetrics.allocMap[data] = new AllocType<T>(data, line);
	return data;
}

void _delete(void* memory, size_t size) {
	s_AllocationMetrics.TotalFreed += (uint32_t)size;
	s_AllocationMetrics.allocMap.erase(memory);
	free(memory);
}
#else
#define NEW(x) new x
#define DELETE(x) delete x
#endif

class Account {
public:
	Account(double money = 0.0) : bal(money) {}
	double getBal() { return bal; }
private:
	double bal = 0.0;
};

int main() {
	int* test = NEW(int);
	Account* account = NEW(Account);
	*test = 5;
	
	DELETE(test);
	PrintMemoryUsage();
	PrintAllocatedMem();

	std::cin.get();
}