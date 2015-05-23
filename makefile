static_lib_list := libjsmn.a
static_libs := -L. $(static_lib_list)

debug := -Wall -g

all:
	gcc $(debug) $(static_libs) socket.c magicbot.c json_manager.c $(static_lib_list) -o oracle

oracle:
	gcc $(debug) socket.c magicbot.c $(lib_dependencies) -o oracle

json:
	gcc $(debug) $(static_libs) json_manager.c $(static_lib_list) -o json_manager

clean:
	rm oracle
	rm json_manager

server:
	echo 'Run: nc -klv localhost 6667; ./oracle localhost 6667'
