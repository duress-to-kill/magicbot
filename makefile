depend := libjsmn.a

all: oracle

oracle:
	gcc -g magicbot.c socket.c $(depend) -o oracle

clean:
	rm oracle

server:
	echo 'Run: nc -klv localhost 6667; ./oracle localhost 6667'
