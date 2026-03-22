#include <algorithm>
#include <future>
#include <iostream>
#include <list>

template <typename T>
std::list<T> parallel_quick_sort(std::list<T> input) {
    if (input.empty()) return input;

    std::list<T> result;
    result.splice(result.begin(), input, input.begin());
    T const& pivot = *result.begin();

    auto divide_point = std::partition(
        input.begin(), input.end(), [&](T const& t) { return t < pivot; });
    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);

    std::future<std::list<T>> new_lower =
        std::async(&parallel_quick_sort<T>, std::move(lower_part));

    // 5. 当前线程递归处理后半部分
    auto new_higher = parallel_quick_sort(std::move(input));

    result.splice(result.end(), new_higher);
    result.splice(result.begin(), new_lower.get());

    return result;
}

int main() {
    std::list<int> data = {5, 3, 8, 4, 1, 7, 2, 9, 6};
    auto sorted = parallel_quick_sort(data);
    for (int x : sorted) std::cout << x << " \n";  // 输出: 1 2 3 4 5 6 7 8 9
    return 0;
}