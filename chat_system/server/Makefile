chat_server:chat_server.cc chat_server_main.cc
	g++ $^ -o $@ -std=c++11 -lpthread -lboost_filesystem -lboost_system -ljsonhpp

.PHONY:clean
clean
	rm chat_server
