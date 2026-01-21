# Search Engine (C++) — финальная работа

Небольшой консольный поисковый движок: читает набор документов, строит инвертированный индекс и обрабатывает поисковые запросы из JSON-конфига, формируя ответы также в JSON.

Проект разделён на два подпроекта:
- `search_engine` — основная программа
- `unit_tests` — модульные тесты (GoogleTest)

---

## Возможности

- Загрузка списка документов из `Conf/config.json`
- Построение **инвертированного индекса** (слово → список документов и частоты)
- Обработка запросов из `Conf/requests.json`
- Вычисление релевантности документов и формирование ответа в `Conf/answers.json`
- Многопоточность через простой thread pool (используется при индексации/поиске)
- Unit-тесты на GoogleTest

---

## Структура проекта

```
p_1/
  CMakeLists.txt
  include/                 # заголовочные файлы
    ConverterJSON.h
    InvertedIndex.h
    SearchServer.h
    threadpool.h
    header.h
    json/json-develop/     # nlohmann/json (подключается CMake)
  src/
    CMakeLists.txt
    main.cpp
    ConverterJSON.cpp
    InvertedIndex.cpp
    SearchServer.cpp
  Test/
    CMakeLists.txt
    test.cpp
    test.h
    test_Search.h
    test_doc_*.txt
  Conf/
    config.json
    requests.json
    answers.json           # генерируется программой
  documents/               # (опционально) папка с файлами документов
  build/                   # (опционально) каталог сборки
```

> Важно: относительные пути в тестах предполагают запуск `unit_tests` **из корня проекта**.

---

## Форматы JSON

### `Conf/config.json`

Пример:
```json
{
  "config": {
    "name": "SearchEngine",
    "version": "0.1",
    "max_responses": 5,
    "threads": 4,
    "files": [
      "documents/doc1.txt",
      "documents/doc2.txt"
    ]
  }
}
```

Поля:
- `files` — массив путей к документам (текстовые файлы).
- `max_responses` — максимальное число документов в выдаче (если не задано, обычно по ТЗ = 5).
- `threads` — количество потоков для распараллеливания.

### `Conf/requests.json`

Пример:
```json
{
  "requests": [
    "milk water",
    "sugar"
  ]
}
```

Каждая строка — запрос (несколько слов через пробел).

### `Conf/answers.json`

Примерный формат:
```json
{
  "answers": {
    "request001": {
      "result": true,
      "relevance": [
        { "docid": 2, "rank": 1.0 },
        { "docid": 0, "rank": 0.7 }
      ]
    },
    "request002": {
      "result": false
    }
  }
}
```

- `requestNNN` — номер запроса (001, 002, …).
- `result` — найдено ли хоть что-то.
- `relevance` — массив найденных документов с `docid` и `rank` (относительная релевантность).

---

## Сборка (Linux / Kali / Debian)

### Требования
- CMake (желательно 3.20+)
- Компилятор C++ (g++/clang++)
- Доступ к интернету **или** локальная копия googletest (если FetchContent тянет из сети)

### Сборка (рекомендуется)
Из корня проекта:

```bash
cmake -S . -B build
cmake --build build -j$(nproc)
```

Где окажутся бинарники — зависит от настроек CMake:
- либо в `build/`
- либо в корне проекта (если выставлен `CMAKE_RUNTIME_OUTPUT_DIRECTORY`)

Проверь:
```bash
ls -la search_engine unit_tests || true
find build -maxdepth 3 -type f -executable || true
```

---

## Запуск основной программы

Из корня проекта:

```bash
./search_engine
```

После выполнения программа формирует:
- `Conf/answers.json`

> Убедись, что `Conf/config.json` и `Conf/requests.json` присутствуют и корректны.

---

## Сборка и запуск тестов

### Собрать тесты
Если target тестов называется `unit_tests`:

```bash
cmake --build build --target unit_tests -j$(nproc)
```

### Запустить тесты
Запуск из корня проекта (важно для путей к `Test/test_doc_*.txt`):

```bash
./unit_tests
```

### Запуск через CTest (если тесты зарегистрированы)
```bash
ctest --test-dir build -V
```

---

## Частые проблемы

### 1) `cmake ..` ругается на отсутствие `CMakeLists.txt`
Запускай `cmake ..` **только из `build/`**:
```bash
mkdir -p build && cd build
cmake ..
```

Или используй современный способ:
```bash
cmake -S . -B build
```

### 2) Бинарник не находится по пути `./build/search_engine`
Проверь, куда кладутся исполняемые файлы:
```bash
find build -maxdepth 3 -type f -executable
```
или:
```bash
ls -la search_engine unit_tests
```

### 3) Тесты не видят файлы `Test/test_doc_*.txt`
Запускай тесты из корня проекта:
```bash
cd /path/to/p_1
./unit_tests
```

---

## Лицензия
Учебный проект.
