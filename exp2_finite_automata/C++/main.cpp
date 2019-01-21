//
//  main.cpp
//  compiler-exp2
//
//  Created by 周亮 on 2017/10/23.
//  Copyright © 2017年 周亮. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <set>

using namespace std;

struct State
{
    int id;//状态集序号
    set<int> s;//状态集包含的状态
    bool ifend;//该状态集是否是终结状态
};

bool ifsetequ(set<int> a ,set<int> b)
{
    if(a.size()!=b.size())
        return false;
    else
    {
        set<int>::iterator it;
        for (set<int>::iterator it = a.begin(); it != a.end(); it++)
        {
            if(find(b.begin(),b.end(),*it)==b.end())//a集合中有元素在b集合中找不到
                return false;
        }
    }
    return true;
}

set<int> eclosure(set<int> s,State *tbl[])
{
    
    set<int> temp;
    return temp;
}




int main()
{
    
    vector<int> endstate;//存放终结状态ID的数组
    vector<char> charset;//存放输入字母表的数组
    //读取文件信息
    ifstream fin("/Users/Zhouliang/Documents/Xcode/compiler-exp2/compiler-exp2/DFA.txt");
    string buf;
    getline(fin,buf);
    int statecount =stoi(buf);//第一行,NFA的状态总数
    cout<<"NFA的状态总数:"<<statecount<<endl;
    getline(fin,buf);
    int endcount =stoi(buf);//第二行,NFA的终结状态数量
    cout<<"NFA的终结状态数量:"<<endcount<<endl;
    for(int i = 0; i < endcount; i++)//后面endcount行都是终结状态
    {
        getline(fin,buf);
        int tempid = stoi(buf);
        endstate.push_back(tempid);
    }
    getline(fin,buf);
    int charcount =stoi(buf);//下一行,NFA的输入字母数量
    for(int i = 0; i < charcount; i++)//后面charcount行都是终结状态
    {
        getline(fin,buf);
        char tempchar = buf[0] ;
        charset.push_back(tempchar);
    }
    //输出读入的文件内容
    vector<int>::iterator it1;
    cout<<"所有终结状态为:";
    for(it1=endstate.begin();it1 != endstate.end();it1++)
    {
        cout<<*it1<<" ";
    }
    cout<<endl;
    vector<char>::iterator it2;
    cout<<"所有输入字符为:";
    for(it2=charset.begin();it2 != charset.end();it2++)
    {
        cout<<*it2<<" ";
    }
    cout<<endl;

    
    //构造初始状态表
    State firststate[statecount+1];// state[0]不用
    for(int i = 1; i <=statecount; i++)
    {
         if(find(endstate.begin(),endstate.end(),i)!=endstate.end())//初始化终结状态
        {
            firststate[i].id = i;
            firststate[i].ifend = true;
            firststate[i].s.insert(i);
        }
        else//初始化非终结状态
        {
            firststate[i].id = i;
            firststate[i].ifend = false;
            firststate[i].s.insert(i);
        }
    }
    //初始化初始状态转换表
    State firsttbl[statecount][charcount+1];
    for(int i = 0; i<statecount; i++ )
    {
        if(i == 0)
        {
            for(int j =0;j<charcount+1;j++)
            {
                firsttbl[i][j].ifend=false;
                firststate[i].s.clear();
            }

        }
        else
        {
            for(int j =0;j<charcount+1;j++)
            {
                firsttbl[i][j].ifend=false;
                firststate[i].s.clear();
            }
        }
    }
    
    //在初始状态转换表中插入状态
    getline(fin,buf);
    int rulecount = stoi(buf);
    cout<<"转换边条数为:"<<rulecount<<endl;
    for(int k =0;k<rulecount;k++)
    {
        getline(fin,buf);
        string templeft ="";//左边
        char tempop ;//输入字符
        string tempright ="";//右边
        int i = 0;
        while (buf[i]!='-')
        {
            templeft=templeft+buf[i];
            i++;
        }
        i++;
        tempop=buf[i];
        i=i+2;
        while (buf[i]!='\0')
        {
            tempright=tempright+buf[i];
            i++;
        }
        int left = stoi(templeft);
        int right = stoi(tempright);
        int op;
        switch (tempop)
        {
            case '*':
            {
                op =0 ;
                break;
            }
            default:
            {
                op = tempop-'a'+1;
            }
        }
        firsttbl[left][op].s.insert(right);
    }
    
    //打印整个转换表
    for(int i = 1; i<statecount; i++ )
    {
        for(int j = 0; j<charcount+1; j++ )
        {
            if(firsttbl[i][j].s.empty()==true)
                cout<<"NULL"<<'\t';
            else
            {
                cout<<"{";
                for (set<int>::iterator sit = firsttbl[i][j].s.begin(); sit != firsttbl[i][j].s.end(); sit++)
                {
                    if(sit == firsttbl[i][j].s.begin())
                        cout << *sit;
                    else
                        cout<<","<<*sit;
                }
                cout<<"}"<<'\t';
            }
        }
        cout<<endl;
    }
    
    //检测是否是NFA
    int flag = 0;//flag==0是DFA flag==1是NFA
    for(int i = 1; i<statecount; i++ )
    {
        for(int j =0;j<charcount+1;j++)
        {
            if(firsttbl[i][j].s.size() > 1)
                flag = 1;
        }
    }
    if(flag == 0) cout<<"这是一个DFA."<<endl;
    else cout<<"这是一个NFA."<<endl;
    
    //NFA转换DFA,构造新状态序列和状态转换表
   
    
    
    
    
    
    
    
    
    
    return 0;

}
