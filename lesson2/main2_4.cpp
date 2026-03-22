#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

template <typename Iterator, typename T>
void accumulate_block(Iterator begin, Iterator end, T& result) {
    result = std::accumulate(begin, end, result);
}

template <typename Iterator, typename T>
T parallel_accumulate(Iterator begin, Iterator end, T init) {
    // 1.计算数据长度
    unsigned long length = std::distance(begin, end);
    // 2.设计线程数
    unsigned long const min_thread_num = 25;
    unsigned long const max_thread_num =
        (length + min_thread_num - 1) / min_thread_num;
    unsigned long const cup_core = std::thread::hardware_concurrency();
    unsigned long const ture_num =
        std::min(cup_core != 0 ? cup_core : 2ul, max_thread_num);

    // 3.每个线程的数据
    int num_per_thread = length / ture_num;
    std::vector<T> results(ture_num, 0);
    std::vector<std::thread> threads;
    // 4.开启线程计算
    Iterator start = begin;
    for (int i = 0; i < ture_num - 1; i++) {
        Iterator last = start;
        std::advance(last, num_per_thread);
        threads.emplace_back(
            accumulate_block<Iterator, T>, start, last, std::ref(results[i]));
        start = last;
    }

    // 5.主线程计算
    accumulate_block<Iterator, T>(start, end, results[ture_num - 1]);
    // 6.等待线程算完
    for (auto& t : threads) {
        t.join();
    }

    // 7.汇总结束
    int sum = 0;
    for (auto& t : results) {
        sum += t;
    }
    return sum;
}

int main() {
    std::vector<int> numbers(1000, 1);  // 1000 个 1
    int sum = parallel_accumulate(numbers.begin(), numbers.end(), 0);
    std::cout << "The sum is: " << sum << std::endl;  // 应该是 1000
    return 0;
}