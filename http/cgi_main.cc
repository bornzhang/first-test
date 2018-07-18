//////////////////////////////////////////////////
//字符串转数字
//1.atoi
//2.sscanf
//3.stringstream
//4.boost lexical_cast
//5.stio （c++11）
//
//数字转字符串
//1.itoa
//2.sprintf
//3.stringstream
//4.boost lexical_cast
//5.to_string （c++11）
///////////////////////////////////////////////

#include <iostream>
#include <string>
#include <sstream>
#include "until.hpp"

void HttpResponse(const std::string& body){
    std::cout << "Content-Length:" << body.size() << "\n";
    std::cout << "\n";    //这个\n是http协议中的空行
    std::cout << body;
    return;
}
//这个代码生成的就是我们的CGI程序，通过这个CGI程序完成具体的业务
//本CGI程序仅仅完成简单的两个数的加法运算
int main(int argc,char* argv[], char* env[]){
    size_t i =0;
    for(;env[i] != NULL;++i){
        std::cerr << "env:" << env[i] << "\n";
    }
    //1.先获取到method
    const char* method = getenv("REQUEST_METHOD");
    if(method == NULL){
        HttpResponse("No env REQUEST_METHOD!");
        return 1;
    }
    StringUtil::UrlParam params;
    if(std::string(method) == "GET"){
    //2.如果是GET请求，从QUERY_STRING读取请求参数 
    //4.解析query_string或者body中的数据 
        const char* query_string = getenv("QUERY_STRING");
        StringUtil::ParseUrlParam(query_string,&params);
    }else if(std::string(mothod) == "POST"){
    //3.如果是POST请求，从CONTENT_LENGTH读取body的长度
    //  根据body的长度，从标准输入中读取请求的body
    //4.解析query_string或者body中的数据
    const char* content_length = getenv("CONTENT_LENGTH");
    char buf[1024 *10] = {0};
    read (0.buf,sizeof(buf) - 1);
    StringUtil::ParserlParam(buf,&params);
    //5.根据业务需要进行计算，此处的计算是a+b的值
    int a = std::stoi(params["a"]);
    int b = std::stoi(params["b"]);
    int result = a + b;
    //6.根据计算结果，构造响应的数据，写回到标准输出中
    std::stringstream ss;
    ss << "<hl> result = " << result << "</hl>";
    HttpResponse(ss.str());
    return 0;
}

