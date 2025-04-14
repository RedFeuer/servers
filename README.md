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

1-й терминал:
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

##  Тестирование
К сожалению, немного не хватило времени исправить Race condition в Integration_tests.cpp
Там возможно ситуация, когда data_server запустится раньше, чем display_server и тогда коннекта не происходит и данные не доходят.
В случае когда запуск происходит в верном порядке 1) display_server 2) data_server 3) client ВСЕ РАБОТАЕТ ПРАВИЛЬНО И ЭТО ВИДНО, ЕСЛИ ПОЗАПУСКАТЬ ТЕСТЫ НЕСКОЛЬКО РАЗ. Рано или поздно все пройдутся
Очень жаль, что не успел исправить и не смог... Уже ночь, голова не соображает...

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

##  Платформа

Разработка и тестирование проекта велись в **Linux-среде**. На **Windows** проект не тестировался, поэтому лучше тестировать в Linux. Работу в Linux я гарантирую!

---

##  Видеодемонстрация

Видеоролик с демонстрацией работы и кратким объяснением проекта доступен: в директории инструкция_по_применению

---

##  Выполнены все дополнительные задания (так называемые "Будет плюсом")

1. **HTTP REST + JSON**:
    - Использован протокол **HTTP/1.1**
    - Заголовки:
        - `Host`
        - `Content-Type: application/json`
    - Формат тела: JSON `{"data": "..."}`

2. **Тесты GTest**:
    - Интеграционные тесты
    - Unit-тесты

3. **CMake** для сборки

4. **Документация Doxygen(дополнительно)**

5. **Покрытие тестами (coverage-rate) (дополнительно)**

---

