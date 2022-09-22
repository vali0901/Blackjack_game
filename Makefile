
build: main list utils hashtable_fct database game
	gcc -o main ./bin/*.o 

main: main.c ./include/list_fct.h ./include/utils.h ./src/hashtable_fct.c ./src/game.c ./src/database.c
	gcc -g -o ./bin/main.o -c main.c 

list:	./include/list_fct.h ./include/utils.h ./src/list_fct.c
	gcc -o ./bin/list.o -c ./src/list_fct.c

utils: ./include/utils.h ./src/utils.c
	gcc -o ./bin/utils.o -c ./src/utils.c

hashtable_fct: ./include/utils.h ./src/hashtable_fct.c
	gcc -o ./bin/hashtable_fct.o -c ./src/hashtable_fct.c

database: ./include/list_fct.h ./include/utils.h ./include/hashtable_fct.h ./src/database.c
	gcc -o ./bin/database.o -c ./src/database.c

game: ./include/list_fct.h ./include/utils.h ./src/game.c
	gcc -o ./bin/game.o -c ./src/game.c

clean:
	rm ./bin/*.o
