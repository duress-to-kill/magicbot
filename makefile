all: oracle

oracle:
	gcc -g magicbot.c -o oracle

clean:
	rm oracle
