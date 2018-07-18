//////////////////////////////////////////////////////////
//此文件放置一些工具类和函数
//为了让这些工具用的方便，直接把声明和实现都放在.hpp中
/////////////////////////////////////////////////////////
#pragma once
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <vector>
#include <sys/time.h>
#include <sys/socket.h>

class TimeUtil{
public:
    //获取到当前的秒级时间戳
    static int64_t TimeStamp(){     
        struct timeval tv;
        gettimeofday(&tv,NULL);
        return tv.tv_sec;
    }
    //获取当前的微秒级时间戳
    static int64_t TimeStampUS(){
        struct timeval tv;
        gettimeofday(&tv,NULL);
        //123456
        return 1000*1000*tv.tv_sec +tv.tv_usec;
    }
};

enum LogLevel{
    DEBUG,
    INFO,     //一般的信息
    WARNING,   //警告
    ERROR,     //一般错误
    CRITIAL,    //致命错误
};

//LOG(INFO) <<"hehe"
//[I3124343 util.hpp:34] hehe
inline std::ostream& Log(LogLevel level,const char* file,int line){
    std::string prefix ="I";
    if(level == WARNING){
        prefix ="W";
    }else if(level == ERROR){
        prefix ="E";
    }else if(level == CRITIAL){
        prefix = "C";
    }else if(level == DEBUG){
        prefix = "D";
    }
    std::cout <<"[" << prefix << TimeUtil::TimeStamp() << "" << file << ":" << line << "]";
    return std::cout;
}

//__FILE__ __LINE__
#define LOG(level) Log(level,__FILE__,__LINE__)


class FileUtil{
public:
    //从文件描述符中读取一行
    //一行的界定标识是\n  \r  \r\n
    //返回的line中是不包含界定标识的
    //例如：
    //aaa\nbbb\nccc
    //调用ReadLine返回的line对象的内容为aaa，不包含\n
    static int ReadLine(int fd,std::string* line){
        line->clear();
        while(true){
            char c = '\0';
            ssize_t read_size = recv(fd, &c, 1, 0);//recv用法和read相似
            if(read_size <= 0){
                return -1;
            }
            if (c == '\n'){
                break;
            }
            //如果当前字符是\r\,把这个情况处理成\n
            if(c == '\r'){
                //虽然从缓冲区读了一个字符，但是缓冲区并没有把它删掉
                recv(fd, &c,1,MSG_PEEK);
                if(c == '\n'){
                    //发现\r后面一个字符刚好就是\n，为了不影响下次循环就需要把这样的字符从缓冲区中干掉
                    recv(fd, &c, 1, 0);
                    break;
                }else{     //如果不是\n则强制设置成\n
                    c = '\n';
                }
            }
            //这个条件涵盖了\r和\r\n的情况
            if (c == '\n'){
                break;            
            }
            line->push_back(c);
        }
        return 0;
    }

    static int ReadN(int fd,size_t len,std::string* output){
        output->clear();
        char c = '\0';
        for(size_t i= 0;i < len;++i)
        {
            recv(fd,&c,1,0);
            output->push_back(c);
        }
        return 0;
    }


    //从文件中读取全部内容到 std::string中
    static int ReadAll(const std::string& file_path,std::string* output)
    {
        std::ifstream file(file_path.c_str());
        if(!file.is_open()){
            LOG(ERROR) << "Open file error! file_path=" << file_path << "\n";
            return -1;
        }
        file.seekg(0,file.end);//seekg调整文件指针的位置，此处将文件指针调整到文件末尾
        int length = file.tellg(); //查询当前文件指针的位置，返回值就是文件指针位置相对于文件起始位置的偏移量
        //为了从头读取文件，需要把文件指针设置到开头位置
        file.seekg(0,file.beg);
        //读取完整的文件内容
        output->resize(length);//开辟内存，开辟保证能存下length的长度
        file.read(const_cast<char*>(output->c_str()),length);
        //万一忘记写close，问题不大，因为ifstream对象会在析构的时候自动的进行关闭文件描述符
        file.close();
        return 0;
    }

    static int ReadAll(int fd,std::string* output){
        //TODO 此函数完成从文件描述符中读取所有数据的操作
        while(true){
            char buf[1024] = {0};
            ssize_t read_size = read(fd, buf, sizeof(buf) - 1);
            if(read_size < 0){
                perror("read");
                return -1;
            }
            if(read_size == 0){
                //文件读完了
                return 0;
            }
            buf[read_size] = '\0';
            (*output) += buf;
        }
        return 0;
    }

    static bool IsDir(const std::string& file_path)
    {
        return boost::filesystem::is_directory(file_path);
    }
};

class StringUtil{
public:
    //把一个字符串，按照split_char进行切分，分成的N个子串，放到output数组中
    //token_compress_on 的含义是：
    //例如分隔符是空格，字符串就是"a  b"(中间有两个空格)
    //对于打开压缩情况，返回的子串就是有两个，"a","b"
    //token_compress_off
    //对于关闭压缩的情况，返回的子串就是有三个，"a"," ","b"
    static int Split(const std::string& input,const std::string& split_char,std::vector<std::string>* output){
        boost::split(*output,input,boost::is_any_of(split_char),boost::token_compress_on);
        return 0;
    }
    
    typedef std::unordered_map<std::string, std::string> UrlParam;
    static int ParseUrlParam(const std::string& input, UrlParam* output){
        //1.按照取地址符号切分成若干个kv
        std::vector<std::string> params;
        Split(input, "&", &params);
        //2.再针对每一个kv，按照 = 切分，放到输出结果中
        //range based for
        for (auto item : params){
            std::vector<std::string> kv;
            Split(item, "=", &kv);
            if(kv.size() != 2){
                //说明该参数非法
                LOG(WARNING) << "kv format error! item=" << item << "\n";
                continue;
            }
            (*output)[kv[0]] = kv[1];
        }
        return 0;
    }
};

