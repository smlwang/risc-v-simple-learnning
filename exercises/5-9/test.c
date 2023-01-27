int foo(int a, int b) {
    int c;
    /* 
        c = a * a + b * b;
    */
    asm volatile (
        "mul %1, %1, %1\n"\
        "mul %2, %2, %2\n"\
        "add %0, %1, %2"
        :"=r"(c)
        :"r"(a), "r"(b)
    );
    return c;
}