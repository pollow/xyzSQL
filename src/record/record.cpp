#include "record.h"
#include <cassert>

void Record::unpack() {
    vector<record_value> &result = values;
    
    auto t = table_info;
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
}

void Record::pack() {
    // vector<unsigned char> result;
    string tmp;

    auto t = table_info;
    auto j = values.begin();
    int ttt;

    for( auto i = t->begin(); i != t->end(); i++, j++) {
        switch((*i)->data_type) {
            case table_column::INTTYPE : 
                ttt = j->as_int();
                tmp.append( (char *)&(ttt), 4 );
                break;
            case table_column::FLOATTYPE :
                tmp.append( (char *)&(ttt), 4 );
                break;
            case table_column::CHARTYPE :
                tmp.append( j->as_str(), (*i)->str_len );
                break;
        }
    }

    buf.assign(tmp.begin(), tmp.end());
}

