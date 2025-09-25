#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

int main() {
    // база данных: имя -> год рождения
    map<string, int> people = {
        {"А.С. Пушкин", 1799},
        {"Джек Лондон", 1876},
        {"М.В. Ломоносов", 1711},
        {"Д.И. Менделеев", 1834},
        {"С.В. Ковалевская", 1850},
        {"Алан Тьюринг", 1912},
        {"Исаак Ньютон", 1643},
        {"С.П. Королёв", 1907}
    };

    srand(time(0));
    string again = "да";

    while (again == "да" || again == "yes") {
        int score = 0;
        int total = 5;

        // выбираем 5 случайных вопросов
        vector<pair<string,int>> questions;
        for (auto &p : people) {
            questions.push_back(p);
        }

        random_shuffle(questions.begin(), questions.end());

        for (int i = 0; i < total; i++) {
            string name = questions[i].first;
            int year = questions[i].second;

            cout << "📅 В каком году родился " << name << "? " << endl;

            // создаём 4 варианта
            vector<int> options;
            options.push_back(year);
            while (options.size() < 4) {
                int y = year + (rand() % 101 - 50); // +-50 лет
                if (find(options.begin(), options.end(), y) == options.end()) {
                    options.push_back(y);
                }
            }
            random_shuffle(options.begin(), options.end());

            // показываем варианты
            for (int j = 0; j < 4; j++) {
                cout << j+1 << ") " << options[j] << endl;
            }

            int ans;
            cout << "Ваш выбор (1-4): ";
            cin >> ans;

            if (options[ans-1] == year) {
                cout << "✅ Верно!" << endl;
                score++;
            } else {
                cout << "❌ Неверно! Правильный ответ: " << year << endl;
            }
            cout << endl;
        }

        cout << "🎯 Результаты:" << endl;
        cout << "- Правильные ответы: " << score << " (" << score*100/total << "%)" << endl;
        cout << "- Ошибки: " << (total-score) << " (" << (total-score)*100/total << "%)" << endl;

        cout << "\nХотите сыграть снова? (да/нет): ";
        cin >> again;
    }

    cout << "Спасибо за игру! 👋" << endl;
    return 0;
}
