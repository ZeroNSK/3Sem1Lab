#pragma once
#include <string>
#include <cstddef>

//M динамический массив
std::size_t M_len();
void M_create(std::size_t n = 0, const std::string& fill = "");
bool M_load(const std::string& file);
bool M_save(const std::string& file);
void M_push(const std::string& v);                        
bool M_insert(std::size_t index, const std::string& v);   
bool M_set(std::size_t index, const std::string& v);
bool M_get(std::size_t index, std::string& out);
bool M_del(std::size_t index);
void M_print();

// F односвязный список
bool F_load(const std::string& file);
bool F_save(const std::string& file);
void F_print();
void F_push_front(const std::string& v);
void F_push_back (const std::string& v);      // alias для добавления в конец
bool F_insert_before(std::size_t index, const std::string& v);
bool F_insert_after (std::size_t index, const std::string& v);
bool F_pop_front();
bool F_pop_back();
bool F_del_value(const std::string& v);
bool F_get(std::size_t index, std::string& out);
bool F_find(const std::string& v, std::size_t* index_out);

//L двусвязный список
bool L_load(const std::string& file);
bool L_save(const std::string& file);
void L_print();
void L_print_rev();
void L_push_front(const std::string& v);
void L_push_back (const std::string& v);
bool L_insert_before(std::size_t index, const std::string& v);
bool L_insert_after (std::size_t index, const std::string& v);
bool L_pop_front();
bool L_pop_back();
bool L_del_value(const std::string& v);
bool L_get(std::size_t index, std::string& out);
bool L_find(const std::string& v, std::size_t* index_out);


// Q очередь 
bool Q_load(const std::string& file);
bool Q_save(const std::string& file);
void Q_print();
void Q_push(const std::string& v);
bool Q_pop(std::string* out);

//S стек
bool S_load(const std::string& file);
bool S_save(const std::string& file);
void S_print();
void S_push(const std::string& v);
bool S_pop (std::string* out);

//T AVL 
bool T_load(const std::string& file);
bool T_save(const std::string& file);
void T_print();
void T_insert(const std::string& key);
void T_erase (const std::string& key);
bool T_get   (const std::string& key);
