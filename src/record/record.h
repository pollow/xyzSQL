#include <vector>
#include "../parser/stmt.h"
#include <cstdint>

class Record {
    private:
        vector<record_value> *values;
        

    public:
		vector<unsigned char> buf;
		Record() { };
        Record( vector<record_value> *t ) : values(t) { }
        Record( const vector<unsigned char> &tmp ) : buf(tmp) {  };

        vector<record_value> unpack( vector<table_column *> *);
        vector<unsigned char> pack( vector<table_column *> *);
		
		inline uint32_t size() {
			return buf.size();
		}
};
