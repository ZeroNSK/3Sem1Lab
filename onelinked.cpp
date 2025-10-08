#include <iostream>
#include <string>
#include <fstream>  // <-- ДОБАВЛЕНО: для F_load/F_save

using namespace std;

struct Node{
    string person;
    Node* next;
};

Node* newNode(const string& v, Node* next = nullptr){
    return new Node{v, next};
}


void print(Node* head){ // печать всего списка
    cout << "------Node------" << endl;
    int i = 0;
    while(head != nullptr){
        cout << i++ << ") " << head->person << endl;
        head = head->next;
    }
}

size_t length(Node* head){ // длина списка
    size_t n = 0;
    for(; head != nullptr; head = head->next) ++n;
    return n;
}

Node* at(Node* head, size_t index){ // чтение по индексу (0..)
    while(head != nullptr && index--){
        head = head->next;
    }
    return head; // nullptr если вышли за границы
}

Node* front(Node* head){ // первый узел
    return head;
}

Node* back(Node* head){ // последний узел
    if(!head) return nullptr;
    while(head->next) head = head->next;
    return head;
}

Node* findFirst(Node* head, const string& x){ // первый узел по значению
    while(head != nullptr){
        if(head->person == x) return head;
        head = head->next;
    }
    return nullptr;
}

int indexOf(Node* head, const string& x){ // индекс первого вхождения
    int i = 0;
    while(head != nullptr){
        if(head->person == x) return i;
        head = head->next; ++i;
    }
    return -1;
}

void push_front(Node** head, Node** tail, const string& v){ // в голову
    Node* n = newNode(v, *head);
    *head = n;
    if(*tail == nullptr) *tail = n;
}

void push_back(Node** head, Node** tail, const string& v){ // в хвост
    Node* n = newNode(v, nullptr);
    if(*tail == nullptr){ 
        *head = n;
        *tail = n;
    }else{
        (*tail)->next = n;
        *tail = n;
    }
}

// вставка после заданного узла
void insert_after(Node* pos, const string& v){
    Node* n = newNode(v, pos->next);
    pos->next = n;
}

// вставка перед заданным узлом (ищем предка)
void insert_before(Node** head, Node** tail, Node* pos, const string& v){
    if(pos == nullptr) return;
    if(pos == *head){ // перед головой 
        push_front(head, tail, v);
        return;
    }
    Node* prev = *head;
    while(prev && prev->next != pos) prev = prev->next;
    if(prev == nullptr) return; 
    prev->next = newNode(v, pos);
}

void pop_front(Node** head, Node** tail){ // удалить голову
    if(*head == nullptr) return;
    Node* t = *head;
    *head = (*head)->next;
    if(*head == nullptr) *tail = nullptr; // список опустел
    delete t;
}

void pop_back(Node** head, Node** tail){ // удалить хвост
    if(*head == nullptr) return;
    if((*head)->next == nullptr){ // один элемент
        delete *head;
        *head = nullptr;
        *tail = nullptr;
        return;
    }
    Node* prev = *head;
    while(prev->next && prev->next->next) prev = prev->next;
    delete prev->next;
    prev->next = nullptr;
    *tail = prev;
}

// удалить узел сразу после pos
void erase_after(Node* pos){
    if(pos == nullptr || pos->next == nullptr) return;
    Node* t = pos->next;
    pos->next = t->next;
    delete t;
}

// удалить узел перед хвостом (т.е. предпоследний)
void erase_before_tail(Node** head, Node** tail){
    if(*head == nullptr || (*head)->next == nullptr) return; // 0 или 1 элемент
    if((*head)->next == *tail){ // всего 2 элемента -> удалить голову
        pop_front(head, tail);
        return;
    }
    Node* prev = *head;
    while(prev->next && prev->next->next != *tail) prev = prev->next; // prev->next — это узел перед хвостом
    Node* t = prev->next;
    prev->next = t->next;
    delete t;
}



// удалить первое вхождение
bool erase_value(Node** head, Node** tail, const string& x){
    if(*head == nullptr) return false;
    if((*head)->person == x){
        pop_front(head, tail);
        return true;
    }
    Node* prev = *head;
    while(prev->next && prev->next->person != x) prev = prev->next;
    if(prev->next == nullptr) return false; // не нашли
    Node* t = prev->next;
    prev->next = t->next;
    if(t == *tail) *tail = prev;
    delete t;
    return true;
}

// удалить все вхождения
int erase_all_values(Node** head, Node** tail, const string& x){
    int cnt = 0;
    while(*head && (*head)->person == x){ pop_front(head, tail); ++cnt; }
    if(*head == nullptr) return cnt;
    Node* cur = *head;
    while(cur->next){
        if(cur->next->person == x){
            Node* t = cur->next;
            cur->next = t->next;
            if(t == *tail) *tail = cur;
            delete t;
            ++cnt;
        }else{
            cur = cur->next;
        }
    }
    return cnt;
}

void clear(Node** head, Node** tail){
    while(*head) pop_front(head, tail);
}


// Глобальное состояние списка под команды 
static Node* g_F_head = nullptr;
static Node* g_F_tail = nullptr;

// Загрузка/сохранение из файла
bool F_load(const std::string& file){
    clear(&g_F_head, &g_F_tail);
    ifstream f(file);
    if(!f) return false;
    string s;
    while(getline(f, s)) push_back(&g_F_head, &g_F_tail, s);
    return true;
}
bool F_save(const std::string& file){
    ofstream f(file);
    if(!f) return false;
    for(Node* p=g_F_head; p; p=p->next) f << p->person << "\n";
    return true;
}

void F_push(const std::string& v){ push_back(&g_F_head, &g_F_tail, v); }
bool F_del_value(const std::string& v){ return erase_value(&g_F_head, &g_F_tail, v); }
bool F_get(size_t idx, std::string& out){
    Node* p = at(g_F_head, idx);
    if(!p) return false;
    out = p->person;
    return true;
}
void F_print(){
    bool first = true;
    for(Node* p=g_F_head; p; p=p->next){
        if(!first) cout << ' ';
        cout << p->person;
        first = false;
    }
    cout << "\n";
}
#ifndef DS_SKIP_ONE_MAIN
int main(){
    Node* head = nullptr;
    Node* tail = nullptr;

    // добавления (голова/хвост)
    push_front(&head, &tail, "Второй");
    push_front(&head, &tail, "Первый");
    push_back(&head, &tail,  "Третий");
    push_back(&head, &tail,  "Четвертый");

    // вставки (после / перед)
    insert_after(head, "ПослеПервого");           // после головы
    insert_before(&head, &tail, tail, "ДоХвоста"); // перед хвостом

    print(head);

    // чтение
    cout << "Начало: " << (front(head)? front(head)->person : string("null")) << endl;
    cout << "Конец:  " << (back(head)?  back(head)->person  : string("null")) << endl;
    cout << "В[2]: " << (at(head,2)?  at(head,2)->person  : string("null")) << endl;

    // поиск / удаление по значению
    cout << "Ищем (\"Третий\"): " << indexOf(head, "Третий") << endl;
    erase_value(&head, &tail, "AfterFirst");
    int removed = erase_all_values(&head, &tail, "Десять");
    cout << "Удаля: " << removed << endl;

    // удаления (голова/перед хвостом/хвост)
    pop_front(&head, &tail);
    erase_before_tail(&head, &tail);
    pop_back(&head, &tail);

    print(head);

    clear(&head, &tail);
    return 0;
}
#endif