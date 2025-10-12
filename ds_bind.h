#pragma once
#include <string>
#include <cstddef>

// M: массив
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


// F: односвязный
bool F_load(const std::string& file);
bool F_save(const std::string& file);
void F_push(const std::string& v);
bool F_del_value(const std::string& v);
bool F_get(std::size_t index, std::string& out);
void F_print();

// L: двусвязный
bool L_load(const std::string& file);
bool L_save(const std::string& file);
void L_push(const std::string& v);
bool L_del_value(const std::string& v);
bool L_get(std::size_t index, std::string& out);
void L_print();

// Q: очередь
bool Q_load(const std::string& file);
bool Q_save(const std::string& file);
void Q_push(const std::string& v);
bool Q_pop (std::string* out);
void Q_print();

// S: стек
bool S_load(const std::string& file);
bool S_save(const std::string& file);
void S_push(const std::string& v);
bool S_pop (std::string* out);
void S_print();

// T: AVL (строки)
bool T_load(const std::string& file);
bool T_save(const std::string& file);
void T_insert(const std::string& key);
void T_erase (const std::string& key);
bool T_get   (const std::string& key);
void T_print();
