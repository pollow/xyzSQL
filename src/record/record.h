#include <vector>
#include "../parser/stmt.h"
#include <cstdint>

class Record {
    private:
        vector<table_column *> *table_info;

        void unpack();
        void pack();

    public:
		vector<unsigned char> buf;
        vector<record_value> values;

		Record() { };

        Record( vector<record_value> &t, vector<table_column *> *col ) : values(t), table_info(col) {
            pack();
        }
        Record( const vector<unsigned char> &tmp, vector<table_column *> *col ) : table_info(col), buf(tmp) {
            unpack();
        };

		inline uint32_t size() {
			return buf.size();
		}
};
