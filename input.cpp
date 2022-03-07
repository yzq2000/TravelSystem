#include<bits/stdc++.h>
using namespace std;

int n;
struct{
    string name;
    double risk;
} city[50];
map<string, int> mp;
struct Transp{
    string start_city, end_city;
    int start_time, duration;
};
vector <Transp> flight, train, bus;

inline double rand_risk(){
    int r = rand() % 3;
    return r == 0 ? 0.2 : r == 1 ? 0.5 : 0.9;
}

void print_city();
void print_schedule(vector<Transp> v);
void generate_flight_schedule();
void generate_train_schedule();
void generate_bus_schedule();

int main(){
    srand(unsigned(time(NULL)));
    setiosflags(ios::right);
    freopen("input.txt", "w", stdout);
    print_city();
    generate_flight_schedule();
    generate_train_schedule();
    generate_bus_schedule();
}
void generate_bus_schedule(){
    int N = 50;
    for(int i = 1; i <= N; i++){
        int a = rand() % n + 1, b = rand() % n + 1;
        if(a == b) {--i; continue;}
        int st = rand() % 24, t = rand() % 5 + 1; // 1-5h
        bus.push_back({city[a].name, city[b].name, st, t});
    }
    cout << "This is the bus schedule: " << endl;
    print_schedule(bus);
}
void generate_train_schedule(){
    int N = 30;
    for(int i = 1; i <= N; i++){
        int a = rand() % n + 1, b = rand() % n + 1;
        if(a == b) {--i; continue;}
        int st = rand() % 24, t = rand() % 4 + 2; // 2-5h
        train.push_back({city[a].name, city[b].name, st, t});
    }
    cout << "This is the train schedule: " << endl;
    print_schedule(train);
}
void generate_flight_schedule(){
    int N = 10;
    for(int i = 1; i <= N; i++){
        int a = rand() % n + 1, b = rand() % n + 1;
        if(a == b) {--i; continue;}
        int st = rand() % 24, t = rand() % 3 + 1; // 1-3h
        flight.push_back({city[a].name, city[b].name, st, t});
    }
    cout << "This is the flight schedule: " << endl;
    print_schedule(flight);
}
void print_schedule(vector<Transp> v){
    cout << v.size() << endl;
    for(int i = 0; i < v.size(); i++){
        cout << setw(12) << v[i].start_city << "   " << setw(12) << v[i].end_city << "    ";
        cout.fill('0');
        cout << setw(2) << v[i].start_time << ":00    ";
        cout.fill(' ');
        cout << setw(2) << v[i].duration<< " h" << endl;
    }
}
void print_city(){
    freopen("city.txt", "r", stdin);
    cin >> n;
    for(int i = 1; i <= n; i++){
        cin >> city[i].name;
        mp[city[i].name] = i;
        city[i].risk = rand_risk();
    }
    cout << "There are " << n << " cities: \n";
    for(int i = 1; i <= n; i++)
        cout << city[i].risk << "   " << city[i].name << endl;
}
