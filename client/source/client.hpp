// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Danila Bespyatov


#ifndef CLIENT_HPP
#define CLIENT_HPP

/**
 * @file Client.hpp
 * @brief Реализация клиентского класса для взаимодействия с сервером через HTTP и JSON
 */

#include "socket.hpp"
#include <string>
#include <nlohmann/json.hpp>

namespace network {
    /**
     * @class Client
     * @brief Класс для управления сетевым клиентом
     *
     * Отправляет данные на сервер в формате HTTP/1.1 с JSON-телом
     * и проверяет подтверждение (ACK).
     */
    class Client {
    public:
        /**
         * @brief Конструктор клиента
         * @param serverHost IP-адрес или доменное имя сервера
         * @param serverPort Порт сервера
         *
         * Инициализирует параметры подключения(адрес и порт сервера), но не устанавливает соединение.
         */
        Client(const std::string& serverHost, int serverPort);  // для коннекта к data_server

        /**
         * @brief Деструктор клиента
         *
         * Автоматически закрывает сокет при уничтожении объекта. Опять же RAII
         */
        ~Client();

        /**
         * @brief Устанавливает соединение с сервером
         * @return true - подключение успешно, false - ошибка
         *
         * Использует внутреннее поле clietnSocket для подключения.
         */
        bool connectToServer();

        /**
         * @brief Отправляет данные на сервер в формате HTTP+JSON
         * @param data Строка с данными для отправки в качестве запроса
         *
         * Формирует HTTP POST-запрос (используется архитектурой REST):
         * - Заголовок `Host`: адрес сервера
         * - Заголовок `Content-Type`: application/json
         * - Тело: JSON-объект вида {"data": "..."}
         */
        void sendData(const std::string& data);

        /**
         * @brief Проверяет подтверждение от сервера
         * @return true - получен статус 200 OK, false - иначе
         *
         * Принимает ответ сервера и проверяет наличие строки "HTTP/1.1 200 OK".
         */
        bool receiveAck();

        /**
         * @brief Геттер поля clientSocket
         * @return clientSocket
         */
        [[nodiscard]] Socket& getClientSocket() {return clientSocket;}

    private:
        Socket clientSocket;   ///< Дескриптор сокета data_server и состояние того, что соединение установлено. Дальнейшие send/recv через него
        std::string serverHost; ///< Адрес data_server
        int serverPort; ///< Порт data_server
    };
}

#endif