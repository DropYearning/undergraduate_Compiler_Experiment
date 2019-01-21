#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
const int N = 10005;
char Buffer[N];
int tail;
bool E();
bool A();
bool T();
bool B();
bool F();
void init(){
    tail = 0;
}
void solve(){
    scanf("%s",Buffer);
    init();
    if(E() && strlen(Buffer) == tail)
        printf("Yes!\n");
    else
        printf("No...\n");
}
int main(){
    int _;
    scanf("%d",&_);
    while(_--) solve();
    return 0;
}
bool E(){/*{{{*/
    bool ok = 1;
    ok = ok && T();
    ok = ok && A();
    return ok;
}/*}}}*/
bool A(){/*{{{*/
    bool ok = 1;
    if(Buffer[tail] == '+'){
        tail ++;
        ok = ok && F();
        ok = ok && B();
        ok = ok && A();
    }
    else ok = 1;
    return ok;
}/*}}}*/
bool T(){/*{{{*/
    bool ok = 1;
    ok = ok && F();
    ok = ok && B();
    return ok;
}/*}}}*/
bool B(){/*{{{*/
    bool ok = 1;
    if(Buffer[tail] == '*'){
        tail ++;
        ok = ok && F();
        ok = ok && B();
    }
    else ok = 1;
    return ok;
}/*}}}*/
bool F(){/*{{{*/
    int ok = 1;
    if(Buffer[tail] == '('){
        tail ++;
        ok = ok && E();
        if(Buffer[tail] != ')') ok = 0;
        else tail ++;
    }
    else if(Buffer[tail] == 'i'){
        tail ++;
        ok = 1;
    }
    else ok = 0;
    return ok;
}/*}}}*/
/*
10
i+(i+i*i)
i+i
i+i+i+i
i+i*i*i
i++++
iii
i+i+(i+i)
c+i
c
i+i+

*/
