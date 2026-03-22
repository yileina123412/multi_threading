#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
// 模拟银行账户
struct BankAccount {
    int balance = 100;
    std::mutex m;
};
// 转帐
void transfer(BankAccount& account, int amount) {
    // 这就算上了锁了
    std::lock_guard<std::mutex> guard(account.m);
    int current = account.balance;
    // 模拟耗时
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    account.balance = current + amount;
}

int main() {
    BankAccount mycount;
    std::thread t1(transfer, std::ref(mycount), 10);
    std::thread t2(transfer, std::ref(mycount), 10);

    t1.join();
    t2.join();
    std::cout << "bank account:" << mycount.balance << "\n" << std::endl;
    return 0;
}