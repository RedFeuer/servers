#include <gtest/gtest.h>
#include "client.hpp"
#include "test_servers.hpp"

TEST(FullCycleTest, ClientToDisplay) {
    /* Конфигурация портов */
    const int DATA_PORT = 9081;
    const int DISPLAY_PORT = 9082;

    /* Запуск серверов */
    TestServers servers(DATA_PORT, DISPLAY_PORT);
    servers.Start();

    /* Запуск клиента*/
    network::Client client("localhost", DATA_PORT);
    ASSERT_TRUE(client.connectToServer());

    /* Отправка тестовых данных */
    std::string test_data = "hello world hello";
    client.sendData(test_data);

    /* Ждем обработки и проверяем вывод display_server*/
    std::this_thread::sleep_for(std::chrono::seconds(1));

    /* Читаем вывод сервера отображения из файла*/
    std::ifstream log("display_server.log");
    ASSERT_TRUE(log.is_open());

    std::string output;
    std::getline(log, output);
    ASSERT_EQ(output, "Received: hello world");
}