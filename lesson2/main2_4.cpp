#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

// 1. 每个线程要执行的求和任务
template <typename Iterator, typename T>
struct accumulate_block {
    void operator()(Iterator first, Iterator last, T& result) {
        result = std::accumulate(first, last, result);  // 计算该块的累加和
    }
};

template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init) {
    unsigned long const length = std::distance(first, last);

    if (!length) return init;  // 范围为空则直接返回

    // 2. 决定线程数量
    unsigned long const min_per_thread = 25;  // 每个线程最少处理 25 个元素
    unsigned long const max_threads =
        (length + min_per_thread - 1) / min_per_thread;
    unsigned long const hardware_threads = std::thread::hardware_concurrency();

    // 最终线程数取 核心数 和 任务建议数 的最小值
    unsigned long const num_threads =
        std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

    unsigned long const block_size = length / num_threads;  // 每块的大小

    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(
        num_threads - 1);  // 除去主线程，需要生成的子线程数

    // 3. 分配任务并启动线程
    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i) {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(accumulate_block<Iterator, T>(), block_start,
            block_end, std::ref(results[i])  // 使用 std::ref 存回结果
        );
        block_start = block_end;
    }

    // 4. 主线程干最后一手活
    accumulate_block<Iterator, T>()(
        block_start, last, results[num_threads - 1]);

    // 5. 等待所有线程完成
    for (auto& entry : threads) entry.join();

    // 6. 最后将所有中间结果再次累加
    return std::accumulate(results.begin(), results.end(), init);
}

int main() {
    std::vector<int> numbers(1000, 1);  // 1000 个 1
    int sum = parallel_accumulate(numbers.begin(), numbers.end(), 0);
    std::cout << "The sum is: " << sum << std::endl;  // 应该是 1000
    return 0;
}