#include "plugin.h"


/* Get the current MPI function from a string, or -1 if the function is not an MPI function */
int get_str_mpi_function(const char* name){
    for ( int mpi_function = 0; mpi_function < LAST_AND_UNUSED_MPI_COLLECTIVE_CODE; ++mpi_function)
    {
        if (strcmp(name,mpi_collective_name[mpi_function]) == 0) {
            return mpi_function;
        }
    }
    return -1;
}
/* Get the MPI function from a stmt, or -1 if the function is not an MPI function */
int get_stmt_mpi_function(gimple * stmt){
    tree current_fn_decl = gimple_call_fndecl(stmt);
    const char* name = get_name(current_fn_decl);
    return get_str_mpi_function(name);
}


void display_monitored_mpi_functions(){
    for ( int mpi_function = 0; mpi_function < LAST_AND_UNUSED_MPI_COLLECTIVE_CODE; ++mpi_function)
    {
        printf("%s: ", mpi_collective_name[mpi_function]);

        if (MONITORED_MPI_COLLECTIVES[mpi_function]) {
            printf("monitored");
        }
        else {
            printf("not monitored");
        }

        printf(".\n");
    }
}
void pragma_set_functions_handle_running_errors(tree x, bool close_paren_needed){
    enum cpp_ttype token;
    if(! close_paren_needed){
        token = pragma_lex (&x);
        if(token != CPP_EOF){
            printf("Error: `#pragma ProjetCA mpicoll_check string` has extra stuff after the function\n");
        }
        break;
    }
}
void pragma_set_functions_handle_ending_errors(tree x, bool close_paren_needed){
    enum cpp_ttype token;
    // Check that there is a closing parenthesis
    if (token == CPP_EOF && close_paren_needed){
        printf("Error: `#pragma ProjetCA mpicoll_check (string[, string]...)` is missing a closing parenthesis\n");
    }
    if (token == CPP_CLOSE_PAREN){
        if(!close_paren_needed){
            // Check that there isn't an unnecessary closing parenthesis
            printf("Error: `#pragma ProjetCA mpicoll_check string` has an unnecessary closing parenthesis\n");
        }
        else{
            // Check that there is nothing after the closing parenthesis
            token = pragma_lex (&x);
            if(token != CPP_EOF){
                printf("Error: `#pragma ProjetCA mpicoll_check (string[, string]...)` has extra stuff after the closing parenthesis\n");
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
            int mpi_function_id = get_str_mpi_function(op);
            if(mpi_function_id != -1){
                if(MONITORED_MPI_COLLECTIVES[mpi_function_id]){
                    printf("Warning: %s is already monitored.\n", op);
                }
                else{
                    MONITORED_MPI_COLLECTIVES[mpi_function_id] = 1;
                    printf("added %s to analysed MPI functions.\n", op);
                }
            }
            else{
                printf("Warning: %s is not an MPI function.\n", op);
            }
            pragma_set_functions_handle_running_errors(x, close_paren_needed)
        }
        token = pragma_lex (&x);
    }
    pragma_set_functions_handle_ending_errors(x, close_paren_needed)
}


int
plugin_init(struct plugin_name_args *plugin_info,
            struct plugin_gcc_version *version) {
    printf("plugin init\n");
    c_register_pragma("ProjetCA", "mpicoll_check", handle_pragma_set_functions);
    return 0;
}
