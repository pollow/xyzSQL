#ifndef __STMT_H__
#define __STMT_H__

#include <iostream>
#include <vector>

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

        void print() {
            cout << name << ' ' << data_type << ' ' << flag << endl;
        }

        table_column(const char *_name, int _data_type, int _str_len, int _flag) :
            name(_name), data_type(_data_type), str_len(_str_len), flag(_flag) {}
};


class attribute {
    public:
        string attribute_name, relation_name, full_name;

        attribute(const char *attr, const char *relt) : attribute_name(attr), relation_name(relt), full_name(relation_name + "." + attribute_name) {}
        
};


class condition {
    public:
        attribute *left_attr, *right_attr;
        string str;
        float fnum;
        int inum;
        int op;

        condition(attribute *l, attribute *r, int _op) : left_attr(l), right_attr(r), op(_op) {}
        condition(attribute *l, float r, int _op) : left_attr(l), fnum(r), op(_op) {}
        condition(attribute *l, int r  , int _op) : left_attr(l), inum(r), op(_op) {}
        condition(attribute *l, string r  , int _op) : left_attr(l), str(r), op(_op) {}

        void calc() {

        }
};

class algbric_node {
    public:
        int op;
        bool flag;
        void *left, *right; 
        string *table;
        vector<condition *> conditions;

        algbric_node(int _op) : op(_op) { flag = false; }

        static const int DIRECT = 0, PROJECTION = 1, SELECTION = 2, JOIN = 3, PRODUCTION = 4;
};

class statement {

};

class select_stmt : public statement {
    public:
        vector<attribute *> *projection_list; 
        vector<string *> *table_list;
        vector<condition *> *condition_list;
        algbric_node *root;

        select_stmt(vector<attribute *> *pl, vector<string *> *tl, vector<condition *> *cl) : 
            statement(), projection_list(pl), table_list(tl), condition_list(cl) {}

        void print() {
            for ( auto x : *projection_list ) {
                cout << x->full_name << ",";
            }
            cout << endl;
            for ( auto x : *table_list) {
                cout << *x << ",";
            }
            cout << endl;
        }

        // void parse_algebric_tree() {
        //     vector<algbric_node *> tables;
        //     for( auto x : *table_list ) {
        //         auto tmp = new algbric_node(algbric_node::DIRECT);
        //         tmp->table = x;
        //         tables.push_back(tmp);
        //     }
        //     // root = new algbric_node(algbric_node::PROJECTION);

        // }
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

        void print() {
            cout << "Table " << name << " contains " << get_col_size() << " columns:" << endl;
            for(auto x : *cols) {
                cout << "\t";
                x->print();
            }
            cout << endl;
        }
};

class create_index_stmt : public statement {
    public:
        string index_name, table_name, attr_name;

        create_index_stmt( const string &_index_name, const string &_table_name, const string &_attr_name ) : 
            statement(), index_name(_index_name), table_name(_table_name), attr_name(_attr_name) {}

};

class record_value {
    public:
        uint32_t value;

        record_value(uint32_t _value) : value(_value) {}
        record_value(char *   _value) { memcpy(&value, &_value, 4); }
        record_value(int      _value) { memcpy(&value, &_value, 4); }
        record_value(float    _value) { memcpy(&value, &_value, 4); }

        float as_float() {
            float tmp = 0; 
            memcpy(&tmp, &value, 4); 
            return tmp;
        }

        char * as_str() {
            char *tmp;
            memcpy(&tmp, &value, 4); 
            return tmp;
        }

        int as_int() {
            return value;
        }
};

class insert_stmt : public statement {
    public:
        string table_name;
        vector<record_value> *values;

        insert_stmt( const string &_table_name, vector<record_value> *_values ) : 
            statement(), table_name(_table_name), values(_values) {}
};

class drop_table_stmt : public statement {
    public:
        string table_name;

        drop_table_stmt( const string &_table_name ) :
            statement(), table_name(_table_name) {}
};

class drop_index_stmt : public statement {
    public:
        string index_name;

        drop_index_stmt( const string &_index_name ) :
            statement(), index_name(_index_name) {}
};

class delete_stmt : public statement {
    public:
        string table_name;
        vector<condition *> *condition_list;

        delete_stmt(const string _table_name, vector<condition *> *_condition_list) :
            statement(), table_name(_table_name), condition_list(_condition_list) {}

};

class exefile_stmt : public statement {
    public:
        string file_name;

        exefile_stmt( const string &_file_name ) :
            statement(), file_name(_file_name) {}
};

#endif
