#define XXX __asm__("nop");

int fun1(int p1) {
    return p1*4;
}

int main() {
    int i = 0;
    int a = 3;
    for(i = fun1(a); i<22; i++) {
        XXX ;
    }
    return 0;
}
