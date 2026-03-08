#include<bits/stdc++.h>

using namespace std;

// P1048 [NOIP 2005 普及组] 采药
// struct Node{
//     int t;//时间
//     int v;//价值
// }a[101];

// int dp[1001];

// int main(){
//     int T, M;
//     cin >> T >> M;

//     for(int i = 0; i < M; i++){
//         cin >> a[i].t >> a[i].v;
//     }

//     for(int i = 0; i < M; i++){//遍历物品
//         for(int j = T; j >= a[i].t; j--){
//             dp[j] = max(dp[j], dp[j - a[i].t] + a[i].v);    
//         }
//     }

//     cout << dp[T] << endl;
// }

// P2871 [USACO07DEC] Charm Bracelet S
// struct Node{
//     int W;//重量
//     int D;//价值
// }a[3410];

// int dp[12881];
// int main(){
//     int N,M;
//     cin >> N >> M;

//     for(int i = 0; i < N; i++){
//         cin >> a[i].W >> a[i].D;
//     }

    
//     for(int i = 0; i < N; i++){
//         for(int j = M; j >= a[i].W; j--){
//             dp[j] = max(dp[j], dp[j - a[i].W] + a[i].D);
//         }
//     }

//     cout << dp[M] << endl;
//     return 0;
// }
