CMP=gcc 

all: master slave
master:
	$(CMP) master.c -o master
slave:
	$(CMP) slave.c -lm -o slave
clean:
	rm master; rm slave

