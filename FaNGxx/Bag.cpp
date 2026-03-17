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


//P1164 小 A 点菜
// int a[101], N, M, f[101][1001];
// int main(){
//     cin >> N >> M;
//     for(int i = 1; i <= N; ++i){
//         cin >> a[i];
//     }

//     for(int i = 0; i <= N; ++i){
//         f[i][0] = 1;
//     }

//     for(int i = 1; i <= N; ++i){
//         for(int j = 1; j <= M; ++j){
//             f[i][j] += f[i-1][j];//不买第i道菜
//             if(j >= a[i]) f[i][j] += f[i-1][j-a[i]];//买第i道菜
//         }
//     }

//     cout << f[N][M] << endl;
// }


//P1060 [NOIP 2006 普及组] 开心的金明
struct node{
    int v;//价格
    int w;//重要度
}p[26];
int f[26][30000];
int main(){
    int n, m;//n为总钱数，m为最大个数
    cin >> n >> m;
    for(int i = 1; i <= m; i++){
        cin >> p[i].v >> p[i].w;
        p[i].w *= p[i].v;
    }

    for(int i = 1; i <= m; i++){
        for(int j = 0; j <= n; j++){
            if(j >= p[i].v){
                f[i][j] = max(f[i-1][j], f[i-1][j-p[i].v] + p[i].w);
            }
            else f[i][j] = f[i-1][j];

        }
    }
    // for(int i = 1; i <= m; i++){
    //     for(int j = n; j >= p[i].v; j--){
    //         f[j] = max(f[j], f[j - p[i].v] + p[i].w);
    //     }
    // }

    cout << f[m][n] << endl;
}