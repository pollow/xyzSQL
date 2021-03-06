#include "stmt.h"
#include <sstream>

string data_type_to_str(int data_type) {
    switch (data_type) {
        case table_column::INTTYPE : return "int";
        case table_column::FLOATTYPE: return "float";
        case table_column::CHARTYPE : return "char";
        default : return "";
    }
} 

float record_value::as_float() const {
    float tmp = 0; 
    memcpy(&tmp, &value, 4); 
    return tmp;
}

char *record_value:: as_str() const {
    // char *tmp;
    // memcpy(&tmp, &value, 4); 
    return (char *)value;
}

int record_value::as_int() const {
    return value;
}

string record_value::to_str(int data_type) {
    stringstream x;
    switch (data_type) {
        case table_column::INTTYPE : 
            x << as_int();
            return x.str();
        case table_column::FLOATTYPE :
            x << as_float();
            return x.str();
        case table_column::CHARTYPE :
            return string(as_str());
        default : return "";
    }
}

int record_value::compare_as_int(const record_value &a, const record_value &b) {
    if (a.as_int() < b.as_int()) return -1;
    else if (a.as_int() > b.as_int()) return 1;
    else return 0;
}

int record_value::compare_as_float(const record_value &a, const record_value &b) {
    if (a.as_float() < b.as_float()) return -1;
    else if (a.as_float() > b.as_float()) return 1;
    else return 0;
}

int record_value::compare_as_str(const record_value &a, const record_value &b) {
    return strcmp(a.as_str(), b.as_str());
}

int record_value::compare(int data_type, const record_value &a, const record_value &b) {
    switch (data_type) {
        case table_column::INTTYPE : 
            return compare_as_int(a, b); break;
        case table_column::FLOATTYPE :
            return compare_as_float(a, b); break;
        case table_column::CHARTYPE :
            return compare_as_str(a, b); break;
        default : return false;
    }
}

bool condition::calc(pair<table_column *, record_value> p1, pair<table_column *, record_value> p2) {
    assert(flag == true);
    switch(op) {
        case EQUALTO : 
            return record_value::compare(p1.first->data_type, p1.second, p2.second) == 0;
        case GREATERTHAN :                                              
            return record_value::compare(p1.first->data_type, p1.second, p2.second) > 0;
        case LESSTHAN :                                                 
            return record_value::compare(p1.first->data_type, p1.second, p2.second) < 0;
        case GREATER_EQUAL :                                            
            return record_value::compare(p1.first->data_type, p1.second, p2.second) >= 0;
        case LESS_EQUAL :                                               
            return record_value::compare(p1.first->data_type, p1.second, p2.second) <= 0;
        case NOT_EQUAL :                                                
            return record_value::compare(p1.first->data_type, p1.second, p2.second) != 0;
        default : 
            return false;
    }
}

bool condition::calc(pair<table_column *, record_value> p) {
    assert(flag == false);
    switch(op) {
        case EQUALTO : 
            return record_value::compare(p.first->data_type, p.second, v) == 0;
        case GREATERTHAN :                                              
            return record_value::compare(p.first->data_type, p.second, v) > 0;
        case LESSTHAN :                                                 
            return record_value::compare(p.first->data_type, p.second, v) < 0;
        case GREATER_EQUAL :                                            
            return record_value::compare(p.first->data_type, p.second, v) >= 0;
        case LESS_EQUAL :                                               
            return record_value::compare(p.first->data_type, p.second, v) <= 0;
        case NOT_EQUAL :                                                
            return record_value::compare(p.first->data_type, p.second, v) != 0;
        default : 
            return false;
    }
}

select_stmt::~select_stmt() {
    for (auto x : *projection_list) delete x;
    delete projection_list;
    for (auto x : *table_list) delete x;
    delete table_list;
    for (auto x : *condition_list) delete x;
    delete condition_list;
}

create_table_stmt::~create_table_stmt() {
}

create_index_stmt::~create_index_stmt() {
    delete attr;
}

insert_stmt::~insert_stmt() {
    for ( auto x : *values ) 
        if (x.data_type == table_column::CHARTYPE)
            delete[] x.as_str();
    delete values;
}

drop_index_stmt::~drop_index_stmt() {
    delete attr;
}

delete_stmt::~delete_stmt() {
    for( auto x : *condition_list ) delete x;
    delete condition_list;
}

condition::~condition() {
    if (left_attr != nullptr) 
        delete left_attr;
    if (right_attr != nullptr)
        delete right_attr;
}

algbric_node::~algbric_node() {
    if (left != nullptr) 
        delete left;
    if (right != nullptr)
        delete right;
}

