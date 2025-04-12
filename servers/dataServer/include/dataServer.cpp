#include "dataServer.hpp"
#include <nlohmann/json.hpp>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_set>

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
        std::string result = processData(data);

        /*формирование запроса на сервер отображения display_server*/
        nlohmann::json outputJson;
        outputJson["result"] = result;
        std::string response = "HTTP/1.1 200 OK\r\n"
                               "Content-Type: application/json\r\n"
                               "Content-Length:" + std::to_string(outputJson.dump().size()) + "\r\n"
                               + "\r\b" + outputJson.dump();

        /*ТУТ ДОПИСАТЬ СВЯЗЬ С display_server и отправку туда данных*/
        /*ПРОВЕРИТЬ ЛОГИКУ connect!!!!! чето я запутался, наверное пора спать*/
        if (!displaySocket.connect(displayHost, displayPort)) {
            throw std::runtime_error("Display server connection failed");
        }
        displaySocket.send(result);
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