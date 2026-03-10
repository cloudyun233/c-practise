// #include<bits/stdc++.h>

// using namespace std;

// //P1125 [NOIP 2008 提高组] 笨小猴
// bool judge(int x){//判断质数
//     int i = 0;
//     if(x < 2) return false;
//     for(int i = 2; i*i <= x; i++){
//         if(x % i == 0) {
//             return false;
//         }
         
//     }
//     return true;
// }

// int a[1000] = {0};
// int minn = INT_MAX,maxn = INT_MIN;
// int main(){
//     string s;
    
//     cin >> s;
//     for(int i = 0; i < s.size(); i++){
//         a[s[i]]++;
//     }

//     for(int i = 0; i < s.size(); i++){
//         minn = min(minn, a[s[i]]);
//         maxn = max(maxn, a[s[i]]); 
//     }

//     int x = maxn - minn;
//     if(judge(x)) cout << "Lucky Word" << endl << x;
//     else cout << "No Answer" << endl << 0;
// }


