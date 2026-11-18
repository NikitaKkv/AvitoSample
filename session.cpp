#include <iostream>
#include <vector>
#include <cstring>

struct Session {
    int id;
    char* username;
    char* token;

    Session(int id, const char* name, const char* tok) : id(id) {
        username = new char[std::strlen(name) + 1];
        std::strcpy(username, name);

        token = new char[std::strlen(tok) + 1];
        std::strcpy(token, tok);
    }

    ~Session() {
        delete[] username;
        delete[] token;
    }
};

class SessionManager {
    std::vector<Session> sessions;

public:
    void add_session(int id, const char* name, const char* tok) {
        sessions.emplace_back(id, name, tok);
    }

    Session* find_by_id(int id) {
        for (auto& s : sessions) {
            if (s.id == id) return &s;
        }
        return nullptr;
    }

    void normalize_name(char* name) {
        for (size_t i = 0; name[i] != '\0'; ++i) {
            if (name[i] >= 'A' && name[i] <= 'Z') {
                name[i] += 32;
            }
        }
    }

    const char* get_fallback_token() {
        char buf[32] = "guest_token_000";
        return buf;
    }
};

int main() {
    SessionManager mgr;
    mgr.add_session(1, "Alice", "tok_111");

    Session* alice = mgr.find_by_id(1);

    // Добавляем ещё пользователей
    for (int i = 2; i <= 10; ++i) {
        mgr.add_session(i, "User", "tok_xxx");
    }

    // Работаем с ранее найденной сессией
    std::cout << "Active user: " << alice->username << "\n";

    // Проверяем несуществующего пользователя
    Session* admin = mgr.find_by_id(999);
    std::cout << "Admin token: " << admin->token << "\n";

    // Нормализация имени из конфига
    char* default_name = (char*)"AdminUser";
    mgr.normalize_name(default_name);

    // Фоллбэк токен
    const char* fallback = mgr.get_fallback_token();
    std::cout << "Fallback: " << fallback << "\n";

    return 0;
}
