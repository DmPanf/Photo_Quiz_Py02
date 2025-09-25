# 📂 Структура проекта (обновлённая)

```
quiz_bot/
├── .github/
│   └── workflows/
│       └── ci-cd.yml
├── CMakeLists.txt
├── main.cpp
├── Dockerfile
├── docker-compose.yml
├── .env   # локальный (на сервере), в git НЕ пушим
```

---

# ✅ GitHub Actions Workflow `.github/workflows/ci-cd.yml`

```yaml
name: CI/CD QuizBot

on:
  push:
    branches:
      - main

jobs:
  build:
    runs-on: ubuntu-latest

    steps:
      - name: Checkout repository
        uses: actions/checkout@v3

      - name: Set up Docker Buildx
        uses: docker/setup-buildx-action@v2

      - name: Login to Docker Hub
        uses: docker/login-action@v2
        with:
          username: ${{ secrets.DOCKER_USERNAME }}
          password: ${{ secrets.DOCKER_PASSWORD }}

      - name: Build and push Docker image
        uses: docker/build-push-action@v4
        with:
          push: true
          tags: ${{ secrets.DOCKER_USERNAME }}/quizbot:latest

  deploy:
    runs-on: ubuntu-latest
    needs: build

    steps:
      - name: Deploy to VPS via SSH
        uses: appleboy/ssh-action@v0.1.10
        with:
          host: ${{ secrets.VPS_HOST }}
          username: ${{ secrets.VPS_USER }}
          key: ${{ secrets.VPS_SSH_KEY }}
          script: |
            cd ~/quiz_bot
            git pull origin main || true
            docker-compose pull
            docker-compose down
            docker-compose up -d --build
```

---

# ✅ Что нужно настроить

## 🔑 Секреты в GitHub → Settings → Secrets → Actions

* `DOCKER_USERNAME` — твой Docker Hub логин.
* `DOCKER_PASSWORD` — пароль/токен от Docker Hub.
* `VPS_HOST` — IP адрес сервера (Ubuntu 20.04).
* `VPS_USER` — пользователь (например, `ubuntu`).
* `VPS_SSH_KEY` — приватный SSH ключ для доступа на сервер (в формате OpenSSH).

---

# ⚙️ Деплой на сервер

1. Один раз склонируй проект на сервер:

```bash
git clone https://github.com/yourname/quiz_bot.git
cd quiz_bot
```

2. Создай `.env` с токеном:

```
BOT_TOKEN=твой_токен_от_BotFather
```

3. Теперь при каждом пуше в `main`:

   * GitHub Actions соберёт Docker-образ.
   * Зальёт его на Docker Hub.
   * Зайдёт на сервер по SSH.
   * Обновит контейнер и перезапустит бота 🚀.

---

# 🚀 Результат

* 👷 Автоматическая сборка при пуше в GitHub.
* 🐳 Docker-образ хранится на Docker Hub.
* 📦 VPS всегда тянет свежую версию.
* 🔄 CI/CD цикл готов к продакшену.

---
