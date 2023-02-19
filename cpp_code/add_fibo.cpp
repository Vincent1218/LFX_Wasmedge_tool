// Fibonacci numbers
int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n-1) + fibonacci(n-2);
}

// Add two numbers
int add(int a, int b) {
    return a + b;
}

// Factorial
int factorial(int n) {
    if (n == 0) {
        return 1;
    }
    return n * factorial(n-1);
}
