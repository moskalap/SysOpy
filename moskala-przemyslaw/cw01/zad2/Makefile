CMP=gcc -Wall -std=c99

all:  teststatic.out testdynamic.out clean export run




teststatic.out: main.c libaddressbook_static.a
	$(CMP) -o $@ $^
testdynamic.out: main.c 
	$(CMP) -o dynamic.out -L./ main.c -laddressbook_shared
clean:
	 rm -f *.o
export: 
	export LD_LIBRARY_PATH=./addressbook_shared:$LD_LIBRARY_PATH
run:
	./teststatic.out
