#include <stddef.h>
#include <stdint.h>

#include <memman.h>
#include <umm_malloc.h>
#include <utl/memory.h>

#ifdef __AVR_ATmega328P__
static constexpr uint16_t heapEndAddr = 0x0900;
#endif

typedef unsigned int LinkScriptValue;
extern "C" LinkScriptValue __data_start;
extern "C" LinkScriptValue __data_end;
extern "C" LinkScriptValue __bss_start;
extern "C" LinkScriptValue __bss_end;
extern "C" LinkScriptValue __heap_start;
// extern "C" LinkScriptValue __heap_end; // Bad Value!!!
extern "C" LinkScriptValue __DATA_REGION_LENGTH__;

// static const uint8_t *heapStart = reinterpret_cast<uint8_t *>(&__heap_start);
// static const uint8_t *heapEnd   = reinterpret_cast<uint8_t *>(heapEndAddr);

namespace memman {

int initMemory() {
    umm_init_heap(&__heap_start,
                  (sizeof(LinkScriptValue) *
                   utl::distance(&__heap_start, reinterpret_cast<LinkScriptValue *>(heapEndAddr))) -
                      0x100);
    return 0;
}

}  // namespace memman

void *operator new(size_t size) {
    void *ptr = umm_malloc(size);
    return ptr;
}

void *operator new[](size_t size) {
    void *ptr = umm_malloc(size);
    return ptr;
}

void operator delete(void *ptr) { umm_free(ptr); }
void operator delete[](void *ptr) { umm_free(ptr); }
void operator delete(void *ptr, size_t) { umm_free(ptr); }
void operator delete[](void *ptr, size_t) { umm_free(ptr); }
