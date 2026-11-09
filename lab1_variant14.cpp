#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Тренер может существовать независимо от спортивного клуба
class Coach {
public:
    string name;

    Coach(string n) : name(n) {}
};

// Участник также существует независимо от секции
class Member {
    string name;
    static int count; // Общий счётчик всех участников

public:
    Member(string n) : name(n) {
        ++count;
    }

    ~Member() {
        --count;
    }

    void print() const {
        cout << "Участник: " << name << endl;
    }

    static int getCount() {
        return count;
    }
};

int Member::count = 0;

// Секция хранит указатели на независимо созданных участников
class Section {
    string name;
    vector<Member*> members;

public:
    Section(string n) : name(n) {}

    void addMember(Member* m) {
        if (m != nullptr) {
            members.push_back(m);
        }
    }

    void print() const {
        cout << "Секция: " << name << endl;

        for (auto m : members) {
            if (m != nullptr) {
                m->print();
            }
        }
    }
};

// Спортивный клуб — единственный экземпляр (Singleton)
class SportsClub {
    vector<Coach*> coaches;
    vector<Section*> sections;

    SportsClub() {} // Запрещаем создание клуба извне

public:
    SportsClub(const SportsClub&) = delete;
    SportsClub& operator=(const SportsClub&) = delete;

    // Получение единственного экземпляра клуба
    static SportsClub& getInstance() {
        static SportsClub club;
        return club;
    }

    // Добавление тренера (агрегация)
    void addCoach(Coach* c) {
        if (c != nullptr) {
            coaches.push_back(c);
        }
    }

    // Добавление секции (агрегация)
    void addSection(Section* s) {
        if (s != nullptr) {
            sections.push_back(s);
        }
    }

    void print() const {
        cout << "\n=== Спортивный клуб ===\n";

        cout << "Тренеры: " << coaches.size() << endl;
        for (auto c : coaches) {
            if (c != nullptr) {
                cout << c->name << endl;
            }
        }

        cout << "\nСекции: " << sections.size() << endl;
        for (auto s : sections) {
            if (s != nullptr) {
                s->print();
            }
        }

        cout << "\nВсего участников: "
             << Member::getCount() << endl;
    }
};

int main() {
    // Независимое создание объектов
    Coach coach("Иван Петров");
    Member m1("Алексей");
    Member m2("Дмитрий");
    Section section("Бокс");

    // Формирование агрегации: секция использует участников
    section.addMember(&m1);
    section.addMember(&m2);

    SportsClub& club = SportsClub::getInstance();
    club.addCoach(&coach);
    club.addSection(&section);

    // Проверка Singleton
    SportsClub& club2 = SportsClub::getInstance();
    cout << "Один экземпляр клуба: "
         << boolalpha << (&club == &club2) << endl;

    club.print();

    // Проверка статического счётчика
    {
        Member m3("Сергей");
        cout << "\nУчастников после создания: "
             << Member::getCount() << endl;
    }

    cout << "Участников после уничтожения временного объекта: "
         << Member::getCount() << endl;

    return 0;
}
