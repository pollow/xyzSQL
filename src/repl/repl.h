#include "../catalog/catalog.h"
#include "../parser/stmt.h"
#include <queue>

queue<pair<stmt_type, statement *>> stmt_queue;
catalog_manager catm("data");

void xyzsql_emit_stmt(stmt_type t, statement *stmt) {
    stmt_queue.push({t, stmt});
}

void xyzsql_batch() {
    cout << "batch!" << endl;
}

void xyzsql_exit() {
    cout << "now exit!" << endl;
    exit(0);
}

void xyzsql_process_create_table() {
    cout << "table created." << endl;
    auto s = dynamic_cast<create_table_stmt *>(stmt_queue.front().second);

    if ( catm.exist_relation(s->name) == NULL ) {
        catm.add_relation(s);
        catm.write_back();
    } else 
        cerr << "Table name already exists." << endl;

}

void xyzsql_process_create_index() {

    cout << "index created." << endl;
}

void xyzsql_process_select() {
    cout << "select parsed." << endl;

}

void xyzsql_process_drop_table() {
    cout << "table dropped." << endl;

}

void xyzsql_process_drop_index() {
    cout << "index dropped." << endl;

}

void xyzsql_process_transaction() {
    cout << "Transaction on." << endl;

}

void xyzsql_process_commit() {
    cout << "Transaction committed." << endl;

}

void xyzsql_process_rollback() {
    cout << "Transaction rollbacked." << endl;

}

void xyzsql_process_delete() {
    cout << "records deleted." << endl;

}

void xyzsql_process_insert() {
    cout << "record inserted." << endl;

}

void xyzsql_unknown_stmt() {
    cout << "unknown statement, check syntax again." << endl;

}

