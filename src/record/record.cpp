#include "record.h"
#include <cassert>

vector<record_value> Record::unpack( vector<table_column *> *t) {
    vector<record_value> result;
    
    auto j = buf.begin();
    for( auto i = t->begin(); i != t->end(); i++, j++) {
        int a;
        float b;
        char * c;
        switch((*i)->data_type) {
            case table_column::INTTYPE : 
                a = ((*j) << 24) | (*(j+1) << 16) | (*(j+2) << 8) | (*(j+3));
                result.push_back(record_value(a));
                break;
            case table_column::FLOATTYPE :
                a = ((*j) << 24) | (*(j+1) << 16) | (*(j+2) << 8) | (*(j+3));
                b = *((float *)&a);
                result.push_back(record_value(b));
                break;
            case table_column::CHARTYPE :
                c = new char[(*i)->str_len];
                memset(c, 0, sizeof(char) * (*i)->str_len);
                for(int k = 0; k < (*i)->str_len; k++) 
                    c[k] = *(j+k);
                result.push_back(record_value(c));
                break;
        }
    }

    return result;
}

vector<unsigned char> Record::pack( vector<table_column *> *t ) {
    vector<unsigned char> result;
    string tmp;

    auto j = values->begin();
    for( auto i = t->begin(); i != t->end(); i++, j++) {
        switch((*i)->data_type) {
            case table_column::INTTYPE : 
                tmp.append( j->as_str(), 4 );
                break;
            case table_column::FLOATTYPE :
                tmp.append( j->as_str(), 4 );
                break;
            case table_column::CHARTYPE :
                tmp.append( j->as_str(), (*i)->str_len );
                break;
        }
    }

    result.assign(tmp.begin(), tmp.end());

    return result;
}

