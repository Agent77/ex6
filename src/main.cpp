#include <iostream>

int main() {
    src::logger lg;
    BOOST_LOG(lg) << "Hello, World!!!!";
    std::cout << "Hello, World!" << std::endl;
    return 0;
}