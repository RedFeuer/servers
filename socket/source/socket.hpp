#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <vector>
#include <string>
#include <cstddef>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h> // для InetPton
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h> // для inet_pton
#endif

namespace network {
    /**
     * @class Socket
     * @brief Инкапсулирует работу с TCP-сокетами
     *
     * Основные возможности:
     * - Создание клиентских/серверных сокетов
     * - Привязка к порту (сервер)
     * - Прослушивание подключений (сервер)
     * - Установка соединения (клиент)
     * - Передача данных
     */
    class Socket {
    public:
        /**
         * @brief Конструктор по умолчанию
         * @throw std::runtime_error Если не удалось инициализировать Winsock (Windows)
         */
        Socket();
        /**
         * @brief Деструктор (автоматически закрывает сокет)
         * Конструктор и деструктор для сокета обеспечивают использовании RAII для управления ресурсами сокетов
         */
        ~Socket();

        /**
         * @brief Создает TCP-сокет
         * @return true - сокет создан, false - ошибка
         *
         * Системные вызовы:
         * - Windows: socket(AF_INET, SOCK_STREAM, 0)
         * - POSIX: socket(AF_INET, SOCK_STREAM, 0)
         *  1) AF_INET -> используем IPv4
         *  2) SOCK_STREAM -> создание TCP
         *  3) 0 - протокол по умолчанию для TCP
         */
        bool create();

        /**
         * @brief Привязывает сокет к порту
         * @param port Номер порта (1-65535)
         * @return true - успех, false - ошибка
         *
         * Системные вызовы:
         * - bind() с параметрами:
         *   - AF_INET: IPv4
         *   - INADDR_ANY: все сетевые интерфейсы
         *   - Порт в сетевом формате (htons) - конвертируем
         */
        bool bind(int port);

        /**
         * @brief Начинает прослушивание подключений (сервер)
         * @param backlog Максимальная длина очереди ожидающих подключений
         * @return true - успех, false - ошибка
         */
        bool listen(int backlog = 5);

        /**
         * @brief Принимает входящее подключение (сервер)
         * @param clientSocket Сокет для общения с клиентом
         * @return true - подключение принято, false - ошибка
         *
         * Создает новый сокет для взаимодействия с клиентом.
         * После успеха clientSocket готов к send/recv.
         *
         * sockfd -  дескриптор серверного сокета (ожидает подключения).
         * (sockaddr*)&clientAddr - указатель на структуру, куда запишется адрес клиента.
         * &len - указатель на размер структуры (корректируется системой).
         */
        bool accept(Socket& clientSocket);

        /**
         * @brief Устанавливает соединение с сервером (клиент ->сервер)
         * @param host IP-адрес или доменное имя сервера
         * @param port Порт сервера
         * @return true - подключено, false - ошибка
         *
         * Преобразует адрес в бинарный формат (inet_pton) и вызывает connect().
         *
         * - AF_INET: IPv4
         *   - Порт в сетевом формате (htons) - конвертируем
         *   - InetPton/inet_pton(Win/Posix) конвертирует строку с хостом в бинарный формат
         */
        bool connect(const std::string& host, int port);

        /**
         * @brief Отправляет данные через сокет
         * @param data Буфер с данными
         * @return Количество отправленных байт или -1 при ошибке
         *
         * sockfd - дескриптор сокета.
         * data.data() - указатель на данные (строка).
         * data.size() - размер данных в байтах.
         * 0 - флаг выбора способа вызова (в данном случае по умолчанию)
         */
        ssize_t send(const std::string& data);
//        int send(const std::vector<char>& data);

        /**
         * @brief Принимает данные из сокета
         * @param[out] buffer Буфер для приема данных
         * @return Количество принятых байт, 0 при корректном закрытии подключения или -1(SOCKET_ERROR) при ошибке
         *
         * sockfd - дескриптор сокета.
         * buffer.data() - указатель на данные (строка).
         * buffer.size() - размер данных в байтах.
         * 0 - флаг выбора способа вызова (в данном случае по умолчанию)
         */
        ssize_t receive(std::string& buffer);
//        int receive(std::vector<char>& buffer);

        /**
         * @brief Закрывает сокет
         *
         * Безопасно закрывает сокет, если он не был закрыт ранее
         */
        void close();

    private:
        #ifdef _WIN32
        SOCKET sockfd = INVALID_SOCKED; ///< Дескриптор сокета (Windows)
        #else
        int sockfd = -1; ///< Дескриптор сокета (POSIX)
        #endif
        bool isClosed = true; ///< Флаг состояния сокета(закрыт/открыт)
    };
}

#endif