#ifndef __CATALOG_H__
#define __CATALOG_H__
#include "../parser/stmt.h"
#include <iostream>
#include <fstream>
#include <map>
#include <cstring>
class catalog {
    private:
        string name;
        int count = 0;

    public:
        vector<table_column *> *cols;

        catalog(const char *_name, vector<table_column *> *_cols) : 
            name(_name), cols(_cols) {}

        catalog(create_table_stmt* _table) :
            name(_table->name), cols(_table->cols) {}

        catalog(const string &_name);

        const string &get_name();
        
        void write_back(const string &);

        table_column *get_column(const string &attr_name);

        bool is_unique(const string &);

        bool is_indexed(const string &);

        string get_primary();

        int get_pos(const string &);

        int get_size();

        void set_size(int);
};

class catalog_manager {
    private:
        string base_addr;
        map<string, catalog *> relations;

    public:
        catalog_manager(const string &base_addr);

        catalog *exist_relation(const string &rel_name);

        catalog *add_relation(create_table_stmt *);

        void write_back();

        table_column *get_column(attribute *t);

        bool is_unique(attribute *t);

        bool is_indexed(attribute *t);

        string get_primary(const string &);

        int get_size( const string & );

        int get_data_type( const attribute * );

        int calc_record_size( const string & );

        void drop_table( const string & );
};


#endif 
