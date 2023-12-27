# MemoryTracker
A single-header implementation adding tracking of dynamic memory allocation and deallocation.

**DISCLAIMER: NEW(type) and DELETE(type) don't currently work with dynamically allocated arrays.**

## To use
1. Add header file to project files.
2. #define TRACKMEM 1 to track memory.
3. #include "TrackMem.h" below #define.
4. Use NEW(type) instead of new type and DELETE(type) instead of delete type.
5. At end of entry point (typically int main), call PrintMemoryUsage() (shows current memory usage) and PrintAllocatedMem() (shows all memory that hasn't been deallocated).
