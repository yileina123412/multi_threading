#include <chrono>
#include <condition_variable>
#include <future>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

// 模拟一个极其不稳定的 AI 推理任务
int unstable_inference() {
    // 模拟有时快（100ms），有时慢（10s）
    std::this_thread::sleep_for(2s);
    return 1;
}

int main() {
    std::cout << "--- 开启限时等待测试 ---" << std::endl;

    // 1. 启动异步任务
    std::future<int> f = std::async(std::launch::async, unstable_inference);

    // 2. 我只愿意等 1 秒钟，超过 1 秒我就不等了
    if (f.wait_for(1s) == std::future_status::ready) {
        std::cout << "成功拿回结果: " << f.get() << std::endl;
    } else {
        // 超时处理：这里你可以取消任务或者记录日志
        std::cout << "推理太慢了，用户已取消等待！" << std::endl;
    }

    // --- 条件变量超时示例 ---
    std::mutex mut;
    std::condition_variable cv;
    bool done = false;

    std::thread t([&] {
        std::this_thread::sleep_for(500ms);
        {
            std::lock_guard<std::mutex> lk(mut);
            done = true;
        }
        cv.notify_one();
    });

    std::unique_lock<std::mutex> lk(mut);
    // wait_until 使用绝对时间点
    auto timeout = std::chrono::steady_clock::now() + 1s;
    if (cv.wait_until(lk, timeout, [&] { return done; })) {
        std::cout << "条件在超时前达成！" << std::endl;
    } else {
        std::cout << "时间到了，条件还是没达成。" << std::endl;
    }

    t.join();
    return 0;
}