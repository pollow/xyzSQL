#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <cctype>

#include "../parser/stmt.h"
#include "../parser.tab.h"

typedef struct yy_buffer_state *YY_BUFFER_STATE;

extern YY_BUFFER_STATE yy_scan_string (const char *yy_str  );
extern void yy_switch_to_buffer (YY_BUFFER_STATE new_buffer  );
extern void yy_delete_buffer (YY_BUFFER_STATE b  );
extern int yyparse ();
 
using namespace std;

void system_init() { 
    cout << "System Initialized!" << endl;
}

int main() {

    system_init();

    while(true) {
        char * line = readline(">>> ");
        add_history(line);

        int len = strlen(line);
        for (int i = 0; i < len; i++) line[i] = toupper(line[i]);
        cout << "What you typed: " << line << endl;
        char *tmp = new char[len + 2];
        strcpy(tmp, line);
        tmp[len + 1] = 0;

        YY_BUFFER_STATE my_string_buffer = yy_scan_string(tmp);
        yy_switch_to_buffer( my_string_buffer );
        yyparse();
        yy_delete_buffer( my_string_buffer );
    }

    return 0;

}

