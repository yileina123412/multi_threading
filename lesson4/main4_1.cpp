#include <chrono>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

// --- 1. 线程安全队列类实现 (参考书 4.1.2 节) ---
template <typename T>
class threadsafe_queue {
private:
    mutable std::mutex mut;  // 互斥量，保护队列数据
    std::queue<T> data_queue;
    std::condition_variable data_cond;

public:
    threadsafe_queue() {}
    // 入队：生产者使用
    void push(T new_value) {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(std::move(new_value));
        data_cond.notify_one();  // 数据准备好后，随即唤醒一个等待中的线程
    }
    // 阻塞式出队：消费者使用
    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lk(mut);

        data_cond.wait(lk, [this] { return !data_queue.empty(); });
        value = std::move(data_queue.front());
        data_queue.pop();
    }
    bool empty() const {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};

struct data_chunk {
    int id;
};

threadsafe_queue<data_chunk> q;
// --- 2. 模拟场景测试 ---

// 生产者线程：模拟 AI 预处理（比如下载图片并解码）
void data_preparation_thread() {
    for (int i = 1; i <= 5; i++) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(500));  // 模拟耗时
        std::cout << "[生产者] 准备好了数据包: " << i << std::endl;
        q.push(data_chunk{i});  // 放入队列并通知
    }
    // 推送一个特殊值 -1 表示结束
    q.push(data_chunk{-1});
}
// 消费者线程：模拟 AI 模型推理
void data_processing_thread() {
    while (true) {
        data_chunk temp;
        q.wait_and_pop(temp);
        if (temp.id == -1) break;
        std::cout << "[消费者] 正在推理数据包: " << temp.id << "..."
                  << std::endl;
        std::this_thread::sleep_for(
            std::chrono::milliseconds(200));  // 模拟推理耗时
    }
    std::cout << "[消费者] 所有任务处理完毕，下班！" << std::endl;
}

int main() {
    std::cout << "--- 开启 AI 推理流水线模拟 ---" << std::endl;

    std::thread t1(data_preparation_thread);
    std::thread t2(data_processing_thread);

    t1.join();
    t2.join();

    return 0;
}