## ⚡ Код `bot.py` (aiogram 3.x + inline клавиатура)

```python
import asyncio
import os
import random
from aiogram import Bot, Dispatcher, types
from aiogram.filters import Command
from aiogram.utils.keyboard import InlineKeyboardBuilder

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
            name, year = self.questions[self.index]
            # Создаем варианты (правильный + 3 случайных)
            options = [year]
            while len(options) < 4:
                y = random.randint(year - 50, year + 50)
                if y not in options:
                    options.append(y)
            random.shuffle(options)
            return name, options
        return None, None

    def check(self, answer: int):
        name, year = self.questions[self.index]
        if answer == year:
            self.score += 1
            feedback = f"✅ Верно! {name} родился в {year} году."
        else:
            feedback = f"❌ Неверно! {name} родился в {year} году."
        self.index += 1
        return feedback

    def finished(self):
        return self.index >= self.total

    def results(self):
        mistakes = self.total - self.score
        return (f"🎯 Результаты:\n"
                f"- Правильные ответы: {self.score} ({self.score*100/self.total:.1f}%)\n"
                f"- Ошибки: {mistakes} ({mistakes*100/self.total:.1f}%)")

# --- Игры по user_id ---
games = {}

# --- Запуск бота ---
bot = Bot(TOKEN)
dp = Dispatcher()


@dp.message(Command("start"))
async def start(message: types.Message):
    games[message.from_user.id] = QuizGame()
    await send_question(message.chat.id)


async def send_question(chat_id: int):
    game = games[chat_id]
    name, options = game.ask()
    if not name:
        await bot.send_message(chat_id, game.results())
        await bot.send_message(chat_id, "Хотите сыграть снова? /start")
        return

    kb = InlineKeyboardBuilder()
    for opt in options:
        kb.button(text=str(opt), callback_data=f"ans:{opt}")
    kb.adjust(2)

    await bot.send_message(chat_id, f"📅 В каком году родился {name}?", reply_markup=kb.as_markup())


@dp.callback_query(lambda c: c.data.startswith("ans:"))
async def handle_answer(callback: types.CallbackQuery):
    chat_id = callback.message.chat.id
    game = games.get(chat_id)
    if not game:
        await callback.answer("Сначала введите /start", show_alert=True)
        return

    year = int(callback.data.split(":")[1])
    feedback = game.check(year)

    await callback.message.answer(feedback)

    if game.finished():
        await callback.message.answer(game.results())
        await callback.message.answer("Хотите сыграть снова? /start")
    else:
        await send_question(chat_id)


async def main():
    await dp.start_polling(bot)


if __name__ == "__main__":
    asyncio.run(main())
```

---

## 📌 Что изменилось

* 🔘 **Ответы через кнопки** (inline-клавиатура, 4 варианта).
* 🧩 Каждый вопрос даёт **случайные варианты** + правильный год.
* 📊 После завершения выводится результат + % правильных.
* 🔄 Лёгкий рестарт через `/start`.

---

## 🚀 Запуск

1. Установи зависимости:

```bash
pip install aiogram==3.*
```

2. Создай `.env`:

```
BOT_TOKEN=твой_токен_от_BotFather
```

3. Запусти:

```bash
python bot.py
```

4. Напиши боту `/start` 🎉

