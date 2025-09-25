#include <tgbot/tgbot.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <map>

using namespace std;
using namespace TgBot;

// --- структура для знаменитостей ---
struct Person {
    string name;
    int year;
    string photo;
};

// база данных
vector<Person> people = {
    {"А.С. Пушкин", 1799, "https://upload.wikimedia.org/wikipedia/commons/1/12/Kiprensky_Pushkin.jpg"},
    {"Джек Лондон", 1876, "https://upload.wikimedia.org/wikipedia/commons/1/12/Jack_London_young.jpg"},
    {"М.В. Ломоносов", 1711, "https://upload.wikimedia.org/wikipedia/commons/0/0f/Lomonosov_Mikhail.jpg"},
    {"Д.И. Менделеев", 1834, "https://upload.wikimedia.org/wikipedia/commons/4/45/Dmitri_Mendeleev_1890s.jpg"},
    {"С.В. Ковалевская", 1850, "https://upload.wikimedia.org/wikipedia/commons/8/89/Sofja_Kovalevskaja_1878.jpg"},
    {"Алан Тьюринг", 1912, "https://upload.wikimedia.org/wikipedia/commons/a/a1/Alan_Turing_Aged_16.jpg"},
    {"Исаак Ньютон", 1643, "https://upload.wikimedia.org/wikipedia/commons/f/f5/Sir_Isaac_Newton_%281642-1727%29.jpg"},
    {"С.П. Королёв", 1907, "https://upload.wikimedia.org/wikipedia/commons/1/16/Sergey_Korolyov.jpg"}
};

// --- структура игры ---
struct QuizGame {
    vector<Person> questions;
    int index = 0;
    int score = 0;
    int total = 5;
};

// все игры по chat_id
map<int64_t, QuizGame> games;

// утилита: случайные вопросы
vector<Person> getRandomQuestions(int count = 5) {
    vector<Person> q = people;
    random_shuffle(q.begin(), q.end());
    q.resize(count);
    return q;
}

// утилита: варианты ответа
vector<int> getOptions(int year) {
    vector<int> opts;
    opts.push_back(year);
    while (opts.size() < 4) {
        int y = year + (rand() % 101 - 50); // +-50 лет
        if (find(opts.begin(), opts.end(), y) == opts.end()) {
            opts.push_back(y);
        }
    }
    random_shuffle(opts.begin(), opts.end());
    return opts;
}

int main() {
    srand(time(0));
    string token = getenv("BOT_TOKEN"); // токен берём из переменной окружения
    Bot bot(token);

    bot.getEvents().onCommand("start", [&bot](Message::Ptr message) {
        QuizGame game;
        game.questions = getRandomQuestions();
        games[message->chat->id] = game;

        Person person = game.questions[0];
        vector<int> options = getOptions(person.year);

        InlineKeyboardMarkup::Ptr kb(new InlineKeyboardMarkup);
        for (int opt : options) {
            InlineKeyboardButton::Ptr btn(new InlineKeyboardButton);
            btn->text = to_string(opt);
            btn->callbackData = "ans:" + to_string(opt);
            kb->inlineKeyboard.push_back({btn});
        }

        bot.getApi().sendPhoto(message->chat->id, person.photo,
            "📅 В каком году родился " + person.name + "?", false, 0, kb);
    });

    bot.getEvents().onCallbackQuery([&bot](CallbackQuery::Ptr query) {
        int64_t chatId = query->message->chat->id;
        if (games.find(chatId) == games.end()) {
            bot.getApi().sendMessage(chatId, "Введите /start чтобы начать игру.");
            return;
        }

        QuizGame &game = games[chatId];
        Person person = game.questions[game.index];
        int chosenYear = stoi(query->data.substr(4));

        string feedback;
        if (chosenYear == person.year) {
            game.score++;
            feedback = "✅ Верно! " + person.name + " родился в " + to_string(person.year) + " году.";
        } else {
            feedback = "❌ Неверно! " + person.name + " родился в " + to_string(person.year) + " году.";
        }
        bot.getApi().sendMessage(chatId, feedback);

        game.index++;
        if (game.index >= game.total) {
            int mistakes = game.total - game.score;
            string result = "🎯 Результаты:\nПравильные ответы: " + to_string(game.score) +
                            " (" + to_string(game.score * 100 / game.total) + "%)\n" +
                            "Ошибки: " + to_string(mistakes) +
                            " (" + to_string(mistakes * 100 / game.total) + "%)";
            bot.getApi().sendMessage(chatId, result);
            bot.getApi().sendMessage(chatId, "Хотите сыграть снова? /start");
        } else {
            Person next = game.questions[game.index];
            vector<int> options = getOptions(next.year);

            InlineKeyboardMarkup::Ptr kb(new InlineKeyboardMarkup);
            for (int opt : options) {
                InlineKeyboardButton::Ptr btn(new InlineKeyboardButton);
                btn->text = to_string(opt);
                btn->callbackData = "ans:" + to_string(opt);
                kb->inlineKeyboard.push_back({btn});
            }
            bot.getApi().sendPhoto(chatId, next.photo,
                "📅 В каком году родился " + next.name + "?", false, 0, kb);
        }
    });

    TgLongPoll longPoll(bot);
    cout << "✅ Bot started..." << endl;
    while (true) {
        longPoll.start();
    }

    return 0;
}
