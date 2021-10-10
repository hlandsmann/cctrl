#include <stddef.h>
#include <stdint.h>

#ifdef __AVR_ATmega328P__
static constexpr uint16_t heapEndAddr = 0x0900;
#endif

typedef unsigned int       LinkScriptValue;
extern "C" LinkScriptValue __heap_start;

// static const uint8_t *heapStart = reinterpret_cast<uint8_t *>(&__heap_start);
// static const uint8_t *heapEnd   = reinterpret_cast<uint8_t *>(heapEndAddr);

constexpr static uint32_t MagicNumber = 0xC001C0DE;


struct MemoryFragment {
    uint32_t check  = MagicNumber;
    uint8_t  distancePrevious;
    union {
        uint8_t freeIdentifier;
        uint8_t distanceNext;
    }code;
};

struct FreeMemoryFragment {
    uint32_t check  = MagicNumber;
    uint8_t  distancePrevious;
    uint8_t  freeIdentifier = 0;
    uint8_t  distanceNext;
};

using MemoryFragmentPtr = MemoryFragment *;

void *operator new(size_t /*size*/) {
    MemoryFragment memfrag;
    memfrag.code.distanceNext = 0;
    void *ptr=nullptr;
    return ptr;
}

void *operator new[](size_t /*size*/) {
    void *ptr=nullptr;
    return ptr;
}

void operator delete(void */*ptr*/) {}
void operator delete[](void */*ptr*/) {}
void operator delete(void*, size_t) {}
void operator delete [](void*, size_t) {}
// extern "C" void *malloc(size_t size) {
//     void *ptr = nullptr;

//     return ptr;
// }

// extern "C" void free(void *ptr) {
//     if (ptr) {
//     }
// }
