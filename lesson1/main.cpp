#include <iostream>
#include <thread>
void hello() {
    std::cout << "\nhello world!\n" << std::endl;
    return;
}

int main() {
    std::thread t(hello);
    t.join();
    return 0;
}