#include <atomic>  // 原子操作头文件
#include <cassert>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

// ==========================================================
// 1. std::atomic_flag - 最基础、保证无锁的标志
// ==========================================================
void demo_atomic_flag() {
    std::cout << "--- Demo 1: std::atomic_flag ---" << std::endl;
    // 必须使用 ATOMIC_FLAG_INIT 初始化为清除状态
    std::atomic_flag lock_flag = ATOMIC_FLAG_INIT;

    // test_and_set(): 设置为true并返回旧值
    bool prev = lock_flag.test_and_set();
    std::cout << "初始设置后旧值 (应为0/false): " << prev << std::endl;

    prev = lock_flag.test_and_set();
    std::cout << "再次设置后旧值 (应为1/true): " << prev << std::endl;

    // clear(): 清除状态（设为false）
    lock_flag.clear();
    std::cout << "已执行 clear()" << std::endl;
}

// ==========================================================
// 2. std::atomic<bool> - 读、写、交换
// ==========================================================
void demo_atomic_bool() {
    std::cout << "\n--- Demo 2: std::atomic<bool> ---" << std::endl;
    std::atomic<bool> b(false);  // 可以用普通 bool 初始化

    // store(): 原子写入
    b.store(true);

    // load(): 原子读取
    std::cout << "load() 读取值: " << b.load() << std::endl;

    // exchange(): 写入新值并返回旧值
    bool old_val = b.exchange(false);
    std::cout << "exchange(false) 返回的旧值: " << old_val << std::endl;
    std::cout << "当前值: " << b.load() << std::endl;
}

// ==========================================================
// 3. Compare and Exchange (CAS) - 无锁编程的核心
// ==========================================================
void demo_cas() {
    std::cout << "\n--- Demo 3: Compare and Exchange (CAS) ---" << std::endl;
    std::atomic<int> atom_val(10);
    int expected = 10;
    int desired = 20;

    // compare_exchange_strong(expected, desired):
    // 1. 如果 atom_val == expected，则 atom_val = desired，返回 true
    // 2. 如果 atom_val != expected，则 expected = atom_val，返回 false
    bool success = atom_val.compare_exchange_strong(expected, desired);

    std::cout << "第一次比较是否成功: " << success << " (应为1)" << std::endl;
    std::cout << "当前 atom_val: " << atom_val.load() << " (应为20)"
              << std::endl;

    // 故意让比较失败
    expected = 50;  // 我以为它是 50
    success = atom_val.compare_exchange_strong(expected, desired);

    std::cout << "第二次比较是否成功: " << success << " (应为0)" << std::endl;
    std::cout << "expected 被自动更新为实际值: " << expected << " (应为20)"
              << std::endl;
}

// ==========================================================
// 4. std::atomic<T*> - 原子指针运算
// ==========================================================
void demo_atomic_pointer() {
    std::cout << "\n--- Demo 4: std::atomic<T*> ---" << std::endl;
    int data[5] = {10, 20, 30, 40, 50};
    std::atomic<int*> p(data);

    // fetch_add(n): 指针移动 n，返回“移动前”的旧指针
    int* old_p = p.fetch_add(2);
    std::cout << "移动前的旧值 (data[0]): " << *old_p << std::endl;
    std::cout << "移动后的新值 (data[2]): " << *p.load() << std::endl;

    // 操作符重载: p -= 1 相当于 p.fetch_sub(1)，但返回“移动后”的新值
    int* new_p = (p -= 1);
    std::cout << "p -= 1 后返回的新值 (data[1]): " << *new_p << std::endl;
}

// ==========================================================
// 5. 原子整型运算 - 计数器
// ==========================================================
void demo_atomic_int() {
    std::cout << "\n--- Demo 5: Atomic Integer Ops ---" << std::endl;
    std::atomic<int> counter(0);

    counter++;                 // 原子自增，返回新值
    counter.fetch_add(5);      // 原子加 5，返回旧值
    counter.fetch_or(0b1010);  // 原子按位或运算

    std::cout << "最终计数器值: " << counter.load() << std::endl;

    // 检查是否硬件无锁
    std::cout << "int 这里的操作是否无锁: " << counter.is_lock_free()
              << std::endl;
}

// ==========================================================
// 6. std::shared_ptr 的原子操作
// ==========================================================
void demo_shared_ptr_atomic() {
    std::cout << "\n--- Demo 6: Atomic std::shared_ptr ---" << std::endl;
    std::shared_ptr<int> p = std::make_shared<int>(100);

    // 智能指针不是原子类型，必须使用非成员函数
    std::shared_ptr<int> local = std::atomic_load(&p);
    std::cout << "原子读取的 shared_ptr 值: " << *local << std::endl;

    std::shared_ptr<int> new_p = std::make_shared<int>(200);
    std::atomic_store(&p, new_p);
    std::cout << "原子更新后的值: " << *std::atomic_load(&p) << std::endl;
}

int main() {
    demo_atomic_flag();
    demo_atomic_bool();
    demo_cas();
    demo_atomic_pointer();
    demo_atomic_int();
    demo_shared_ptr_atomic();

    return 0;
}