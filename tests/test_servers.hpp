#include <cstdio>
#include <string>
#include <thread>
#include <fstream>
#include <chrono>

class TestServers {
public:
    TestServers(int data_port, int display_port)
            : data_port_(data_port), display_port_(display_port) {}

    void Start() {
        /* Запуск сервера отображения в отдельном процессе*/
        display_server_ = std::thread([this]() {
            std::system(("./servers/displayServer/display_server_exec " + std::to_string(display_port_)).c_str());
        });

        /* Запуск сервера обработки данных */
        data_server_ = std::thread([this]() {
            // передаем только порты
            std::system((
                                "./servers/dataServer/data_server_exec " +
                                std::to_string(data_port_) + " " +
                                std::to_string(display_port_)
                        ).c_str());
        });

        /* Ждем инициализации серверов*/
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    ~TestServers() {
        /* Остановка серверов (для Linux)*/
        std::system("pkill -f data_server_exec");
        std::system("pkill -f display_server_exec");
        if (data_server_.joinable()) data_server_.join();
        if (display_server_.joinable()) display_server_.join();
    }

private:
    int data_port_;
    int display_port_;
    std::thread data_server_;
    std::thread display_server_;
};