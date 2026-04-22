#include<bits/stdc++.h>

using namespace std;

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


// int main(){
//     string s;
//     int n = 0;
//     getline(cin, s);
//     for(char c : s){
//         if(c != ' ' && c != '\n') n++;
//     }
//     cout << n << endl;
// }

// int main(){
//     string s,l;
//     int q;//操作次数
//     cin >> q >> s;
//     for(int i = 0; i < q; i++){
//         getline(cin, l);
//         if(l[0] == '1'){
//             string str = l.substr(2);
//             str.insert(0, s);
//             cout << str << endl;
//         }
//         else if(l[0] == '2'){
//             s.substr(l.substr(2,1), l.substr(4,1))
//             cout << s << endl;
//         }
//         else if(l[0] == '3'){
            
//         }
//     }
// }


//P1308 [NOIP 2011 普及组] 统计单词数
// string s, t;
// int counts = 0;
// int sta = -1;
// string temp;

// int main(){
// 	getline(cin, s);
// 	getline(cin, t);
// 	t += ' ';

// 	for(char &c: s){
// 		c = tolower(c);
// 	} 
// 	for(char &c: t){
// 		c = tolower(c);
// 	} 
	
// 	int len = s.size();
	
// 	for(int i = 0; i < t.size(); i++){
// 		if(t[i] == ' '){
// 			if(temp == s){
// 				counts++;
// 				if(sta == -1) sta = i - len;
// 			}
//             temp = "";
// 		}
// 		else temp += t[i];
// 	}
	
// 	if(sta != -1){
// 		cout << counts << " " << sta << endl;
// 	} 
// 	else cout << "-1" << endl;
	
// } 

// int main(){
//     int N;
//     cin >> N;
//     vector<long long> a(N);
    
//     for(int i = 0; i < N; i++){
//         cin >> a[i];
//     }

//     sort(a.begin(), a.end());
//     for(int i = 0; i < N; i++){
//         cout << a[i] << (i == N -1 ? "" : " ");
//     }
// }


//P5734 【深基6.例6】文字处理软件
// int main(){
// 	int q;
// 	int i, j;
// 	string doc;
// 	cin >> q >> doc;
// 	string s[q+1];
// 	for(i = 0; i <= q; i++){
// 		getline(cin,s[i]);
// 	}
// 	for(i = 1; i <= q; i++){
// 		//1st
// 		if(s[i][0] == '1'){
// 			for(j = 2; j < s[i].size(); j++){
// 				doc += s[i][j];
// 			}
// 			cout << doc << endl;
// 		}
// 		//2nd&3rd
// 		if(s[i][0] == '2' || s[i][0] == '3'){
// 			string a,b;
// 			for(j = 2; j < s[i].size(); j++){
// 				if(s[i][j] != ' '){
// 					a += s[i][j];
// 				}
//                 else break;
// 			}
// 			for(int k = j; k < s[i].size(); k++){
// 				b += s[i][k];
// 			}	
// 			if(s[i][0] == '2'){
// 				int a1 = stoi(a);
// 				int b1 = stoi(b);
// 				string temp;
// 				for(j = a1; j < a1+b1; j++){
// 					temp += s[i][j];
// 				}
// 				cout << temp << endl;				
// 			}
// 			if(s[i][0] == '3'){
// 				int a1 = stoi(a);
//                 string temp1 = doc;
// 				temp1.insert(a1, b);
// 				cout << temp1 << endl;
// 			}
// 		}
// 		//4th
// 		if(s[i][0] == '4'){
// 			string a;
// 			for(j = 2; j < s[i].size(); j++){
// 				if(s[i][j] != ' '){
// 					a += s[i][j];
// 				}
// 			}
// 			for(j = 0; j < doc.size(); j++){
// 				if(doc.substr(j, a.size()) == a) {
//                     cout << j << endl;
//                     break;
//                 }
// 				else cout << "-1" << endl;
				
// 			}
// 		}
// 	} 
// }


//P1321 单词覆盖还原
// int main(){
// 	string s;
// 	cin >> s;
// 	int boy = 0,girl = 0;
// 	int i = 0;
// 	for(i = 0; i < s.size(); i++){
// 		if(s[i] == '.' || s[i+1] == '.' || s[i+2] == '.') continue;
// 		if(s[i] == 'b' || s[i+1] == 'o' || s[i+2] == 'y'){
// 			boy++;
// 		}
// 		if(s[i] == 'g' || s[i+1] == 'i' || s[i+2] == 'r' || s[i+3] == 'l'){
// 			girl++;
// 		}
// 	}
// 	cout << boy << endl << girl << endl;
// }


//P3370 【模板】字符串哈希
//不会

//P2543 [AHOI2004] 奇怪的字符串

//1暴力（有问题）
// int main(){
//     string x, y;
//     cin >> x >> y;
//     int i=0, j=0, len[9999] = {0};
//     for(i = 0; i < x.size(); i++){
//         int k = i;
//         j = 0;
//         while(k < x.size()){
//             if(x[k] == y[j]){
//                 k++;
//                 len[i]++;
//             } 
//             else{
//                 if(j >= y.size()){
//                     break;
//                 } 
//             }
//             j++;
//         }
//         if(k == x.size()) break;
//     }

//     int max_len = 0;
//     for(j = 0; j <= i; j++){
//         max_len = max(len[i], max_len);
//     }
//     cout << max_len << endl;
// }

//dp
// int dp[10000][10000] = {0};
// int main(){
//     string x, y;
//     cin >> x >> y;
//     int i, j;
//     for(i = 1; i <= x.size(); i++){
//         for(j = 1; j <= y.size(); j++){
//             dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
//             if(x[i-1] == y[j-1]) dp[i][j] = max(dp[i][j], dp[i-1][j-1]+1);
//         }
//     }

//     cout << dp[x.size()][y.size()] << endl;
// }

//pipoj1026: a+b问题
string s;//表达式
int main(){
    while(getline(cin, s)){
        string t[2];
        int i = 0, j = 0;
        while(i < s.size()){
            if(s[i] == '='){
                break;
            }

            if(s.substr(i,3) == "one"){
                i += 4;
                t[j] += '1';
            }
            else if(s.substr(i,3) == "two"){
                i += 4;
                t[j] += '2';
            }
            else if(s.substr(i,5) == "three"){
                i += 6;
                t[j] += '3';
            }
            else if(s.substr(i,4) == "four"){
                i += 5;
                t[j] += '4';
            }
            else if(s.substr(i,4) == "five"){
                i += 5;
                t[j] += '5';
            }
            else if(s.substr(i,3) == "six"){
                i += 4;
                t[j] += '6';
            }
            else if(s.substr(i,5) == "seven"){
                i += 6;
                t[j] += '7';
            }
            else if(s.substr(i,5) == "eight"){
                i += 6;
                t[j] += '8';
            }
            else if(s.substr(i,4) == "nine"){
                i += 5;
                t[j] += '9';
            }
            else if(s.substr(i,4) == "zero"){
                i += 5;
                t[j] += '0';            
            }
            else if(s[i] == '+'){
                i += 2;
                j++;
            }
            else break;
        }
        if(t[0] == "0" && t[1] == "0") return 0;
        else{
            int a = stoi(t[0]);
            int b = stoi(t[1]);
            int ans = a + b;
            cout << ans << endl;

        }

    }

}