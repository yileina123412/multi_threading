#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int unsafe_num = 0;
std::atomic<int> safe_num(0);

void work() {
    for (int i = 0; i < 100000; i++) {
        unsafe_num++;
        safe_num.fetch_add(1);
    }
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back(work);
    }
    for (auto& t : threads) t.join();
    std::cout << "理论应为: 100000" << std::endl;
    std::cout << "普通 int 结果: " << unsafe_num << " (大概率不对)"
              << std::endl;
    std::cout << "原子 atomic 结果: " << safe_num.load() << " (永远正确)"
              << std::endl;
    return 0;
}