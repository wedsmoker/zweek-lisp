#include <iostream>
#include <vector>
#include <string>
#include <memory>

int add(int a, int b) {
    return (a + b);
}

int main() {
    auto x = 10;
    auto y = 20;
    std::cout << "Hello from Zweek!" << std::endl;
    std::cout << "10 + 20 =" << add(x, y) << std::endl;
    return 0;
}

