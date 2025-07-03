// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Danila Bespyatov

#ifndef SERVER_HPP
#define SERVER_HPP

/**
 * @file server.hpp
 * @brief Базовый абстрактный класс для серверных компонентов
 */

#include "socket.hpp"

namespace network {
    /**
     * @class Server
     * @brief Абстрактный класс для реализации различных серверов
     *
     * Определяет базовый набор операций, которые должны поддерживать все серверы:
     * - Запуск/остановка сервера
     * - Обработка клиентских подключений
     * - Работа с сокетами
     *
     * @note Для использования необходимо наследование и реализация чисто виртуальных методов
     */
    class Server {
    public:
        virtual ~Server() = default;
        virtual void start(int port) = 0;
        virtual void stop() = 0;
        virtual void handleClient(Socket& client) = 0;
    };
}

#endif