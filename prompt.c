#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <editline.h>
#include "lib/mpc.h"
#include "lval.h"

int main(int argc, char **argv) {
        mpc_parser_t *Number = mpc_new("number");
        mpc_parser_t *Symbol = mpc_new("symbol");
        mpc_parser_t *Sexpr = mpc_new("sexpr");
        mpc_parser_t *Expr = mpc_new("expr");
        mpc_parser_t *Lispy = mpc_new("lispy");

        mpca_lang(MPCA_LANG_DEFAULT,
                  "                                            \
		      number : /-?[0-9]+/ ;                    \
		      symbol : '+' | '-' | '*' | '/' ;         \
		      sexpr  : '(' <expr>* ')' ;               \
		      expr   : <number> | <symbol> | <sexpr> ; \
		      lispy  : /^/ <expr>* /$/ ;               \
		  ",
                  Number, Symbol, Sexpr, Expr, Lispy);
        puts("Lispy Version 0.0.0.0.1");
        puts("Press Ctrl+c to Exit\n");

        while (1) {
                char *input = readline("lispy> ");
                add_history(input);

                mpc_result_t r;
                if (mpc_parse("<stdin>", input, Lispy, &r)) {
                        // mpc_ast_print(r.output);
                        // printf("No you're a %s\n", input);
                        Lval_ptr result = lval_read(r.output);
                        lval_println(result);
                        lval_delete(result);
                        mpc_ast_delete(r.output);
                } else {
                        mpc_err_print(r.error);
                        mpc_err_delete(r.error);
                }

                free(input);
        }

        mpc_cleanup(5, Number, Symbol, Sexpr, Expr, Lispy);
        return 0;
}
