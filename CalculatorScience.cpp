#include <iostream>
#include <cmath>

int main() {
    float x, y, result;
    char operation;

    std::cout << "Enter first number: ";
    std::cin >> x;

    std::cout << "Select operation (+, -, *, /, s - sin, c - cos, t - tan, n - ln, l - log10): ";
    std::cin >> operation;

    if (operation != 's' && operation != 'c' && operation != 't' && operation != 'n' && operation != 'l') {
        std::cout << "Enter second number: ";
        std::cin >> y;
    }

    asm (
        "fld %[x]\n\t"
        "cmp $'+', %[operation]\n\t"
        "je addition\n\t"
        "cmp $'-', %[operation]\n\t"
        "je subtraction\n\t"
        "cmp $'*', %[operation]\n\t"
        "je multiplication\n\t"
        "cmp $'/', %[operation]\n\t"
        "je division\n\t"
        "cmp $'s', %[operation]\n\t"
        "je sin_func\n\t"
        "cmp $'c', %[operation]\n\t"
        "je cos_func\n\t"
        "cmp $'t', %[operation]\n\t"
        "je tan_func\n\t"
        "cmp $'n', %[operation]\n\t"
        "je ln_func\n\t"
        "cmp $'l', %[operation]\n\t"
        "je log10_func\n\t"
        "jmp finish\n\t"

        "addition:\n\t"
        "fld %[y]\n\t"
        "fadd\n\t"
        "jmp finish\n\t"

        "subtraction:\n\t"
        "fld %[y]\n\t"
        "fsub\n\t"
        "jmp finish\n\t"

        "multiplication:\n\t"
        "fld %[y]\n\t"
        "fmul\n\t"
        "jmp finish\n\t"

        "division:\n\t"
        "fld %[y]\n\t"
        "fdiv\n\t"
        "jmp finish\n\t"

        "sin_func:\n\t"
        "fsin\n\t"
        "jmp finish\n\t"

        "cos_func:\n\t"
        "fcos\n\t"
        "jmp finish\n\t"

        "tan_func:\n\t"
        "fptan\n\t"
        "fstp %%st(0)\n\t"  // Wyrzuć tangent, zostaw tylko wynik
        "jmp finish\n\t"

        "ln_func:\n\t"
        "fldln2\n\t"
        "fdiv\n\t"
        "jmp finish\n\t"

        "log10_func:\n\t"
        "fldlg2\n\t"
        "fdiv\n\t"
        "jmp finish\n\t"

        "finish:\n\t"
        "fstp %[result]"
        : [result] "=t" (result)
        : [x] "m" (x), [y] "m" (y), [operation] "m" (operation)
        : "memory"
    );

    std::cout << "Result: " << result << std::endl;

    return 0;
}