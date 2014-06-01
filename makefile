all: oracle

oracle:
	gcc -g magicbot.c socket.c -o oracle

clean:
	rm oracle
