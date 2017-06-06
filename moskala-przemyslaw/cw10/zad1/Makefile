CMP=gcc -w -std=c99

all: server client
server:
	$(CMP) server.c -lpthread -o server.out
client: 
	$(CMP) client.c -o client.out
clean:
	rm *.out 

