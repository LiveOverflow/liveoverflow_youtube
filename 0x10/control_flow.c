#include <stdint.h>

#define XXX __asm__("nop");

int main() {
    XXX;
    volatile int a = 0;
    XXX;
    XXX;
    XXX;
    if(a>0xff) {
        XXX;
    }
    XXX;
    XXX;
    XXX;
    while(a<10) {
        XXX;
        a++;
        XXX;
    }
    XXX;
    XXX;
    XXX;
    for (a = 0; a < 10; a++) {
        XXX;
    }
    XXX;
    XXX;
    XXX;
    return 0;
}

