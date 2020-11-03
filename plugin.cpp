#include "plugin.h"


void display_monitored_mpi_functions(){
    int i;
    const char* ptr;
    for (i = 0; MPICOLLECTIVES.iterate (i, &ptr); i++){
        printf("%s\n", ptr);
    }
}
void pragma_set_functions_handle_ending_errors(tree x, enum cpp_ttype token, bool close_paren_needed){
    // Check that there is a closing parenthesis
    if (token == CPP_EOF && close_paren_needed){
        fprintf(stderr, "Error: `#pragma ProjetCA mpicoll_check (string[, string]...)` is missing a closing parenthesis\n");
    }
    if (token == CPP_CLOSE_PAREN){
        if(!close_paren_needed){
            // Check that there isn't an unnecessary closing parenthesis
            fprintf(stderr, "Error: `#pragma ProjetCA mpicoll_check string` has an unnecessary closing parenthesis\n");
        }
        else{
            // Check that there is nothing after the closing parenthesis
            token = pragma_lex (&x);
            if(token != CPP_EOF){
                fprintf(stderr, "Error: `#pragma ProjetCA mpicoll_check (string[, string]...)` has extra stuff after the closing parenthesis\n");
            }
        }
    }
}
/* Handle ProjetCA mpicoll_check pragma */
static void handle_pragma_set_functions(cpp_reader *ARG_UNUSED(dummy)){
    if (cfun)
    {
        throw std::logic_error( "#pragma instrument function is not allowed inside functions\n" );
    }

    enum cpp_ttype token;
    tree x;
    bool close_paren_needed = false;
    token = pragma_lex (&x);

    if (token == CPP_OPEN_PAREN)
    {
        close_paren_needed = true;
        token = pragma_lex (&x);
    }
    while (token != CPP_EOF && token != CPP_CLOSE_PAREN){
        if (token == CPP_NAME){
            const char *op = IDENTIFIER_POINTER(x);
            if(MPICOLLECTIVES.contains(op)){
                fprintf(stderr, "Warning: %s is already monitored.\n", op);
            }
            else{
                MPICOLLECTIVES.safe_push(op);
            }
            if(! close_paren_needed){
                token = pragma_lex (&x);
                if(token != CPP_EOF){
                    fprintf(stderr, "Error: `#pragma ProjetCA mpicoll_check string` has extra stuff after the function\n");
                }
                break;
            }
        }
        token = pragma_lex (&x);
    }
    pragma_set_functions_handle_ending_errors(x, token, close_paren_needed);
}


int
plugin_init(struct plugin_name_args *plugin_info,
            struct plugin_gcc_version *version) {
    c_register_pragma("ProjetCA", "mpicoll_check", handle_pragma_set_functions);
    return 0;
}
