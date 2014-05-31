all: client

client:
	gcc -g client.c -o client

clean:
	rm client
