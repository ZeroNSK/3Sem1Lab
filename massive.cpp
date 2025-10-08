// massive.cpp — модуль "Массив" (строковый), интерфейс M_*
//
// Операции:
//  - M_create(n, fill)         — создание нового массива (n элементов, заполненных fill; по умолчанию пустой)
//  - M_load(file) / M_save(file) — загрузка/сохранение (по строке на элемент)
//  - M_push(v)                 — добавление в конец
//  - M_insert(idx, v)          — вставка по индексу (0..size), можно в конец
//  - M_set(idx, v)             — замена по индексу
//  - M_get(idx, out)           — получение по индексу
//  - M_del(idx)                — удаление по индексу
//  - M_len()                   — длина массива
//  - M_print()                 — печать элементов в одну строку через пробел + '\n'

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;

static vector<string> gM;

// создание нового массива
void M_create(size_t n = 0, const string& fill = "") {
    gM.assign(n, fill);
}

// загрузка: по одной строке на элемент
bool M_load(const string& file) {
    gM.clear();
    std::ifstream f(file);
    if (!f) return false;
    string s;
    while (std::getline(f, s)) gM.push_back(s);
    return true;
}

// сохранение: каждый элемент — отдельная строка
bool M_save(const string& file) {
    std::ofstream f(file);
    if (!f) return false;
    for (const auto& s : gM) f << s << '\n';
    return true;
}

// длина массива
size_t M_len() {
    return gM.size();
}

// добавление в конец
void M_push(const string& v) {
    gM.push_back(v);
}

// вставка по индексу (0..size) — true, если успешно
bool M_insert(size_t index, const string& v) {
    if (index > gM.size()) return false; // допускаем вставку в конец (index == size)
    gM.insert(gM.begin() + static_cast<long>(index), v);
    return true;
}

// замена по индексу — true, если успешно
bool M_set(size_t index, const string& v) {
    if (index >= gM.size()) return false;
    gM[index] = v;
    return true;
}

// получение по индексу — true, если успешно
bool M_get(size_t index, string& out) {
    if (index >= gM.size()) return false;
    out = gM[index];
    return true;
}

// удаление по индексу — true, если успешно
bool M_del(size_t index) {
    if (index >= gM.size()) return false;
    gM.erase(gM.begin() + static_cast<long>(index));
    return true;
}

// печать массива в одну строку
void M_print() {
    bool first = true;
    for (const auto& s : gM) {
        if (!first) std::cout << ' ';
        std::cout << s;
        first = false;
    }
    std::cout << '\n';
}
