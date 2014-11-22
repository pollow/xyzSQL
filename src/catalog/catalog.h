#include "../parser/stmt.h"
#include <iostream>
#include <fstream>

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
        
        void write_back();

};
