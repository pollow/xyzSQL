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
    char *tmp;
    memcpy(&tmp, &value, 4); 
    return tmp;
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

bool condition::calc(pair<table_column *, record_value> p) {
    assert(flag == false);
    switch(op) {
        case EQUALTO : 
            return record_value::compare(p.first->data_type, v, p.second) == 0;
        case GREATERTHAN :
            return record_value::compare(p.first->data_type, v, p.second) > 0;
        case LESSTHAN :
            return record_value::compare(p.first->data_type, v, p.second) < 0;
        case GREATER_EQUAL :
            return record_value::compare(p.first->data_type, v, p.second) >= 0;
        case LESS_EQUAL :
            return record_value::compare(p.first->data_type, v, p.second) <= 0;
        case NOT_EQUAL :
            return record_value::compare(p.first->data_type, v, p.second) != 0;
        default : 
            return false;
    }
}
