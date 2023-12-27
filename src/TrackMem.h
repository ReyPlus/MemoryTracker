#include <unordered_map>
#include <typeinfo>

#if TRACKMEM
class AllocData {
public:
	AllocData(const char* file, size_t line) : file(file), line(line) {}
	virtual void printData() { printf("[ERROR]\n"); };

protected:
	const char* file;
	size_t line = 0;
};

template<typename T>
class AllocType : public AllocData {
public:
	AllocType(T* data, const char* file, size_t line) : data(data), AllocData(file, line) {}
	void printData() override {
		printf("%p [%s] FILE %s | LINE %d\n", data, typeid(*data).name(), file, (int)line);
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
	printf("Memory Usage: %d bytes\n", int(s_AllocationMetrics.CurrentUsage()));
}

//Outputs all memory that hasn't been freed.
//Should be used at end of application.
static void PrintAllocatedMem() {
	printf("Memory Still in Use:\n");
	for (auto& [key, val] : s_AllocationMetrics.allocMap) val->printData();
}

#define NEW(x) _new<x>(sizeof(x), __FILE__, __LINE__)
#define DELETE(x) _delete(x, sizeof(*x))

template<typename T>
T* _new(size_t size, const char* file, size_t line) {
	s_AllocationMetrics.TotalAllocated += (uint32_t)size;
	T* data = (T*)malloc(size);
	s_AllocationMetrics.allocMap[data] = new AllocType<T>(data, file, line);
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