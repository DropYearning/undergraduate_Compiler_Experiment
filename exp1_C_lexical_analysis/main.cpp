#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>
using namespace std;

string keyword[14]={"int", "char", "void", "if", "else", "switch", "case", "default", "while", "do", "for", "break", "continue", "return"};

vector<string> keywords(keyword,keyword+14);

bool ifnumend(char c)//判断是否可作为数字结尾
{
    if( (c>='a' && c<='z')||(c>='A' && c<='Z') ||c=='_')
        return false;
    else
        return true;
}


bool ifletter(char c)//判断是否是字母
{
    if((c>='a' && c<='z')||(c>='A' && c<='Z'))
        return true;
    else
        return false;
}

bool ifnumber(char c)//判断是否是数字
{
    if(c>='0' && c<='9')
        return true;
    else
        return false;
}

bool ifkeyword(string s)//判断是否是关键字
{
    if(find(keywords.begin(),keywords.end(),s)!=keywords.end())
        return true;
    else
        return false;
}


int scan(char *p)//扫描函数,返回值是识别出的单词符号的长度,用于在main函数内修改*ptr,指向下一个单词符号的首字符
{
    int lenth = 0;
    string temp="";//用于存放识别出的单词符号
    if(*p == '_')//如果以_开头,则该字段是一个标识符(这个判断其实多余了)
    {
        temp += *p;
        p++;lenth++;
        while(ifnumber(*p) || ifletter(*p)||*p =='_' )//如果读取的下一个字符是字母或数字
        {
            temp += *p;
            p++;lenth++;
        }
        cout<<"< 标识符 , "<<temp<<" >"<<endl;
        return lenth;
    }
    
    else if(ifletter(*p)== true)//如果以字母开头,则该字段可能是一个标识符,也有可能是关键字
    {
        temp += *p;
        p++;lenth++;
        int flag = 0;//字段中间是否出现下划线_,出现_一定不是关键字
        while(ifnumber(*p) || ifletter(*p) ||*p =='_')//如果读取的下一个字符是字母或数字或下划线
        {
            if(*p =='_') flag =1;
            temp += *p;
            p++;lenth++;
        }
        if(flag==0&&ifkeyword(temp) == true)//如果该字段是关键字
        {
            cout<<"< 关键字 , "<<temp<<" >"<<endl;
            return lenth;
        }
        else//如果该字段是标识符
        {
            cout<<"< 标识符 , "<<temp<<" >"<<endl;
            return lenth;
        }
    }
    
    else if(ifnumber(*p) == true) //如果以数字开头,则该字段应该是一个数字(可能是整数\0开头八进制\0x开头十六进制)
    {
        temp += *p;
        if(*p == '0')//如果以0开头,有可能是十进制0或八进制或十六进制
        {
            p++;lenth++;
            if(*p == 'x') //是十六进制
            {
                temp += *p;
                p++;lenth++;
                while (ifnumber(*p) == true)
                {
                    temp += *p;
                    p++;lenth++;
                }
                if(ifnumend(*p) == true)
                    cout<<"< 整数(十六进制) , "<<temp<<" >"<<endl;
                else
                {cout<<"Error:输入了错误的整数!"<<endl;exit(0);}
                return lenth;
            }
            else if(ifnumber(*p) == true)//是八进制
            {
                temp += *p;
                p++;lenth++;
                while (ifnumber(*p) == true)
                {
                    temp += *p;
                    p++;lenth++;
                }
                if(ifnumend(*p) == true)
                     cout<<"< 整数(八进制) , "<<temp<<" >"<<endl;
                else
                     {cout<<"Error:输入了错误的整数!"<<endl;exit(0);}
                return lenth;
            }
            else //是十进制0
            {
                if(ifnumend(*p) == true)
                    cout<<"< 整数(十进制) , "<<temp<<" >"<<endl;
                else
                     {cout<<"Error:输入了错误的整数!"<<endl;exit(0);}
                return lenth;

            }
        }
        else //是十进制数
        {
            p++;lenth++;
            while (ifnumber(*p) == true)
            {
                temp += *p;
                p++;lenth++;
            }
            if(ifnumend(*p) == true)
                 cout<<"< 整数(十进制) , "<<temp<<" >"<<endl;
            
            else
                cout<<"Error:输入了错误的整数!"<<endl;
            return lenth;

        }
    }
    else if(*p == ',')
    {
        temp += *p;
        lenth++;
        cout<<"< 分界符 , "<<temp<<" >"<<endl;return lenth;
    }
    else if(*p == ':')
    {
        temp += *p;
        lenth++;
        cout<<"< 分界符 , "<<temp<<" >"<<endl;return lenth;
    }
    else if(*p == ';')
    {
        temp += *p;
        lenth++;
        cout<<"< 分界符 , "<<temp<<" >"<<endl;return lenth;
    }
    else if(*p == '[')
    {
        temp += *p;
        lenth++;
        cout<<"< 分界符 , "<<temp<<" >"<<endl;return lenth;
    }
    else if(*p == ']')
    {
        temp += *p;
        lenth++;
        cout<<"< 分界符 , "<<temp<<" >"<<endl;return lenth;
    }
    else if(*p == '(')
    {
        temp += *p;
        lenth++;
        cout<<"< 分界符 , "<<temp<<" >"<<endl;return lenth;
    }
    else if(*p == ')')
    {
        temp += *p;
        lenth++;
        cout<<"< 分界符 , "<<temp<<" >"<<endl;return lenth;
    }
    else if(*p == '{')
    {
        temp += *p;
        lenth++;
        cout<<"< 分界符 , "<<temp<<" >"<<endl;return lenth;
    }
    else if(*p == '}')
    {
        temp += *p;
        lenth++;
        cout<<"< 分界符 , "<<temp<<" >"<<endl;return lenth;
    }
    else if(*p == '*')
    {
        temp += *p;
        lenth++;
        cout<<"< 运算符 MUL  , "<<temp<<" >"<<endl;return lenth;
    }
    else if(*p == '/')
    {
        temp += *p;
        lenth++;
        cout<<"< 运算符 DIV  , "<<temp<<" >"<<endl;return lenth;
    }
    else if(*p == '%')
    {
        temp += *p;
        lenth++;
        cout<<"< 运算符 MOD  , "<<temp<<" >"<<endl;return lenth;
    }
    else if(*p == '%')
    {
        temp += *p;
        lenth++;
        cout<<"< 运算符 MOD  , "<<temp<<" >"<<endl;return lenth;
    }
    else if(*p == '=')
    {
        temp += *p;
        lenth++;
        p++;
        if( *p == '=')
        {
            temp += *p;
            lenth++;
            cout<<"< 运算符 EQU  , "<<temp<<" >"<<endl;return lenth;
        }
        else
        {
            cout<<"< 赋值号 ASSIGN  , "<<temp<<" >"<<endl;return lenth;
        }
    }
    else if(*p == '!')
    {
        temp += *p;
        lenth++;
        p++;
        if( *p == '=')
        {
            temp += *p;
            lenth++;
            cout<<"< 运算符 NEQU  , "<<temp<<" >"<<endl;return lenth;
        }
        else
        {
            cout<<"< 运算符 NOT  , "<<temp<<" >"<<endl;return lenth;
        }
    }
    else if(*p == '+')
    {
        temp += *p;
        lenth++;
        p++;
        if( *p == '+')
        {
            temp += *p;
            lenth++;
            cout<<"< 运算符 INC  , "<<temp<<" >"<<endl;return lenth;
        }
        else
        {
            cout<<"< 运算符 ADD  , "<<temp<<" >"<<endl;return lenth;
        }
    }
    else if(*p == '-')
    {
        temp += *p;
        lenth++;
        p++;
        if( *p == '-')
        {
            temp += *p;
            lenth++;
            cout<<"< 运算符 DEC  , "<<temp<<" >"<<endl;return lenth;
        }
        else
        {
            cout<<"< 运算符 SUB  , "<<temp<<" >"<<endl;return lenth;
        }
    }
    else if(*p == '&')
    {
        temp += *p;
        lenth++;
        p++;
        if( *p == '&')
        {
            temp += *p;
            lenth++;
            cout<<"< 运算符 AND  , "<<temp<<" >"<<endl;return lenth;
        }
        else
        {
            cout<<"< 按位与  , "<<temp<<" >"<<endl;return lenth;
        }
    }
    else if(*p == '|')
    {
        temp += *p;
        lenth++;
        p++;
        if( *p == '|')
        {
            temp += *p;
            lenth++;
            cout<<"< 运算符 OR  , "<<temp<<" >"<<endl;return lenth;
        }
        else
        {
            cout<<"< 按位或  , "<<temp<<" >"<<endl;return lenth;
        }
    }
    else if(*p == '<')
    {
        temp += *p;
        lenth++;
        p++;
        if( *p == '=')
        {
            temp += *p;
            lenth++;
            cout<<"< 运算符 IE  , "<<temp<<" >"<<endl;return lenth;
        }
        else
        {
            cout<<"< 运算符 IT  , "<<temp<<" >"<<endl;return lenth;
        }
    }
    else if(*p == '>')
    {
        temp += *p;
        lenth++;
        p++;
        if( *p == '=')
        {
            temp += *p;
            lenth++;
            cout<<"< 运算符 GE  , "<<temp<<" >"<<endl;return lenth;
        }
        else
        {
            cout<<"< 运算符 GT  , "<<temp<<" >"<<endl;return lenth;
        }
    }
    else if(*p == '\'')
    {
        temp += *p;
        lenth++;
        p++;
        temp = "";//去掉之前保存的单引号
        while (*p != '\'')
        {
            temp += *p;
            p++;lenth++;
        }
        if(lenth > 3)
            cout<<"Error:输入了错误的字符！"<<endl;
        else
            cout<<"< 字符 , "<<temp<<" >"<<endl;
        lenth++;//加上右引号的1长度
        return lenth;
    }
    else if(*p == '\"')
    {
        temp += *p;
        lenth++;
        p++;
        temp = "";//去掉之前保存的双引号
        while (*p != '\"')
        {
            temp += *p;
            p++;lenth++;
        }
        lenth++;//加上右引号的1长度
        cout<<"< 字符串 , "<<temp<<" >"<<endl;
        return lenth;
    }
    else//出现了该自动机无法识别的字段,报错
    {
        cout<<"Error:检测到无法匹配字段,程序结束!"<<endl;
        exit(0);
    }
    return lenth;
}

int main()
{
    ifstream fin("./test/test1.txt");
    char  buf;
    char text[1000];
    while((buf=fin.get())!=EOF)//将文件内容读入text[]
    {
        text[strlen(text)] = buf;
        text[strlen(text)+1] = '\0';
    }
    char *ptr = text;
    
    while(*ptr != '\0')
    {
        if( *ptr == ' ' || *ptr == '\t' || *ptr == '\n')
        {
            ptr++;
            continue;
        }
        int len = scan(ptr);
        ptr = ptr + len;
    }
    
}
