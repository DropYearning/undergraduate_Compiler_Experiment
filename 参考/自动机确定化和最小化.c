#include<iostream>
#include<queue>
#include<iomanip>
#include<map>
#include<vector>
#include<cstring>
#include<algorithm>
using namespace std;
const int N = 1005;
const int M = 1005;
const int CSIZE = 26;
struct DFA{/*{{{*/
    struct Node{
        int x,y,id;
        Node(){}
        Node(int x,int y,int id):x(x),y(y),id(id){}
        bool operator < (const Node &a)const{
            if(x != a.x) return x < a.x;
            return y < a.y;
        }
        bool operator == (const Node &a)const{
            return x == a.x && y == a.y;
        }
    };
    int ch[N][CSIZE];
    int cnt,C;
    bool accept[N];
    void init(){
        cnt = 0;
        memset(ch,-1,sizeof(ch));
        memset(accept,0,sizeof(accept));
    }
    void addTrans(int u,int v,int c){
        ch[u][c] = v;
    }
    void output(){
        cout<<"*******DFA********"<<endl;
        for(int i = 0 ; i < cnt ; i ++){
            cout<<i<<": ";
            for(int c = 0 ; c < C ; c ++)
                cout<<setw(2)<<ch[i][c]<<' ';
            cout<<endl;
        }
        for(int i = 0 ; i < cnt ; i ++)
            cout<<accept[i]<<" ";
        cout<<endl;
    }
    DFA GetMinDFA(){
        DFA ret;
        ret.init();
        int tmp[2][N],cur = 0;
        int ocnt = 0,tc = 0;
        int hcnt;
        Node nd[N];
        for(int i = 0 ; i < cnt ; i ++) tmp[cur][i] = accept[i];
        do{
            cur ^= 1;
            for(int i = 0 ; i < cnt ; i ++){
                if(ch[i][tc] == -1) tmp[cur][i] = -1;
                else tmp[cur][i] = tmp[cur ^ 1][ ch[i][tc] ];
            }
            for(int i = 0 ; i < cnt ; i ++)
                nd[i] = Node(tmp[cur ^ 1][i],tmp[cur][i],i);
            sort(nd,nd + cnt);
            hcnt = 0;
            tmp[cur][ nd[0].id ] = hcnt ++;
            for(int i = 1 ; i < cnt ; i ++){
                if(nd[i] == nd[i - 1]) tmp[cur][ nd[i].id ] = tmp[cur][ nd[i - 1].id ];
                else tmp[cur][ nd[i].id ] = hcnt ++;
            }
            for(int i = 0 ; i < cnt ; i ++)
                if(tmp[cur][i] != tmp[cur ^ 1][i]) ocnt = -1;
            ocnt ++;
            tc = (tc + 1) % C;
        }while(ocnt < C);
        map<int,int> tmap;
        tmap.clear();
        hcnt = 0;
        for(int i = 0 ; i < cnt ; i ++){
            if(tmap.find(tmp[cur][i]) == tmap.end()) tmap[ tmp[cur][i] ] = hcnt ++;
            tmp[cur][i] = tmap[ tmp[cur][i] ];
        }
        ret.cnt = hcnt;
        ret.C = C;
        for(int i = 0 ; i < cnt ; i ++){
            for(int c = 0 ; c < C ; c ++){
                if(ch[i][c] == -1) ret.addTrans(tmp[cur][i],-1,c);
                ret.addTrans(tmp[cur][i],tmp[cur][ ch[i][c] ],c);
            }
        }
        for(int i = 0 ; i < cnt ; i ++)
            if(accept[i]) ret.accept[ tmp[cur][i] ] = 1;
        return ret;
    }
};/*}}}*/
struct NFA{/*{{{*/
    struct Edge{
        int u,v,c;
        int next;
    }e[M];
    struct Node{/*{{{*/
        int cnt;
        vector<int> vt;
        Node(){}
        Node(bool id[]){
            cnt = 0;
            for(int i = 0 ; i < N ; i ++)
                if(id[i]){
                    cnt ++;
                    vt.push_back(i);
                }
            vt.resize(cnt);
        }
        bool operator < (const Node &a)const{
            if(cnt != a.cnt) return cnt < a.cnt;
            for(int i = 0 ; i < cnt ; i ++)
                if(vt[i] != a.vt[i]) return vt[i] < a.vt[i];
            return 0;
        }
        void output(){
            cout<<"{";
            for(int i = 0 ; i < cnt ; i ++)
                cout<<vt[i]<<" ";
            cout<<"}";
            cout<<endl;
        }
    };/*}}}*/
    struct OpHash{/*{{{*/
        int cnt;
        map<char,int> mp;
        char op[CSIZE];
        void init(){
            cnt = 0;
            mp.clear();
        }
        int Insert(char c){
            if(c == '~') return CSIZE + 1;
            if(mp.find(c) == mp.end()){
                op[cnt] = c;
                mp[c] = cnt ++;
            }
            return mp[c];
        }
        char Find(int c){
            return op[c];
        }
    }OpHash;/*}}}*/
    struct NdHash{/*{{{*/
        int cnt;
        map<Node,int> mp;
        Node nd[N];
        void init(){
            cnt = 0;
            mp.clear();
            bool tmp[N];
            memset(tmp,0,sizeof(tmp));
            mp[Node(tmp)] = -1;
        }
        int Insert(Node c){
            if(mp.find(c) == mp.end()){
                nd[cnt] = c;
                mp[c] = cnt ++;
            }
            return mp[c];
        }
        Node Find(int c){
            return nd[c];
        }
    }NdHash;/*}}}*/
    int head[N],sz;
    int n,m;
    int S,C;
    int T[N],Tcnt;
    void addedge(){
        int u,v;
        char c;
        cin>>u>>v>>c;
        e[sz].u = u;
        e[sz].v = v;
        e[sz].c = OpHash.Insert(c);
        e[sz].next = head[u];
        head[u] = sz ++;
    }
    void init(){
        memset(head,-1,sizeof(head));
        memset(T,0,sizeof(T));
        sz = Tcnt = 0;
        NdHash.init();
        OpHash.init();
    }
    void read(){
        int t;
        cin>>n>>m;
        for(int i = 0 ; i < m ; i ++) addedge();
        cin>>S>>Tcnt;
        for(int i = 0  ; i < Tcnt ; i ++){
            cin>>t;
            T[t] = 1;
        }
        C = OpHash.cnt;
    }
    void check(){
        bool ok = 1;
        for(int i = 0 ; i < m ; i ++)
            if(e[i].c == CSIZE + 1) ok = 0;
        if(!ok){
            cout<<"This is NFA"<<endl;
            return;
        }
        int reach[N][CSIZE];
        memset(reach,-1,sizeof(reach));
        for(int i = 0 ; i < m ; i ++){
            int u = e[i].u,v = e[i].v;
            if(reach[u][ e[i].c ] != -1 && reach[u][ e[i].c ] != v){
                cout<<"This is NFA"<<endl;
                return;
            }
            reach[u][ e[i].c ] = v;
        }
        cout<<"This is DFA"<<endl;
    }
    void close(int u,bool vis[]){
        vis[u] = 1;
        for(int i = head[u] ; i != -1 ; i = e[i].next){
            int v = e[i].v;
            if(e[i].c != CSIZE + 1) continue;
            if(vis[v]) continue;
            close(v,vis);
        }
    }
    void trans(int u,int c,bool vis[]){
        for(int i = head[u] ; i != -1 ; i = e[i].next){
            if(e[i].c != c) continue;
            int v = e[i].v;
            close(v,vis);
        }
    }
    DFA ChangeToDFA(){
        DFA ret;
        ret.init();
        bool tmp[N],vis[N];
        queue<int> Q;
        memset(tmp,0,sizeof(tmp));
        memset(vis,0,sizeof(vis));
        tmp[S] = 1;
        Q.push(NdHash.Insert(Node(tmp)));
        while(!Q.empty()){
            int uid = Q.front();Q.pop();
            Node u = NdHash.Find(uid);
            for(int c = 0 ; c < C ; c ++){
                memset(tmp,0,sizeof(tmp));
                for(int i = 0 ; i < u.cnt ; i ++)
                    trans(u.vt[i],c,tmp);
                int v = NdHash.Insert(Node(tmp));
                ret.addTrans(uid,v,c);
                if(v != -1 && vis[v] == 0){
                    vis[v] = 1;
                    Q.push(v);
                }
            }
        }
        ret.cnt = NdHash.cnt;
        ret.C = OpHash.cnt;
        for(int i = 0 ; i < ret.cnt ; i ++){
            Node u = NdHash.Find(i);
            for(int j = 0 ; j < u.cnt ; j ++)
                if(T[ u.vt[j] ]) ret.accept[i] = 1;
        }
        return ret;
    }
};/*}}}*/
NFA NFA;
DFA DFA,MDFA;
int main(){
    NFA.init();
    NFA.read();
    NFA.check();
    DFA = NFA.ChangeToDFA();
    DFA.output();
    MDFA = DFA.GetMinDFA();
    MDFA.output();
    return 0;
}
