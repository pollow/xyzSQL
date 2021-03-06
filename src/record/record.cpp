#include "record.h"
#include <cassert>
#include <cstring>

void Record::unpack() {
    vector<record_value> &result = values;
    
    auto t = table_info;
    auto j = buf.begin();
    for( auto i = t->begin(); i != t->end(); j += (*i)->str_len, i++) {
        int a;
        float b;
        char * c;
        switch((*i)->data_type) {
            case table_column::INTTYPE : 
                a = (*(j+3) << 24) | (*(j+2) << 16) | (*(j+1) << 8) | (*(j));
                result.push_back(record_value(a));
                break;
            case table_column::FLOATTYPE :
                a = (*(j+3) << 24) | (*(j+2) << 16) | (*(j+1) << 8) | (*(j));
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
    char * c;
    int ttt;

    for( auto i = t->begin(); i != t->end(); i++, j++) {
        switch((*i)->data_type) {
            case table_column::INTTYPE : 
                ttt = j->as_int();
                tmp.append( (char *)&(ttt), 4 );
                break;
            case table_column::FLOATTYPE :
                ttt = j->as_int();
                tmp.append( (char *)&(ttt), 4 );
                break;
            case table_column::CHARTYPE :
                c = new char[(*i)->str_len];
                memset(c, 0, sizeof(char) * (*i)->str_len);
                strcpy(c, j->as_str());
                tmp.append(c, (*i)->str_len);
                break;
        }
    }

    buf.assign(tmp.begin(), tmp.end());
}

