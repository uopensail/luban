#include "metric.h"
#include <iostream>

void test_func()
{
    for (int i = 0; i < 1000; i++)
    {
        std::string name("test_func_" + std::to_string(i % 10));
        std::cout << name << std::endl;
        std::cout << 11111111 << std::endl;
        metric::Unit u(name);
        std::cout << 22222222 << std::endl;
        //std::this_thread::sleep_for(std::chrono::microseconds(1));

        std::cout << 11111111 << std::endl;
        u.SetCount(10);
        std::cout << 44444444 << std::endl;
        u.End();
        std::cout << 55555555 << std::endl;
    }
}

int main(int argc, char **argv)
{
    FLAGS_log_dir = "/tmp";
    FLAGS_max_log_size = 4;
    google::InitGoogleLogging(argv[0]);
    metric::start();
    for (int i = 0; i < 5; i++)
    {
        std::thread t(test_func);
        t.join();
    }
    metric::stop();
    google::ShutdownGoogleLogging();
}
