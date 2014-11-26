#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <cctype>
#include <sstream>
#include <ctime>
#include "../parser/stmt.h"
#include "../../parser.tab.h"
#include "evaluator.h"

typedef struct yy_buffer_state *YY_BUFFER_STATE;

extern YY_BUFFER_STATE yy_scan_string (const char *yy_str  );
extern void yy_switch_to_buffer (YY_BUFFER_STATE new_buffer  );
extern void yy_delete_buffer (YY_BUFFER_STATE b  );
extern int yyparse ();
 
using namespace std;

BufferManager BufferManager;
IndexManager IndexManager(&BufferManager);
catalog_manager catm(".");
RecordManager RecordManager;

string base_addr = ".";

void system_init() { 
    cout << "System Initialized!" << endl;

    RecordManager.Init(&BufferManager, &catm, &IndexManager);

}

int main() {

    system_init();

    clock_t start_time = 0, end_time;

    while(true) {
        if ( stmt_queue.empty() ) {
            end_time = clock();
            if (start_time != 0 ) {
                cout << (end_time - start_time) / (float)CLOCKS_PER_SEC * 1000 << " ms used. " << end_time - start_time << endl;
            }
            char * line = readline(">>> ");
            add_history(line);
            start_time = clock();

            int len = strlen(line);
            char *tmp = new char[len + 2];
            strcpy(tmp, line);
            tmp[len + 1] = 0;

            YY_BUFFER_STATE my_string_buffer = yy_scan_string(tmp);
            yy_switch_to_buffer( my_string_buffer );
            yyparse();
            yy_delete_buffer( my_string_buffer );

            delete[] tmp;
        }

        try {
            while( !stmt_queue.empty() ) {

                switch( stmt_queue.front().first ) {
                    case stmt_type::_create_table_stmt:
                        xyzsql_process_create_table();
                        break;
                    case stmt_type::_create_index_stmt:
                        xyzsql_process_create_index();
                        break;
                    case stmt_type::_select_stmt:
                        xyzsql_process_select();
                        break;
                    case stmt_type::_insert_stmt:
                        xyzsql_process_insert();
                        break;
                    case stmt_type::_delete_stmt:
                        xyzsql_process_delete();
                        break;
                    case stmt_type::_drop_table_stmt:
                        xyzsql_process_drop_table();
                        break;
                    case stmt_type::_drop_index_stmt:
                        xyzsql_process_drop_index();
                        break;
                    case stmt_type::_transaction_stmt:
                        xyzsql_process_transaction();
                        break;
                    case stmt_type::_commit_stmt:
                        xyzsql_process_commit();
                        break;
                    case stmt_type::_rollback_stmt:
                        xyzsql_process_rollback();
                        break;
                    case stmt_type::_quit_stmt:
                        xyzsql_exit();
                        break;
                    case stmt_type::_exefile_stmt:
                        xyzsql_batch();
                        break;
                    default: xyzsql_unknown_stmt();
                }

                if ( stmt_queue.front().second != nullptr)
                    delete stmt_queue.front().second;
                stmt_queue.pop();
            }

        } catch( exception &t ) {
            cout << t.what() << endl;
            stmt_queue.pop();
        }
    }

    xyzsql_finalize();

    return 0;
}

