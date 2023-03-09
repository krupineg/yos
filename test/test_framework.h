#include <stdio.h>
#define BEGIN_TESTING int test(int argc, char **argv) {
#define END_TESTING return 0;}
#define TEST(TEST_NAME) if (run_test(TEST_NAME, argc, argv))

int errors = 0;
#define ASSERT(expression, expected) ({int val = expression; if(val != expected) { errors++; printf("\033[31massert failed: \033[0m expected: %d but was %d, file name: %s, line number = %d.\n", expected, val, __FILE__, __LINE__);} else { printf("\033[32massert passed \033[0m file name: %s, line number = %d.\n", __FILE__, __LINE__); } })

int run_test(const char* test_name, int argc, char **argv) {
    printf("%s\n", test_name);
    // we run every test by default
    if (argc == 1) { 
        return 1; 
    }
    // else we run only the test specified as a command line argument
    /*
    for (int i = 1; i < argc; i++) {
        if (!strcmp(test_name, argv[i])) { return 0; }
    } */
    if(errors > 0) {
        printf("\033[31m%d failed tests \033[0m\n", errors);
        return 1;
    }
    return 0;
}