#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <sstream>

using namespace std::chrono_literals;

class IntegrationTest : public ::testing::Test {
protected:
    static constexpr int DISPLAY_PORT = 9090;
    static constexpr int DATA_PORT = 9080;
    static constexpr const char* DISPLAY_HOST = "127.0.0.1";

    std::thread display_thread;
    std::thread data_thread;
    std::string test_output;

    void SetUp() override {
        /*запуск display_server в отдельном потоке*/
        display_thread = std::thread([this]() {
            std::system("./servers/displayServer/display_server_exec 9090 > test_output.log 2>&1");
        });

        /*запуск data_server в отдельном потоке*/
        data_thread = std::thread([]() {
            std::system("./servers/dataServer/data_server_exec 9090 9080");
        });
    }

    void TearDown() override {
        // Остановка серверов
        std::system("pkill -f display_server_exec");
        std::system("pkill -f data_server_exec");

        if (display_thread.joinable()) display_thread.join();
        if (data_thread.joinable()) data_thread.join();
    }
};

TEST_F(IntegrationTest, FullCycleTestExample1) {
    /*сообщение клиента*/
    const std::string test_message = "HELLO hello WORLD world";
    /*ожидаемый вывод для такого сообщения*/
    const std::string expected_output = "Received data: \"HELLO WORLD\"";

    /*временный файл с входными данными*/
    std::ofstream input_file("test_input.txt");
    input_file << test_message << "\nexit\n";
    input_file.close();

    /* запуск приложение клиента с перенаправлением ввода на чтение из test.input.txt*/
    std::string command =
            "./client/client_exec 127.0.0.1 9080 < test_input.txt > /dev/null 2>&1";

    int result = std::system(command.c_str());
    ASSERT_EQ(result, 0) << "Client execution failed";

    /*дам время на обработку данных*/
    std::this_thread::sleep_for(0.5s);

    /*проверка вывода на display_server*/
    std::ifstream log_file("test_output.log");
    test_output = std::string((std::istreambuf_iterator<char>(log_file)),
                              std::istreambuf_iterator<char>());
//    std::cout<<std::endl <<test_output << std::endl << std::endl;
    ASSERT_NE(test_output.find(expected_output), std::string::npos)
                                << "Expected output not found: " << expected_output;
}

TEST_F(IntegrationTest, FullCycleTestExample2) {
    /*сообщение клиента*/
    const std::string test_message = "     hello    worlD      hElLo    WORLD              WORLD           hello           HELLO           world         word          WOrlD       worLd    worlds     hello    worlD      hElLo    WORLD              WORLD           hello           HELLO           world         word          WOrlD       worLd    worlds     hello    worlD      hElLo    WORLD              WORLD           hello           HELLO           world         word          WOrlD       worLd    worlds     hello    worlD      hElLo    WORLD              WORLD           hello           HELLO           world         word          WOrlD       worLd    worlds     hello    worlD      hElLo    WORLD              WORLD           hello           HELLO           world         word          WOrlD       worLd    worlds";
    /*ожидаемый вывод для такого сообщения*/
    const std::string expected_output = "Received data: \"hello worlD word worlds\"";

    /*временный файл с входными данными*/
    std::ofstream input_file("test_input.txt");
    input_file << test_message << "\nexit\n";
    input_file.close();

    /* запуск приложение клиента с перенаправлением ввода на чтение из test.input.txt*/
    std::string command =
            "./client/client_exec 127.0.0.1 9080 < test_input.txt > /dev/null 2>&1";

    int result = std::system(command.c_str());
    ASSERT_EQ(result, 0) << "Client execution failed";

    /*дам время на обработку данных*/
    std::this_thread::sleep_for(1s);

    /*проверка вывода на display_server*/
    std::ifstream log_file("test_output.log");
    test_output = std::string((std::istreambuf_iterator<char>(log_file)),
                              std::istreambuf_iterator<char>());
//    std::cout<<std::endl <<test_output << std::endl << std::endl;
    ASSERT_NE(test_output.find(expected_output), std::string::npos)
                                << "Expected output not found: " << expected_output;
}