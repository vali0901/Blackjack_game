
build: main.o list.o utils.o
	gcc -o main ./bin/*.o 

main.o: main.c
	gcc -g -o ./bin/main.o -c main.c 

list.o:	./include/list_fct.h ./include/utils.h ./src/list_fct.c
	gcc -o ./bin/list.o -c ./src/list_fct.c

utils.o: ./include/utils.h ./src/utils.c
	gcc -o ./bin/utils.o -c ./src/utils.c
clean: *.o
	rm ./bin/*.o
