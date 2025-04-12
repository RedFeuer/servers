#include "dataServer.hpp"
#include <nlohmann/json.hpp>

namespace network {
    DataServer::DataServer(const std::string &displayHost, int displayPort)
    : displayHost(displayHost), displayPort(displayPort) {}

    DataServer::~DataServer() {
//        serverSocket.close();
//        displaySocket.close();
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
            Socket cliendSocket;
            if (serverSocket.accept(cliendSocket)) {
                try { // так как handleClient выкидывает исключения
                    handleClient(cliendSocket);
                }
                catch (...) { // все ошибки прокидываем дальше, чтобы потом обработать
                    throw;
                }
            }
            else {
                /*Буквально по заданию
                 * Если сервер не доступен или произошла ошибка подключения, то следует вывести ошибку на экран
                 * и прекратить ожидание ввода данных.*/
                throw std::runtime_error("Connection error"); // ВЫВОД ОШИБКИ ПРИ ОШИБКЕ ПОДКЛЮЧЕНИЯ
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

        if (!inputJson.contains("data")) { // чтобы не получить nullptr
            /*в теле сообщения нет data(клиент не передал сообщение)*/
            std::string errorResponse = "HTTP/1.1 400 Bad Request\r\n"
                                        "Content-Type: application/json\r\n"
                                        "Content-Length: 47\r\n\r\n"
                                        "{\"error\":\"Missing required 'data' field\"}";
            client.send(errorResponse);
            return;
        }
        std::string data = inputJson["data"];

        /*удаление из сообщения клиента дубликатов слов*/
        std::string result = processData(data);

        /*ТУТ ДОПИСАТЬ СВЯЗЬ С display_server и отправку туда данных*/
    }
}