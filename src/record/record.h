#ifndef __RECORD_H__
#define __RECORD_H__

#include <vector>
#include <cstdint>

#include "../parser/stmt.h"

class Record {
    private:
        void unpack();
        void pack();

    public:
		vector<unsigned char> buf;
        vector<record_value> values;
        vector<table_column *> *table_info;

		Record() { };

        Record( vector<record_value> &t, vector<table_column *> *col ) : values(t), table_info(col) {
            pack();
        }

        Record( const vector<unsigned char> &tmp, vector<table_column *> *col ) : buf(tmp), table_info(col) {
            unpack();
        };

        record_value get_value(table_column *t) {
            for(auto x = table_info->begin(); x != table_info->end(); x++) {
                if ((*x)->name == t->name) {
                    return values[x-table_info->begin()];
                }
            }

            // should never reached here.
            return record_value(0);
        }

        record_value get_value(const string t) {
            for(auto x = table_info->begin(); x != table_info->end(); x++) {
                if ((*x)->name == t) {
                    return values[x-table_info->begin()];
                }
            }

            // should never reached here.
            return record_value(0);
        }

		inline uint32_t size() {
			return buf.size();
		}
};

#endif
