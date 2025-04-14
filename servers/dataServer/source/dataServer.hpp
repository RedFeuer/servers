#ifndef DATASERVER_HPP
#define DATASERVER_HPP

/**
 * @file dataServer.hpp
 * @brief Класс сервера обработки данных DataServer
 */

#include "server.hpp"
#include <string>

namespace network {
    /**
     * @class DataServer
     * @brief Сервер для обработки данных клиентов и передачи результатов на сервер отображения
     *
     * Наследуется от базового класса Server. Обрабатывает подключения клиентов, удаляет дубликаты слов
     * (без учета регистра) и пересылает результаты на сервер отображения.
     */
    class DataServer : public Server {
    public:
        /**
         * @brief Конструктор сервера обработки данных
         * @param displayHost Хост сервера отображения
         * @param displayPort Порт сервера отображения
         */
        DataServer(const std::string& displayHost, int displayPort);

        /**
         * @brief Деструктор сервера
         * @note Автоматически останавливает сервер и закрывает соединения
         */
        ~DataServer() override;

        /**
         * @brief Запуск сервера на указанном порту
         * @param port Номер порта для прослушивания
         * @throw std::runtime_error При ошибках привязки, прослушивания порта или подключения
         */
        void start(int port) override;

        /**
         * @brief Остановка сервера(закрытие соединений)
         */
        void stop() override;

        /**
         * @brief Обработка клиентского подключения
         * @param client Сокет подключенного клиента
         * @throw std::invalid_argument При некорректных HTTP-запросах
         * @throw std::runtime_error При ошибках подключения к серверу отображения
         */
        void handleClient(Socket& client) override;

        /**
         * @brief Геттер поля serverSocket
         * @return serverSocket
         */
        [[nodiscard]] Socket& getServerSocket() {return serverSocket;}

        /**
         * @brief Геттер поля displaySocket
         * @return displaySocket
         */
        [[nodiscard]] Socket& getDisplaySocket() {return displaySocket;}

        /**
         * @brief Геттер поля isRunning
         * @return isRunning
         */
        [[nodiscard]] bool getIsRunning() const {return isRunning;}

        /**
         * @brief Сеттер поля isRunning
         * Устанавливаем в isRunning переданный статус
         * @param status Новый статус работы
         */
        void setIsRunning(bool status) {isRunning = status;}

    private:
        Socket serverSocket;   ///< Сокет для подключения клиентов
        Socket displaySocket; ///< Сокет для связи с сервером отображения

        std::string displayHost; ///< Адрес сервера отображения
        int displayPort; ///< Порт сервера отображения
        bool isRunning = false; ///< Статус работы сервера

        /**
         * @brief Обработка входной строки (удаление дубликатов слов)
         * Удаление всех похожих слов независимо от регистра, оставляя первое вошеднее уникальное слово
         * @param input Входная строка от клиента
         * @return std::string Обработанная строка без дубликатов
         * @example "Hello hello WORLD world" → "Hello WORLD"
         */
        std::string processData(const std::string& input);
    };
}

#endif