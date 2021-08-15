#include "stata.h"
#include <iostream>

void test_func() {
    for (int i = 0; i < 100000; i++) {
        std::string name("test_func_" + std::to_string(i % 10));
        std::cout << name << std::endl;
        stata::Unit u(name);
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        u.SetCount(10);
        u.End();
    }
}


int main(int argc, char **argv) {
    FLAGS_log_dir = "/tmp";
    FLAGS_max_log_size = 4;
    google::InitGoogleLogging(argv[0]);
    for (int i = 0; i < 5; i++) {
        std::thread t(test_func);
        t.join();
    }
    google::ShutdownGoogleLogging();
}
