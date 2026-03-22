#include <chrono>
#include <cmath>
#include <condition_variable>
#include <deque>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
int calculate_huge_data(int input) {
    return input * 2;
}
void phase_1_demo() {
    // 发起异步任务
    std::future<int> result = std::async(calculate_huge_data, 100);
    std::cout << "主线程正在干活..." << std::endl;
    int val = result.get();
    std::cout << "拿到结果了:" << val << std::endl;
}
std::deque<std::packaged_task<int()>> tasd_deque;
void worker_thread() {
    if (!tasd_deque.empty()) {
        std::packaged_task<int()> task = std::move(tasd_deque.front());
        tasd_deque.pop_front();
        task();
    }
}
void phase_2_demo() {
    // 包装一个任务 1. 创建任务
    std::packaged_task<int()> task([]() { return 777; });
    // 2. 获取future
    std::future<int> res = task.get_future();
    // 把任务塞进队列，等别人执行
    tasd_deque.push_back(std::move(task));
    // 3.执行任务
    worker_thread();
    // 4.获取结果
    std::cout << "从任务队列拿到的结果: " << res.get() << std::endl;
}
void phase_3_demo() {
    std::promise<std::string> p;
    std::future<std::string> f = p.get_future();
    std::thread t([&p]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        p.set_value("这是来自远方的承诺");
    });
    std::cout << "等待承诺兑现..." << std::endl;
    std::cout << "结果: " << f.get() << std::endl;
    t.join();
}
double square_root(double x) {
    if (x < 0) throw std::out_of_range("x不能小于0");  //
    return sqrt(x);
}
void phase_4_demo() {
    std::future<double> f = std::async(square_root, -1.0);

    try {
        double res = f.get();  // 这一步会重新抛出后台线程的异常
    } catch (const std::exception& e) {
        std::cout << "捕获到了异步任务的异常: " << e.what() << std::endl;
    }
}
void phase_5_demo() {
    std::promise<int> p;
    // 使用 share() 将普通 future 转为可拷贝的 shared_future
    std::shared_future<int> sf = p.get_future().share();
    auto task = [sf](int id) {
        std::cout << "线程 " << id << " 等待中...\n";
        std::cout << "线程 " << id << " 拿到结果: " << sf.get() << "\n";
    };
    std::thread t1(task, 1);
    std::thread t2(task, 2);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    p.set_value(12345);
    t1.join();
    t2.join();
}
void sleep_demo() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);

    auto tomorrow = std::chrono::system_clock::now() + 24h;
    std::this_thread::sleep_until(tomorrow);
}
int main() {
    phase_1_demo();
    phase_2_demo();
    phase_3_demo();
    phase_4_demo();
    phase_5_demo();
    return 0;
}