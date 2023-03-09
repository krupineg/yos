#include "./test_framework.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "../src/drivers/screen.h"

BEGIN_TESTING

TEST("passing test") {
    ASSERT(1, 1);
}

TEST("empty test") {
    //assert(1 == 2);
}

TEST("clear screen without initialization should fail") {
    ASSERT(clear_screen(), ERROR_UNCONFIGURED);
    //assert(1 == 2);
}

TEST("can configure screen") {
    char * buffer = "AAAAAAAAAA";
    ASSERT(configure_screen(buffer, 5, 1, 2, 1, 0), 0);
}

TEST("can clear configured screen without format byte") {
    char buffer[10] = {'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'};
    ASSERT(configure_screen(buffer, 10, 1, 1, 0, 1), 0);
    ASSERT(clear_screen(), 0);
    for(int i = 0; i < 10; i++) {
        ASSERT(buffer[i], ' ');
    }
}

TEST("can clear configured screen with format byte") {
    char buffer[10] = {'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'};
    ASSERT(configure_screen(buffer, 5, 1, 1, 0, 2), 0);
    ASSERT(clear_screen(), 0);
    for(int i = 0; i < 5; i++) {
        ASSERT(buffer[i * 2], ' ');
        ASSERT(buffer[i * 2 + 1], 0x0f);
    }
}

TEST("can clear configured screen multiline") {
    char buffer[20] = {'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'};
    char result_buffer[20] = {' ', 0x0f, ' ', 0x0f, ' ', 0x0f, ' ', 0x0f, ' ', 0x0f, ' ', 0x0f, ' ', 0x0f, ' ', 0x0f, ' ', 0x0f, ' ', 0x0f};
    ASSERT(configure_screen(buffer, 5, 2, 1, 0, 2), 0);
    ASSERT(clear_screen(), 0);
    for(int i = 0; i < 20; i++) {
        ASSERT(buffer[i], result_buffer[i]);
    }
}

TEST("check buffer assertion") {
    char buffer[20] = {'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'};
    char result_buffer[20] = {' ', 0x0f, ' ', 0x0f, ' ', 0x0f, ' ', 0x0f, ' ', 0x0f, ' ', 0x0f, ' ', 0x0f, ' ', 0x0f, ' ', 0x0f, ' ', 0x0f};

    for(int i = 0; i < 10; i++) {
        ASSERT(buffer[i * 2], 'A');
        ASSERT(buffer[i * 2 + 1], 'A');
    }

    for(int i = 0; i < 10; i++) {
        ASSERT(result_buffer[i * 2], ' ');
        ASSERT(result_buffer[i * 2 + 1], 0x0f);
    }
}

TEST("can print text white") {
    char buffer[20] = {'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'};
    char result_buffer[20] = {' ', 0x0f, ' ', 0x0f, ' ', 0x0f, ' ', 0x0f, ' ', 0x0f, 'h', 0x0f, 'e', 0x0f, 'l', 0x0f, 'l', 0x0f, 'o', 0x0f}; //first row is empty
    ASSERT(configure_screen(buffer, 5, 2, 1, 0, 2), 0);
    ASSERT(clear_screen(), 0);
    ASSERT(kprint_at("hello", 0, 0), 0);

    for(int i = 0; i < 20; i++) {
        ASSERT(buffer[i], result_buffer[i]);
    }
}

TEST("can print text white on zero row (error row)") {
    char buffer[20] = {'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'};
    char result_buffer[20] = {'h', 0x0f, 'e', 0x0f, 'l', 0x0f, 'l', 0x0f, 'o', 0x0f, ' ', 0x0f, ' ', 0x0f, ' ', 0x0f, ' ', 0x0f, ' ', 0x0f}; //first row is empty
    ASSERT(configure_screen(buffer, 5, 2, 0, 0, 2), 0);
    ASSERT(clear_screen(), 0);
    ASSERT(kprint_at("hello", 0, 0), 0);

    for(int i = 0; i < 20; i++) {
        ASSERT(buffer[i], result_buffer[i]);
    }
}


END_TESTING