#include<bits/stdc++.h>

using namespace std;


// const int Max = 5000;
// int a[Max],b[Max],ans[Max];
// int M;

// int fl(int N){
//     if(N == 1) ans[1] = 1;
//     if(N == 2) ans[1] = 2;

//     a[1] = 1;
//     b[1] = 2;
//     M = 1;

//     for(int i = 3; i <= N; i++){
//         for(int j = 1; j <= M; j++) ans[j] = a[j] + b[j];
//         for(int j = 1; j <= M; j++){
//             if(ans[j] >= 10){
//                 ans[j] = ans[j]%10;
//                 ++ans[j+1];
//                 if(j == M) ++M;
//             }
            
//         }
        
//         for(int j = 1; j <= M; j++){
//             a[j] = b[j];
//             b[j] = ans[j];
//         }
//     }
//     return 0;
// }

// int main(){
//     int N;
//     cin >> N;
//     fl(N);
//     for(int i = M;i >= 1; i--){
//         cout << ans[i];
//     }
//     cout << endl;
// }