#include "window.h"
#include <iostream>
#include <locale.h>

namespace client {

Window::window(){
    //设置字符编码
    setlocale(LC_ALL,"");
    //对整个bcurses进行初始化
    initscr();
    //隐藏光标
    curs_set(0);
}

Window::~Window(){
    //对整个ncurses进行销毁动作
    //如果忘记调用这个函数，终端会显示混乱
    endwin();
}

void Window::DrawHeader(){
    //LINES是ncurses提供的宏，表示当前窗口的最大行数
    int h = LINES / 5;
    //COLS也是ncurses提供的宏，表示当前窗口的最大列数
    int w = CLOS;
    int y = 0;
    int x = 0;
    header_win_ = newwin(h,w,y,x);
    std::string title = "我是一个聊天室";
    PutStrToWin(header_win_,h/2,w/2-titel.size()/2,title);
    box(header_win_,'|','-');
    wrefresh(header_win_);
}

void Window::PutStrToWin(WINDOW* win, int y, int x, const std::string& str)
{
    mvwaddstr(win, y ,x,str.c_str());
}

void Window::GetStrFromWin(WINDOW* win,std::string* str)
{
    char buf[1024*10] = {0};
    wgetnstr(win,buf,sizeof(buf)-1);
    *str = buf;
}

void Window::DrawInput()
{
    int h = LINES / 5;
    int w = CLOS;
    int y = LINES * 4 / 5;
    int x = 0;
    input_win_ = newwin(h,w,y,x);
    std::string title = "请输入消息";
    //此时的坐标其实是窗口左上角为原点的坐标系
    //此处从1，1开始填充提示符，是为了和边框不重复
    //边框已经占用了第0行和第0列
    PutStrToWin(input_win_,1,2,title);
    box(input_win_,'|','-');
    wrefresh(input_win_);
}

void Window::DrawOutput(){
    int h = LINES*3/5;
    int w = CLOS*3/4;
    int y = LINES/5;
    int x = 0;
    output_win_ = newwin(h,w,y,x);
    box(output_win_,'|','-');
    for(size_t i = 0; i < msgs_.size();++i){
        PutStrToWin(output_win_,i+1,2,msgs_[i]);
    }
    wrefresh(output_win_);
}

void Window::AddMsg(const std:tring& msg){
    //此处由于我们的串口显示的消息条数有限，不能无止境的插入新消息
    //当msgs包含的消息数目超过一定的阀值，就把旧消息删除掉
    msgs_.push_back(msg);
    if(msgs_,size() > 20){
        msgs_.front();
    }
} 
void Window::DrawFriendList(){
    int h = LINES*3/5;
    int w = COLS /4;
    int y = LINES/5;
    int x = COLS * 3 / 4;
    friend_list_win_ = newwin(h,w,y,x);
    box(friend_list_,'|','-');
    //遍历好友列表，把好友信息显示上来
    size_t i =0;
    for(auto item : friend_list_){
        PutStrToWin(friend_list_win_,i+1,1,item);
        ++i;
    }
    wrefresh(friend_list_win_);
}

void Window::AddFriend(const std::string& friend_info){
    friend_list_.insert(friend_info);
}
void DelFreind(const std::string& friend_info){
    friend_list_.erase(friend_info);
}

}//end client

#include <unistd.h>

int main(){
    client::Window win;
    win.DrawHeader();
    win.DrawInput();
    win/AddMsg("haha");
    win.DrawOutput();
    //如果好友列表一行显示不下，可修改好友列表宽度.坐标
    win.AddFriend("num1 | xian");
    win.DrawFriendList();
    sleep(5);
    return 0;
}

