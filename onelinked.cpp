#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>

// ============== ВНУТРЕННИЕ ТИПЫ И ДАННЫЕ ==============
struct Node {
    std::string value;
    Node* next;
};

static Node* g_F_head = nullptr;
static Node* g_F_tail = nullptr;

// ============== ВНУТРЕННИЕ УТИЛИТЫ ==============
static void clear_list(Node** head, Node** tail) {
    Node* p = *head;
    while (p) {
        Node* nx = p->next;
        delete p;
        p = nx;
    }
    *head = *tail = nullptr;
}

static std::size_t list_size(Node* head) {
    std::size_t n = 0;
    for (Node* p = head; p; p = p->next) ++n;
    return n;
}

static Node* at(Node* head, std::size_t index) {
    for (Node* p = head; p; p = p->next) {
        if (index == 0) return p;
        --index;
    }
    return nullptr;
}

static int indexOf(Node* head, const std::string& v) {
    int i = 0;
    for (Node* p = head; p; p = p->next, ++i) {
        if (p->value == v) return i;
    }
    return -1;
}

static void push_front(Node** head, Node** tail, const std::string& v) {
    Node* n = new Node{v, *head};
    *head = n;
    if (!*tail) *tail = n;
}

static void push_back(Node** head, Node** tail, const std::string& v) {
    Node* n = new Node{v, nullptr};
    if (!*tail) {
        *head = *tail = n;
    } else {
        (*tail)->next = n;
        *tail = n;
    }
}

static void pop_front(Node** head, Node** tail) {
    if (!*head) return;
    Node* del = *head;
    *head = (*head)->next;
    if (!*head) *tail = nullptr;
    delete del;
}

static void pop_back(Node** head, Node** tail) {
    if (!*head) return;
    if (*head == *tail) {
        delete *head;
        *head = *tail = nullptr;
        return;
    }
    Node* p = *head;
    while (p->next != *tail) p = p->next;
    delete *tail;
    *tail = p;
    p->next = nullptr;
}

static void insert_before(Node** head, Node** tail, Node* pos, const std::string& v) {
    if (!pos || !*head) return;
    if (pos == *head) {
        push_front(head, tail, v);
        return;
    }
    Node* prev = *head;
    while (prev && prev->next != pos) prev = prev->next;
    if (!prev) return;
    Node* n = new Node{v, pos};
    prev->next = n;
}

static void insert_after(Node** head, Node** tail, Node* pos, const std::string& v) {
    if (!pos) return;
    Node* n = new Node{v, pos->next};
    pos->next = n;
    if (*tail == pos) *tail = n;
}

static bool erase_value(Node** head, Node** tail, const std::string& v) {
    if (!*head) return false;
    if ((*head)->value == v) {
        pop_front(head, tail);
        return true;
    }
    Node* prev = *head;
    while (prev->next && prev->next->value != v) prev = prev->next;
    if (!prev->next) return false;
    Node* del = prev->next;
    prev->next = del->next;
    if (del == *tail) *tail = prev;
    delete del;
    return true;
}

// ============== ЭКСПОРТИРУЕМЫЕ ФУНКЦИИ (F_*) ==============
bool F_load(const std::string& file) {
    std::ifstream in(file);
    if (!in) return false;
    clear_list(&g_F_head, &g_F_tail);
    std::string line;
    while (std::getline(in, line)) {
        push_back(&g_F_head, &g_F_tail, line);
    }
    return true;
}

bool F_save(const std::string& file) {
    std::ofstream out(file);
    if (!out) return false;
    for (Node* p = g_F_head; p; p = p->next) out << p->value << "\n";
    return true;
}

void F_print() {
    bool first = true;
    for (Node* p = g_F_head; p; p = p->next) {
        if (!first) std::cout << ' ';
        std::cout << p->value;
        first = false;
    }
    std::cout << "\n";
}

void F_push_front(const std::string& v) { push_front(&g_F_head, &g_F_tail, v); }
void F_push_back (const std::string& v) { push_back (&g_F_head, &g_F_tail, v); }

bool F_insert_before(std::size_t index, const std::string& v) {
    Node* pos = at(g_F_head, index);
    if (!pos) return false;
    insert_before(&g_F_head, &g_F_tail, pos, v);
    return true;
}

bool F_insert_after(std::size_t index, const std::string& v) {
    Node* pos = at(g_F_head, index);
    if (!pos) return false;
    insert_after(&g_F_head, &g_F_tail, pos, v);
    return true;
}

bool F_pop_front() {
    if (!g_F_head) return false;
    pop_front(&g_F_head, &g_F_tail);
    return true;
}

bool F_pop_back() {
    if (!g_F_tail) return false;
    pop_back(&g_F_head, &g_F_tail);
    return true;
}

bool F_del_value(const std::string& v) { return erase_value(&g_F_head, &g_F_tail, v); }

bool F_get(std::size_t index, std::string& out) {
    Node* p = at(g_F_head, index);
    if (!p) return false;
    out = p->value;
    return true;
}

bool F_find(const std::string& v, std::size_t* index_out) {
    int i = indexOf(g_F_head, v);
    if (i < 0) return false;
    if (index_out) *index_out = static_cast<std::size_t>(i);
    return true;
}

#ifndef DS_SKIP_ONE_MAIN
// Тестовый main (отключается флагом -DDS_SKIP_ONE_MAIN)
int main() {
    F_push_back("A");
    F_push_front("B");
    F_insert_after(0, "C");
    F_print(); // B C A
    return 0;
}
#endif
