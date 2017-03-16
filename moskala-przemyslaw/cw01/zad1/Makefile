CMP=gcc -Wall -std=c99

all: libaddressbook_static.a libaddressbook_shared.so clean

libaddressbook_static.a: libaddressbook_static.o
	ar rc $@ $^
libaddressbook_static.o: addressbook.c addressbook.h
	$(CMP) -I. -c -o $@ $<
libaddressbook_shared.so: libaddressbook_shared.o
	$(CMP) -shared -o $@ $<
libaddressbook_shared.o: addressbook.c addressbook.h
	$(CMP) -fPIC -I. -c -o  $@ $<

clean:
	 rm -f *.o
