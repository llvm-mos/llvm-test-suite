#include <new>
#include <stdlib.h>
#include <stdio.h>

// This test requires predictable allocation failure conditions
// and the ability to extend the heap in the new_handler.
#ifdef __LLVM_MOS_SDK

int main() {
    __set_heap_limit(500);

    struct KilobyteBuf {
        unsigned char m_buf[1024];
    };

    std::set_new_handler([]() {
        puts("NEW HANDLER INVOKED");

        // make more space available.
        __set_heap_limit(1040);

        if (__heap_bytes_free() > 1040 || __heap_bytes_free() < 1024) {
            puts("BAD FREE SPACE VALUE");
            abort();
        }

        // don't retry if making more space available didn't work.
        std::set_new_handler(nullptr);
    });

    auto alloc = new KilobyteBuf{};

    if (__heap_limit() != 1040) {
        puts("HEAP LIMIT IS WRONG");
        abort();
    }

    if (std::get_new_handler()) {
        puts("NEW HANDLER STILL SET");
        abort();
    }

    // Test the non-throwing default handler path.
    auto alloc2 = new (std::nothrow) KilobyteBuf{};
    if (alloc2) {
        puts("UNEXPECTED ALLOC SUCCESS");
        abort();
    }

    // Test exit from the new handler.
    std::set_new_handler([] {
        exit(3);
    });

    alloc2 = new KilobyteBuf{};

    exit(0);
}
#else
int main() {
    puts("NEW HANDLER INVOKED");
    exit(3);
}
#endif
