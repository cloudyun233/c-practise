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
string s, t;
int counts = 0;
int sta = -1;
string temp;

int main(){
	getline(cin, s);
	getline(cin, t);
	t += ' ';

	for(char &c: s){
		c = tolower(c);
	} 
	for(char &c: t){
		c = tolower(c);
	} 
	
	int len = s.size();
	
	for(int i = 0; i < t.size(); i++){
		if(t[i] == ' '){
			if(temp == s){
				counts++;
				if(sta == -1) sta = i - len;
			}
            temp = "";
		}
		else temp += t[i];
	}
	
	if(sta != -1){
		cout << counts << " " << sta << endl;
	} 
	else cout << "-1" << endl;
	
} 