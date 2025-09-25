# 📂 Структура проекта

```
quiz_bot/
├── CMakeLists.txt
├── main.cpp
├── Dockerfile
├── docker-compose.yml
```

---

# ✅ `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.10)
project(quiz_bot)

set(CMAKE_CXX_STANDARD 17)

find_package(TgBot REQUIRED)

add_executable(quiz_bot main.cpp)
target_link_libraries(quiz_bot TgBot pthread ssl crypto)
```

---

# ✅ `main.cpp`

(тот же код викторины с фото, который я тебе написал ранее — оставляем без изменений)

---

# ✅ `Dockerfile`

```dockerfile
FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

# Устанавливаем зависимости
RUN apt-get update && apt-get install -y \
    g++ cmake make git \
    libssl-dev zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*

# Скачиваем и собираем tgbot-cpp
WORKDIR /opt
RUN git clone https://github.com/reo7sp/tgbot-cpp.git && \
    cd tgbot-cpp && cmake . && make -j4 && make install

# Копируем проект
WORKDIR /app
COPY . .

# Собираем
RUN cmake . && make -j4

# Запуск
CMD ["./quiz_bot"]
```

---

# ✅ `docker-compose.yml`

```yaml
version: "3.8"

services:
  quizbot:
    build: .
    container_name: quiz_bot
    restart: always
    environment:
      - BOT_TOKEN=${BOT_TOKEN}
```

---

# ✅ `.env`

Создай `.env` рядом с `docker-compose.yml`:

```
BOT_TOKEN=твой_токен_от_BotFather
```

---

# ⚙️ Сборка и запуск

1. Сборка образа:

```bash
docker-compose build
```

2. Запуск бота:

```bash
docker-compose up -d
```

3. Логи:

```bash
docker-compose logs -f
```

---

# 🚀 Что мы получили

* 🐳 Контейнер с C++ ботом.
* 📸 Викторина с фото + inline-кнопки.
* 🔄 Автоперезапуск при падении.
* 🧩 Удобный деплой на сервер (Ubuntu 20.04).

