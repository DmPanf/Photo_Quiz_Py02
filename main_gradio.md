import gradio as gr
import random

# База вопросов
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
            return self.results()

        name, year = self.questions[self.index]
        if str(answer).isdigit() and int(answer) == year:
            self.score += 1
            feedback = f"✅ Верно! {name} родился в {year} году."
        else:
            feedback = f"❌ Неверно! {name} родился в {year} году."

        self.index += 1
        if self.index == self.total:
            return feedback + "\n\n" + self.results()
        return feedback + f"\n\nСледующий вопрос: {self.ask()}"

    def results(self):
        mistakes = self.total - self.score
        return (f"🎯 Результаты:\n"
                f"- Правильные ответы: {self.score} ({self.score*100/self.total:.1f}%)\n"
                f"- Ошибки: {mistakes} ({mistakes*100/self.total:.1f}%)")

# --- Gradio UI ---
game = QuizGame()

def start_game():
    global game
    game = QuizGame()
    return game.ask()

def answer_game(ans):
    return game.check(ans)

with gr.Blocks() as demo:
    gr.Markdown("## 🎓 Викторина: угадай год рождения!")
    btn_start = gr.Button("🚀 Начать новую игру")
    question = gr.Textbox(label="Вопрос", interactive=False)
    answer = gr.Textbox(label="Введите год")
    btn_submit = gr.Button("Ответить")
    output = gr.Textbox(label="Результат", interactive=False)

    btn_start.click(fn=start_game, outputs=question)
    btn_submit.click(fn=answer_game, inputs=answer, outputs=output)

demo.launch(server_name="0.0.0.0", share=True)
