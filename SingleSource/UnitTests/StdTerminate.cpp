#include <exception>
#include <stdlib.h>
#include <stdio.h>

int main() {

    std::set_terminate([]()
    {
        puts("TERMINATE CALLED. EXITING.");
        exit(1);
    });

    std::terminate();
    exit(0);
}
