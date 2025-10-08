//вариант 2. авл дерево
#include <iostream>
#include <string>
#include <fstream>
#include <string>

using namespace std;

struct Node{
    string key;
    int h;
    Node* right;
    Node* left;
};

int height(Node* p){ //находим высоту Нодки
    if(p){
        return p->h;
    }
    return 0;
}

void fixHeight(Node* p){ // пересчет высоты конкретного узла 
    int hLeft = height(p->left);
    int hRight = height(p->right);
    
    if(hLeft > hRight){
        p->h = hLeft + 1;
    }
    else{
        p->h = hRight + 1;
    }
}

int balanceFactor(Node* p){ // баланс фактор 
    int right_h = height(p->right);
    int left_h = height(p->left);
    int factor = right_h - left_h;

    return factor;

}

Node* rotateRight(Node* p){ // Возвращает новый корень поддерева, делает малый поворот вправо
    Node* q = p->left;
    p->left = q->right;
    q->right = p;
    fixHeight(p); 
    fixHeight(q);
    return q;
}

Node* rotateLeft(Node* p){ // малый поворот влево
    Node* q = p->right;
    p->right = q->left;
    q->left = p;
    fixHeight(p);
    fixHeight(q);
    return q;
}

Node* balance(Node* p){ // тут сразу проверяем нужен ли большой поворот во втором if, если нет, то выполняем просто малый
    fixHeight(p);
    int factor = balanceFactor(p);
    if(factor == 2){
        if (balanceFactor(p->right) < 0){ // правый сын наклонен влево 
            p->right = rotateRight(p->right); // выпрямляем правого сына правым поворотом
        }
        return rotateLeft(p); // устраняем общий перекос
    
    }
    if(factor == -2){
        if (balanceFactor(p->left) > 0){
            p->left = rotateLeft(p->left);

        }
        return rotateRight(p); 
    }
    
    return p;
}
Node* newNode(const string& key){ // создаем нвоый узел
    Node* n = new Node;
    n->key = key;
    n->h = 1;
    n->right = nullptr;
    n->left = nullptr;
    return n;
}

// ниже доп функции, не относящиеся к логике работы авл дерева

Node* insert(Node* p, const string& key){
    if (p==nullptr){ //создаем лист и возвращаем его
        return newNode(key);
    }

    if(key < p->key){
        p -> left = insert(p->left, key);
    }
    
    else if(key > p->key){
        p -> right = insert(p->right,key);
    }
    else{
        return p;
    }

    return balance(p);
}

Node* findMin(Node* p){ // если есть левее, то он меньше
    if (p->left){
        return findMin(p->left);
    }
    return p;
}

Node* deleteMin(Node* p){
    if(p->left == nullptr){ // текущий p минимальный
        Node* q = p -> right;  //запоминаем правого сына, он и встанет на его место
        delete p;
        return q;
    }
    p -> left = deleteMin(p->left);
    return balance(p);
}

Node* remove(Node* p, const string& key){
    if(p == nullptr){ //если пусто, то не удаляем ничего
        return nullptr;
    }
    if(key < p->key){
        p -> left = remove(p->left, key);
    }
    else if(key > p-> key){
        p -> right = remove(p->right, key);
    }
    else{
        if(p->right == nullptr){ // если правого поддерева нет, то возвращаем левое, оно станет новым корнем поддерева
            Node* L = p -> left;
            delete p;
            return L;
        }

        Node* m = findMin(p->right); //берем минимальный узел в правом поддереве
        p->key = m->key; // копируем ключ в текущий узел
        p->right = deleteMin(p->right); // удаляем минимум из правого поддерева и вешаем остаток вправо от m
        return balance(p);
    }
    return balance(p);
}

Node* search(Node* p, const string& key){
    Node* current = p;
    
    while(current != nullptr){
        if(key == current->key){
            return current;
        }
    
    if (key < current->key){ // если ключ меньше - идем влево и заменяем
        current = current->left;
    }
    else{ // иначе вправо
        current = current -> right;
        }
    }
    return nullptr; // если не нашли никакого ключа, то возвращаем пустой
}


static void print(Node* p) { // прямой обход: узел -> левый -> правый
    if (p == nullptr) {
        return;
    }
    cout << p->key << " ";
    print(p->left);
    print(p->right);
}

void printInOrder(Node* p) { // симметричный обход
    if (p == nullptr) {
        return;
    }
    printInOrder(p->left);
    cout << p->key << " ";
    printInOrder(p->right);
}


void clear(Node* &p){ // очищаем память(удаляем поддерево и зануляем указатель)
    if (p==nullptr){
        return; 
    }
    clear(p->left);
    clear(p->right);
    delete p;
    p = nullptr;
}

// дальше будут функции, нужные для совместимости с заданием 2.
static Node* g = nullptr;

// загрузка дерева из файла (по одному ключу на строку)
bool T_load(const string& file) {
    clear(g);
    ifstream f(file);
    if (!f) return false;
    string s;
    while (getline(f, s)) {
        if (!s.empty()) {
            g = insert(g, s);
        }
    }
    return true;
}

// сохранение дерева в файл в отсортированном порядке (inorder)
bool T_save(const string& file) {
    ofstream f(file);
    if (!f) return false;
    struct H {
        static void dfs(Node* p,ofstream& out) {
            if (!p) return;
            dfs(p->left, out);
            out << p->key << "\n";
            dfs(p->right, out);
        }
    };
    H::dfs(g, f);
    return true;
}

// вставка ключа
void T_insert(const string& k) {
    g = insert(g, k);
}

// удаление ключа
void T_erase(const string& k) {
    g = remove(g, k);
}

// поиск ключа
bool T_get(const string& k) {
    return search(g, k) != nullptr;
}

// печать (симметричный обход)
void T_print() {
    printInOrder(g);
    cout << "\n";
}



#ifndef DS_SKIP_ONE_MAIN
int main() {
    Node* mainTree = nullptr;
    const string words[] = {
        "Манго",
        "Банан",
        "Персик",
        "Яблоко",
        "Вишня",
        "Апельсин",
        "Виноград",
        "Киви",
        "Лимон",
        "Слива"
    };

    for (const auto& s : words) {
        mainTree = insert(mainTree, s);
    }

    cout << "------Наше дерево------" << "\n";
    print(mainTree);

    cout << "\n" << "симметричный обход (inorder): ";
    printInOrder(mainTree);
    cout << "\n";

    cout << "прямой обход (preorder): ";
    print(mainTree);
    cout << "\n";

    // ищем существующий и несуществующий ключ
    const string q1 = "Вишня";
    const string q2 = "Ягода";

    Node* f1 = search(mainTree, q1);
    if (f1) {
        cout << "поиск \"" << q1 << "\": найдено" << "\n";
    } else {
        cout << "поиск \"" << q1 << "\": не найдено" << "\n";
    }

    Node* f2 = search(mainTree, q2);
    if (f2) {
        cout << "поиск \"" << q2 << "\": найдено" << "\n";
    } else {
        cout << "поиск \"" << q2 << "\": не найдено" << "\n";
    }

    // удалим некоторые разные элементы
    mainTree = remove(mainTree, "Манго");
    mainTree = remove(mainTree, "Яблоко");

    cout << "------После удаления \"Манго\" и \"Яблоко\"------" << "\n";
    print(mainTree);

    cout << "\n" << "симметричный обход: ";
    printInOrder(mainTree);
    cout << "\n";

    cout << "прямой обход: ";
    print(mainTree);
    cout << "\n";
    clear(mainTree);

    if (mainTree == nullptr) {
        cout << "дерево очищено" << "\n";
    }

    return 0;
}
#endif
