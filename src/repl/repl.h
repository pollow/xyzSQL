#include "../parser/stmt.h"

extern stmt_type last_stmt;

void xyzsql_emit_stmt(stmt_type t, statement *stmt) {
    last_stmt = t;
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

