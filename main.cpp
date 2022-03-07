#include <bits/stdc++.h>
#include "EasyLog.h"
#include <Windows.h>
using namespace std;
#define N 50
#define DAY 24
const int INF = 9999;
const int risk_bus = 2, risk_train = 5, risk_flight = 9;
string lg;

struct City{
    string name;
    double risk;
} city[N];
map<string, int> mp;//城市对应的编号 

struct Edge{
    int to, t;
    string wr;
    double r;
};
vector<Edge> edge[N * DAY]; //将一个城市拆成24个点 

struct P{
    int fa, t;
    string wr;//where, 状态 
};
P p[N * DAY][N * DAY];//p[i][j]表示i点在j时刻的父亲节点，时刻以及状态 

double dp[N * DAY][N * DAY]; //dp[i][j]表示i点在j时刻的最小风险值 

void add_edge(int st_city, int ed_city, int t, string wr, double r){
    edge[st_city].push_back({ed_city, t, wr, r});
}
int to_num(string s){
    int h = (s[0] - '0') * 10 + s[1] - '0';
    return h;
}
void error(string s){//报错，s是非法输入 
    lg = "the input _" + s + "_ is invalid";
    EasyLog::Inst() -> Log(lg);
    cout << "\n!!!Error!!!\n_" << s << "_ is invalid input\n" << endl;
}
void read(int &cnt, int R, string wr);
void Init();
void error(string s);
void solve(string name, int st_city, int ed_range, int max_time);
int main(){
    Init();
    EasyLog::Inst() -> Log("init the map");

    freopen("CON","r",stdin);
    while(1){
        string s, name, st_city, ed_city;
        int stra, t, start_time;

        cout << "please input the instruction (a => add a passenger, e => end)" << endl;

        cin >> s;

        if(s == "a"){ //add traveler
            cout << "please input the information about the passenger" << endl;
            cout << "name: "; cin >> name;
            cout << "starting city: "; cin >> st_city;
            if(!mp[st_city]){ error(st_city); continue;}
            cout << "destination city: "; cin >> ed_city;
            if(!mp[ed_city]){ error(ed_city); continue;}
            cout << "start time (0 - 23): "; cin >> start_time;
            if(start_time < 0 || start_time >= 24){ error(to_string(start_time)); continue;}
            cout << "travel strategy (0 / 1 => unlimited time / limited time): "; cin >> stra;
            if(stra != 0 && stra != 1){
                error(to_string(stra));
                continue;
            }
            t = INF;
            if(stra) {
                cout << "please input the limited time: ";
                cin >> t; //limited time
            }
            lg = "add passenger: " + name + " from " + st_city + " to " + ed_city + " in " + (stra ? "limited time " : "unlimited time ");
            if(stra) lg = lg + to_string(t) + "h";
            EasyLog::Inst() -> Log(lg);
            solve(name, mp[st_city] * DAY + start_time, mp[ed_city], t);
        }
        else if(s == "e"){ //end
            cout << "end" << endl;
            EasyLog::Inst() -> Log("end");
            break;
        }
        else{
            error(s);
        }
    }
    cout << "Thanks" << endl;
}
void solve(string name, int st_city, int ed_range, int max_t){
    for(int i = 0; i < N * DAY; i++){
        for(int j = 0; j < N * DAY; j++)
            dp[i][j] = INF;
    }
    dp[st_city][0] = 0;
    p[st_city][0] = {-1, 0, "start"};

    struct PQ{
        int ct, t;
        double r;
        bool operator > (const PQ &x) const {
            return r > x.r;//r越小，优先级越大，在优先队列中越早弹出 
        }
    };
    priority_queue <PQ, vector<PQ>, greater<PQ> > q;
    while(!q.empty()) q.pop();
    bool flag = false;
    int ed_city, ed_time;
    double ans_r;
    q.push({st_city, 0, 0});
    while(!q.empty()){//堆优化（priority_queue） 的dijkstra 
        int u = q.top().ct, t = q.top().t;
        double r = q.top().r;
        q.pop();
        if(u >= ed_range * DAY && u < ed_range * DAY + DAY) {// 终点范围 
            flag = true;
            ed_city = u, ed_time = t;
            ans_r = r;
            break;
        }
        for(int i = 0; i < edge[u].size(); i++){
            int v = edge[u][i].to, nt = t + edge[u][i].t;
            double nr = r + edge[u][i].r;
            string wr = edge[u][i].wr;
            if(nt > max_t) continue;
            if(nr < dp[v][nt]){
                dp[v][nt] = nr;
                p[v][nt] = {u, t, wr};
                q.push({v, nt, nr});
            }
        }
    }
    if(!flag){//限定时间内无法完成 
        cout << "\n" << name << " cannot reach " << city[ed_range].name << " from " << city[st_city / DAY].name << " within the limited time" << endl;
        lg = name + " cannot reach " + city[ed_range].name + " from " + city[st_city / DAY].name + " within the limited time";
        EasyLog::Inst() -> Log(lg);
    }
    else{
        struct ANS{//记录路径的数据结构 
            string st_city, ed_city, wr;
            int st_time, ed_time;
        };
        stack<ANS> AnsS;
        while(!AnsS.empty()) AnsS.pop();

        int now = ed_city, t = ed_time;
        string wr = p[now][t].wr;
        while(p[now][t].fa != -1){
            if(wr == "flight" || wr == "train" || wr == "bus")
                AnsS.push({city[p[now][t].fa / DAY].name, city[now / DAY].name, wr, p[now][t].fa % DAY, now % DAY});
            int tt = p[now][t].t;
            now = p[now][t].fa;
            t = tt;
            wr = p[now][t].wr;
        }

        cout << "\nthe risk is " << ans_r << ", and the rout is as follows: " << endl;
        queue<ANS> AnsQ; while(!AnsQ.empty()) AnsQ.pop();
        while(!AnsS.empty()){
            ANS u = AnsS.top(); AnsS.pop(); AnsQ.push(u);
            cout << u.wr << ", " << u.st_time << ":00 - " << u.ed_time << ":00, from " << u.st_city << " to " << u.ed_city <<endl;
        }
        for(int i = st_city % DAY; !AnsQ.empty(); i = (i + 1) % DAY){
            if(i == AnsQ.front().st_time){//状态是在交通工具上 
                for(; ; i = (i + 1) % DAY){
                    Sleep(500);
                    lg = to_string(i) + ":00, " + name + " is in " + AnsQ.front().wr + ", from " + AnsQ.front().st_city + " to " + AnsQ.front().ed_city;
                    EasyLog::Inst() -> Log(lg);
                    cout << i << ":00, " << name << " is in " << AnsQ.front().wr << ", from " << AnsQ.front().st_city << " to " << AnsQ.front().ed_city << endl;
                    if(i == AnsQ.front().ed_time) break;
                }
                AnsQ.pop();
            }
            else{//状态是在城市里 
                Sleep(500);
                lg = to_string(i) + ":00, " + name + " is in " + AnsQ.front().st_city;
                EasyLog::Inst() -> Log(lg);
                cout << i << ":00, " << name << " is in " << AnsQ.front().st_city << endl;
            }
            if(AnsQ.empty()) {//到达 
                Sleep(500);
                lg = to_string(i) + ":00, " + name + " arrives in " + city[ed_range].name;
                EasyLog::Inst() -> Log(lg);
                cout << i << ":00, " << name << " arrives in " << city[ed_range].name << endl;
                break;
            }
        }
    }
    cout << endl;
}
void Init(){
    freopen("input.txt", "r", stdin);
    int n, cnt_flight, cnt_train, cnt_bus;
    scanf("%*s%*s%d%*s", &n);
    lg = "there are " + to_string(n) + " cities";
    EasyLog::Inst() -> Log(lg);
    for(int i = 1; i <= n; i++){//输入城市 
        cin >> city[i].risk >> city[i].name;
        lg = "the risk of " + city[i].name + " is " + to_string(city[i].risk);
        EasyLog::Inst() -> Log(lg);
        mp[city[i].name] = i;
    }
    for(int i = 1; i <= n; i++){//将一个城市划分为24个时刻对应的24个点，使每个点都具有时空特性 
        for(int j = 0; j < DAY; j++){//假如点编号为Node，则 Node/DAY 为城市编号，Node%DAY为时刻 
            add_edge(i * DAY + j, i * DAY + (j + 1) % DAY, 1, city[i].name, city[i].risk);
        }
    }
    read(cnt_flight, risk_flight, "flight");//读入航班时刻表 
    read(cnt_train, risk_train, "train");//读入列车时刻表 
    read(cnt_bus, risk_bus, "bus");//读入巴士时刻表 
}
void read(int &cnt, int R, string wr){ 
    scanf("%*s%*s%*s%*s%*s");
    string s1, s2, s3, s4, lg;
    int st_city, ed_city, dt;
    double r;
    cin >> cnt;
    lg = "the number of " + wr + " is " + to_string(cnt);
    EasyLog::Inst() -> Log(lg);
    for(int i = 1; i <= cnt; i++){
        cin >> s1 >> s2 >> s3 >> dt >> s4;
        st_city = mp[s1] * DAY + to_num(s3);
        ed_city = mp[s2] * DAY + (to_num(s3) + dt) % DAY;
        r = 1.0 * R * city[mp[s1]].risk * dt;
        lg = to_string(to_num(s3)) + ":00 - " + to_string((to_num(s3) + dt) % DAY) + ":00, " + wr + ", from " + s1 + " to " + s2 + ", the risk is " + to_string(r);
        EasyLog::Inst() -> Log(lg);
        add_edge(st_city , ed_city, dt, wr, r);
    }
}
