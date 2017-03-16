CC = gcc
FLAGS = -std=c99 -Wall
NAME=test
LIBSDIR= ./lib
TXT = result.txt

all:  static shared dynamic
	
static:  $(NAME)_static
 
$(NAME)_static: $(LIBSDIR)/libaddressbook_static.a test.c
	$(CC) $(FLAGS) -static test.c -L$(LIBSDIR) -laddressbook_static -o test_static


shared: $(NAME)_shared

$(NAME)_shared:	$(LIBSDIR)/libaddressbook_shared.so test.c
	$(CC) $(FLAGS) test.c -L$(LIBSDIR) -laddressbook_shared -Wl,-rpath=$(LIBSDIR) -o test_shared

dynamic: $(NAME)_dynamic

$(NAME)_dynamic:
	gcc dynamic.c -ldl -o test_dynamic

run: 	
	printf "\ntest_static" >>result.txt	
	./test_static >> result.txt
	printf "\ntest_shared" >>result.txt
	./test_shared >> result.txt	
	printf "\ntest_dynamic" >>result.txt
	./test_dynamic >> result.txt	
	
