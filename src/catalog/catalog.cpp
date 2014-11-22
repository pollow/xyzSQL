#include "catalog.h"
#include <sys/stat.h>
#include <cassert>

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

void catalog::write_back(const string &addr) {
    ofstream out(addr + "/catalog");

    out << name << endl;
    for(auto x : *cols) {
        out << x->name << " " << x->data_type << " " << 
            x->str_len << " " << x->flag << endl;
    }

    out.close();
}

const string &catalog::get_name() {
    return name;
}

catalog_manager::catalog_manager(const string &_base_addr) 
    : base_addr(_base_addr.back() == '/' ? _base_addr : _base_addr + "/") 
{
    assert( base_addr.back() == '/' );

    ifstream in(base_addr + "catalog");

    if (in.is_open()) {
        while (in) {
            string tmp;
            in >> tmp;
            relations[tmp] = new catalog(base_addr + tmp + "/catalog");
        }
    }

    in.close();
}

catalog * catalog_manager::exist_relation(const string &rel_name) {
    auto tmp = relations.find(rel_name);
    if ( tmp == relations.end() )
        return NULL; 
    else return tmp->second;
}

catalog * catalog_manager::add_relation(create_table_stmt *tmp) {
    auto new_catalog = new catalog(tmp);
    relations[tmp->name] = new_catalog;

    // mkdir and write back
    mkdir((base_addr + "/" + new_catalog->get_name()).c_str(), 0777);
    // TODO exception handler

    new_catalog->write_back((base_addr + new_catalog->get_name()));

    cout << "[debug] " << (base_addr + new_catalog->get_name() + "/catalog") << endl;

    return new_catalog;
}

void catalog_manager::write_back() {
    ofstream out(base_addr + "/catalog");

    for ( auto x : relations ) {
        out << x.first << endl;
        x.second->write_back(base_addr + x.second->get_name());
    }

    out.close();
}

