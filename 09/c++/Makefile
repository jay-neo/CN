CXX = g++
CXXFLAGS = -Wall

all: server client

server: server.cpp
	@$(CXX) $(CXXFLAGS) -o server server.cpp

client: client.cpp
	@$(CXX) -o client client.cpp

run:
	@./server & ./client

clean:
	@pkill server & rm -f server client
