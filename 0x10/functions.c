#include <stdint.h>

#define XXX __asm__("nop");

// function without parameter and return value
void fun1() {
    XXX;
}

// function with integer return value
int fun2() {
    XXX;
    return 0x1234;
}

// function with parameter and return value
int fun3(int p1) {
    XXX;
    return p1+1;
}

// function with multiple parameters and return value
int fun4(int p1, int p2) {
    XXX;
    return p1+p2;
}

// function with a lot of parameters
int fun5(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13, int p14, int p15, int p16) {
    XXX;
    return p1+p2+p3+p4+p5+p6+p7+p8+p9+p10+p11+p12+p13+p14+p15+p16;
}

int main() {
    XXX;
    fun1();
    XXX;
    volatile int a = fun2();
    XXX;
    volatile int b = fun3(a);
    XXX;
    volatile int c = fun4(a, b);
    XXX;
    volatile int d = fun5(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
    XXX;
    return 0;
}
