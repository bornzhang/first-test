#pragma once

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../server/api.hpp"

namespace slient{

class ChatClient{
public:
    //客户端初始化,主要是进行socket的初始化
    int Init(const std::string& ip,short port);
    //设置用户信息，姓名和学校
    //让客户端启动时提示用户输入姓名学习
    //从标准
    int SetUserInfo(const std::string& name, const std::string& school);
    //发送消息
    void SendMsg();
    //接收消息
    void RecvMsg();

private:
    int sock_;
    sockaddr_in server_addr_;

    std::string name;
    std::string school;
};

} //end client
