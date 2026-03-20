#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

void some_function() {
    std::cout << "Thread A is running\n";
}
void some_other_function() {
    std::cout << "Thread B is running\n";
}

std::thread create_thread() {
    return std::thread(some_function);
}

class scoped_thread {
    std::thread t_;

public:
    explicit scoped_thread(std::thread t) : t_(std::move(t)) {
        if (!t_.joinable()) {
            throw std::logic_error("No thread");
        }
    }
    ~scoped_thread() {
        t_.join();
    }
    scoped_thread(const scoped_thread&) = delete;
    scoped_thread& operator=(const scoped_thread&) = delete;
};

int main() {
    std::thread t1(some_function);
    std::thread t2 = std::move(t1);

    t1 = std::thread(some_other_function);
    if (t1.joinable()) t1.join();
    if (t2.joinable()) t2.join();

    std::vector<std::thread> threads;
    for (unsigned i = 0; i < 5; i++) {
        threads.emplace_back(
            [](int id) { std::cout << "Worker" << id << "\n"
                                   << std::endl; },
            i);
    }
    for (auto& t : threads) {
        t.join();
    }
    int count = std::thread::hardware_concurrency();
    std::cout << "thr core of cpu is " << count << "\n" << std::endl;
    return 0;
}