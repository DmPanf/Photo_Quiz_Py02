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
