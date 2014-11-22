#include "../catalog/catalog.h"
#include "../parser/stmt.h"
#include <queue>

extern queue<pair<stmt_type, statement *> > stmt_queue;
extern catalog_manager catm;

void xyzsql_emit_stmt(stmt_type t, statement *stmt) ;

void xyzsql_batch() ;

void xyzsql_exit() ;

void xyzsql_process_create_table() ;

void xyzsql_process_create_index() ;

void xyzsql_process_select() ;

void xyzsql_process_drop_table() ;

void xyzsql_process_drop_index() ;

void xyzsql_process_transaction() ;

void xyzsql_process_commit() ;

void xyzsql_process_rollback() ;

void xyzsql_process_delete() ;

void xyzsql_process_insert() ;

void xyzsql_unknown_stmt() ;

void xyzsql_finalize();
