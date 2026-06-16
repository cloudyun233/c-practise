#include<bits/stdc++.h>

using namespace std;

int main(){
	string s;
	while(cin >> s){
		if(s == ".") break;
		int len = s.size();
		int n = 0;
		int max_n = 1;
		for(n = len;n >= 2;--n){
			if(len % n != 0) continue;
			int l = len / n; //子串长度
			string a = s.substr(0,l); //子串
			bool valid = true;
			for(int i = 1;i < n;i++){
				if(s.substr(i*l,l) != a) {
					valid = false;
					break;
				}
			}
			if(valid){
				max_n = n;
				break;
			}
		}
		cout << max_n << endl;
	}
	
}
