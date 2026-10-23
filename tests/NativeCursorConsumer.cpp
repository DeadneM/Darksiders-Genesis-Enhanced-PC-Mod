#include <cstdint>

// Separate translation unit: consume the full native enum return in EAX,
// as Slate does. An inlined byte-return fixture would mask this ABI error.
__declspec(noinline) void ReadCursorAsEngine(std::int32_t (*query)(void*),void* controller,std::int32_t* reply){
    *reply=query(controller);
}
