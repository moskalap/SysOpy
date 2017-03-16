CC = gcc
FLAGS = -std=c99 -Wall
NAME=test
LIBSDIR= ./lib
TXT = result.txt

all: static shared test
	
static:  $(NAME)_core
 
$(NAME)_core: $(LIBSDIR)/libaddressbook_static.a test.c
	$(CC) $(FLAGS) -g -static test.c -L$(LIBSDIR) -laddressbook_static -o test_core

	

	





