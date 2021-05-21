#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "lval.h"
#include "lib/mpc.h"

Lval_ptr lval_num(long num) {
        Lval_ptr l;
        l = malloc(sizeof(Lval));
        assert(l != NULL);
        l->type = LVAL_NUM;
        l->num = num;
        return l;
}

Lval_ptr lval_err(char *e) {
        Lval_ptr l;
        l = malloc(sizeof(Lval));
        assert(l != NULL);
        l->type = LVAL_ERR;
        l->err = malloc(strlen(e) + 1);
	strcpy(l->err, e);
        return l;
}

Lval_ptr lval_sym(char *s) {
        Lval_ptr l;
        l = malloc(sizeof(Lval));
        assert(l != NULL);
        l->type = LVAL_SYM;
        l->sym = malloc(strlen(s) + 1);
        strcpy(l->sym, s);
        return l;
}

Lval_ptr lval_sexpr(void) {
        Lval_ptr l;
        l = malloc(sizeof(Lval));
        assert(l != NULL);
        l->type = LVAL_SEXPR;
        l->count = 0;
        l->cell = NULL;
        return l;
}

Lval_ptr eval_op(Lval_ptr l, char *op, Lval_ptr r) {
        if (l->type == LVAL_ERR) {
                return l;
        }
        if (r->type == LVAL_ERR) {
                return r;
        }

        if (strcmp(op, "+") == 0) {
                return lval_num(l->num + r->num);
        }
        if (strcmp(op, "-") == 0) {
                return lval_num(l->num - r->num);
        }
        if (strcmp(op, "*") == 0) {
                return lval_num(l->num * r->num);
        }
        if (strcmp(op, "/") == 0) {
                return r->num == 0 ? lval_err(ERR(LERR_DIV_ZERO))
                                   : lval_num(l->num / r->num);
        }

        return lval_err(ERR(LERR_BAD_OP));
}

Lval_ptr eval(mpc_ast_t *t) {
        if (strstr(t->tag, "number")) {
                errno = 0;
                long x = strtol(t->contents, NULL, 10);
                return errno != ERANGE ? lval_num(x) : lval_err(ERR(LERR_BAD_NUM));
        }

        char *op = t->children[1]->contents;
        Lval_ptr l;
        // l = malloc(sizeof(Lval));
        // assert(l != NULL);
        l = eval(t->children[2]);

        int i = 3;
        while (strstr(t->children[i]->tag, "expr")) {
                l = eval_op(l, op, eval(t->children[i]));
                i++;
        }
        return l;
}

void lval_delete(Lval_ptr l) {
        switch (l->type) {
        case LVAL_NUM:
                break;
        case LVAL_ERR:
                free(l->err);
                break;
        case LVAL_SYM:
                free(l->sym);
                break;
        case LVAL_SEXPR:
                for (int i = 0; i < l->count; i++) {
                        lval_delete(l->cell[i]);
                }
                free(l->cell);
        }
        free(l);
}

void lval_expr_print(Lval_ptr l, char *open, char *close) {
        printf("%s", open);
        for (int i = 0; i < l->count; i++) {
                lval_print(l->cell[i]);
                if (i != (l->count - 1)) {
                        printf(" ");
                }
        }
        printf("%s", close);
}

void lval_print(Lval_ptr l) {
        switch (l->type) {
        case LVAL_NUM:
                printf("%li", l->num);
                break;
        case LVAL_ERR:
                printf("Error: %s", l->err);
                break;
        case LVAL_SYM:
                printf("%s", l->sym);
                break;
        case LVAL_SEXPR:
                lval_expr_print(l, "(", ")");
                break;
        }
}

void lval_println(Lval_ptr l) {
        lval_print(l);
        printf("\n");
}

Lval_ptr lval_read_num(mpc_ast_t *t) {
        errno = 0;
        long x = strtol(t->contents, NULL, 10);
        return errno != ERANGE ? lval_num(x) : lval_err(ERR(LERR_BAD_NUMBER));
}

Lval_ptr lval_read(mpc_ast_t *t) {
        if (strstr(t->tag, "number")) {
                return lval_read_num(t);
        }
        if (strstr(t->tag, "symbol")) {
                return lval_sym(t->contents);
        }

        Lval_ptr l = NULL;
        if (strcmp(t->tag, ">") == 0) {
                l = lval_sexpr();
        }
        if (strstr(t->tag, "sexpr")) {
                l = lval_sexpr();
        }

        for (int i = 0; i < t->children_num; i++) {
                if (strcmp(t->children[i]->contents, "(") == 0) {
                        continue;
                }
                if (strcmp(t->children[i]->contents, ")") == 0) {
                        continue;
                }
                if (strcmp(t->children[i]->tag, "regex") == 0) {
                        continue;
                }
                l = lval_add(l, lval_read(t->children[i]));
        }

        return l;
}

Lval_ptr lval_add(Lval_ptr l, Lval_ptr new) {
        l->count++;
        l->cell = realloc(l->cell, sizeof(Lval_ptr) * l->count);
        l->cell[l->count - 1] = new;
        return l;
}
