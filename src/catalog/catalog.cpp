#include "catalog.h"

catalog::catalog(const string &_name) {
    ifstream in(_name);

    in >> name;
    while(in) {
        string col_name;
        int data_type, str_len, flag;
        in >> col_name >> data_type >> str_len >> flag;

        cols->push_back(new table_column(col_name.c_str(), data_type, str_len, flag));
    }

    in.close();
}

void catalog::write_back() {
    ofstream out(name+".catalog");

    out << name << endl;
    for(auto x : *cols) {
        out << x->name << " " << x->data_type << " " << 
            x->str_len << " " << x->flag << endl;
    }

    out.close();
}

