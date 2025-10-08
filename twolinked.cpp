#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct DNode{
    string person;
    DNode* prev;
    DNode* next;
};

DNode* newNode(const string& v, DNode* p=nullptr, DNode* n=nullptr){
    return new DNode{v, p, n};
}


void print(DNode* head){
    cout << "------Двусвязный список------" << endl;
    int i = 0;
    while(head){
        cout << i++ << ") " << head->person << endl;
        head = head->next;
    }
}

size_t length(DNode* head){
    size_t n = 0;
    for(; head; head = head->next) ++n;
    return n;
}

DNode* at(DNode* head, size_t index){
    while(head && index--) head = head->next;
    return head;
}

DNode* front(DNode* head){ return head; }

DNode* back(DNode* tail){ return tail; }

DNode* findFirst(DNode* head, const string& x){
    while(head){
        if(head->person == x) return head;
        head = head->next;
    }
    return nullptr;
}

int indexOf(DNode* head, const string& x){
    int i = 0;
    while(head){
        if(head->person == x) return i;
        head = head->next; ++i;
    }
    return -1;
}

void push_front(DNode** head, DNode** tail, const string& v){
    DNode* n = newNode(v, nullptr, *head);
    if(*head) (*head)->prev = n;
    *head = n;
    if(*tail == nullptr) *tail = n;
}

void push_back(DNode** head, DNode** tail, const string& v){
    DNode* n = newNode(v, *tail, nullptr);
    if(*tail){
        (*tail)->next = n;
        *tail = n;
    }else{
        *head = *tail = n;
    }
}

void insert_after(DNode** head, DNode** tail, DNode* pos, const string& v){
    (void)head; (void)tail;
    if(pos == nullptr) return;
    DNode* n = newNode(v, pos, pos->next);
    if(pos->next) pos->next->prev = n; else *tail = n;
    pos->next = n;
}

void insert_before(DNode** head, DNode** tail, DNode* pos, const string& v){
    if(pos == nullptr) return;
    if(pos == *head){ push_front(head, tail, v); return; }
    DNode* n = newNode(v, pos->prev, pos);
    pos->prev->next = n;
    pos->prev = n;
}

void pop_front(DNode** head, DNode** tail){
    if(*head == nullptr) return;
    DNode* t = *head;
    *head = (*head)->next;
    if(*head) (*head)->prev = nullptr; else *tail = nullptr;
    delete t;
}

void pop_back(DNode** head, DNode** tail){
    if(*tail == nullptr) return;
    DNode* t = *tail;
    *tail = (*tail)->prev;
    if(*tail) (*tail)->next = nullptr; else *head = nullptr;
    delete t;
}

// удалить узел СРАЗУ после pos
void erase_after(DNode** head, DNode** tail, DNode* pos){
    (void)head; (void)tail;
    if(pos == nullptr || pos->next == nullptr) return;
    DNode* t = pos->next;
    pos->next = t->next;
    if(t->next) t->next->prev = pos; else *tail = pos;
    delete t;
}

// удалить узел СРАЗУ перед pos
void erase_before(DNode** head, DNode** tail, DNode* pos){
    (void)tail;
    if(pos == nullptr || pos->prev == nullptr) return;
    DNode* t = pos->prev;
    if(t->prev) t->prev->next = pos; else *head = pos;
    pos->prev = t->prev;
    delete t;
}

// удаление по значению

bool erase_value(DNode** head, DNode** tail, const string& x){
    DNode* cur = *head;
    while(cur && cur->person != x) cur = cur->next;
    if(cur == nullptr) return false;

    if(cur->prev) cur->prev->next = cur->next; else *head = cur->next;
    if(cur->next) cur->next->prev = cur->prev; else *tail = cur->prev;
    delete cur;
    return true;
}

int erase_all_values(DNode** head, DNode** tail, const string& x){
    int cnt = 0;
    DNode* cur = *head;
    while(cur){
        if(cur->person == x){
            DNode* del = cur;
            cur = cur->next;

            if(del->prev) del->prev->next = del->next; else *head = del->next;
            if(del->next) del->next->prev = del->prev; else *tail = del->prev;
            delete del;
            ++cnt;
        }else{
            cur = cur->next;
        }
    }
    return cnt;
}


void clear(DNode** head, DNode** tail){
    while(*head) pop_front(head, tail);
}


static DNode* g_L_head = nullptr;
static DNode* g_L_tail = nullptr;

bool L_load(const string& file){
    clear(&g_L_head, &g_L_tail);
    ifstream f(file);
    if(!f) return false;
    string s;
    while(getline(f, s)) push_back(&g_L_head, &g_L_tail, s);
    return true;
}
bool L_save(const string& file){
    ofstream f(file);
    if(!f) return false;
    for(DNode* p=g_L_head; p; p=p->next) f << p->person << "\n";
    return true;
}
void L_push(const string& v){ push_back(&g_L_head, &g_L_tail, v); }
bool L_del_value(const string& v){ return erase_value(&g_L_head, &g_L_tail, v); }
bool L_get(size_t idx, string& out){
    DNode* p = at(g_L_head, idx);
    if(!p) return false;
    out = p->person;
    return true;
}
void L_print(){
    bool first = true;
    for(DNode* p=g_L_head; p; p=p->next){
        if(!first) cout << ' ';
        cout << p->person;
        first = false;
    }
    cout << "\n";
}

#ifndef DS_SKIP_ONE_MAIN
int main(){
    DNode* head = nullptr;
    DNode* tail = nullptr;

    // добавление
    push_front(&head, &tail, "B");
    push_front(&head, &tail, "A");
    push_back (&head, &tail, "C");
    push_back (&head, &tail, "D");

    insert_after(&head, &tail, head, "A+");     // после головы
    insert_before(&head, &tail, tail, "C.5");   // перед хвостом

    print(head);

    // чтение
    cout << "Начало: " << (front(head)? front(head)->person : string("null")) << endl;
    cout << "Конец:  " << (back(tail)?  back(tail)->person  : string("null")) << endl;
    cout << "В [3]: " << (at(head,3)?  at(head,3)->person  : string("null")) << endl;

    // поиск/удаление по значению
    cout << "Ищем \"C\": " << indexOf(head, "C") << endl;
    erase_value(&head, &tail, "A+");
    int removed = erase_all_values(&head, &tail, "Ghost");
    cout << "удалено: " << removed << endl; // было "removed: "

    // удаления (голова/после узла/перед узлом/хвост)
    pop_front(&head, &tail);
    erase_after(&head, &tail, head);         // удалить после текущей головы
    erase_before(&head, &tail, tail);        // удалить перед текущим хвостом
    pop_back(&head, &tail);

    print(head);

    clear(&head, &tail);
    return 0;
}
#endif