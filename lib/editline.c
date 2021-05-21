#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readline.h"

#define BUF_SIZE 2048

char *readline(const char *prompt) {
	fputs(prompt, stdout);
	fgets(buffer, BUF_SIZE, stdin);
	char *cpy = malloc(strlen(buffer) + 1);
	strcpy(cpy, buffer);
	cpy[strlen(cpy)-1] = '\0';
	return cpy;
}

void add_history(const char *line) {}
