.PHONY:clean
	all:test_win test_client
test_win:window.cc
	g++ $^ -o $@ -lpthread -lncurses -std=c++11

test_client:chat_client.cc
	g++ $^	-o $@ -lpthread -ljsoncpp - lboost_filesystem
