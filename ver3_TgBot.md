## ⚡ Код `bot.py` (aiogram 3.x)

```python
import asyncio
import random
import os
from aiogram import Bot, Dispatcher, types, F
from aiogram.filters import Command

# --- Настройки ---
TOKEN = os.getenv("BOT_TOKEN")  # токен берём из переменной окружения
people = {
    "А.С. Пушкин": 1799,
    "Джек Лондон": 1876,
    "М.В. Ломоносов": 1711,
    "Д.И. Менделеев": 1834,
    "С.В. Ковалевская": 1850,
    "Исаак Ньютон": 1643,
    "Алан Тьюринг": 1912,
    "С.П. Королёв": 1907,
}

# --- Логика игры ---
class QuizGame:
    def __init__(self, questions=5):
        self.questions = random.sample(list(people.items()), questions)
        self.index = 0
        self.score = 0
        self.total = questions

    def ask(self):
        if self.index < self.total:
            name, _ = self.questions[self.index]
            return f"📅 В каком году родился {name}?"
        return "✅ Викторина завершена!"

    def check(self, answer):
        if self.index >= self.total:
            return self.results(), True

        name, year = self.questions[self.index]
        if str(answer).isdigit() and int(answer) == year:
            self.score += 1
            feedback = f"✅ Верно! {name} родился в {year} году."
        else:
            feedback = f"❌ Неверно! {name} родился в {year} году."

        self.index += 1
        if self.index == self.total:
            return feedback + "\n\n" + self.results(), True
        return feedback + f"\n\nСледующий вопрос: {self.ask()}", False

    def results(self):
        mistakes = self.total - self.score
        return (f"🎯 Результаты:\n"
                f"- Правильные ответы: {self.score} ({self.score*100/self.total:.1f}%)\n"
                f"- Ошибки: {mistakes} ({mistakes*100/self.total:.1f}%)")

# --- Хранилище игр для пользователей ---
games = {}

# --- Запуск бота ---
bot = Bot(TOKEN)
dp = Dispatcher()

@dp.message(Command("start"))
async def start(message: types.Message):
    games[message.from_user.id] = QuizGame()
    await message.answer("🎓 Викторина: угадай год рождения!\n\n" + games[message.from_user.id].ask())

@dp.message(F.text)
async def handle_answer(message: types.Message):
    user_id = message.from_user.id
    if user_id not in games:
        await message.answer("Введите /start чтобы начать игру.")
        return

    game = games[user_id]
    feedback, finished = game.check(message.text)

    await message.answer(feedback)

    if finished:
        await message.answer("Хотите сыграть ещё раз? Напишите /start")

async def main():
    await dp.start_polling(bot)

if __name__ == "__main__":
    asyncio.run(main())
```

---

## 📌 Как запустить

1. Установи зависимости:

```bash
pip install aiogram==3.*
```

2. Создай `.env` файл:

```
BOT_TOKEN=твой_токен_от_BotFather
```

3. Запусти:

```bash
python bot.py
```

4. В Telegram напиши боту `/start` 🚀

---

## 🚀 Что умеет бот

* 🎓 Задаёт **случайные вопросы** о годах рождения.
* ✅ Проверяет ответы и даёт фидбек.
* 📊 Считает проценты правильных и неправильных.
* 🔄 После завершения предлагает сыграть заново.

---
