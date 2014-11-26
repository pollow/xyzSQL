#ifndef __STMT_H__
#define __STMT_H__

#include <iostream>
#include <vector>
#include <cassert>
#include <sstream>
#include <cstring>
using namespace std;

enum stmt_type {
    _create_table_stmt = 1,
    _create_index_stmt,
    _insert_stmt,
    _select_stmt,
    _delete_stmt,
    _drop_index_stmt,
    _drop_table_stmt,
    _transaction_stmt,
    _commit_stmt,
    _rollback_stmt,
    _quit_stmt,
    _exefile_stmt
};

class table_column {
    public:
        static const int INTTYPE = 0, FLOATTYPE = 1, CHARTYPE = 2;
        static const int primary_attr = 1, index_attr = 2, notnull_attr = 4, unique_attr = 8;
        
        string name;
        int data_type, str_len, flag;

        table_column(const char *_name, int _data_type, int _str_len, int _flag) :
            name(_name), data_type(_data_type), str_len(_str_len), flag(_flag) {}
};

string data_type_to_str(int data_type);

class attribute {
    public:
        string attribute_name, relation_name, full_name;

        attribute(const char *relt, const char *attr) : attribute_name(attr), relation_name(relt), full_name(relation_name + "." + attribute_name) {attribute_name = full_name;}
        
};

class record_value {
    public:
        uint32_t value;
        int data_type; 

        record_value(uint32_t _value) : value(_value) {}
        record_value(char *   _value) : data_type(table_column::CHARTYPE) { memcpy(&value, &_value, 4); }
        record_value(int      _value) : data_type(table_column::INTTYPE) { memcpy(&value, &_value, 4); }
        record_value(float    _value) : data_type(table_column::FLOATTYPE) { memcpy(&value, &_value, 4); }

        float as_float() const ;

        char * as_str() const ;

        int as_int() const ;

        string to_str(int data_type) ;

        static int compare_as_int(const record_value &a, const record_value &b) ;

        static int compare_as_float(const record_value &a, const record_value &b) ;

        static int compare_as_str(const record_value &a, const record_value &b) ;

        static int compare(int data_type, const record_value &a, const record_value &b) ;
};

class condition {
    public:
        attribute *left_attr, *right_attr;
        // string str;
        // float fnum;
        // int inum;
        record_value v;
        int op;
        bool flag;

        condition(attribute *l, attribute *r,   int _op) : 
            left_attr(l), right_attr(r), v(0), op(_op), flag(true) {}
        condition(attribute *l, float r,        int _op) : 
            left_attr(l), v(r), op(_op), flag(false) {}
        condition(attribute *l, int r,          int _op) : 
            left_attr(l), v(r), op(_op), flag(false) {}
        condition(attribute *l, char *r,        int _op) : 
            left_attr(l), v(r), op(_op), flag(false) {}

        bool calc(pair<table_column *, record_value>) ;
        bool calc(pair<table_column *, record_value>, pair<table_column *, record_value> ) ;

        static const int EQUALTO = 1, GREATERTHAN = 2, LESSTHAN = 3, GREATER_EQUAL = 4, LESS_EQUAL = 5, NOT_EQUAL = 6;

        ~condition();
};

class algbric_node {
    public:
        int op;
        bool flag;
        algbric_node *left, *right; 
        string table;
        vector<condition *> conditions;
        vector<attribute *> *projection_list; 

        algbric_node(int _op) : op(_op) { flag = false; }

        static const int DIRECT = 0, PROJECTION = 1, SELECTION = 2, JOIN = 3, PRODUCTION = 4;

        ~algbric_node();
};

class statement {

    public:
        virtual ~statement() {};

};

class select_stmt : public statement {
    public:
        vector<attribute *> *projection_list; 
        vector<string *> *table_list;
        vector<condition *> *condition_list;

        select_stmt(vector<attribute *> *pl, vector<string *> *tl, vector<condition *> *cl) : 
            statement(), projection_list(pl), table_list(tl), condition_list(cl) {}

        ~select_stmt();
};

class create_table_stmt : public statement {
    public:
        string name;
        vector<table_column *> *cols;

        int get_col_size() {
            return cols->size();
        }

        create_table_stmt(const char *_name, vector<table_column *> *_cols) : 
            statement(), name(_name), cols(_cols) {}

        ~create_table_stmt();
};

class create_index_stmt : public statement {
    public:
        attribute *attr;

        create_index_stmt( attribute *_a ) : 
            statement(), attr(_a) {}

        ~create_index_stmt();
};


class insert_stmt : public statement {
    public:
        string table_name;
        vector<record_value> *values;

        insert_stmt( const string &_table_name, vector<record_value> *_values ) : 
            statement(), table_name(_table_name), values(_values) {}

        ~insert_stmt();
};

class drop_table_stmt : public statement {
    public:
        string table_name;

        drop_table_stmt( const string &_table_name ) :
            statement(), table_name(_table_name) {}

        ~drop_table_stmt();
};

class drop_index_stmt : public statement {
    public:
        attribute *attr;

        drop_index_stmt( attribute *_a ) : 
            statement(), attr(_a) {}

        ~drop_index_stmt();
};

class delete_stmt : public statement {
    public:
        string table_name;
        vector<condition *> *condition_list;

        delete_stmt(const string _table_name, vector<condition *> *_condition_list) :
            statement(), table_name(_table_name), condition_list(_condition_list) {}

        ~delete_stmt();
};

class exefile_stmt : public statement {
    public:
        string file_name;

        exefile_stmt( const string &_file_name ) :
            statement(), file_name(_file_name) {}

        ~exefile_stmt();
};

#endif

