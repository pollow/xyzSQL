#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <cctype>
#include <set>
#include <cstdlib>
#include <uuid/uuid.h>

#include "evaluator.h"
#include "../record/RecordManager.h"
#include "../buffer/BufferManager.h"
#include "../index/IndexManager.h"

queue<pair<stmt_type, statement *> > stmt_queue;
catalog_manager catm("data");
extern string base_addr;
extern RecordManager RecordManager;
extern BufferManager BufferManager;
extern IndexManager IndexManager;


string create_temp_table(vector<table_column *> *t) {
    uuid_t out;
    uuid_string_t uuid_str;
    uuid_generate(out);
    uuid_unparse(out, uuid_str);
    create_table_stmt *cs = new create_table_stmt(uuid_str, t);

    xyzsql_process_create_table(cs);

    return string(uuid_str);
}

bool calc_conditions(vector<condition *> *conditions, record_value c) {
    for(auto x : *conditions) {
        if (x->calc( {catm.get_column(x->left_attr), c} ) == false) 
            return false;
    }
    return true;
}

void calc_algric_tree(algbric_node *root) {
    switch ( root->op ) {
        case algbric_node::DIRECT :
            root->flag = true;
            return;
        case algbric_node::PROJECTION :
            auto tmp = new vector<table_column *>;
            for( auto x : *(root->projection_list) ) {
                auto att = catm.exist_relation(x->relation_name)->get_column(x->relation_name);
                tmp->push_back(new table_column(x->full_name.c_str(), att->data_type, att->str_len, 0 ));
            }
            string table_name = create_temp_table(tmp);

            indexIterator a;
            IndexManager.getStarter(a, base_addr + s->table_name + "/index_" + catm.get_primary(s->table_name) + ".db");
            int b = 0, c = 0;
            while (a.next(b, c) == 0) {
                Record a = RecordManager.getRecord(table_name, b, c, catm.calc_record_size(table_name));
                auto z = catm.exist_relation(table_name);
                auto x = a.unpack(z->cols);
                record_value y = x[z->get_pos(table_name)];
            }
            
    }
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

void xyzsql_process_create_table(create_table_stmt *s ) {
    cout << "table created." << endl;
    if ( s == NULL ) 
        s = dynamic_cast<create_table_stmt *>(stmt_queue.front().second);

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
    

    // push selection
    vector< algbric_node * > leaf_nodes;
    for(auto x : *(s->table_list)) {
        // add to select queue
        algbric_node *direct = new algbric_node(algbric_node::DIRECT);
        direct->table = x;
        algbric_node *select = new algbric_node(algbric_node::SELECTION);
        for ( auto y : *(s->condition_list) ) {
            if ( y->flag == false && y->left_attr->relation_name == *(x) ) {
                select->conditions.push_back(y);
            }
        }
        if (select->conditions.empty()) {
            leaf_nodes.push_back(direct);
            delete select;
        } else {
            select->left = direct;
            calc_algric_tree(select);
            leaf_nodes.push_back(select);
        }
    }
    
    algbric_node *root = NULL;
    set<string> rel_set;
    while (!leaf_nodes.empty()) {
        unsigned int tmp = -1;
        auto label = leaf_nodes.begin(); 
        for(auto i = leaf_nodes.begin(); i != leaf_nodes.end(); i++) {
            if ( catm.get_size((*i)->table) < tmp ) {
                tmp = catm.get_size(*((*i)->table));
                label = i;
            }
        }
        
        if (root == NULL) {
            root = new algbric_node(algbric_node::JOIN);
            root->left = *label;
        } else {
            root->right = *label;
            for(auto x : *(s->condition_list)) {
                if (x->flag == true) {
                    if (
                            (rel_set.count(x->left_attr->relation_name) && *(root->right->table) == x->right_attr->relation_name) || 
                            (rel_set.count(x->right_attr->relation_name) && *(root->right->table) == x->left_attr->relation_name) ) {

                        root->conditions.push_back(x);
                    }
                }
            }

            rel_set.insert(*(root->right->left->table));

            auto tmp = new algbric_node(algbric_node::JOIN);
            tmp->left = root;
            root = tmp;
            
            leaf_nodes.erase(label);
        }
    }

    // set root
    auto tmp = new algbric_node(algbric_node::PROJECTION);
    tmp->projection_list = s->projection_list;
    tmp->left = root;

    root = tmp;

    // auto cursor = root->left;
    // for ( auto c : *(s->condition_list) ) {
    //    if (!c->flag) cursor->conditions.push_back(c);
    // }

    // cursor->left = new algbric_node(algbric_node::JOIN);
    // cursor = cursor->left;
    // for ( auto c : *(s->condition_list) ) {
    //    if (c->flag) cursor->conditions.push_back(c);
    // }

    // for (auto i = s->table_list->begin(); i != s->table_list->end(); i++) {
    //     cursor->right = new algbric_node(algbric_node::DIRECT);
    //     cursor->right->table = *i;

    //     cursor->left  = new algbric_node(algbric_node::PROJECTION);
    //     cursor = cursor->left;
    // }

    // {
    //     cursor->op = algbric_node::DIRECT;
    //     cursor->table = cursor->right->table;
    //     delete cursor->right;
    //     cursor->right = NULL;
    // }

    calc_algric_tree(root);
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
            int asdf = IndexManager.selectNode(a, base_addr + t->relation_name + "/index_" + t->attribute_name + ".db", "1", "1.1");
            int b = 0, c = 0;
            while (a.next(b, c) == 0) {
                Record a = RecordManager.getRecord(t->relation_name, b, c);
                auto z = catm.exist_relation(t->relation_name);
                auto x = a.unpack(z->cols);
                record_value y = x[z->get_pos(t->attribute_name)];
                if (calc_conditions(s->condition_list, y))
                    BufferManager.appendTrashCan(b, c);
            }
        } else {
            indexIterator a;
            IndexManager.getStarter(a, base_addr + s->table_name + "/index_" + catm.get_primary(s->table_name) + ".db");
            int b = 0, c = 0;
            while (a.next(b, c) == 0) {
                Record a = RecordManager.getRecord(s->table_name, b, c);
                auto z = catm.exist_relation(s->table_name);
                auto x = a.unpack(z->cols);
                record_value y = x[z->get_pos(s->table_name)];
                if (calc_conditions(s->condition_list, y))
                    BufferManager.appendTrashCan(b, c);
            }
        }
        BufferManager.emptyTrashCan();
    }
    cout << "records deleted." << endl;
}

void xyzsql_process_insert(insert_stmt *s ) {
    if ( s == NULL )
        s = dynamic_cast<insert_stmt *>(stmt_queue.front().second);

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
