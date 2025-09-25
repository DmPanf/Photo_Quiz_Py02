## ⚡ Код `bot_photo_quiz.py` (aiogram 3.x + фото)

```python
import asyncio
import os
import random
from aiogram import Bot, Dispatcher, types
from aiogram.filters import Command
from aiogram.utils.keyboard import InlineKeyboardBuilder

# === Настройки ===
TOKEN = os.getenv("BOT_TOKEN")  # Токен берём из переменной окружения

# Список знаменитостей: имя + год рождения + фото (например, с Wikipedia)
people = [
    {"name": "А.С. Пушкин", "year": 1799,
     "photo": "https://upload.wikimedia.org/wikipedia/commons/thumb/1/12/Kiprensky_Pushkin.jpg/330px-Kiprensky_Pushkin.jpg"},
    {"name": "Джек Лондон", "year": 1876,
     "photo": "https://upload.wikimedia.org/wikipedia/commons/thumb/1/12/Jack_London_young.jpg/330px-Jack_London_young.jpg"},
    {"name": "М.В. Ломоносов", "year": 1711,
     "photo": "https://upload.wikimedia.org/wikipedia/commons/thumb/0/0f/Lomonosov_Mikhail.jpg/330px-Lomonosov_Mikhail.jpg"},
    {"name": "Д.И. Менделеев", "year": 1834,
     "photo": "https://upload.wikimedia.org/wikipedia/commons/thumb/4/45/Dmitri_Mendeleev_1890s.jpg/330px-Dmitri_Mendeleev_1890s.jpg"},
    {"name": "С.В. Ковалевская", "year": 1850,
     "photo": "https://upload.wikimedia.org/wikipedia/commons/thumb/8/89/Sofja_Kovalevskaja_1878.jpg/330px-Sofja_Kovalevskaja_1878.jpg"},
    {"name": "Алан Тьюринг", "year": 1912,
     "photo": "https://upload.wikimedia.org/wikipedia/commons/thumb/a/a1/Alan_Turing_Aged_16.jpg/330px-Alan_Turing_Aged_16.jpg"},
    {"name": "Исаак Ньютон", "year": 1643,
     "photo": "https://upload.wikimedia.org/wikipedia/commons/thumb/f/f5/Sir_Isaac_Newton_%281642-1727%29.jpg/330px-Sir_Isaac_Newton_%281642-1727%29.jpg"},
    {"name": "С.П. Королёв", "year": 1907,
     "photo": "https://upload.wikimedia.org/wikipedia/commons/thumb/1/16/Sergey_Korolyov.jpg/330px-Sergey_Korolyov.jpg"},
]

# === Логика игры ===
class QuizGame:
    def __init__(self, questions=5):
        self.questions = random.sample(people, questions)
        self.index = 0
        self.score = 0
        self.total = questions

    def ask(self):
        if self.index < self.total:
            person = self.questions[self.index]
            options = [person["year"]]
            while len(options) < 4:
                y = random.randint(person["year"] - 50, person["year"] + 50)
                if y not in options:
                    options.append(y)
            random.shuffle(options)
            return person, options
        return None, None

    def check(self, answer: int):
        person = self.questions[self.index]
        if answer == person["year"]:
            self.score += 1
            feedback = f"✅ Верно! {person['name']} родился в {person['year']} году."
        else:
            feedback = f"❌ Неверно! {person['name']} родился в {person['year']} году."
        self.index += 1
        return feedback

    def finished(self):
        return self.index >= self.total

    def results(self):
        mistakes = self.total - self.score
        return (f"🎯 Результаты:\n"
                f"- Правильные ответы: {self.score} ({self.score*100/self.total:.1f}%)\n"
                f"- Ошибки: {mistakes} ({mistakes*100/self.total:.1f}%)")

# Хранилище игр
games = {}

# === Telegram Bot ===
bot = Bot(TOKEN)
dp = Dispatcher()

@dp.message(Command("start"))
async def start(message: types.Message):
    games[message.from_user.id] = QuizGame()
    await send_question(message.chat.id)

async def send_question(chat_id: int):
    game = games[chat_id]
    person, options = game.ask()
    if not person:
        await bot.send_message(chat_id, game.results())
        await bot.send_message(chat_id, "Хотите сыграть снова? /start")
        return

    kb = InlineKeyboardBuilder()
    for opt in options:
        kb.button(text=str(opt), callback_data=f"ans:{opt}")
    kb.adjust(2)

    await bot.send_photo(
        chat_id,
        photo=person["photo"],
        caption=f"📅 В каком году родился {person['name']}?",
        reply_markup=kb.as_markup()
    )

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

## 🚀 Как это работает

1. Бот показывает **фото знаменитости**.
2. Предлагает **4 варианта годов рождения** (inline-кнопки).
3. Игрок выбирает → бот даёт фидбек ✅❌.
4. В конце — **результаты с процентами**.

---

## 📌 Запуск

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
python bot_photo_quiz.py
```

4. В Telegram напиши `/start` 🎉

