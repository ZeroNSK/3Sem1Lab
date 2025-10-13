#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>

struct DNode {
    std::string value;
    DNode* prev;
    DNode* next;
};

static DNode* g_L_head = nullptr;
static DNode* g_L_tail = nullptr;

static void clear_list(DNode** head, DNode** tail) {
    DNode* p = *head;
    while (p) {
        DNode* nx = p->next;
        delete p;
        p = nx;
    }
    *head = *tail = nullptr;
}

static std::size_t list_size(DNode* head) {
    std::size_t n = 0;
    for (DNode* p = head; p; p = p->next) ++n;
    return n;
}

static DNode* at(DNode* head, std::size_t index) {
    for (DNode* p = head; p; p = p->next) {
        if (index == 0) return p;
        --index;
    }
    return nullptr;
}

static int indexOf(DNode* head, const std::string& v) {
    int i = 0;
    for (DNode* p = head; p; p = p->next, ++i) {
        if (p->value == v) return i;
    }
    return -1;
}

static void push_front(DNode** head, DNode** tail, const std::string& v) {
    DNode* n = new DNode{v, nullptr, *head};
    if (*head) (*head)->prev = n;
    *head = n;
    if (!*tail) *tail = n;
}

static void push_back(DNode** head, DNode** tail, const std::string& v) {
    DNode* n = new DNode{v, *tail, nullptr};
    if (*tail) (*tail)->next = n;
    *tail = n;
    if (!*head) *head = n;
}

static void pop_front(DNode** head, DNode** tail) {
    if (!*head) return;
    DNode* del = *head;
    *head = del->next;
    if (*head) (*head)->prev = nullptr; else *tail = nullptr;
    delete del;
}

static void pop_back(DNode** head, DNode** tail) {
    if (!*tail) return;
    DNode* del = *tail;
    *tail = del->prev;
    if (*tail) (*tail)->next = nullptr; else *head = nullptr;
    delete del;
}

static void insert_before(DNode** head, DNode** tail, DNode* pos, const std::string& v) {
    if (!pos) return;
    if (pos == *head) { push_front(head, tail, v); return; }
    DNode* n = new DNode{v, pos->prev, pos};
    pos->prev->next = n;
    pos->prev = n;
}

static void insert_after(DNode** head, DNode** tail, DNode* pos, const std::string& v) {
    if (!pos) return;
    if (pos == *tail) { push_back(head, tail, v); return; }
    DNode* n = new DNode{v, pos, pos->next};
    pos->next->prev = n;
    pos->next = n;
}

static bool erase_value(DNode** head, DNode** tail, const std::string& v) {
    for (DNode* p = *head; p; p = p->next) {
        if (p->value == v) {
            if (p == *head) { pop_front(head, tail); }
            else if (p == *tail) { pop_back(head, tail); }
            else {
                p->prev->next = p->next;
                p->next->prev = p->prev;
                delete p;
            }
            return true;
        }
    }
    return false;
}
bool L_load(const std::string& file) {
    std::ifstream in(file);
    if (!in) return false;
    clear_list(&g_L_head, &g_L_tail);
    std::string line;
    while (std::getline(in, line)) push_back(&g_L_head, &g_L_tail, line);
    return true;
}

bool L_save(const std::string& file) {
    std::ofstream out(file);
    if (!out) return false;
    for (DNode* p = g_L_head; p; p = p->next) out << p->value << "\n";
    return true;
}

void L_print() {
    bool first = true;
    for (DNode* p = g_L_head; p; p = p->next) {
        if (!first) std::cout << ' ';
        std::cout << p->value;
        first = false;
    }
    std::cout << "\n";
}

void L_print_rev() {
    bool first = true;
    for (DNode* p = g_L_tail; p; p = p->prev) {
        if (!first) std::cout << ' ';
        std::cout << p->value;
        first = false;
    }
    std::cout << "\n";
}

void L_push_front(const std::string& v) { push_front(&g_L_head, &g_L_tail, v); }
void L_push_back (const std::string& v) { push_back (&g_L_head, &g_L_tail, v); }

bool L_insert_before(std::size_t index, const std::string& v) {
    DNode* pos = at(g_L_head, index);
    if (!pos) return false;
    insert_before(&g_L_head, &g_L_tail, pos, v);
    return true;
}

bool L_insert_after(std::size_t index, const std::string& v) {
    DNode* pos = at(g_L_head, index);
    if (!pos) return false;
    insert_after(&g_L_head, &g_L_tail, pos, v);
    return true;
}

bool L_pop_front() {
    if (!g_L_head) return false;
    pop_front(&g_L_head, &g_L_tail);
    return true;
}

bool L_pop_back() {
    if (!g_L_tail) return false;
    pop_back(&g_L_head, &g_L_tail);
    return true;
}

bool L_del_value(const std::string& v) { return erase_value(&g_L_head, &g_L_tail, v); }

bool L_get(std::size_t index, std::string& out) {
    DNode* p = at(g_L_head, index);
    if (!p) return false;
    out = p->value;
    return true;
}

bool L_find(const std::string& v, std::size_t* index_out) {
    int i = indexOf(g_L_head, v);
    if (i < 0) return false;
    if (index_out) *index_out = static_cast<std::size_t>(i);
    return true;
}

#ifndef DS_SKIP_ONE_MAIN
int main() {
    L_push_back("1");
    L_push_front("0");
    L_insert_after(0, "0.5");
    L_print();      
    L_print_rev();  
    return 0;
}
#endif
