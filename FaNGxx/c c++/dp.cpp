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

//记忆化搜索2
// int f1(string s, vector<int>& dp){
//     int i;
//     int n = s.size();

//     dp[n] = 1;
//     for(i = n-1; i >= 0; i--){
//         if(s[i] == '0') dp[i] = 0;
//         else{
//             dp[i] = dp[i+1];
//             if(i+1 < n && ((s[i] - '0')*10 + (s[i+1] - '0')) <= 26){
//                 dp[i] += dp[i+2];
//             }
//         }
//     }
//     return dp[0];
// }

// int main(){
//     string s;
//     cin >> s;
//     vector<int> dp(s.size()+1);
//     int answer = f1(s, dp);
//     cout << answer << endl;
// }


//P1028 [NOIP 2001 普及组] 数的计算

// int f[1001] = {0};
// int main(){
//     int n;
//     f[1] = 1;
//     f[2] = 2;
//     f[3] = 2;
//     cin >> n;
//     for(int i = 4; i <= n; i++){
//         for(int j = 1; j <= i/2; j++){
//             f[i] += f[j];
//         }
//         f[i]++;
//     }

//     cout << f[n] << endl;
// }


//P2437 蜜蜂路线
//1滚动变量
// int main(){
//     int m, n;
//     cin >> m >> n;
//     int i;
//     int lastlast = 1;
//     int last = 1;
//     int cur;
//     for( i = m+2; i <= n; i++){
//         cur = lastlast + last;
//         lastlast = last;
//         last = cur;
//     }

//     cout << last << endl;
// }

//2递归
// int find(int m, int n,  vector<int> dp){
//     if(n == m) return 1;
//     if(n == m+1) return 1;

//     if(dp[n] != -1) return dp[n];
//     int ans = find(m, n-1, dp) + find(m, n-2, dp);
//     dp[n] = ans;
//     return ans;
// }
// int main(){
//     int m, n;
//     cin >> m >> n;
//     vector<int> dp(1001, -1);
//     int ans = find(m, n, dp);
//     cout << ans << endl;
// }

//3高精度版
// const int N = 1000;
// int a[N], b[N], ans[N];
// int l, i, j;
// int main(){
//     int m, n;
//     cin >> m >> n;
//     a[m] = 1;
//     b[m] = 1;
//     l = m;

//     for(i = m+2; i <= n; i++){
//         for(j = m; j <= l; j++) ans[j] = a[j] + b[j];
//         for(j = m; j <= l; j++){
//             if(ans[j] >= 10){
//                 ans[j] = ans[j]%10;
//                 ++ans[j+1];
//                 if(j == l) l++; 
//             }
//         }

//         for(j = m; j <= l; j++){
//             b[j] = a[j];
//             a[j] = ans[j];
            
//         }
//     }

//     for(i = l; i >= m; i--){
//         cout << ans[i];
//     }
// }

//P1464 [PacNW 1999] Function
// int main(){
//     int a, b ,c;
//     while (cin >> a >> b >> c)
//     {
        
//     }
    
// }

//P1036 [NOIP 2002 普及组] 选数
int ans, a[30], k, n;
bool isprime(int x){//判断素数
    if(x == 1) return false;
    for(int i = 2; i*i <= x; i++){
        if(x % i == 0) return false;
    }

    return true;
}

void dfs(int now,int sum, int j){//
    if(now == k){
        if(isprime(sum)) ++ans;
        return;
    }

    for(int i = j; i <= n - (k - now - 1); ++i){
        dfs(now+1, sum + a[i], i+1);
    }
    return;
}

int main(){
    cin >> n >> k;
    for(int i = 1; i <= n ;++i){
        cin >> a[i];
    }
    dfs(0, 0, 1);
    cout << ans << endl;
}