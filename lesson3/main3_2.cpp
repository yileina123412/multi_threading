#include <exception>
#include <iostream>
#include <memory>
#include <mutex>
#include <stack>
#include <thread>
#include <vector>

// 1. 自定义异常类：当尝试从空栈弹出数据时抛出
struct empty_stack : std::exception {
    const char* what() const throw() {
        return "Empty stack! Nothing to pop.";
    }
};

template <typename T>
class threadsafe_stack {
private:
    std::stack<T> data;  // 内部封装的普通栈

    // 核心：互斥锁。它就像房门的钥匙，谁拿到谁才能改数据
    // mutable 关键字很重要：它允许我们在 const 函数（如 empty()）里上锁
    mutable std::mutex m;

public:
    threadsafe_stack() : data(std::stack<T>()) {}

    // 2. 线程安全的拷贝构造函数
    threadsafe_stack(const threadsafe_stack& other) {
        // 在拷贝之前，先锁住对方，保证拷贝过程中对方的数据不会变
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    }

    // 不允许直接赋值，防止产生复杂的锁管理问题
    threadsafe_stack& operator=(const threadsafe_stack&) = delete;

    // 3. 入栈操作
    void push(T new_value) {
        std::lock_guard<std::mutex> lock(m);  // 自动加锁
        data.push(std::move(new_value));
        // 函数结束，lock 自动解锁
    }

    // 4. 出栈操作（版本 A）：返回一个智能指针
    // 这样做是为了防止：在取出数据后、返回数据前，如果发生异常导致数据丢失
    std::shared_ptr<T> pop() {
        std::lock_guard<std::mutex> lock(m);

        // 在操作前先检查。这一步在锁内，所以不会被别人插队
        if (data.empty()) throw empty_stack();

        // 关键：在修改栈之前分配好返回值
        std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
        data.pop();
        return res;
    }

    // 出栈操作（版本 B）：通过引用把值传出去
    void pop(T& value) {
        std::lock_guard<std::mutex> lock(m);
        if (data.empty()) throw empty_stack();

        value = data.top();
        data.pop();
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(
            m);  // 只读也要锁，防止读到一半别人在改
        return data.empty();
    }
};

// --- 下面是测试代码 ---

int main() {
    threadsafe_stack<int> s;

    // 开启一个线程去塞数据
    std::thread t1([&s]() {
        for (int i = 0; i < 5; ++i) {
            s.push(i);
            std::cout << "Push: " << i << std::endl;
        }
    });

    // 开启另一个线程去抢数据
    std::thread t2([&s]() {
        for (int i = 0; i < 5; ++i) {
            try {
                // 尝试弹出并打印结果
                std::shared_ptr<int> res = s.pop();
                std::cout << "Pop: " << *res << std::endl;
            } catch (const empty_stack& e) {
                std::cout << e.what() << std::endl;
            }
        }
    });

    t1.join();
    t2.join();

    std::cout << "All work done!" << std::endl;
    return 0;
}