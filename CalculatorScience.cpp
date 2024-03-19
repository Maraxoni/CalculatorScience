#include <iostream>

using namespace std;

float calculate_science(float x, char op) {
    float result;

    asm volatile (
        "flds %1 \n"              // Load x onto FPU stack as a single-precision floating-point value
        "movb %2, %%al \n"        // Move op to al register

        // Check operator, Trigonometric functions
        "cmpb $'s', %%al \n"      // If 's' (sine) jump to sine
        "je sine \n"
        "cmpb $'c', %%al \n"      // If 'c' (cosine) jump to cosine
        "je cosine \n"
        "cmpb $'t', %%al \n"      // If 't' (tangent) jump to tangent
        "je tangent \n"

        // Check operator, Logarithm
        "cmpb $'l', %%al \n"      // If 'l' (logarithm) jump to logarithm
        "je logarithm \n"
        "cmpb $'n', %%al \n"      // If 'n' (natural logarithm) jump to natural_logarithm
        "je natural_logarithm \n"

        "jmp endsc \n"

        // Sine
        "sine: \n"
        "fsin \n"                 // Compute sine
        "fstps %0 \n"             // Store result from FPU stack to float value result
        "jmp endsc \n"

        // Cosine
        "cosine: \n"
        "fcos \n"                 // Compute cosine
        "fstps %0 \n"             // Store result from FPU stack to result as a single-precision floating-point value
        "jmp endsc \n"

        // Tangent
        "tangent: \n"
        "fptan \n"                // Compute tangent
        "fstp %%st(0) \n"         // Pop the unneeded value (1) from the FPU stack
        "fstps %0 \n"             // Store result (tangent) from FPU stack to result as a single-precision floating-point value
        "jmp endsc \n"

        // Logarithm (base 10)
        "logarithm: \n"
        "fldlg2\n"            // Load log base 10 (log10(2)) onto FPU stack
        "fld %1\n"            // Load x onto FPU stack
        "fyl2x\n"             // Compute log base 10
        "fstp %0\n"           // Store result from FPU stack to 'result'
        "jmp endsc \n"

        // Natural Logarithm (ln)
        "natural_logarithm: \n"
        "fldln2\n"            // Load natural log base 2 (ln(2)) onto FPU stack
        "fld %1\n"            // Load x onto FPU stack
        "fyl2x\n"             // Compute natural logarithm
        "fstp %0\n"           // Store result from FPU stack to 'result'
        "jmp endsc \n"

        "endsc: \n"
        : "=m" (result)           // Output - result
        : "m" (x), "g" (op)       // Input - x, op
        : "eax"                   // EAX register used
    );

    return result;
}

float calculate(float x, float y, char op) {
    float result;

    asm volatile (
        "movss %1, %%xmm0 \n"   // Move x to xmm0
        "movss %2, %%xmm1 \n"   // Move y to xmm1

        // Check operator
        "cmp $'+', %3 \n"       // If '+' jump to addition
        "je addition \n"
        "cmp $'-', %3 \n"       // If '-' jump to subtraction
        "je subtraction \n"
        "cmp $'*', %3 \n"       // If '*' jump to multiplication
        "je multiplication \n"
        "cmp $'/', %3 \n"       // If '/' jump to division
        "je division \n"
        "cmp $'s', %3 \n"       // If 's' (sine) jump to sine

        "addition: \n"
        "addss %%xmm1, %%xmm0 \n" // Add xmm1 to xmm0
        "jmp end \n"

        "subtraction: \n"
        "subss %%xmm1, %%xmm0 \n" // Subtract xmm1 from xmm0
        "jmp end \n"

        "multiplication: \n"
        "mulss %%xmm1, %%xmm0 \n" // Multiply xmm0 by xmm1
        "jmp end \n"

        "division: \n"
        "divss %%xmm1, %%xmm0 \n" // Divide xmm0 by xmm1
        "jmp end \n"

        "end: \n"
        "movss %%xmm0, %0 \n"   // Move result from xmm0 to 'result'
        : "=m" (result)         // Output - result
        : "m" (x), "m" (y), "r" (op) // Input - x, y, op
        : "xmm0", "xmm1"        // SSE registers xmm0 and xmm1 used in SSE instructions
    );

    return result;
}

int main() {
    float num1, num2;
    char operation;

    cout << "Enter operation(+, -, *, /, s(sin), c(cos), t(tan), l(logarithm), n(natural logarithm)): ";
    cin >> num1 >> operation;

    if (cin && (operation == '+' || operation == '-' || operation == '*' || operation == '/')) {
        cin >> num2;

        float result = calculate(num1, num2, operation);

        cout << "Result: " << result << "\n";
    }
    else if (operation == 's' || operation == 'c' || operation == 't' || operation == 'l' || operation == 'n') {
        num2 = 0;

        float result = calculate_science(num1, operation);

        cout << "Result: " << result << "\n";
    }
    else {
        cout << "Invalid input!" << endl;
    }

    return 0;
}
