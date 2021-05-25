#ifndef __LVAL_H_
#define __LVAL_H_
#include "lib/mpc.h"

#define ERR(x) #x

// lval struct
typedef struct lval {
        int type;
        long num;

        char *err;
        char *sym;

        int count;
        struct lval **cell;
} Lval;

typedef Lval *Lval_ptr;

// lval types
enum {
        LVAL_ERR,
        LVAL_NUM,
        LVAL_SYM,
        LVAL_SEXPR,
};

// error types
enum {
        LERR_DIV_ZERO,
        LERR_BAD_OP,
        LERR_BAD_NUM,
        LERR_BAD_NUMBER,
};

Lval_ptr lval_num(long num);
Lval_ptr lval_err(char *e);
Lval_ptr lval_sym(char *s);
Lval_ptr lval_sexpr(void);
Lval_ptr lval_take(Lval_ptr l, int i);
Lval_ptr lval_pop(Lval_ptr l, int i);

Lval_ptr lval_read_num(mpc_ast_t *t);
Lval_ptr lval_add(Lval_ptr l, Lval_ptr x);
Lval_ptr lval_read(mpc_ast_t *t);
Lval_ptr eval_sexpr(Lval_ptr l);
Lval_ptr eval_op(Lval_ptr l, char *op, Lval_ptr r);
Lval_ptr eval(mpc_ast_t *t);

Lval_ptr builtin_op(Lval_ptr l, char *op);

void lval_delete(Lval_ptr l);
void lval_expr_print(Lval_ptr l, char *open, char *close);
void lval_print(Lval_ptr l);
void lval_println(Lval_ptr l);

#endif /* __LVAL_H_ */
