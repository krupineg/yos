#include "./util.h"

void memory_copy(char *source, char *dest, int nbytes) {
    for(int i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
        //dest[i] = source[i];
    }
}

void int_to_ascii(int n, char str[]) {
    int len = 0;
    int sign = 0;
    if( n < 0 ) {
        sign = -1;
        n *= -1;
    }
    do {
        str[len++] = n % 10 + '0' ;
    } while((n/= 10) > 0);

    if(sign < 0) {
        str[len ++] = '-';
    }
    reverse_substring(str, 0, len);
    str[len] = '\0';
}

void swap_chars (char from[], int lo, int hi) {
    char buf = from[lo];
    from[lo] = from[hi];
    from[hi] = buf;
} 

void reverse_substring(char from[], int start, int len) {
    int lo = start;
    int hi = start + len - 1;
    while(lo < hi) {
        swap_chars(from, lo, hi);
        lo ++;
        hi --;
    }
}