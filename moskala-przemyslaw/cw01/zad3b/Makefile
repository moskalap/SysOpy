CC = gcc
FLAGS = -std=c99 -Wall
NAME=test
LIBSDIR= ./lib
TXT = result.txt

all: static shared test
	
static:  $(NAME)_staticO0 $(NAME)_staticO1 $(NAME)_staticO2 $(NAME)_staticO3 $(NAME)_staticOs $(NAME)_staticOg  
 
$(NAME)_staticO0: $(LIBSDIR)/libaddressbook_static.a test.c
	$(CC) $(FLAGS) -O0 -static test.c -L$(LIBSDIR) -laddressbook_static -o test_static-0

$(NAME)_staticO1: $(LIBSDIR)/libaddressbook_static.a test.c
	$(CC) $(FLAGS) -O1 -static test.c -L$(LIBSDIR) -laddressbook_static -o test_static-1

$(NAME)_staticO2: $(LIBSDIR)/libaddressbook_static.a test.c
	$(CC) $(FLAGS) -O2 -static test.c -L$(LIBSDIR) -laddressbook_static -o test_static-2

$(NAME)_staticO3: $(LIBSDIR)/libaddressbook_static.a test.c
	$(CC) $(FLAGS) -O3 -static test.c -L$(LIBSDIR) -laddressbook_static -o test_static-3

$(NAME)_staticOs: $(LIBSDIR)/libaddressbook_static.a test.c
	$(CC) $(FLAGS) -Os -static test.c -L$(LIBSDIR) -laddressbook_static -o test_static-s

$(NAME)_staticOg: $(LIBSDIR)/libaddressbook_static.a test.c
	$(CC) $(FLAGS) -Og -static test.c -L$(LIBSDIR) -laddressbook_static -o test_static-g

shared: $(NAME)_sharedO0 $(NAME)_sharedO1 $(NAME)_sharedO2 $(NAME)_sharedO3 $(NAME)_sharedOs $(NAME)_sharedOg

$(NAME)_sharedO0:	$(LIBSDIR)/libaddressbook_shared.so test.c
	$(CC) $(FLAGS) -O0 test.c -L$(LIBSDIR) -laddressbook_shared -Wl,-rpath=$(LIBSDIR) -o test_shared-0

$(NAME)_sharedO1:	$(LIBSDIR)/libaddressbook_shared.so test.c
	$(CC) $(FLAGS) -O1 test.c -L$(LIBSDIR) -laddressbook_shared -Wl,-rpath=$(LIBSDIR) -o test_shared-1
$(NAME)_sharedO2:	$(LIBSDIR)/libaddressbook_shared.so test.c
	$(CC) $(FLAGS)  -O2 test.c -L$(LIBSDIR) -laddressbook_shared -Wl,-rpath=$(LIBSDIR) -o test_shared-2

$(NAME)_sharedO3:	$(LIBSDIR)/libaddressbook_shared.so test.c
	$(CC) $(FLAGS) -O3 test.c -L$(LIBSDIR) -laddressbook_shared -Wl,-rpath=$(LIBSDIR) -o test_shared-3

$(NAME)_sharedOs:	$(LIBSDIR)/libaddressbook_shared.so test.c
	$(CC) $(FLAGS) -Os test.c -L$(LIBSDIR) -laddressbook_shared -Wl,-rpath=$(LIBSDIR) -o test_shared-s

$(NAME)_sharedOg:	$(LIBSDIR)/libaddressbook_shared.so test.c
	$(CC) $(FLAGS)  -Og test.c -L$(LIBSDIR) -laddressbook_shared -Wl,-rpath=$(LIBSDIR) -o test_shared-g
test:
	printf "\ntest_static-g" >>result.txt	
	./test_static-g >> result.txt
	printf "\ntest_static-s" >>result.txt
	./test_static-s >> result.txt
	printf "\ntest_static-3" >>result.txt	
	./test_static-2 >> result.txt
	printf "\ntest_static-2" >>result.txt	
	./test_static-0 >> result.txt
	printf "\ntest_static-1" >>result.txt	
	./test_static-1 >> result.txt
	printf "\ntest_static-0" >>result.txt	
	./test_static-0 >> result.txt
	printf "\ntest_shared-g" >>result.txt	
	./test_shared-g >> result.txt
	printf "\ntest_shared-s" >>result.txt
	./test_shared-s >> result.txt
	printf "\ntest_shared-3" >>result.txt	
	./test_shared-2 >> result.txt
	printf "\ntest_shared-2" >>result.txt	
	./test_shared-0 >> result.txt
	printf "\ntest_shared-1" >>result.txt	
	./test_shared-1 >> result.txt
	printf "\ntest_shared-0" >>result.txt	
	./test_shared-0 >> result.txt
	

	





