#include <iostream>
#include <memory>
#include <string>
#include <thread>
void f(int i, std::string const& s) {
    std::cout << "Thread f: " << i << ", " << s << std::endl;
}

struct widget_data {
    int value = 0;
};
void update_data(widget_data& data) {
    data.value = 42;
}

class X {
public:
    void do_work(int i) {
        std::cout << "Member func: " << i << std::endl;
    }
};

class thread_guard {
public:
    std::thread& t;
    thread_guard(std::thread& t_) : t(t_) {}
    ~thread_guard() {
        if (t.joinable()) {
            t.join();
        }
    }
    thread_guard(thread_guard const&) = delete;
    thread_guard& operator=(thread_guard const) = delete;
};

void do_somethis() {
    std::cout << "\ndo_somethis\n" << std::endl;
}
class background_task {
public:
    void operator()() {
        do_somethis();
    }
};

struct big_object {
    void prepare(int n) {
        std::cout << "Big obj: " << n << std::endl;
    }
};
void process_big_object(std::unique_ptr<big_object> p) {
    p->prepare(100);
}
int main() {
    // background_task f;
    std::thread t((background_task()));
    t.join();
    char buffer[] = "data";
    std::thread t1(f, 1, std::string(buffer));
    t1.detach();

    widget_data data1;
    std::thread t2(update_data, std::ref(data1));
    // std::thread t2(update_data, data1);
    t2.join();
    std::cout << "data1:" << data1.value << std::endl;

    std::unique_ptr<big_object> p(new big_object);
    std::thread t3(process_big_object, std::move(p));
    t3.join();
    return 0;
}