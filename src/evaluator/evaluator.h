#ifndef __EVALUATOR_H__
#define __EVALUATOR_H__

#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <cctype>
#include <set>
#include <cstdlib>
#include <uuid/uuid.h>
#include <sstream>

#include "../catalog/catalog.h"
#include "../parser/stmt.h"
#include "../record/record.h"
#include "../buffer/Block.h"
#include "../buffer/File.h"
#include "../buffer/BufferManager.h"
#include "../index/IndexManager.h"
#include "../record/RecordManager.h"

#include <queue>

extern queue<pair<stmt_type, statement *> > stmt_queue;
extern catalog_manager catm;

void xyzsql_emit_stmt(stmt_type t, statement *stmt) ;

void xyzsql_batch() ;

void xyzsql_exit() ;

void xyzsql_process_create_table(create_table_stmt *s = NULL) ;

void xyzsql_process_create_index() ;

void xyzsql_process_select() ;

void xyzsql_process_drop_table() ;

void xyzsql_process_drop_index() ;

void xyzsql_process_transaction() ;

void xyzsql_process_commit() ;

void xyzsql_process_rollback() ;

void xyzsql_process_delete() ;

void xyzsql_process_insert(insert_stmt *s = NULL);

void xyzsql_unknown_stmt() ;

void xyzsql_finalize();

#endif

