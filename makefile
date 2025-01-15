CC = g++
FLAGS = -Wall -g
LIBS = -lws2_32 -static 

remote :
	$(CC) remoteserver.cpp -o server $(FLAGS) $(LIBS)
	$(CC) remoteclient.cpp -o client $(FLAGS) $(LIBS)
