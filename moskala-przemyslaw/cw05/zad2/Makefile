CMP=gcc 

all: master slave
master:
	$(CMP) ./src/master.c -o master
slave:
	$(CMP) ./src/slave.c -lm -o slave
clean:
	rm master; rm slave

