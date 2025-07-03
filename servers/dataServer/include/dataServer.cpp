// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Danila Bespyatov

#include "dataServer.hpp"
#include <nlohmann/json.hpp>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <iostream>

namespace network {
    DataServer::DataServer(const std::string &displayHost, int displayPort)
    : displayHost(displayHost), displayPort(displayPort) {}

    DataServer::~DataServer() {
        stop();
    };

    void DataServer::start(int port) {
        /*не смогли привязать сокет к порту*/
        if (!serverSocket.bind(port)) {throw std::runtime_error("Bind to client port" + std::to_string(port) + "failed");}
        /*не смогли активировать прослушивание*/
        if (!serverSocket.listen()) {throw std::runtime_error("Listen failed");}
        isRunning = true;

        /*Буквально по заданию
         * После запуска, сервер обработки данных начинает ожидать подключения клиента.
        Сервер обработки данных должен подключиться к серверу отображения результата.
        */
        while (isRunning) {
            Socket clientSocket;
            if (serverSocket.accept(clientSocket)) {
                try { // так как handleClient выкидывает исключения
                    handleClient(clientSocket);
                }
                catch (...) { // все ошибки прокидываем дальше, чтобы потом обработать
                    throw;
                }
            }
            else {
                /*Буквально по заданию
                 * Если сервер не доступен или произошла ошибка подключения, то следует вывести ошибку на экран
                 * и прекратить ожидание ввода данных.*/
                throw std::runtime_error("Connection error(Client->Data_Server)"); // ВЫВОД ОШИБКИ ПРИ ОШИБКЕ ПОДКЛЮЧЕНИЯ
            }
        }
    }

    void DataServer::stop() {
        if (isRunning) {
            serverSocket.close();
            displaySocket.close();
            isRunning = false;
        }
    }

    /*ПОСЛЕ ПРОВЕРКИ НА РАБОТОСПОСОБНОСТЬ РАЗДЕЛИТЬ НА МЕТОДЫ ПОМЕНЬШЕ*/
    void DataServer::handleClient(network::Socket &client) {
        std::string request;
        client.receive(request); // получаем данные от клиента в буфер request

        /*подстрока "\r\n\r\n" - есть разделитель между заголовком и телом в HTTP POST*/
        std::size_t body_pos = request.find("\r\n\r\n");
        if (body_pos == std::string::npos) {
            throw std::invalid_argument("Invalid HTTP request");
        }

        /*пропускаем "\r\n\r\n", чтобы попасть к телу сообщения*/
        std::string jsonBody = request.substr(body_pos + 4);
        /*парсим строку, чтобы получить объект nlohmann::json*/
        nlohmann::json inputJson = nlohmann::json::parse(jsonBody);

        /*ПРОВЕРКА ПЕРЕДАННЫХ ДАННЫХ НА КОРРЕКТНОСТЬ*/
        if (!inputJson.contains("data")) { // чтобы не получить nullptr
            /*в теле сообщения нет data(клиент не передал сообщение)*/
            std::string errorResponse = "HTTP/1.1 400 Bad Request\r\n"
                                        "Content-Type: application/json\r\n"
                                        "Content-Length: 47\r\n\r\n"
                                        "{\"error\":\"Missing required 'data' field\"}";

            /*передаем ошибку клиенту*/
            client.send(errorResponse);
            return;
        }
        std::string data = inputJson["data"];

        /*удаление из сообщения клиента дубликатов слов*/
        std::string resultData = processData(data);

        /*формирование запроса на сервер отображения display_server*/
        nlohmann::json resultJson;
        resultJson["result"] = resultData;
        std::string response = "HTTP/1.1 200 OK\r\n"
                               "Content-Type: application/json\r\n"
                               "Content-Length:" + std::to_string(resultJson.dump().size()) + "\r\n"
                               + "\r\b" + resultJson.dump();
        /*сообщаем клиенту, что инфа дошла*/
        client.send(response);

        std::string result = "POST /process HTTP/1.1\r\n"         // метод запроса и путь
                              "Host: " + displayHost + "\r\n"       // адрес сервера
                              "Content-Type: application/json\r\n" // тип содержимого
                              "Content-Length: " + std::to_string(resultJson.dump().size()) + "\r\n" // длина тела запроса
                              "\r\n" + resultJson.dump();   // тело запроса

        /*ТУТ ДОПИСАТЬ СВЯЗЬ С display_server и отправку туда данных*/
        /*ПРОВЕРИТЬ ЛОГИКУ connect!!!!! чето я запутался, наверное пора спать*/

        /*каждый раз создаем новый коннект с data_server, иначе это не новое соединение
         * и if (serverSocket.accept(clientSocket)) не пропустит к обработке данных
         * и отправке на сервер*/
        network::Socket displaySocket_;
        if (!displaySocket_.connect(displayHost, displayPort)) {
            throw std::runtime_error("Display server connection failed");
        }
        displaySocket_.send(result);
        displaySocket_.close();
    }

    /*НЕ СОВСЕМ ПОНЯЛ В ИТОГЕ ЧТО ИМЕЕТСЯ В ВИДУ ПОД ДУБЛИКАТАМИ
     * Я УДАЛЯЛ ВСЕ ПОХОЖИЕ СЛОВА НЕЗАВИСИМО ОТ РЕГИСТРА, ОСТАВЛЯЯ ПЕРВОЕ ВОШЕДШЕЕ УНИКАЛЬНОЕ.
     * ОБЪЯСНЮ НА ПРИМЕРЕ:
     * Ввод: "Hello hello WORLD world"
     * результат: "Hello WORLD"*/
    std::string DataServer::processData(const std::string &input) {
        /*создаем поток iss из строки input. последовательно возвращает слова из input*/
        std::istringstream iss(input);
        /*вектор уникальных слов в оригинальном регистре в правильном порядке. из него будем делать результат*/
        std::vector<std::string> uniqueWords;
        /*вспомогательное множество просмотренных слов в нижнем регистре. в нем нет повторяющихся(по определению set)*/
        std::unordered_set<std::string> seenWords;

        std::string word;
        while (iss >> word) {
            std::string lowerWord;
            for (char c: word) {
                lowerWord += static_cast<char>(std::tolower(c));
            }
            if (seenWords.find(lowerWord) == seenWords.end()) { // такого нет еще в seenWords
                uniqueWords.push_back(word);   // сохраняем оригинальное слово
                seenWords.insert(lowerWord); // добавляем для сравнение слово в нижнем регистре
            }
        }

        std::ostringstream oss;
        for (const auto& wrd : uniqueWords) {
            oss << wrd << ' '; // пишем слова в поток вывода ЧЕРЕЗ ОДИН ПРОБЕЛ(последний нужно удалить)
        }
        /*если уникальных слов в сообщении не было(ну то есть вообще никаких слов в сообщении не было),
         * то лишний пробел и не поставится*/

        std::string result = oss.str(); // получаем результат с лишним пробелом в конце
        if (!result.empty()) { // если строка пустая, то лишнего пробела нет
            result.pop_back(); // удаляем лишний пробел
        }

        return result;
    }
}