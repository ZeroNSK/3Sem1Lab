#include "ds_bind.h"
#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <fstream>

using namespace std;

// ---------- настройки файлов по умолчанию ----------
static const string M_FILE = "M.txt";
static const string F_FILE = "F.txt";
static const string L_FILE = "L.txt";
static const string Q_FILE = "Q.txt";
static const string S_FILE = "S.txt";
static const string T_FILE = "T.txt";

// ---------- состояние автозагрузки/сохранения ----------
struct AutoState {
    bool tried_load = false;   // уже пытались автозагружать?
    bool dirty = false;        // были изменения (надо сохранить)?
};
static AutoState ST_M, ST_F, ST_L, ST_Q, ST_S, ST_T;

// ---------- утилиты ----------
static vector<string> tok(const string& line){
    vector<string> out; string cur; bool q=false;
    for(char c: line){
        if(c=='"'){ q=!q; continue; }
        if(!q && isspace((unsigned char)c)){ if(!cur.empty()){out.push_back(cur); cur.clear();} }
        else cur.push_back(c);
    }
    if(!cur.empty()) out.push_back(cur);
    return out;
}
static bool to_size_t(const string& s, size_t& v){
    if(s.empty()) return false;
    char* e=nullptr; unsigned long long x=strtoull(s.c_str(), &e, 10);
    if(!e || *e!='\0') return false; v=(size_t)x; return true;
}
static bool file_exists(const string& p){ ifstream f(p); return (bool)f; }

static void help(){
    cout <<
"Команды (строки — в кавычках). HELP/EXIT — справка/выход.\n"
"Файлы автозагружаются при первом обращении к структуре, \n"
"и автоматически сохраняются после изменений (M.txt/F.txt/... по умолчанию).\n\n"

"Массив (M):\n"
"  MCREATE N [fill] | MLEN | MLOAD \"f\" | MSAVE \"f\"\n"
"  MPUSH \"v\" | MINSERT idx \"v\" | MSET idx \"v\" | MGET idx | MDEL idx | PRINT M\n\n"

"Односвязный (F):\n"
"  FLOAD \"f\" | FSAVE \"f\" | FPUSH_FRONT \"v\" | FPUSH_BACK \"v\"\n"
"  FINSERT_BEFORE idx \"v\" | FINSERT_AFTER idx \"v\"\n"
"  FPOP_FRONT | FPOP_BACK | FDEL \"v\" | FGET idx | FFIND \"v\" | PRINT F\n\n"

"Двусвязный (L):\n"
"  LLOAD \"f\" | LSAVE \"f\" | LPUSH_FRONT \"v\" | LPUSH_BACK \"v\"\n"
"  LINSERT_BEFORE idx \"v\" | LINSERT_AFTER idx \"v\"\n"
"  LPOP_FRONT | LPOP_BACK | LDEL \"v\" | LGET idx | LFIND \"v\" | LPRINT_REV | PRINT L\n\n"

"Очередь (Q):  QLOAD \"f\" | QSAVE \"f\" | QPUSH \"v\" | QPOP | PRINT Q\n"
"Стек (S):     SLOAD \"f\" | SSAVE \"f\" | SPUSH \"v\" | SPOP | PRINT S\n"
"Дерево (T):   TLOAD \"f\" | TSAVE \"f\" | TINSERT \"k\" | TDEL \"k\" | TGET \"k\" | PRINT T\n";
}

// ---------- автозагрузка/сохранение ----------
static void ensure_autoload_M(){ if(!ST_M.tried_load){ ST_M.tried_load=true; if(file_exists(M_FILE)) M_load(M_FILE); } }
static void ensure_autoload_F(){ if(!ST_F.tried_load){ ST_F.tried_load=true; if(file_exists(F_FILE)) F_load(F_FILE); } }
static void ensure_autoload_L(){ if(!ST_L.tried_load){ ST_L.tried_load=true; if(file_exists(L_FILE)) L_load(L_FILE); } }
static void ensure_autoload_Q(){ if(!ST_Q.tried_load){ ST_Q.tried_load=true; if(file_exists(Q_FILE)) Q_load(Q_FILE); } }
static void ensure_autoload_S(){ if(!ST_S.tried_load){ ST_S.tried_load=true; if(file_exists(S_FILE)) S_load(S_FILE); } }
static void ensure_autoload_T(){ if(!ST_T.tried_load){ ST_T.tried_load=true; if(file_exists(T_FILE)) T_load(T_FILE); } }

static void autosave_M(){ if(ST_M.dirty){ M_save(M_FILE); ST_M.dirty=false; } }
static void autosave_F(){ if(ST_F.dirty){ F_save(F_FILE); ST_F.dirty=false; } }
static void autosave_L(){ if(ST_L.dirty){ L_save(L_FILE); ST_L.dirty=false; } }
static void autosave_Q(){ if(ST_Q.dirty){ Q_save(Q_FILE); ST_Q.dirty=false; } }
static void autosave_S(){ if(ST_S.dirty){ S_save(S_FILE); ST_S.dirty=false; } }
static void autosave_T(){ if(ST_T.dirty){ T_save(T_FILE); ST_T.dirty=false; } }

// ---------- обработчики ----------
static bool handle_M(vector<string> a){
    ensure_autoload_M();
    if(a[0]=="MCREATE"){
        if(a.size()<2){ cout<<"N?\n"; return true; }
        size_t n{}; if(!to_size_t(a[1],n)){ cout<<"N?\n"; return true; }
        M_create(n, a.size()>=3? a[2]: "");
        ST_M.dirty=true; autosave_M(); cout<<"OK "<<M_len()<<"\n"; return true;
    }
    if(a[0]=="MLEN"){ cout<<M_len()<<"\n"; return true; }
    if(a[0]=="MLOAD"){ cout<<(a.size()>=2 && M_load(a[1])?"OK\n":"ERR\n"); ST_M.tried_load=true; ST_M.dirty=false; return true; }
    if(a[0]=="MSAVE"){ cout<<(a.size()>=2 && M_save(a[1])?"OK\n":"ERR\n"); return true; }
    if(a[0]=="MPUSH"){ if(a.size()<2){ cout<<"val?\n"; return true; } M_push(a[1]); ST_M.dirty=true; autosave_M(); cout<<"OK\n"; return true; }
    if(a[0]=="MINSERT"){ size_t i{}; if(a.size()<3 || !to_size_t(a[1],i)){ cout<<"idx?\n"; return true; }
        cout<<(M_insert(i,a[2])?"OK\n":"ERR\n"); ST_M.dirty=true; autosave_M(); return true; }
    if(a[0]=="MSET"){ size_t i{}; if(a.size()<3 || !to_size_t(a[1],i)){ cout<<"idx?\n"; return true; }
        cout<<(M_set(i,a[2])?"OK\n":"ERR\n"); ST_M.dirty=true; autosave_M(); return true; }
    if(a[0]=="MGET"){ size_t i{}; if(a.size()<2 || !to_size_t(a[1],i)){ cout<<"idx?\n"; return true; }
        string out; cout<<(M_get(i,out)? out+"\n":"ERR\n"); return true; }
    if(a[0]=="MDEL"){ size_t i{}; if(a.size()<2 || !to_size_t(a[1],i)){ cout<<"idx?\n"; return true; }
        cout<<(M_del(i)?"OK\n":"ERR\n"); ST_M.dirty=true; autosave_M(); return true; }
    return false;
}

static bool handle_F(vector<string> a){
    ensure_autoload_F();
    if(a[0]=="FLOAD"){ cout<<(a.size()>=2 && F_load(a[1])?"OK\n":"ERR\n"); ST_F.tried_load=true; ST_F.dirty=false; return true; }
    if(a[0]=="FSAVE"){ cout<<(a.size()>=2 && F_save(a[1])?"OK\n":"ERR\n"); return true; }
    if(a[0]=="FPUSH_FRONT"){ if(a.size()<2){ cout<<"val?\n"; return true; } F_push_front(a[1]); ST_F.dirty=true; autosave_F(); cout<<"OK\n"; return true; }
    if(a[0]=="FPUSH_BACK"){  if(a.size()<2){ cout<<"val?\n"; return true; } F_push_back (a[1]); ST_F.dirty=true; autosave_F(); cout<<"OK\n"; return true; }
    if(a[0]=="FINSERT_BEFORE"){ size_t i{}; if(a.size()<3 || !to_size_t(a[1],i)){ cout<<"idx?\n"; return true; }
        cout<<(F_insert_before(i,a[2])?"OK\n":"ERR\n"); ST_F.dirty=true; autosave_F(); return true; }
    if(a[0]=="FINSERT_AFTER"){ size_t i{}; if(a.size()<3 || !to_size_t(a[1],i)){ cout<<"idx?\n"; return true; }
        cout<<(F_insert_after(i,a[2])?"OK\n":"ERR\n"); ST_F.dirty=true; autosave_F(); return true; }
    if(a[0]=="FPOP_FRONT"){ cout<<(F_pop_front()?"OK\n":"ERR\n"); ST_F.dirty=true; autosave_F(); return true; }
    if(a[0]=="FPOP_BACK"){  cout<<(F_pop_back ()?"OK\n":"ERR\n"); ST_F.dirty=true; autosave_F(); return true; }
    if(a[0]=="FDEL"){       if(a.size()<2){ cout<<"val?\n"; return true; } cout<<(F_del_value(a[1])?"OK\n":"ERR\n"); ST_F.dirty=true; autosave_F(); return true; }
    if(a[0]=="FGET"){ size_t i{}; if(a.size()<2 || !to_size_t(a[1],i)){ cout<<"idx?\n"; return true; }
        string out; cout<<(F_get(i,out)? out+"\n":"ERR\n"); return true; }
    if(a[0]=="FFIND"){ if(a.size()<2){ cout<<"val?\n"; return true; } size_t pos{}; bool ok=F_find(a[1], &pos); cout<<(ok? to_string(pos)+"\n":"NOT_FOUND\n"); return true; }
    return false;
}

static bool handle_L(vector<string> a){
    ensure_autoload_L();
    if(a[0]=="LLOAD"){ cout<<(a.size()>=2 && L_load(a[1])?"OK\n":"ERR\n"); ST_L.tried_load=true; ST_L.dirty=false; return true; }
    if(a[0]=="LSAVE"){ cout<<(a.size()>=2 && L_save(a[1])?"OK\n":"ERR\n"); return true; }
    if(a[0]=="LPUSH_FRONT"){ if(a.size()<2){ cout<<"val?\n"; return true; } L_push_front(a[1]); ST_L.dirty=true; autosave_L(); cout<<"OK\n"; return true; }
    if(a[0]=="LPUSH_BACK"){  if(a.size()<2){ cout<<"val?\n"; return true; } L_push_back (a[1]); ST_L.dirty=true; autosave_L(); cout<<"OK\n"; return true; }
    if(a[0]=="LINSERT_BEFORE"){ size_t i{}; if(a.size()<3 || !to_size_t(a[1],i)){ cout<<"idx?\n"; return true; }
        cout<<(L_insert_before(i,a[2])?"OK\n":"ERR\n"); ST_L.dirty=true; autosave_L(); return true; }
    if(a[0]=="LINSERT_AFTER"){ size_t i{}; if(a.size()<3 || !to_size_t(a[1],i)){ cout<<"idx?\n"; return true; }
        cout<<(L_insert_after(i,a[2])?"OK\n":"ERR\n"); ST_L.dirty=true; autosave_L(); return true; }
    if(a[0]=="LPOP_FRONT"){ cout<<(L_pop_front()?"OK\n":"ERR\n"); ST_L.dirty=true; autosave_L(); return true; }
    if(a[0]=="LPOP_BACK"){  cout<<(L_pop_back ()?"OK\n":"ERR\n"); ST_L.dirty=true; autosave_L(); return true; }
    if(a[0]=="LDEL"){       if(a.size()<2){ cout<<"val?\n"; return true; } cout<<(L_del_value(a[1])?"OK\n":"ERR\n"); ST_L.dirty=true; autosave_L(); return true; }
    if(a[0]=="LGET"){ size_t i{}; if(a.size()<2 || !to_size_t(a[1],i)){ cout<<"idx?\n"; return true; }
        string out; cout<<(L_get(i,out)? out+"\n":"ERR\n"); return true; }
    if(a[0]=="LFIND"){ if(a.size()<2){ cout<<"val?\n"; return true; } size_t pos{}; bool ok=L_find(a[1], &pos); cout<<(ok? to_string(pos)+"\n":"NOT_FOUND\n"); return true; }
    if(a[0]=="LPRINT_REV"){ L_print_rev(); return true; }
    return false;
}

static bool handle_Q(vector<string> a){
    ensure_autoload_Q();
    if(a[0]=="QLOAD"){ cout<<(a.size()>=2 && Q_load(a[1])?"OK\n":"ERR\n"); ST_Q.tried_load=true; ST_Q.dirty=false; return true; }
    if(a[0]=="QSAVE"){ cout<<(a.size()>=2 && Q_save(a[1])?"OK\n":"ERR\n"); return true; }
    if(a[0]=="QPUSH"){ if(a.size()<2){ cout<<"val?\n"; return true; } Q_push(a[1]); ST_Q.dirty=true; autosave_Q(); cout<<"OK\n"; return true; }
    if(a[0]=="QPOP"){  string out; cout<<(Q_pop(&out)? out+"\n":"ERR\n"); ST_Q.dirty=true; autosave_Q(); return true; }
    return false;
}

static bool handle_S(vector<string> a){
    ensure_autoload_S();
    if(a[0]=="SLOAD"){ cout<<(a.size()>=2 && S_load(a[1])?"OK\n":"ERR\n"); ST_S.tried_load=true; ST_S.dirty=false; return true; }
    if(a[0]=="SSAVE"){ cout<<(a.size()>=2 && S_save(a[1])?"OK\n":"ERR\n"); return true; }
    if(a[0]=="SPUSH"){ if(a.size()<2){ cout<<"val?\n"; return true; } S_push(a[1]); ST_S.dirty=true; autosave_S(); cout<<"OK\n"; return true; }
    if(a[0]=="SPOP"){  string out; cout<<(S_pop(&out)? out+"\n":"ERR\n"); ST_S.dirty=true; autosave_S(); return true; }
    return false;
}

static bool handle_T(vector<string> a){
    ensure_autoload_T();
    if(a[0]=="TLOAD"){ cout<<(a.size()>=2 && T_load(a[1])?"OK\n":"ERR\n"); ST_T.tried_load=true; ST_T.dirty=false; return true; }
    if(a[0]=="TSAVE"){ cout<<(a.size()>=2 && T_save(a[1])?"OK\n":"ERR\n"); return true; }
    if(a[0]=="TINSERT"){ if(a.size()<2){ cout<<"key?\n"; return true; } T_insert(a[1]); ST_T.dirty=true; autosave_T(); cout<<"OK\n"; return true; }
    if(a[0]=="TDEL"){    if(a.size()<2){ cout<<"key?\n"; return true; } T_erase (a[1]); ST_T.dirty=true; autosave_T(); cout<<"OK\n"; return true; }
    if(a[0]=="TGET"){    if(a.size()<2){ cout<<"key?\n"; return true; } cout<<(T_get(a[1])?"YES\n":"NO\n"); return true; }
    return false;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout<<"HELP — справка, EXIT — выход.\n";
    string line;
    while(true){
        cout<<"> ";
        if(!getline(cin,line)) break;
        auto a = tok(line);
        if(a.empty()) continue;

        for(auto& s: a) for(char& c: s) c=(char)toupper((unsigned char)c);

        if(a[0]=="HELP"){ help(); continue; }
        if(a[0]=="EXIT") break;

        if(a[0]=="PRINT"){
            if(a.size()<2){ cout<<"PRINT M|F|L|Q|S|T\n"; continue; }
            string t=a[1];
            if(t=="M"){ ensure_autoload_M(); M_print(); continue; }
            if(t=="F"){ ensure_autoload_F(); F_print(); continue; }
            if(t=="L"){ ensure_autoload_L(); L_print(); continue; }
            if(t=="Q"){ ensure_autoload_Q(); Q_print(); continue; }
            if(t=="S"){ ensure_autoload_S(); S_print(); continue; }
            if(t=="T"){ ensure_autoload_T(); T_print(); continue; }
            cout<<"PRINT M|F|L|Q|S|T\n"; continue;
        }

        bool ok=false;
        char p = a[0].empty()? '\0' : a[0][0];
        switch(p){
            case 'M': ok=handle_M(a); break;
            case 'F': ok=handle_F(a); break;
            case 'L': ok=handle_L(a); break;
            case 'Q': ok=handle_Q(a); break;
            case 'S': ok=handle_S(a); break;
            case 'T': ok=handle_T(a); break;
            default: ok=false;
        }
        if(!ok) cout<<"Неизвестная команда. Введи HELP.\n";
    }
    return 0;
}
