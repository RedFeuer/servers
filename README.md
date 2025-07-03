# Клиент-серверное приложение на C++

##  Суть проекта

Проект реализует клиент-серверную архитектуру с двумя серверами и клиентов:
- `data_server` — принимает данные от клиента и обрабатывает их.
- `display_server` — получает результат от `data_server` и отображает его.
- `client` - принимает ввод с клавиатуры и отправляет на обработку `data_server`

Взаимодействие между клиентом и серверами реализовано через HTTP/1.1 с использованием REST-архитектуры и передачи JSON-объектов.

Клиент отправляет POST-запрос следующего вида:

POST /process HTTP/1.1
Host: 127.0.0.1:9080
Content-Type: application/json
Content-Length: 23

{"data":"Hello hello world"}


---

##  Инструкция по запуску проекта

Также можно посмотреть как я делал это в видео в директории инструкция_по_применению

1-й терминал(в конце прервать через Ctrl+C):
```bash
rm -rf build && mkdir build && cd build
clear && cmake .. && make && clear
./servers/displayServer/display_server_exec 9090
```
2-й терминал:
```bash
cd build && clear
./servers/dataServer/data_server_exec 9090 9080
```
3-й терминал:
```bash
cd build && clear
./client/client_exec 127.0.0.1 9080
```

---

```bash
rm -rf build && mkdir build && cd build
clear && cmake .. -DTESTING_BUILD=ON && make
./tests/full_tests
```

###  Реализованы все уровни тестирования:
- **Интеграционные тесты**: тестируют взаимодействие между клиентом и серверами.
- **Unit-тесты**: покрывают отдельные методы и функции.

Тесты написаны с использованием **Google Test (GTest)**.

---

##  Документация Doxygen

Документация сгенерирована с помощью Doxygen.

```bash
doxygen Doxyfile
```

---

##  Покрытие тестами (Coverage)

```bash
rm -rf build && mkdir build && cd build
clear && cmake .. -DTESTING_BUILD=ON && make
./tests/full_tests
lcov --ignore-errors inconsistent --capture --directory . --output-file coverage.info
genhtml --ignore-errors inconsistent coverage.info --output-directory coverage_report
```

---

