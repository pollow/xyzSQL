#include "evaluator.h"

queue<pair<stmt_type, statement *> > stmt_queue;
extern catalog_manager catm;
extern string base_addr;
extern RecordManager RecordManager;
extern BufferManager BufferManager;
extern IndexManager IndexManager;
extern FILE *yyin;
extern int yyparse();

#define YY_BUF_SIZE 16384
typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_create_buffer ( FILE *file, int size );
extern void yy_switch_to_buffer (YY_BUFFER_STATE new_buffer  );
extern void yy_delete_buffer (YY_BUFFER_STATE b  );

bool verify_validation(vector<record_value> *r, vector<table_column *> *t) {
    auto i = r->begin();
    auto j = t->begin();

    if (r->size() != t->size()) return false;

    for( ; i != r->end(); i++, j++) {
        if ((*i).data_type != (*j)->data_type) return false;
        if ((*i).data_type == table_column::CHARTYPE && strlen((*i).as_str()) >= (*j)->str_len) return false;
    }

    return true;
} 

string create_temp_table(vector<table_column *> *t) {
    uuid_t out;
    char *uuid_str = new char[36];
    uuid_generate(out);
    uuid_unparse(out, uuid_str);
    create_table_stmt *cs = new create_table_stmt(uuid_str, t);


    xyzsql_process_create_table(cs);

    string res(uuid_str);
    delete uuid_str;

    return res;
}

bool calc_conditions(vector<condition *> *conditions, Record &c, Record &d) {
    for(auto x : *conditions) {
        auto col1 = catm.get_column(x->left_attr);
        auto col2 = catm.get_column(x->right_attr);
        if (x->calc( {col2, c.get_value(col1)}, {col2, d.get_value(col2)} ) == false) 
            return false;
    }
    return true;
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
    if (root->flag == true) 
        return;
    string table_name;
    int blockNum, offset;
    auto new_col_list = new vector<table_column *>, old_col_list = new_col_list, old_col_list2 = new_col_list;
    // auto old_col_list = catm.exist_relation((root->left->table))->cols;

    switch ( root->op ) {
        case algbric_node::DIRECT :
            root->flag = true;
            return;
        case algbric_node::PROJECTION : 
        {
            if (!root->left->flag) calc_algric_tree(root->left);
            old_col_list = catm.exist_relation((root->left->table))->cols;

            for( auto x : *(root->projection_list) ) {
                auto att = catm.exist_relation(x->relation_name)->get_column(x->attribute_name);
                new_col_list->push_back(new table_column((root->left->op == algbric_node::DIRECT ? x->attribute_name.c_str() : x->full_name.c_str()), att->data_type, att->str_len, 0 ));
            }
            table_name = create_temp_table(new_col_list);
            root->table = table_name;

            auto cursor = RecordManager.getCursor(root->left->table, catm.calc_record_size(root->left->table));
            while (cursor->next()) {
                Record r = cursor->getRecord();
                vector<record_value> result;
                for(auto i = new_col_list->begin(); i != new_col_list->end(); i++) {
                    for(auto j = old_col_list->begin(); j != old_col_list->end(); j++ ) {
                        if ( (*i)->name == (*j)->name ) {
                            result.push_back(r.values[j-old_col_list->begin()]);
                        }
                    }
                }
                RecordManager.insertRecord(table_name, Record(result, new_col_list), blockNum, offset);
            }

            if (root->left->op != algbric_node::DIRECT) catm.drop_table(root->left->table);

            root->flag = true;
            return;
        }
        case algbric_node::SELECTION : 
        {
            string left_name = root->left->table;
            old_col_list = catm.exist_relation(left_name)->cols;
            for( auto x : *(old_col_list) ) {
                new_col_list->push_back( new table_column(x->name.c_str(), x->data_type, x->str_len, x->flag ));
            }

            table_name = create_temp_table(new_col_list);
            root->table = table_name;

            condition *p = NULL, *eq = NULL;
            for(auto x : (root->conditions)) {
                if ( catm.is_indexed(x->left_attr) ) {
                    p = x;
                    if (x->op == condition::EQUALTO) 
                        eq = x;
                }
            }

            if ( eq != NULL ) p = eq;

            if ( p != NULL ) {
                cout << "Index used: " << p->left_attr->full_name << endl;
                int record_size = catm.calc_record_size(root->left->table);
                auto t = p->left_attr;
                indexIterator cursor;
                int asdf = IndexManager.selectNode(cursor, t->relation_name + "/index_" + t->attribute_name + ".db", 
                        p->op, (p->v).to_str(catm.get_data_type(t)));
                if ( asdf == 0 ) {
                    int b = 0, c = 0;
                    while (cursor.next(b, c) == 0) {
                        Record a = RecordManager.getRecord(t->relation_name, b, c, record_size);
                        if (calc_conditions(&(root->conditions), a))
                            RecordManager.insertRecord(table_name, a, blockNum, offset);
                    }
                }
            } else {
                string t = root->left->table;
                int record_size = catm.calc_record_size(t);
                indexIterator cursor;
                int asdf = IndexManager.getStarter(cursor, t + "/index_" + catm.get_primary(t) + ".db");
                if ( asdf == 0 ) {
                    int b = 0, c = 0;
                    while (cursor.next(b, c) == 0) {
                        Record a = RecordManager.getRecord(t, b, c, record_size);
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
            if (!root->left->flag) calc_algric_tree(root->left);
            if (!root->right->flag) calc_algric_tree(root->right);
            
            old_col_list = catm.exist_relation((root->left->table))->cols;
            old_col_list2= catm.exist_relation((root->right->table))->cols;

            for( auto x : *old_col_list ) {
                new_col_list->push_back(new table_column(x->name.c_str(), x->data_type, x->str_len, 0 ));
            }

            for( auto x : *old_col_list2 ) {
                new_col_list->push_back(new table_column(x->name.c_str(), x->data_type, x->str_len, 0 ));
            }

            table_name = create_temp_table(new_col_list);
            root->table = table_name;

            auto outter_table = catm.exist_relation(root->left->table), inner_table = catm.exist_relation(root->right->table);
            int outter_size = catm.calc_record_size(root->left->table), inner_size = catm.calc_record_size(root->left->table);
            outter_table->get_size();
            condition * p = NULL;

            for ( auto x : root->conditions ) {
                if ( outter_table->get_column(x->left_attr->full_name) != NULL && inner_table->get_column(x->right_attr->full_name) != NULL) {

                } else if ( inner_table->get_column(x->left_attr->full_name) != NULL && outter_table->get_column(x->right_attr->full_name) != NULL) {
                    auto tmp = x->left_attr;
                    x->left_attr = x->right_attr;
                    x->right_attr = tmp;
                }
                assert( outter_table->get_column(x->left_attr->full_name) != NULL && inner_table->get_column(x->right_attr->full_name) != NULL);

                if ( inner_table->is_indexed(x->right_attr->full_name) ) {
                    p = x;
                }

            }

            auto cursor1 = RecordManager.getCursor(root->left->table, outter_size);

            while (cursor1->next()) {
                Record r1 = cursor1->getRecord();
                if ( p ) {
                    // nested-index join
                    cout << "Index used: " << p->left_attr->full_name << endl;
                    indexIterator a;
                    int asdf = IndexManager.getStarter(a, root->right->table + "/index_" + p->right_attr->full_name);
                    if (asdf == 0) {
                        int b = 0, c = 0;
                        while (a.next(b, c) == 0) {
                            Record r2 = RecordManager.getRecord(root->right->table, b, c, inner_size);
                            if ( calc_conditions(&(root->conditions), r1, r2) )  {
                                vector<record_value> result(r1.values);
                                result.insert(result.end(), r2.values.begin(), r2.values.end());
                                RecordManager.insertRecord(table_name, Record(result, new_col_list), blockNum, offset);
                            }
                        }
                    }
                } else {
                    // nested-loop join
                    auto cursor2 = RecordManager.getCursor(root->right->table, inner_size);
                    while (cursor2->next()) {
                        Record r2 = cursor2->getRecord();
                        if ( calc_conditions(&(root->conditions), r1, r2) )  {
                            vector<record_value> result(r1.values);
                            result.insert(result.end(), r2.values.begin(), r2.values.end());
                            RecordManager.insertRecord(table_name, Record(result, new_col_list), blockNum, offset);
                        }
                    }
                    delete cursor2;
                }
            }
            delete cursor1;

            if (root->right->op != algbric_node::DIRECT) catm.drop_table(root->right->table);
            if (root->left->op != algbric_node::DIRECT) catm.drop_table(root->left->table);

            root->flag = true;
            return;
        }
    }
}

void xyzsql_emit_stmt(stmt_type t, statement *stmt) {
    stmt_queue.push({t, stmt});
}

void xyzsql_batch() {
    auto s = dynamic_cast<exefile_stmt *>(stmt_queue.front().second);
    FILE *bat = fopen(s->file_name.c_str(), "r");
    if ( bat == NULL ) 
        throw invalid_argument("Can not open file '" + s->file_name + "'");

    yyin = bat;
    YY_BUFFER_STATE new_buffer = yy_create_buffer( yyin, YY_BUF_SIZE );
    yy_switch_to_buffer(new_buffer);
    yyparse();
    yy_delete_buffer(new_buffer);

    fclose(bat);
}

void xyzsql_finalize() {
    catm.write_back();
    BufferManager.flushQ();
}

void xyzsql_exit() {
    cout << "now exit!" << endl;

    xyzsql_finalize();

    exit(0);
}

void xyzsql_process_create_table(create_table_stmt *s ) {
    cout << "table created." << endl;
    if ( s == NULL ) {
        s = dynamic_cast<create_table_stmt *>(stmt_queue.front().second);
        for ( auto x : *(s->cols) ) {
            x->name = s->name + "." + x->name;
        }
    }

    catm.add_relation(s);
    catm.write_back();
    RecordManager.createMaster(s->name);
    for(auto x : *(s->cols)) {
        if(x->flag & (table_column::unique_attr | table_column::primary_attr)) {
            IndexManager.createIndex(s->name + "/index_" + x->name + ".db", data_type_to_str(x->data_type), x->str_len, 0,
                    {}, {}, {});
        } 
    }
}

void xyzsql_process_create_index() {
    auto s = dynamic_cast<create_index_stmt *>(stmt_queue.front().second);
    int &flag = catm.get_column(s->attr)->flag;
    if ( flag & table_column::unique_attr ) flag |= table_column::index_attr;
    else throw invalid_argument("Index must be created in unique attribute!");
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
            direct->flag = true;
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
            root = *label;
            rel_set.insert(root->table);
        } else {
            auto tmp = new algbric_node(algbric_node::JOIN);
            tmp->left = root;
            tmp->right = *label;
            root = tmp;
            string right_name = root->right->op == algbric_node::DIRECT ? (root->right->table) : (root->right->left->table);

            for(auto x : *(s->condition_list)) {
                if (x->flag == true) {
                    if (
                            (rel_set.count(x->left_attr->relation_name) && right_name == x->right_attr->relation_name) || 
                            (rel_set.count(x->right_attr->relation_name) && right_name == x->left_attr->relation_name) ) {

                        root->conditions.push_back(x);
                    }
                }
            }

            rel_set.insert(right_name);

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

    calc_algric_tree(root);

    auto cursor = RecordManager.getCursor(root->table, catm.calc_record_size(root->table));
    while (cursor->next()) {
        Record t = cursor->getRecord();
        auto j = t.table_info->begin();
        for (record_value x : t.values)  {
            cout << x.to_str((*j)->data_type) << " ";
            j++;
        }
        cout << endl;
    }

    if (root->op != algbric_node::DIRECT) catm.drop_table(root->table);
    delete root;
}

void xyzsql_process_drop_table() {
    auto s = dynamic_cast<drop_table_stmt *>(stmt_queue.front().second);
    catm.drop_table(s->table_name);
    cout << "table dropped." << endl;
}

void xyzsql_process_drop_index() {
    auto s = dynamic_cast<drop_index_stmt *>(stmt_queue.front().second);
    int &flag = catm.get_column(s->attr)->flag;
    flag |= table_column::index_attr;
    flag -= table_column::index_attr;

    cout << "index dropped." << endl;
}

void xyzsql_process_delete() {

    auto s = dynamic_cast<delete_stmt *>(stmt_queue.front().second);

    if (s->condition_list->empty()) {
        // delete all
        system(("rm " + s->table_name + "/*.db").c_str());
        RecordManager.createMaster(s->table_name);
        auto cols = catm.exist_relation(s->table_name)->cols;
        for(auto x : *cols)
            if(x->flag & (table_column::unique_attr | table_column::primary_attr))
                IndexManager.createIndex(s->table_name + "/index_" + x->name + ".db", data_type_to_str(x->data_type), 
                        x->str_len, 0, {}, {}, {});
    } else {
        int record_size = catm.calc_record_size(s->table_name);
        BufferManager.newTrashCan();
        // unique 
        condition *p = NULL, *eq = NULL;
        for(auto x : *(s->condition_list)) {
            if ( catm.is_indexed(x->left_attr) ) {
                p = x;
                if (x->op == condition::EQUALTO) 
                    eq = x;
            }
        }

        if ( eq != NULL || p != NULL ) {
            cout << "Index used: " << p->left_attr->full_name << endl;
            auto t = eq == NULL ? p->left_attr : eq->left_attr;
            if (eq != NULL) p = eq;
            indexIterator a;
            int asdf = IndexManager.selectNode(a, base_addr + "/" + t->relation_name + "/index_" + t->attribute_name + ".db", 
                    p->op, (p->v).to_str(catm.get_data_type(t)));
            if ( asdf == 0 ) {
                int b = 0, c = 0;
                while (a.next(b, c) == 0) {
                    Record a = RecordManager.getRecord(t->relation_name, b, c, record_size);
                    if (calc_conditions(s->condition_list, a))
                        BufferManager.appendTrashCan(b, c);
                }
            }
        } else {
            indexIterator a;
            int asdf = IndexManager.getStarter(a, s->table_name + "/index_" + catm.get_primary(s->table_name) + ".db");
            if (asdf == 0) {
                int b = 0, c = 0;
                while (a.next(b, c) == 0) {
                    Record a = RecordManager.getRecord(s->table_name, b, c, record_size);
                    if (calc_conditions(s->condition_list, a))
                        BufferManager.appendTrashCan(b, c);
                }
            }
        }

        BufferManager.beginFetchTrash();
        int blockNum, offset;
        while(BufferManager.fetchTrash(blockNum, offset)) {
            auto r = RecordManager.getRecord(s->table_name, blockNum, offset, record_size);

            for(auto x : *(r.table_info)) {
                if(x->flag & (table_column::unique_attr | table_column::primary_attr)) {
                    indexIterator cursor;
                    IndexManager.deleteNode(s->table_name + "/index_" + x->name + ".db", r.get_value(x).to_str(x->data_type));
                } 
            }

            RecordManager.deleteRecord(s->table_name, blockNum, offset, record_size);
        }

    }
    cout << "records deleted." << endl;
}

void xyzsql_process_insert(insert_stmt *s ) {
    if ( s == NULL )
        s = dynamic_cast<insert_stmt *>(stmt_queue.front().second);

    auto t = catm.exist_relation(s->table_name)->cols;
    if (verify_validation(s->values, t) == false) 
        throw invalid_argument("Uncapatable values");
    
    auto table_info = catm.exist_relation(s->table_name);
    Record r(*(s->values), table_info->cols);

    for(auto x : *(r.table_info)) {
        if(x->flag & (table_column::unique_attr | table_column::primary_attr)) {
            string filename;
            indexIterator cursor;
            cout << r.get_value(x).to_str(x->data_type) << endl;
            int asdf = IndexManager.selectNode(cursor, s->table_name + "/index_" + x->name + ".db", 
                    condition::EQUALTO, r.get_value(x).to_str(x->data_type));
            if (asdf == 0) throw invalid_argument("Unique Key already exists.");
        } 
    }

    int blockNum, offset;
    RecordManager.insertRecord(s->table_name, r, blockNum, offset);

    for(auto x : *(r.table_info)) {
        if(x->flag & (table_column::unique_attr | table_column::primary_attr)) {
            IndexManager.insertNode(s->table_name + "/index_" + x->name + ".db", 
                    r.get_value(x->name).to_str(x->data_type) , blockNum, offset);
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
