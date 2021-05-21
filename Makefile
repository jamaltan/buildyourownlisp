SOURCE := prompt.c lval.c lib/mpc.c
TARGET := prompt

CC 	:= gcc
LDFLAGS := -leditline
CFLAGS  := -std=c99

default :
	$(obj)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCE) -o $(TARGET)

.PHONY : clean
clean :
	-rm prompt
