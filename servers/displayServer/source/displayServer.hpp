// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Danila Bespyatov

#ifndef DISPLAYSERVER_HPP
#define DISPLAYSERVER_HPP

/**
 * @file displayServer.hpp
 * @brief Класс сервера отображения данных DisplayServer
 */

#include "server.hpp"

namespace network {
    /**
     * @class DisplayServer
     * @brief Сервер для получения и отображения обработанных данных
     *
     * Наследуется от базового класса Server. Принимает данные от сервера обработки
     * и выводит их в стандартный вывод(stdout).
     */
    class DisplayServer : public Server {
    public:
        /**
         * @brief Деструктор сервера
         * @note Автоматически останавливает сервер и закрывает соединения
         */
        ~DisplayServer() override;

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
         * @brief Обработка подключения от сервера обработки данных
         * @param client Сокет подключенного клиента (data server)
         * @throw std::invalid_argument При некорректных HTTP-запросах
         */
        void handleClient(Socket& client) override;

        /**
         * @brief Геттер поля serverSocket
         * @return serverSocket
         */
        [[nodiscard]] Socket & getServerSocket() {return serverSocket;}

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
        Socket serverSocket;  ///< Сокет для подключений от data server
        bool isRunning = false; ///< Статус работы сервера
    };
}

#endif