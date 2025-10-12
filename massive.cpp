// massive.cpp — собственная реализация массива строк под интерфейс M_*
// Соответствует пункту: создание, добавление (по индексу и в конец),
// получение по индексу, удаление по индексу, замена по индексу, длина, чтение/запись.

#include "ds_bind.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

// ---------- внутренняя структура динамического массива ----------
struct MArray {
    char** data;     // массив указателей на C-строки
    size_t size;     // фактическое число элементов
    size_t capacity; // вместимость
};

static MArray g_M { nullptr, 0, 0 }; // единый контейнер "Массив"

// ---------- утилиты ----------
static inline char* dup_cstr(const string& s) {
    size_t n = s.size() + 1;
    char* p = (char*)std::malloc(n);
    if (p) std::memcpy(p, s.c_str(), n);
    return p;
}

static inline void ensure_capacity(MArray& a, size_t need) {
    if (a.capacity >= need) return;
    size_t cap = a.capacity ? a.capacity : 1;
    while (cap < need) cap <<= 1;
    char** nd = (char**)std::realloc(a.data, cap * sizeof(char*));
    if (!nd) return; // если не удалось — оставим как есть (операция выше проверит границы)
    a.data = nd;
    a.capacity = cap;
}

static inline bool in_range(const MArray& a, size_t i) {
    return i < a.size;
}

static inline void clear_array(MArray& a) {
    for (size_t i = 0; i < a.size; ++i) std::free(a.data[i]);
    std::free(a.data);
    a.data = nullptr; a.size = 0; a.capacity = 0;
}

// ---------- публичный интерфейс M_* (как в ds_bind.h) ----------
size_t M_len() { return g_M.size; }

void M_create(size_t n /*=0*/, const std::string& fill /*=""*/) {
    clear_array(g_M);
    if (n == 0) return;
    ensure_capacity(g_M, n);
    for (size_t i = 0; i < n; ++i) {
        g_M.data[i] = dup_cstr(fill);
    }
    g_M.size = n;
}

// загрузка из файла: по строке на элемент
bool M_load(const std::string& file) {
    if (file.empty()) return false;
    ifstream fin(file);
    if (!fin) return false;

    // временный буфер
    MArray temp { nullptr, 0, 0 };
    string line;
    while (std::getline(fin, line)) {
        ensure_capacity(temp, temp.size + 1);
        if (temp.capacity < temp.size + 1) { // не хватило памяти
            clear_array(temp);
            return false;
        }
        temp.data[temp.size++] = dup_cstr(line);
    }
    if (!fin.eof() && fin.fail()) { clear_array(temp); return false; }

    clear_array(g_M);
    g_M = temp;
    return true;
}

// сохранение в файл: по одному элементу на строку
bool M_save(const std::string& file) {
    if (file.empty()) return false;
    ofstream fout(file, ios::trunc);
    if (!fout) return false;

    for (size_t i = 0; i < g_M.size; ++i) {
        fout << (g_M.data[i] ? g_M.data[i] : "") << '\n';
        if (!fout) return false;
    }
    return true;
}

// добавить в конец
void M_push(const std::string& v) {
    ensure_capacity(g_M, g_M.size + 1);
    if (g_M.capacity < g_M.size + 1) return; // out-of-memory — тихо игнорируем
    g_M.data[g_M.size++] = dup_cstr(v);
}

// вставить по индексу (0..size)
bool M_insert(size_t index, const std::string& v) {
    if (index > g_M.size) return false;
    ensure_capacity(g_M, g_M.size + 1);
    if (g_M.capacity < g_M.size + 1) return false;
    for (size_t i = g_M.size; i > index; --i) g_M.data[i] = g_M.data[i - 1];
    g_M.data[index] = dup_cstr(v);
    g_M.size++;
    return true;
}

// заменить по индексу
bool M_set(size_t index, const std::string& v) {
    if (!in_range(g_M, index)) return false;
    std::free(g_M.data[index]);
    g_M.data[index] = dup_cstr(v);
    return true;
}

// получить по индексу
bool M_get(size_t index, std::string& out) {
    if (!in_range(g_M, index)) return false;
    out = g_M.data[index] ? g_M.data[index] : "";
    return true;
}

// удалить по индексу
bool M_del(size_t index) {
    if (!in_range(g_M, index)) return false;
    std::free(g_M.data[index]);
    for (size_t i = index; i + 1 < g_M.size; ++i) g_M.data[i] = g_M.data[i + 1];
    g_M.size--;
    return true;
}

// печать
void M_print() {
    cout << "M.size = " << g_M.size << '\n';
    if (g_M.size == 0) { cout << "  [пусто]\n"; return; }
    for (size_t i = 0; i < g_M.size; ++i) {
        cout << "  [" << i << "] " << (g_M.data[i] ? g_M.data[i] : "") << '\n';
    }
}
