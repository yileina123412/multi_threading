#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>

struct some_resource {
    void dosomething() {
        std::cout << "Rosource working...\n" << std::endl;
    }
};

std::shared_ptr<some_resource> resource_ptr;
std::once_flag resource_flag;

void init_resource() {
    resource_ptr.reset(new some_resource);
}

void foo() {
    std::call_once(resource_flag, init_resource);
    resource_ptr->dosomething();
}

// --- 场景 2: 使用 std::shared_mutex 实现读写锁 (DNS 缓存) ---
class dns_cache {
    std::map<std::string, std::string> entries;
    mutable std::shared_mutex entry_mutex;  // 核心：读写锁

public:
    // 读者：共享访问，多个线程可以同时进入
    std::string find_entry(const std::string& domain) const {
        std::shared_lock<std::shared_mutex> lk(entry_mutex);  // 1. 共享锁
        auto it = entries.find(domain);
        return (it == entries.end()) ? "" : it->second;
    }

    // 作者：独占访问，必须等所有读者和作者都退出
    void update_or_add_entry(const std::string& domain, const std::string& ip) {
        std::lock_guard<std::shared_mutex> lk(entry_mutex);  // 2. 独占锁
        entries[domain] = ip;
    }
};

int main() {
    // std::call_once(init_flag, expensive_init);
    foo();
    dns_cache cache;
    cache.update_or_add_entry("google.com", "8.8.8.8");
    std::cout << "IP: " << cache.find_entry("google.com") << std::endl;

    return 0;
}