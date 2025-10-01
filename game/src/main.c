#include "engine/include/engine.h"


int main(int argc, char **argv) {
    ns_throw_error("lol ll", 5, 3);

    nsError error = ns_get_error();

    printf("Error code: %u\n", error.code);
    printf("Error severity: %u\n", error.severity);
    printf("Error message: %s\n", error.message);

    return EXIT_SUCCESS;
}