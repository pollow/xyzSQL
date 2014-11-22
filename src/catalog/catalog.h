#include "../parser/stmt.h"
#include <iostream>
#include <fstream>
#include <map>

class catalog {
    private:
        string name;
        vector<table_column *> *cols;

    public:

        catalog(const char *_name, vector<table_column *> *_cols) : 
            name(_name), cols(_cols) {}

        catalog(create_table_stmt* _table) :
            name(_table->name), cols(_table->cols) {}

        catalog(const string &_name);

        const string &get_name();
        
        void write_back(const string &);
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

};

