#include "catalog.h"
#include <sys/stat.h>
#include <cassert>

catalog::catalog(const string &_name) {
    ifstream in(_name);

    cols = new vector<table_column *>;

    in >> name;
    string col_name;
    while(in >> col_name) {
        int data_type, str_len, flag;
        in >> data_type >> str_len >> flag;

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

table_column *catalog::get_column(const string &attr_name) {
    for(auto x : *cols) {
        if (x->name == attr_name) return x;
    }

    return NULL;
}

bool catalog::is_unique(const string &attr_name) {
    for(auto x : *cols) {
        if (x->name == attr_name) return x->flag & (table_column::unique_attr | table_column::primary_attr);
    }

    return false;
}

bool catalog::is_indexed(const string &attr_name) {
    for(auto x : *cols) {
        if (x->name == attr_name) return x->flag & (table_column::index_attr | table_column::primary_attr);
    }

    return false;
}

void catalog::set_size(int _count) {
    count = _count;
}

int catalog::get_size() {
    return count;
}


string catalog::get_primary() {
    for(auto x : *cols) {
        if (x->flag & table_column::primary_attr) return x->name;
    }

    return "";
}

const string &catalog::get_name() {
    return name;
}

int catalog::get_pos(const string &attr_name) {
    for(auto i = cols->begin(); i != cols->end(); i++) 
        if ((*i)->name == attr_name) return (i-cols->begin());

    return -1;
}

catalog_manager::catalog_manager(const string &_base_addr) 
    : base_addr(_base_addr.back() == '/' ? _base_addr : _base_addr + "/") 
{
    assert( base_addr.back() == '/' );

    ifstream in(base_addr + "catalog");

    string tmp;
    if (in.is_open()) {
        while (in >> tmp) {
            relations[tmp] = new catalog(base_addr + tmp + "/catalog");
            int count;
            in >> count;
            relations[tmp]->set_size(count);
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

    // cout << "[debug] " << (base_addr + new_catalog->get_name() + "/catalog") << endl;

    return new_catalog;
}

void catalog_manager::write_back() {
    ofstream out(base_addr + "/catalog");

    for ( auto x : relations ) {
        out << x.first << " " << x.second->get_size() << endl;
        x.second->write_back(base_addr + x.second->get_name());
    }

    out.close();
}

table_column *catalog_manager::get_column(attribute *t) {
    return exist_relation(t->relation_name)->get_column(t->attribute_name);
}

bool catalog_manager::is_unique(attribute *t) {
    return exist_relation(t->relation_name)->is_unique(t->attribute_name);
}

bool catalog_manager::is_indexed(attribute *t) {
    return exist_relation(t->relation_name)->is_indexed(t->attribute_name);
}

string catalog_manager::get_primary(const string &rel_name) {
    return exist_relation(rel_name)->get_primary();
}

int catalog_manager::get_size(const string &rel_name) {
    return exist_relation(rel_name)->get_size();
}

int catalog_manager::get_data_type(const attribute *t) {
    return exist_relation(t->relation_name)->get_column(t->attribute_name)->data_type;
}

int catalog_manager::calc_record_size(const string &rel_name ) {
    auto t = exist_relation(rel_name);
    int c = 0;
    for( auto x : *(t->cols) ) { 
        c += x->str_len;
    }

    return c;
}

bool catalog_manager::drop_table(const string &rel_name) {
    system(("rm -rf " + rel_name).c_str());
    delete relations[rel_name];
    return relations.erase(rel_name);
}
