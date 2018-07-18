#include <iostream>
#include <api.hpp>

    pthread_create(&consumer,NULL,Consumse);
    pthread_join(productor,NULL);
    pthread_jion(consumer,NULL);
    return 0;
}

//线程入口函数
void* ChatServer::Product(void* arg){
    //生产者线程，主要做的实情读取socket中的数据，并且写入到BlockQueue
    ChatServer* server = reinterpret_cast<ChatServer*>(arg);
    while(true){
        //RecvMsg读取一次数据，并且写入到BlockQueue
        server->RecvMsg();
    }
    return NULL;
}

void* ChatServer::Consume(void* arg){
    ChatServer* server = reinterpret_cast<ChatServer*>(arg);
    while(true){
        //RecvMsg读取一次数据，并且写入到BlockQueue
         server->BroadCast();    
    }    
    return NULL;
}

int ChatServer::RecvMsg(){
    //1.从socket中读取数据
    char buf[1024*5] = {0};
    sockaddr_in peer;
    socklen_t len = sizeof(peer);
    ssize_t read_size = recvfrom(sock_,buf,sizeof(buf),-1,0,(sockaddr*)&peer,&len);
    if(read_size < 0){
        peeror("recvfrom");
        return -1;
    }
    buf[read_size] = '\0';
    //2.把数据插入到BlockQueue中
    Context context;
    context.str =buf;
    context.addr = peer
    queue_.PushBack(context);
    return 0;
}

int ChatServer::BroadCast(){
    //1.从队列中读取一个元素
    Context context;
    queue_.PopFront(&context);
    //2.对取出的字符串数据进行反序列化
    Data data;
    date.UnSerialize(context.str);
    //3.根据这个消息来更新在线好友列表
    if(data.cmd == "quit"){
        //  b）如果这个成员当前是一个下线消息（command是一个特殊的值） 把这样的一个成员从好友列表中删除；
        DelUser(context.addr,data.name);
    }else{
        //  a｝如果这个成员当前不在好友列表中，加入进来
        //  准备使用[]方法来操作 在线好友列表
        AddUser(context.addr,data.name);
    }
    //4.遍历在线好友列表，把这个数据一次发送给每一个客户端
    //（由于发送消息的用户也存在好友列表中，因此在遍历列表时也会给自己发消息，从而达到发送者能够看到自己发送的消息的效果，
    //  但是这种实现方式比较蠢，完全可以控制客户端，不经过网络传输就实现这个功能）
    for(auto item : online_friend_list_){
        SendMsg(context.str,item.second);
    }
    return 0;
}

void ChatServer::AddUsr(sockaddr_in addr,const std::string& name){
    //这里的key相当于对ip地址和用户名进行拼接
    //之所以使用name和ip地址共同进行拼接，本质上是因为仅仅使用ip可能会出现重复ip的情况
    //（如果N个客户端处在同一个局域网中，那么服务器端看到的ip地址就是相同的。因为NAT）
    ste::stringstream ss;
    ss << name << ":" << addr.sin_addr.s_addr;
    //[] map unordered_map
    //1.如果不存在，就插入
    //2.如果存在，就修改
    //ValueType& operator[](const KeyType& key)
    //const ValueType& operator[](const KeyType& key) const
    //
    //insert 返回值类型：迭代器，插入成功后的位置；bool成功与失败
    online_friend_list_[ss.str()] = value;
}

Void ChatServer::DelUser(sockaddr_in addr,const std::string& name){
    std::stringstream ss;
    ss << name << ":" << addr.sin_addr.s_addr;
    online_friend_list_erase(aa.str());
    return;
}

Void ChatServer::SendMsg(const std::string& data, sockaddr_in addr){
    //把这个数据通过sendto发送给客户端
    sendto(sock_,data.data(),data.size(),0,(sockaddr*)&addr,sizeof(addr));
    LOG(INFO) << "[Response]" << inet_ntoa(addr.sin_addr) << ":" << nthos(addr.)
}


} //end server
