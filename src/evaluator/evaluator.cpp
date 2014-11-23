#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <cctype>
#include <cstdlib>

#include "evaluator.h"
#include "../record/RecordManager.h"
#include "../buffer/BufferManager.h"

queue<pair<stmt_type, statement *> > stmt_queue;
catalog_manager catm("data");
extern string base_addr;
extern RecordManager RecordManager;
extern BufferManager BufferManager;

void create_temp_table() {

}

bool calc_conditions(vector<condition *> conditions, record_value c) {
    for(auto x : conditions) {
        if (x->calc( {catm.get_column(x->left_attr), c} ) == false) 
            return false;
    }
    return true;
}

void xyzsql_emit_stmt(stmt_type t, statement *stmt) {
    stmt_queue.push({t, stmt});
}

void xyzsql_batch() {
    cout << "batch!" << endl;
}

void xyzsql_exit() {
    cout << "now exit!" << endl;

    xyzsql_finalize();

    exit(0);
}

void xyzsql_process_create_table() {
    cout << "table created." << endl;
    auto s = dynamic_cast<create_table_stmt *>(stmt_queue.front().second);

    if ( catm.exist_relation(s->name) == NULL ) {
        catm.add_relation(s);
        catm.write_back();
        RecordManager.createMaster(s->name);
    } else 
        cerr << "Table name already exists." << endl;

}

void xyzsql_process_create_index() {

    cout << "index created." << endl;
}

void xyzsql_process_select() {
    cout << "select parsed." << endl;

    auto s = dynamic_cast<select_stmt *>(stmt_queue.front().second);

    // checker
     
    // set root
    auto root = new algbric_node(algbric_node::PROJECTION);
    root->projection_list = s->projection_list;
    root->left = new algbric_node(algbric_node::SELECTION);

    auto cursor = root->left;
    for ( auto c : *(s->condition_list) ) {
       if (!c->flag) cursor->conditions.push_back(c);
    }

    cursor->left = new algbric_node(algbric_node::JOIN);
    cursor = cursor->left;
    for ( auto c : *(s->condition_list) ) {
       if (c->flag) cursor->conditions.push_back(c);
    }

    for (auto i = s->table_list->begin(); i != s->table_list->end(); i++) {
        cursor->right = new algbric_node(algbric_node::DIRECT);
        cursor->right->table = *i;

        cursor->left  = new algbric_node(algbric_node::PROJECTION);
        cursor = cursor->left;
    }

    {
        cursor->op = algbric_node::DIRECT;
        cursor->table = cursor->right->table;
        delete cursor->right;
        cursor->right = NULL;
    }

    root->calc();
}

void xyzsql_process_drop_table() {
    cout << "table dropped." << endl;
}

void xyzsql_process_drop_index() {
    cout << "index dropped." << endl;
}

void xyzsql_process_delete() {

    auto s = dynamic_cast<delete_stmt *>(stmt_queue.front().second);

    if (s->condition_list->empty()) {
        // delete all
        assert(base_addr.back() == '/');
        system(("rm " + base_addr + s->table_name + "/*.db").c_str());
    } else {
        BufferManager.newTrashCan();
        // unique 
        condition *p = NULL, *eq = NULL;
        for(auto x : *(s->condition_list)) {
            if ( catm.is_unique(x->left_attr) ) {
                p = x;
                if (x->op == condition::EQUALTO) 
                    eq = x;
            }
        }
        
        if ( eq != NULL || p != NULL ) {
            // use index
            auto t = eq == NULL ? p->left_attr : eq->left_attr;
            indexIterator a;
            IndexManager.selectNode(a, base_addr + t->relation_name + "/index_" + t->attribute + ".db", 1, string(atos(1.1)));
            int b = 0, c = 0;
            while (a.next(b, c) == 0) {
                Record a = RecordManager.getRecord(t->relation_name, b, c);
                calc_conditions(s->condition_list, a.unpack(catm.exist_relation(t->relation_name)->cols));
                BufferManager.appendTrashCan(b, c);
            }
        } else {
            indexIterator a;
            IndexManager.getStarter(a, base_addr + s->table_name + "/index_" + catm.get_primary(s->table_name) + ".db");
            int b = 0, c = 0;
            while (a.next(b, c) == 0) {
                Record a = RecordManager.getRecord(s->table_name, b, c);
                calc_conditions(s->condition_list, a.unpack(catm.exist_relation(s->table_name)->cols));
                BufferManager.appendTrashCan(b, c);
            }
        }
        BufferManager.emptyTrashCan();
    }
    cout << "records deleted." << endl;
}

void xyzsql_process_insert() {
    auto s = dynamic_cast<insert_stmt *>(stmt_queue.front().second);

    // RecordManager.insert(s->table_name, Record(s->values));
    cout << "record inserted." << endl;
}

void xyzsql_unknown_stmt() {
    cout << "unknown statement, check syntax again." << endl;

}












void xyzsql_process_commit() {
    cout << "Transaction committed." << endl;

}

void xyzsql_process_rollback() {
    cout << "Transaction rollbacked." << endl;

}

void xyzsql_process_transaction() {
    cout << "Transaction on." << endl;

}
