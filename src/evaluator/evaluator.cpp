#include "evaluator.h"

queue<pair<stmt_type, statement *> > stmt_queue;
extern catalog_manager catm;
extern string base_addr;
extern RecordManager RecordManager;
extern BufferManager BufferManager;
extern IndexManager IndexManager;


string create_temp_table(vector<table_column *> *t) {
    uuid_t out;
    char *uuid_str;
    uuid_generate(out);
    // uuid_unparse(out, uuid_str);
    create_table_stmt *cs = new create_table_stmt(uuid_str, t);

    xyzsql_process_create_table(cs);

    return string(uuid_str);
}

bool calc_conditions(vector<condition *> *conditions, Record &c) {
    for(auto x : *conditions) {
        auto col = catm.get_column(x->left_attr);
        if (x->calc( {col, c.get_value(col)} ) == false) 
            return false;
    }
    return true;
}

void calc_algric_tree(algbric_node *root) {
    string table_name;
    int b = 0, c = 0;
    int blockNum, offset;
    auto new_col_list = new vector<table_column *>, old_col_list = new_col_list;
    // auto old_col_list = catm.exist_relation((root->left->table))->cols;

    switch ( root->op ) {
        case algbric_node::DIRECT :
            root->flag = true;
            return;
        case algbric_node::PROJECTION : 
        {
            old_col_list = catm.exist_relation((root->left->table))->cols;

            for( auto x : *(root->projection_list) ) {
                auto att = catm.exist_relation(x->relation_name)->get_column(x->relation_name);
                new_col_list->push_back(new table_column(x->full_name.c_str(), att->data_type, att->str_len, 0 ));
            }
            table_name = create_temp_table(new_col_list);

            indexIterator cursor;
            IndexManager.getStarter(cursor, base_addr + (root->left->table) + "/index_" + catm.get_primary(table_name) + ".db");
            while (cursor.next(b, c) == 0) {
                Record a = RecordManager.getRecord(table_name, b, c, 0);
                vector<record_value> result;
                for(auto i = new_col_list->begin(); i != new_col_list->end(); i++) {
                    for(auto j = old_col_list->begin(); j != old_col_list->end(); j++ ) {
                        if ( (*i)->name == (*j)->name ) {
                            result.push_back(a.values[j-old_col_list->begin()]);
                        }
                    }
                }
                RecordManager.insertRecord(table_name, Record(result, new_col_list), blockNum, offset);
            }
            return;
        }
        case algbric_node::SELECTION : 
        {
            old_col_list = catm.exist_relation((root->left->table))->cols;

            table_name = create_temp_table(catm.exist_relation(root->left->table)->cols);
            root->table = table_name;

            condition *p = NULL, *eq = NULL;
            for(auto x : (root->conditions)) {
                if ( catm.is_unique(x->left_attr) ) {
                    p = x;
                    if (x->op == condition::EQUALTO) 
                        eq = x;
                }
            }

            if ( eq != NULL ) p = eq;

            if ( p != NULL ) {
                auto t = p->left_attr;
                indexIterator cursor;
                int asdf = IndexManager.selectNode(cursor, base_addr + t->relation_name + "/index_" + t->attribute_name + ".db", 
                        p->op, (p->v).to_str(catm.get_data_type(t)));
                if ( asdf == 1 ) {
                    int b = 0, c = 0;
                    while (cursor.next(b, c) == 0) {
                        Record a = RecordManager.getRecord(t->relation_name, b, c, 0);
                        if (calc_conditions(&(root->conditions), a))
                            RecordManager.insertRecord(table_name, a, blockNum, offset);
                    }
                }
            } else {
                auto t = p->left_attr;
                indexIterator cursor;
                int asdf = IndexManager.getStarter(cursor, base_addr + t->relation_name + "/index_" + catm.get_primary(table_name) + ".db");
                if ( asdf == 1 ) {
                    int b = 0, c = 0;
                    while (cursor.next(b, c) == 0) {
                        Record a = RecordManager.getRecord(t->relation_name, b, c, 0);
                        if (calc_conditions(&(root->conditions), a))
                            RecordManager.insertRecord(table_name, a, blockNum, offset);
                    }
                }
            }

            root->flag = true;
            return;
        }
        case algbric_node::JOIN :
        {

        }
    }
}

void xyzsql_emit_stmt(stmt_type t, statement *stmt) {
    stmt_queue.push({t, stmt});
}

void xyzsql_batch() {
    cout << "batch!" << endl;
}

void xyzsql_finalize() {

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
        for(auto x : *(s->cols)) {
            if(x->flag & (table_column::unique_attr | table_column::primary_attr)) {
                IndexManager.createIndex(s->name + "/index_" + x->name + ".db", data_type_to_str(x->data_type), x->str_len, 0,
                        {}, {}, {});
            } 
        }
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
        direct->table = *x;
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
        int tmp = 0xFFFFFF;
        auto label = leaf_nodes.begin(); 
        for(auto i = leaf_nodes.begin(); i != leaf_nodes.end(); i++) {
            if ( catm.get_size(((*i)->table)) < tmp ) {
                tmp = catm.get_size(((*i)->table));
                label = i;
            }
        }

        if (root == NULL) {
            // root = new algbric_node(algbric_node::JOIN);
            root = *label;
        } else {
            auto tmp = new algbric_node(algbric_node::JOIN);
            tmp->left = root;
            tmp->right = *label;
            root = tmp;
            for(auto x : *(s->condition_list)) {
                if (x->flag == true) {
                    if (
                            (rel_set.count(x->left_attr->relation_name) && (root->right->table) == x->right_attr->relation_name) || 
                            (rel_set.count(x->right_attr->relation_name) && (root->right->table) == x->left_attr->relation_name) ) {

                        root->conditions.push_back(x);
                    }
                }
            }

            rel_set.insert(root->right->op == algbric_node::DIRECT ? (root->right->table) : (root->right->left->table));

            //tmp = new algbric_node(algbric_node::JOIN);
            //tmp->left = root;
            //root = tmp;

        }
        leaf_nodes.erase(label);
    }

    // set root
    if (!s->projection_list->empty()) {
        auto tmp = new algbric_node(algbric_node::PROJECTION);
        tmp->projection_list = s->projection_list;
        tmp->left = root;

        root = tmp;
    }

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

    indexIterator cursor;
    IndexManager.getStarter(cursor, root->table + "/index_" + catm.get_primary(root->table) + ".db");

    int blockNum, offset;
    while (cursor.next(blockNum, offset) != -1) {
        Record t = RecordManager.getRecord(root->table, blockNum, offset, catm.calc_record_size(root->table));
        auto j = t.table_info->begin();
        for (record_value x : t.values)  {
            cout << x.to_str((*j)->data_type) << " ";
            j++;
        }
        cout << endl;
    }
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
            if (eq != NULL) p = eq;
            indexIterator a;
            int asdf = IndexManager.selectNode(a, base_addr + t->relation_name + "/index_" + t->attribute_name + ".db", 
                    p->op, (p->v).to_str(catm.get_data_type(t)));
            if ( asdf == 1 ) {
                int b = 0, c = 0;
                while (a.next(b, c) == 0) {
                    Record a = RecordManager.getRecord(t->relation_name, b, c, 0);
                    if (calc_conditions(s->condition_list, a))
                        BufferManager.appendTrashCan(b, c);
                }
            }
        } else {
            indexIterator a;
            int asdf = IndexManager.getStarter(a, base_addr + s->table_name + "/index_" + catm.get_primary(s->table_name) + ".db");
            if (asdf == 1) {
                int b = 0, c = 0;
                while (a.next(b, c) == 0) {
                    Record a = RecordManager.getRecord(s->table_name, b, c, 0);
                    if (calc_conditions(s->condition_list, a))
                        BufferManager.appendTrashCan(b, c);
                }
            }
        }
        BufferManager.emptyTrashCan();
    }
    cout << "records deleted." << endl;
}

void xyzsql_process_insert(insert_stmt *s ) {
    if ( s == NULL )
        s = dynamic_cast<insert_stmt *>(stmt_queue.front().second);

    int blockNum, offset;
    Record r(*(s->values), catm.exist_relation(s->table_name)->cols);
    RecordManager.insertRecord(s->table_name, r, blockNum, offset);

    for(auto x : *(r.table_info)) {
        if(x->flag & (table_column::unique_attr | table_column::primary_attr)) {
            IndexManager.insertNode(s->table_name + "/index_" + x->name + ".db", 
                    r.get_value(catm.get_primary(s->table_name)).to_str(table_column::INTTYPE) , blockNum, offset);
        } 
    }

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
