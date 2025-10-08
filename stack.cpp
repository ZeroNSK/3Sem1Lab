#include <iostream>
#include <string>
#include <fstream> 
#include <vector>
using namespace std;

struct SNode{
    string person;
    SNode* next;
};

SNode* newNode(const string& v, SNode* next = nullptr){
    return new SNode{v, next};
}

void print(SNode* top){ // печать стека сверху вниз
    cout << "------Стек------" << endl;
    int i = 0;
    while(top){
        cout << i++ << ") " << top->person << endl;
        top = top->next;
    }
}

size_t size(SNode* top){
    size_t n = 0;
    for(; top; top = top->next) ++n;
    return n;
}

bool empty(SNode* top){ return top == nullptr; }

SNode* peek(SNode* top){ // вершина (nullptr если пуст)
    return top;
}

void push(SNode** top, const string& v){
    *top = newNode(v, *top);
}

bool pop(SNode** top, string* out = nullptr){
    if(*top == nullptr) return false;
    SNode* t = *top;
    if(out) *out = t->person;
    *top = (*top)->next;
    delete t;
    return true;
}

void clear(SNode** top){
    string tmp;
    while(pop(top, &tmp)){}
}

static SNode* g_S_top = nullptr;

bool S_load(const string& file){
    clear(&g_S_top);
    ifstream f(file);
    if(!f) return false;
    // читаем низ->верх: просто пушим по порядку
    string s;
    while(getline(f, s)) push(&g_S_top, s);
    return true;
}
bool S_save(const string& file){
    ofstream f(file);
    if(!f) return false;
    // сохраняем низ->верх: соберём и выведем в обратном порядке
    vector<string> v;
    for(SNode* p=g_S_top; p; p=p->next) v.push_back(p->person);
    for(int i=(int)v.size()-1; i>=0; --i) f << v[i] << "\n";
    return true;
}
void S_push(const string& v){ push(&g_S_top, v); }
bool S_pop (string* out){    return pop(&g_S_top, out); }
void S_print(){
    bool first = true;
    for(SNode* p=g_S_top; p; p=p->next){
        if(!first) cout << ' ';
        cout << p->person;
        first = false;
    }
    cout << "\n";
}

#ifndef DS_SKIP_ONE_MAIN
int main(){
    SNode* top = nullptr;
    push(&top, "Первый");
    push(&top, "Второй");
    push(&top, "Третий");
    push(&top, "Четвертый");

    print(top);
    cout << "Размер: " << size(top) << endl;
    cout << "Пустой: " << (empty(top) ? "да" : "нет") << endl;

    cout << "Верх: " << (peek(top) ? peek(top)->person : string("null")) << endl;

    string val;
    if(pop(&top, &val)) cout << "Снято: " << val << endl;
    if(pop(&top, &val)) cout << "Снято: " << val << endl;

    print(top);
    cout << "Размер: " << size(top) << endl;

    clear(&top);
    cout << "Пустой после очистки: " << (empty(top) ? "да" : "нет") << endl;
    return 0;
}
#endif