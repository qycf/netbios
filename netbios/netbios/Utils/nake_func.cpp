#include "nake_func.h"


void naked_bind_function(unsigned char* func, size_t size, void* bind_func, void** ppPointer) {
	*(void**)(func + 0x56) = bind_func;
	if (ppPointer) *(void**)(func + 0xac) = ppPointer;
}