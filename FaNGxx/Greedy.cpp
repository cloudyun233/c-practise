#include<bits/stdc++.h>

using namespace std;

//P1223 排队接水
// struct Node{
//     int T;
// }P[1001];

// long long int t[1001] = {0};
// int main(){
//     long int x;
//     int n;
//     cin >> n;
//     for(int i = 1; i <= n; i++){
//         cin >> x;
//         t[i] = 1001*x + i;
//     }

//     sort(t + 1, t + n + 1);

//     double sum = 0;
//     for(int i = 1; i <= n; i++){
//         cout << t[i]%1001 << " ";
//         sum += t[i]/1001*(n - i);
        
//     }
//     cout << endl;
//     double ave = sum / n;
//     cout << fixed << setprecision(2) << ave << endl;
// }


struct Node{
    int a;//开始时间
    int b;//结束时间
}p[1000001];

bool cmp(Node x, Node y){
    if(x.b != y.b) return x.b < y.b;
    else return x.a > y.a;
}
int main(){
    int n;
    cin >> n;
    int i,j = 0;
    for(i = 1; i <= n; i++){
        cin >> p[i].a >> p[i].b;
    }

    sort(p + 1, p + n + 1, cmp);

    int sum = 0;
    int end = -1;
    for(j = 1; j <= n; j++){
        if(p[j].a >= end){
            end = p[j].b;
            sum++;
        }
    }
    cout << sum << endl;
}


