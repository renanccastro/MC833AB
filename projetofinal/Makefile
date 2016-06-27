all : 
	g++ -o client client.cpp models.cpp utils.cpp -lncurses -ggdb -ljansson
	g++ -o server server.cpp models.cpp utils.cpp -ljansson -ggdb

# client.o : client.cpp
	# gcc -c client.c
# server.o : server.cpp
	# gcc -c server.c
clean :
	rm client server client.o server.o
