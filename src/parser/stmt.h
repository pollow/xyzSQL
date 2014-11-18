#ifndef __STMT_H__

#include <iostream>
#include <vector>

using namespace std;

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

class table_info {
    public:
        string name;
        vector<table_column *> *cols;

        int get_col_size() {
            return cols->size();
        }

        // void add_column(table_column *a) {
        //     cols.push_back(a);
        // }

        table_info(const char *_name, vector<table_column *> *_cols) : name(_name), cols(_cols) {}

        void print() {
            cout << "Table " << name << " contains " << get_col_size() << " columns:" << endl;
            for(auto x : *cols) {
                cout << "\t";
                x->print();
            }
            cout << endl;
        }

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

class select_stmt {
    public:
        vector<attribute *> *projection_list; 
        vector<string *> *table_list;
        vector<condition *> *condition_list;
        algbric_node *root;

        select_stmt(vector<attribute *> *pl, vector<string *> *tl, vector<condition *> *cl) : projection_list(pl), table_list(tl), condition_list(cl) {}

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

        void parse_algebric_tree() {
            vector<algbric_node *> tables;
            for( auto x : *table_list ) {
                auto tmp = new algbric_node(algbric_node::DIRECT);
                tmp->table = x;
                tables.push_back(tmp);
            }
            // root = new algbric_node(algbric_node::PROJECTION);

        }
};

#endif 

