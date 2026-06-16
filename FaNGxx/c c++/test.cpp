
#include<bits/stdc++.h>

using namespace std;

// int main(void){
//     set <int> s;
//     s.insert(2);
//     s.insert(4);
//     s.insert(5);
//     for(auto p = s.begin(); p != s.end(); p++){
//         cout << *p << endl;
//     }
//     cout << (s.find(5) != s.end()) << endl;
// }


// int main(void){
//     map <string,int> m;
//     m["hello"] = 1;
//     m["zytsb"] = 10;
//     m["zytdl"] = 100;

//     for(auto p = m.begin();p != m.end();p++){
//         cout << p->first << ":" << p->second << endl;
//     }
// }


// int a[105];
// int main() {
//     int n;
//     scanf("%d", &n);
//     for (int i = 0; i < n; i++) {
//         scanf("%d", &a[i]);
//     }
//     sort(a, a+n);
//     for (int i = 0; i < n; i++)
//     printf("%d ", a[i]);
//     return 0;
// }


// int main(){
//     int count = 0;
// 	for(auto a = 100; a != 1001;a++){
// 		if(a%30 == 0){
//             if(count%10 == 0 && count != 0){
//                 cout << endl;
//             }
//             else if(count != 0) cout << " " ;
//             cout << a;
//             count++;
//         }
//     }
// }

int main(){
    int m,n;
    
    while(cin >> m >> n){
        bool isfind = false;
        if(m == 0 && n == 0) break;
        for(auto i = m;i <= n;i++){
            int x = pow(i/100,3) + pow(i%100/10,3) + pow(i%10,3);
            if(x == i) {
                cout << i << " ";
                isfind = true;
            }
        }
        if(isfind == false) cout << "no";
        cout << endl;
    }
    return 0;
}


// int main(){
//     int a,b,c;
//     cin >> a >> b >> c;
//     cout << max({a,b,c}) << endl;
// }


// int main(){
//     int a; int n;
//     int temp = 0;
//     int Sn = 0;
//     cin >> a >> n;
//     for(n;n != 0;n--){
//         temp = 10*temp + a;
//         Sn = temp + Sn;
//     }
//     cout << Sn << endl;
// }

// int main(){
//     int a,b,c;
//     int sum = 0;
//     auto pow_val = 0;
//     double sum_reverse = 0;
//     cin >> a >> b >> c ;
//     for(int i = 1;i <= a;i++){
//         sum += i;
//     }
//     for(int i = 1;i <= b;i++){ 
//         pow_val += pow(i,2);
//     }
//     for(int i = 1;i <= c;i++){
        
//         sum_reverse += pow(i,-1);
//     }
//     double sum_all = sum + pow_val + sum_reverse;
//     cout << fixed << setprecision(2);
//     cout << sum_all << endl;
// }


// int func(int n){
//     if(n == 0 || n == 1) return 1;
//     if(n == 2) return 2;
//     if(n > 2) return func(n - 1) + func(n - 2) + func(n - 3);
// }

// int main(){
//     int n;
//     int x;
//     while(cin >> n){
//         x = func(n);
//         cout << x << endl;
//     }  
// }

// int main(){
//     int n;
//     int sum = 0;
//     cin >> n;
//     for(int i = 1;i <= n; i++){
//         sum += i;
//     }
//     cout << sum << endl;
// }

// int main(){
//     string s;
//     cin >> s;
//     for(int i = s.length()-1; i >= 0; i--){
//         cout << s[i];
//     }
//     cout << endl;
// }



// int main(){
//     int month, day;
//     int restday;
//     cin >> month >> day;
    
//     int temp[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31}; // 修正：正常月份天数
//     string zyt[7] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
    
//     if(month == 4){
//         restday = day - 12;
//         cout << zyt[(restday+4)%7] << endl; // 添加换行符保持一致性
//     }
//     else if(month > 4){  // 使用else if确保逻辑互斥
//         restday = day - 12;
//         for(int i = 5; i <= month; i++){
//             restday += temp[i-1]; // 注意这里应该是i-1，因为我们希望加上第i-1个月的天数
//         }
//         cout << zyt[(restday+4)%7] << endl; // 修正公式
//     }
//     else if(month < 4){ // 添加对小于4月的处理
//         restday = day - 12;
//         for(int i = month; i <= 3; i++){
//             restday -= temp[i-1];
//         }
//         cout << zyt[(restday+4)%7] << endl;
//     }
    
//     return 0;
// }

// int main(){
//     string s,res;
//     cin >> s;
//     for(char c : s){
//         if(!isdigit(c)) res += c;
//     }
//     for(char c : s){
//         if(isdigit(c)) res += c;
//     }
//     cout << res << endl;
// }

// int main(){
//     string s;
//     getline(cin,s);
//     for(char &c : s){
//         if(c >= 'A' && c <= 'Z'){ //大写字母
//             c = (c - 'A' + 3)%26 + 'A';
//         }
//         if(c >= 'a' && c <= 'z'){ //大写字母
//             c = (c - 'a' + 3)%26 + 'a';
//         }
//     }
//     cout << s << endl;
// }

// int main(){
//     string s,a,b,others;
//     cin >> s;
//     for(char &c : s){
//         if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) a += c;
//         else if(c >= '0' && c <= '9') b += c;
//         else others += c;
//     }
//     cout << a << endl << b << endl << others << endl;
// }

// int main(){
//     string s;
//     getline(cin, s);
//     while(s.find("gzu") != string :: npos || s.find("Gzu") != string :: npos){
//         s.erase(s.find("gzu"),3);
//     }
//     cout << s << endl;
// }

// int main(){
//     string s;
//     getline(cin,s);
//     int i,j;
//     int len = s.length();
//     while(true){
//         bool found = false;
//         for(i = 0;i < len - 3;++i){
//             if((s[i] == 'G'||s[i] == 'g') && (s[i+1] == 'Z'||s[i+1] == 'z') && (s[i+2] == 'U'||s[i+2] == 'u')){
//                 s.erase(i,3);
//                 found = true;
//                 break;
//             }
//         }
//         if(!found) break;
//     }
//     cout << s << endl;
// }


// int main(){
//     int n;
//     cin >> n;
//     while(n--){
//         string a,b;
//         cin >> a >> b;
//         sort(a.begin(),a.end());
//         sort(b.begin(),b.end());
//         if(a == b) cout << "Yes" << endl;
//         else cout << "No" << endl;
//     }
    
// }

// int main(){
//     int n,i,j;
//     long long sum = 0;
//     cin >> n;
//     if(n == 0) sum = 1;
//     else{
//         for(i = 1;i <= n;i++){
//             long long temp = 1;
//             for(j = 1;j <= i;j++){     
//                 temp *= j;
//             }
//             sum += temp;
//         }
//     }
//     cout << sum << endl;
// }

//杨辉三角
// int main(){
//     int n,i,j;
//     while(cin >> n){
//         if(n == 0) break;
//         int arr[20][20] = {{0}};
//         for(i = 0;i < n;i++){
//             arr[i][0] = 1;

//         }
//         for(i = 1;i < n;i++){
//             for(j = 1;j < i+1;j++){
//                 arr[i][j] = arr[i-1][j-1] + arr[i-1][j];
//             }
//         }
//         for(i = 0; i < n; i++){
//         bool first = true; // 标记是否是当前行的第一个有效数字
//         for(j = 0; j < n; j++){
//             if(arr[i][j] != 0){
//                 if (!first) {
//                     cout << " "; // 如果不是第一个数字，则先输出空格
//                 }
//                 cout << arr[i][j]; // 输出当前数字
//                 first = false;     // 更新标记
//             }
//         }
//         cout << endl; // 每行结束后换行
//         }
//     }
// }

// int main(){
//     int arr[10];
//     int height;
//     int num = 0;
//     for(int i = 0;i < 10;i++){
//         cin >> arr[i];
//     }
//     cin >> height;
//     for(int i = 0;i < 10;i++){
//         if(height >= arr[i] || height + 30 >= arr[i]){
//             num++;
//         }
//     }
//     cout << num << endl;
// }

