#include "../common/util.hpp"
#include "chat_client.h"
#include "window.h"
#include ""

client::ChatClient* g_client = NULL;
client::Window* g_window = NULL;

//gooooogle 常用的C++开源库
//protobuf：用来序列化
//glog：打日志
//gflag：配置项管理
//grpc：RPC框架
//gtest：单元测试框架

void* Send(void* arg){
    //  a）发送线程：读取输入数据，发送给服务器
    (void)arg;
    while(1){
        std::string msg;
        s_window->DrawInput();
        s_window->GetStrFromWin(g_window->input_win_,&msg);
        g_client->SendMsg(msg);
    }
    return NULL;
}

void* Recv(void* arg){
    //  b）接受线程：从服务器读取数据，显示到界面上
    (void)arg;
    while(1){
        //此处需要绘制两个窗口，因为收到一条数据之后output窗口需要更新，好友列别也可能需要更新
        g_window->DrawOutput();
        g_window->DrawFriendList();
        server::Data data;
        g_client-> RecvMsg(&data);
        if(data.cmd == "quit"){
            //删除的方式和插入的方式能够对应上
            g_window->DelFriend();
        }else{
            g_window->AddFrieng(data.name + "|" + data.school);
            g_window->AddMsg(data.name + ":" +data.msg);
        }
    }
    return NULL;
}

voidQuit(int sig){
    (void) sig;
    //此处的delete主要的目的是为了执行析构函数，尤其是g_window的析构函数。因为这个函数对ncurses进行了销毁动作
    //如果不做这个动作，很可能会导致终端显示混乱
    delete g_window;
    delete g_client;
    exit(0);
}

void Run(const std::string& ip, short port){
    //0.捕捉
    //1.初始化客户端核心模块
    g_client = new client::ChatClient();
    int ret = g_client->Init(ip,port);
    if(ret < 0){
        //此处打印日志的方式模仿了glog
        LOG(ERROR) << "client Init failed!\n";
        return;
    }
    //2.提示用户输入用户名和学校
    std::string name,school;
    std::cout << "请输入用户名："<< std::endl;
    std::cin >> name;
    std::cout << "请输入学校：" << std::endl;
    std::cin >> school;
    g_client->SetUserInfo(name,school);
    //3.初始化用户界面
    g_window = new client::Window();
    g_window->DrawHeader();
    //4.创建两个线程：
    pthread_t stid, rtid;
    //  a）发送线程：读取输入数据，发送给服务器
    pthread_creat(&stid,NULL,Send)
    //  b）接受线程：从服务器读取数据，显示到界面上
    //5.对进程退出时进行处理
}

int main(int argc, char* argv[]){
    if(argc !=3){
        LOG(ERROE) <<  "Usage ./chat_client [ip] [port]\n";
        return 1;
    }
    Run(argv[1],atoi(argv[2]));
    return 0;
}
