#include <vector>
#include "../parser/stmt.h"

class Record {
    private:
        vector<record_value> *values;
        vector<unsigned char> buf;

    public:

        Record( vector<record_value> *t ) : values(t) { }
        Record( const vector<unsigned char> &tmp ) : buf(tmp) {  };

        vector<record_value> unpack( vector<table_column *> *);
        vector<unsigned char> pack( vector<table_column *> *);
};