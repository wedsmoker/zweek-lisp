#include <iostream>
#include <vector>
#include <string>
#include <memory>

int factorial(int n) {
    if ((n <= 1)) {
        return 1;
    } else {
        return (n * factorial((n - 1)));
    }
}

int main() {
    std::cout << "Factorial of 5:" << std::endl;
    std::cout << factorial(5) << std::endl;
    return 0;
}

