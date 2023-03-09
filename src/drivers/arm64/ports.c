#include <stdio.h>
char array[1000];
char *array_ptr = array;

/**
 * Read a byte from the specified port (mock)
 */
unsigned char port_byte_in (unsigned short port) {
    printf("port_byte_in %d \n", port);
    printf("port_byte_in value %d \n", array_ptr[port]);
    return array_ptr[port];
}

void port_byte_out (unsigned short port, unsigned char data) {
    printf("port_byte_out %d \n", port);
    printf("port_byte_out value %d \n", data);
    array_ptr[port] = data;
}

unsigned short port_word_in (unsigned short port) {
    return 0;
}

void port_word_out (unsigned short port, unsigned short data) {
}