#pragma once

#include <string.h>
#include <deque>
#include <ncurses.h>
#include <unordered_set>

namespace client{

class Window{
public:
    Window();
    ~Window();
    //绘制标题子窗口
    void DrawHeader();
    //绘制输入框
    void DrawInput();
    //绘制输出框
    void DrawOutput();
    //绘制好友列表
    void DrawFriendList();
    //往窗口中写字符串
    void PutStrToWin(WINDOW* win, int y, int x, const std::string& str);
    //从窗口中读字符串
    void GetStrFromWin(WINDOW* win,str::string* str);
    void AddMsg(const std::string& msg);
    void AddFriend(const std::string& friend_info);
    void DelFreind(const std::string& friend_info);

private:
    WINDOW* header_win_;
    WINDOW* intput_win_;
    WINDOW* output_win_;
    WINDOW* frient_list_win_;
    std::deque<std::string> msgs_;
    std::unordered_set<std::string> friend_list_;
};

}
