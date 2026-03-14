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


// int a,b,n,m;
// bool ma[23][23];
// long long x[23][23];
// void work(int x,int y){
// 	ma[x][y]=true;
// 	ma[x-1][y-2]=true;
// 	ma[x-2][y-1]=true;
// 	ma[x-2][y+1]=true;
// 	ma[x-1][y+2]=true;
// 	ma[x+1][y-2]=true;
// 	ma[x+2][y-1]=true;
// 	ma[x+2][y+1]=true;
// 	ma[x+1][y+2]=true;
// }
// int main(){
// 	cin>>n>>m>>a>>b;
// 	a+=2;
// 	b+=2;
// 	n+=2;
// 	m+=2;
// 	work(a,b);
// 	x[2][2]=1;
// 	for(int i=2;i<=n;i++){
// 		for(int j=2;j<=m;j++){
// 			if(i==2&&j==2) continue;
// 			if(!ma[i][j]) x[i][j]=x[i-1][j]+x[i][j-1];
// 		}
// 	}
// 	cout<<x[n][m];
// 	return 0;
// }

//leecode91密码解码

//暴力
// int f1(string s, int i){
//     if(i == s.length()){
//         return 1;
//     }

//     int ans;
//     if(s[i] == '0'){
//         ans = 0;
//     }
//     else{
//         ans = f1(s, i+1);
//         if(i+1 < s.length() && ((s[i] - '0')*10 + (s[i+1] - '0')) <= 26){
//             ans += f1(s, i+2);
//         }
//     }
//     return ans;
// }

// int main(){
//     string s;
//     int i = 0;
//     cin >> s;
//     int answer = f1(s, i);
//     cout << answer << endl;
// }

//记忆化搜索
// int f1(string s, int i, vector<int> dp){
//     if(i == s.length()){
//         return 1;
//     }

//     if(dp[i] != -1){
//         return dp[i];
//     }
//     int ans;
//     if(s[i] == '0'){
//         ans = 0;
//     }
//     else{
//         ans = f1(s, i+1, dp);//检查1位
//         if(i+1 < s.length() && ((s[i] - '0')*10 + (s[i+1] - '0')) <= 26){//检查2位
//             ans += f1(s, i+2, dp);
//         }
//     }
//     dp[i] = ans;
//     return ans;
// }

// int main(){
//     string s;
//     cin >> s;
//     vector<int> dp(s.size(), -1);
//     int answer = f1(s, 0, dp);
//     cout << answer << endl;
// }

int f1(string s, vector<int>& dp){
    int i;
    int n = s.size();

    dp[n] = 1;
    for(i = n-1; i >= 0; i--){
        if(s[i] == '0') dp[i] = 0;
        else{
            dp[i] = dp[i+1];
            if(i+1 < n && ((s[i] - '0')*10 + (s[i+1] - '0')) <= 26){
                dp[i] += dp[i+2];
            }
        }
    }
    return dp[0];
}

int main(){
    string s;
    cin >> s;
    vector<int> dp(s.size()+1);
    int answer = f1(s, dp);
    cout << answer << endl;
}