all: client

client : main.o client.o
	gcc main.o client.o -o client

main.o : main.c client.h
	gcc -c main.c

client.o : client.c client.h
	gcc -c client.c

clean:
	rm *.o client