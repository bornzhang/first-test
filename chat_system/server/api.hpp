#pragma once
#include <string>
#include <jsoncpp/json/json.h>

namespace server{

//此处的Data相当于服务器给客户端提供的API
struct Data{
    std::string name;
    std::string school;
    std::string msg;
    std::string cmd;

    //反序列化
    void UnSerialize(std::string* output){
        //char buf[1024] ={0};
        //sprintf("{name:%s,school:%s,msg:%s,cmd:%s}",name.c_str(),school.c_str(),msg.c_str(),cmd.c_str);
        //*output = buf;
        //可以把Json::Value近似理解成一个unordered_map
        Josn::Value value;
        value["name"] = name;
        value["school"] = school;
        value["msg"] = msg;
        value["cmd"] =cmd;
        Json::FastWriter writer;
        *output = writer.write(value);
        return;
    }
//序列化
    void Serialize(const std::string& input){
        Json::Value value;
        Json::Reader reader;
        reader.parse(input,value);
        //为了让代码更严谨，可以使用isString/isInt来判断Json对象中存储的数据类型事都符合要求
        name = value["name"].asString();
        school = value["school"].asString();
        msg = value["msg"].asString();
        cmd = value["cmd"].asString();
        return;
    }
    
};
} // end server
