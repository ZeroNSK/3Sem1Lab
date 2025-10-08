#include <iostream>
#include <string>
#include "ds_bind.h"

using namespace std;

extern bool M_load(const string&); extern bool M_save(const string&);
extern void M_push(const string&); extern bool M_del(size_t); extern bool M_get(size_t,string&); extern void M_print(); 

extern bool F_load(const string&); extern bool F_save(const string&);
extern void F_push(const string&); extern bool F_del_value(const string&); extern bool F_get(size_t,string&); extern void F_print();

extern bool L_load(const string&); extern bool L_save(const string&);
extern void L_push(const string&); extern bool L_del_value(const string&); extern bool L_get(size_t,string&); extern void L_print();

extern bool Q_load(const string&); extern bool Q_save(const string&);
extern void Q_push(const string&); extern bool Q_pop(string*); extern void Q_print();

extern bool S_load(const string&); extern bool S_save(const string&);
extern void S_push(const string&); extern bool S_pop(string*); extern void S_print();

extern bool T_load(const string&); extern bool T_save(const string&);
extern void T_insert(const string&); extern void T_erase(const string&); extern bool T_get(const string&); extern void T_print();

static void split(const string& line, string& cmd, string& arg){
    size_t p = line.find(' ');
    if(p==string::npos){ cmd=line; arg.clear(); } else { cmd=line.substr(0,p); arg=line.substr(p+1); }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    M_load("M.data"); F_load("F.data"); L_load("L.data");
    Q_load("Q.data"); S_load("S.data"); T_load("T.data");

    string line, cmd, arg;
    while (getline(cin, line)){
        if(line.empty()) continue;
        split(line, cmd, arg);

        // M
        if(cmd=="MPUSH"){ M_push(arg); M_save("M.data"); }
        else if(cmd=="MDEL"){ size_t i=stoull(arg); if(!M_del(i)) cout<<"ERR\n"; else M_save("M.data"); }
        else if(cmd=="MGET"){ size_t i=stoull(arg); string out; if(M_get(i,out)) cout<<out<<"\n"; else cout<<"null\n"; }

        // F
        else if(cmd=="FPUSH"){ F_push(arg); F_save("F.data"); }
        else if(cmd=="FDEL"){ if(!F_del_value(arg)) cout<<"not found\n"; else F_save("F.data"); }
        else if(cmd=="FGET"){ size_t i=stoull(arg); string out; if(F_get(i,out)) cout<<out<<"\n"; else cout<<"null\n"; }

        // L
        else if(cmd=="LPUSH"){ L_push(arg); L_save("L.data"); }
        else if(cmd=="LDEL"){ if(!L_del_value(arg)) cout<<"not found\n"; else L_save("L.data"); }
        else if(cmd=="LGET"){ size_t i=stoull(arg); string out; if(L_get(i,out)) cout<<out<<"\n"; else cout<<"null\n"; }

        // Q
        else if(cmd=="QPUSH"){ Q_push(arg); Q_save("Q.data"); }
        else if(cmd=="QPOP"){ string out; if(Q_pop(&out)) { cout<<out<<"\n"; Q_save("Q.data"); } else cout<<"null\n"; }

        // S
        else if(cmd=="SPUSH"){ S_push(arg); S_save("S.data"); }
        else if(cmd=="SPOP"){ string out; if(S_pop(&out)) { cout<<out<<"\n"; S_save("S.data"); } else cout<<"null\n"; }

        // T
        else if(cmd=="TINSERT"){ T_insert(arg); T_save("T.data"); }
        else if(cmd=="TDEL"){ T_erase(arg); T_save("T.data"); }
        else if(cmd=="TGET"){ cout << (T_get(arg) ? "found" : "not found") << "\n"; }

        else if(cmd=="PRINT"){
            if(arg=="M"||arg=="m") M_print();
            else if(arg=="F"||arg=="f") F_print();
            else if(arg=="L"||arg=="l") L_print();
            else if(arg=="Q"||arg=="q") Q_print();
            else if(arg=="S"||arg=="s") S_print();
            else if(arg=="T"||arg=="t") T_print();
            else cout<<"ERR\n";
        }
        else { cout<<"ERR\n"; }
    }

    // финальное сохранение
    M_save("M.data"); F_save("F.data"); L_save("L.data");
    Q_save("Q.data"); S_save("S.data"); T_save("T.data");
    return 0;
}
