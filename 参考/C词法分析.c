#include<cstdio>
#include<string>
#include<map>
#include<cstring>
#include<algorithm>
using namespace std;
const int N = 25;
const int M = 100005;
const int TSIZE = 105;
char Buffer[M];
char Keyword[N][N];
char Oper[N][N];
char tmp[N * 4][TSIZE];
int tail,tlen[N * 4];
int tcnt,Blen;
int Opernum,Kewdnum;
int val1[N],val2[N];
int Numval,Varval;
map<string,int> mp;
bool isblank(char c){
    if(c == ' ') return 1;
    if(c == '\n') return 1;
    if(c < 32) return 1;
    return 0;
}
bool isalpha(char c){
    if(c <= 'z' && c >= 'a') return 1;
    if(c <= 'Z' && c >= 'A') return 1;
    return 0;
}
bool isnum(char c){
    if(c >= '0' && c <= '9') return 1;
    return 0;
}
bool isoper(char c){
    bool ok = 0;
    for(int i = 0 ; i < Opernum ; i ++)
        for(int j = 0 ; j < strlen(Oper[i]) ; j ++){
            if(Oper[i][j] == c) ok = 1;
        }
    return ok;
}
void getwordornum(){
    tlen[tcnt] = 0;
    int &len = tlen[tcnt];
    while(tail < Blen && (isalpha(Buffer[tail]) || isnum(Buffer[tail]))){
        tmp[tcnt][len ++] = Buffer[tail ++];
    }
    tmp[tcnt][len] = '\0';
    tcnt ++;
}
void getoper(){
    tlen[tcnt] = 0;
    int &len = tlen[tcnt];
    while(tail < Blen && (isoper(Buffer[tail]))){
        tmp[tcnt][len ++] = Buffer[tail ++];
    }
    tmp[tcnt][len] = '\0';
    tcnt ++;
}
bool check(){
    for(int i = 0 ; i < tcnt ; i ++){
        int ok = 0;
        for(int j = 0 ; j < Opernum ; j ++)
            if(strcmp(Oper[j],tmp[i]) == 0) ok = 1;
        for(int j = 0 ; j < Kewdnum ; j ++)
            if(strcmp(Keyword[j],tmp[i]) == 0) ok = 1;
        if(ok) continue;
        if(isalpha(tmp[i][0])) continue;
        else if(isnum(tmp[i][0])){
            for(int k = 0 ; k < tlen[i] ; k ++)
                if(isalpha(tmp[i][k])) return 0;
            continue;
        }
        else return 0;
    }
    return 1;
}
int Hash(char s[]){
    if(mp.find(string(s)) != mp.end())
        return mp[string(s)];
    if(isalpha(s[0])) return Varval;
    else return Numval;
}
void solve(){
    printf("%s\n",Buffer);
    tail = tcnt = 0;
    Blen = strlen(Buffer);
    while(tail < Blen){
        while(tail < Blen && isblank(Buffer[tail])) tail ++;
        if(tail == Blen) break;
        if(isalpha(Buffer[tail]) || isnum(Buffer[tail])) getwordornum();
        else if(isoper(Buffer[tail])) getoper();
        else{
            //printf("!!!! %c %d\n",Buffer[tail],isnum(Buffer[tail]));
            printf("Run Error!\n");
            return;
        }
    }
    if(!check()){
        printf("Run Error!\n");
        return;
    }
    for(int i = 0 ; i < tcnt ; i ++)
        printf("(%d,%s)",Hash(tmp[i]),tmp[i]);
    printf("\n");
}
void init(){
    mp.clear();
    FILE *f1 = fopen("keyword.in","r");
    FILE *f2 = fopen("operator.in","r");
    FILE *f3 = fopen("value.in","r");
    fscanf(f1,"%d",&Kewdnum);
    fscanf(f2,"%d",&Opernum);
    int tv;
    for(int i = 0 ; i < Kewdnum ; i ++){
        fscanf(f1,"%s%d",Keyword[i],&tv);
        mp[string(Keyword[i])] = tv;
    }
    for(int i = 0 ; i < Opernum ; i ++){
        fscanf(f2,"%s%d",Oper[i],&tv);
        mp[string(Oper[i])] = tv;
    }
    fscanf(f3,"%d%d",&Varval,&Numval);
    fclose(f1);
    fclose(f2);
    fclose(f3);
}
int main(){
    init();
    while(gets(Buffer) != NULL) solve();
    return 0;
}
