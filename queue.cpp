#include <iostream>
#include <string>
#include <fstream>  

using namespace std;

struct QNode{
    string person;
    QNode* next;
};

QNode* newNode(const string& v, QNode* n=nullptr){
    return new QNode{v, n};
}


void print(QNode* head){
    cout << "------Очередь------" << endl;
    int i = 0;
    while(head){
        cout << i++ << ") " << head->person << endl;
        head = head->next;
    }
}

size_t size(QNode* head){
    size_t n = 0;
    for(; head; head = head->next) ++n;
    return n;
}

bool empty(QNode* head){ return head == nullptr; }

QNode* front(QNode* head){ return head; }

QNode* back(QNode* tail){ return tail; }

void push(QNode** head, QNode** tail, const string& v){
    QNode* n = newNode(v);
    if(*tail){
        (*tail)->next = n;
        *tail = n;
    }else{
        *head = *tail = n;
    }
}

bool pop(QNode** head, QNode** tail, string* out = nullptr){
    if(*head == nullptr) return false;
    QNode* t = *head;
    if(out) *out = t->person;
    *head = (*head)->next;
    if(*head == nullptr) *tail = nullptr;
    delete t;
    return true;
}

void clear(QNode** head, QNode** tail){
    string tmp;
    while(pop(head, tail, &tmp)){}
}

inline void enqueue(QNode** head, QNode** tail, const string& v){ push(head, tail, v); }
inline bool dequeue(QNode** head, QNode** tail, string* out=nullptr){ return pop(head, tail, out); }

static QNode* g_Q_head = nullptr;
static QNode* g_Q_tail = nullptr;

bool Q_load(const string& file){
    clear(&g_Q_head, &g_Q_tail);
    ifstream f(file);
    if(!f) return false;
    string s;
    while(getline(f, s)) push(&g_Q_head, &g_Q_tail, s);
    return true;
}
bool Q_save(const string& file){
    ofstream f(file);
    if(!f) return false;
    for(QNode* p=g_Q_head; p; p=p->next) f << p->person << "\n";
    return true;
}

void Q_push(const string& v){ push(&g_Q_head, &g_Q_tail, v); }
bool Q_pop (string* out){    return pop (&g_Q_head, &g_Q_tail, out); }

void Q_print(){
    bool first = true;
    for(QNode* p=g_Q_head; p; p=p->next){
        if(!first) cout << ' ';
        cout << p->person;
        first = false;
    }
    cout << "\n";
}

#ifndef DS_SKIP_ONE_MAIN
int main(){
    QNode* head = nullptr;
    QNode* tail = nullptr;

    push(&head, &tail, "Первый");
    push(&head, &tail, "Второй");
    push(&head, &tail, "Третий");
    push(&head, &tail, "Четвертый");

    print(head);
    cout << "размер: " << size(head) << endl;
    cout << "пусто: " << (empty(head) ? "да" : "нет") << endl;

    cout << "первый: " << (front(head)? front(head)->person : string("null")) << endl;
    cout << "последний:  " << (back(tail)?  back(tail)->person  : string("null")) << endl;

    string val;
    if(pop(&head, &tail, &val)) cout << "извлечено: " << val << endl;
    if(pop(&head, &tail, &val)) cout << "извлечено: " << val << endl;

    print(head);
    cout << "размер: " << size(head) << endl;

    clear(&head, &tail);
    cout << "пусто после очистки: " << (empty(head) ? "да" : "нет") << endl;
    return 0;
}
#endif
