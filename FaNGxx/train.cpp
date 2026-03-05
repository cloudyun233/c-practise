#include<bits/stdc++.h>

using namespace std;

// int main(){
//     int A,B,C,D;
//     while(cin >> A >> B >> C >> D){
        
//     }
// }


// int main(){
//     double x1,y1,x2,y2,x3,y3;
//     while(cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3){
//         double a = sqrt(pow(x1-x2,2) + pow(y1 - y2,2));
//         double b = sqrt(pow(x1-x3,2) + pow(y1 - y3,2));
//         double c = sqrt(pow(x2-x3,2) + pow(y2 - y3,2));
//         double p = (a+b+c)/2;
//         double s = p*(p - a)*(p - b)*(p - c);
//         double S = (s >= 0) ? sqrt(s) : 0.00;
//         cout << fixed << setprecision(2) << S << endl;
//     }
// 	return 0;
// }

// int main(){
//     double D,X,Y,T;
//     while(cin >> D >> X >> Y >> T){
//         if(T + D/X >= D/Y) cout << "NO" << endl;
//         else cout << "YES" << endl;
//     }
// }

// int main(){
//     int n,i,j; 
//     int temp;

//     cin >> n;
//     int arr[n][n];
//     for(i = 0;i < n;i++){
//         for(j = 0;j < n;j++){
//             cin >> arr[i][j];
//         }
//     }

//     for(i = 0;i < n/2;i++){
//         for(j = 0;j < n;j++){
//             temp = arr[i][j];
//             arr[i][j] = arr[n - i - 1][j];
//             arr[n - i - 1][j] = temp;
//         }
//     }

//     for(i = 0;i < n;i++){
//         for(j = 0;j < n;j++){
//             cout << arr[i][j] << " ";
//         }
//         cout << endl;
//     }
// }

// int main(){
//     int n,i,j; 
//     int temp;

//     cin >> n;
//     int arr[n][n];
//     int s[n] = {0};
//     for(i = 0;i < n;i++){
//         for(j = 0;j < n;j++){
//             cin >> arr[i][j];
//         }
//     }

//     for(j = 0;j < n;j++){
//         for(i = 0;i < n;i++){
//             s[j] += arr[i][j];
//         }
//         cout << s[j] << " ";
//     }
//     cout << endl;
// }

// int main(){
// 	string str,substr;
// 	while(cin >> str >> substr){
//         int p = 0;
// 	    for(int i = 0; i < str.size(); i++){
//         if(str[i] > str[p]) p = i;
//         }
//         str = str.insert(p+1,substr);
//         cout << str << endl;
//     }
		
// }


// int main(){
// 	double k;
//     double x;
// 	string a,b; 
// 	cin >> k >> a >> b;
	

	
// 	int len = a.size(); 
// 	int n = 0;
// 	for(int i = 0; i < len; i++){
// 		if(a[i] == b[i]) n++;
// 	}  	
// 	x = static_cast<double>(n) / len;
// 	if(x >= k) cout << "yes";
// 	else cout << "no"; 	
// 	return 0; // 添加返回值
// }


// int main(){
// 	string s,s1,s2;
// 	getline(cin,s,',');
// 	getline(cin,s1,',');
// 	getline(cin,s2);
// 	int a = s.find(s1);
// 	int b = s.rfind(s2);
// 	if(a == -1 || b == -1){
//         cout << "-1" << endl;
//     }   
//     else{
//         if(a+s1.size() > b) cout << "-1" << endl;
//         else cout << b - a - s1.size() << endl;
//     }     
// }

// int main(){
// 	string a;
//     char b[100];
// 	getline(cin, a);
// 	int len = a.size();
// 	for(int i = 0;i < len;i++){
// 		int sum = a[i] + a[(i+1)%len];
// 		b[i] = (char)sum;
// 	}
//     b[len] = '\0';
// 	cout << b << endl;
// }

// int main(){
// 	int N,M;
// 	string P,S,s;
// 	cin >> N >> P >> M >> S;
// 	for(int i = 0;i < S.size()-N+1;i++){
//         s = S.substr(i,N);
// 		if(s == P) cout << i << ' ';
// 	}
// }

// int main(){
// 	string s1,s2,s;
// 	int j = 0;
// 	cin >> s1 >> s2;
// 	int l1 = s1.size();
// 	int l2 = s2.size();
// 	bool find = true;
// 	for(int i = 0;i < l2;i++){
// 		if(s2[i] == s1[(i + j)%l1]) continue;
// 		else i = 0; j++;
// 		if(j == l1 - 1){
// 			find = false;
// 			break;
// 		}
// 	}
// 	if(find){
// 		cout << "true" << endl;
// 	}
// 	else cout << "false" << endl;
// } 

// int main(){
// 	const int n = 6;
// 	int N;
// 	string s[n];
// 	string b;
// 	while(cin >> N){
//         if(N == 0) break;
//         bool find = true;
//         int k = 1;
//         for(int i = 0; i < N; i++){
//             cin >> s[i];
//             reverse(s[i].begin(),s[i].end());
//         }
        
//         while(k <= s[0].size() && find){
            
            
//             for(int i = 0; i < N-1; i++){
//                 if(k > s[i].size() || k > s[i+1].size()){
//                     find = false;
// 				    break;
//                 }
//                 if(s[i].substr(0,k) != s[i+1].substr(0,k)){
//                     find = false; 
//                     break;
//                 } 
//             }
//             if(find) k++;
            
//         }
        
//         if(k == 1)	cout << "" << endl;
//         else {
//             b = s[0].substr(0,k-1);
//             reverse(b.begin(),b.end());
//             cout << b << endl;
//         } 
//     }
       
// }

// string del(string &s){
//     string spe = "-;_";
//     for(int i = 0;i < s.size();i++){
//         if(spe.find(s[i]) != string :: npos){
//             s.erase(i,1);
//             --i;
//         }
//     }
//     for(char &c : s){
//         //if(c < 97) c = c + 32;
//         c = tolower(c);
//     }
//     return s;
// }
// int main(){
//     string s1,s2,s3;
//     int n;
//     const int N = 1010;
//     string q[N];
//     cin >> s1 >> s2 >> s3;
//     cin >> n;
//     for(int i = 0;i < n;i++){
//         cin >> q[i];
//     }
    
//     del(s1);del(s2);del(s3);//删除特殊字符并全变小写
//     for(int i = 0;i < n;i++){
//         del(q[i]);
//     }

//     int l1 = s1.size();
//     int l2 = s2.size();
//     int l3 = s3.size();

    
//     bool match = true;
//     for(int i = 0;i < n;i++){
//         int k = 0;
//         match = true;
//         while(match && k < q[i].size()){
//             bool matched = false;

//             if(q[i].substr(k,l1) == s1 && k + l1 <= q[i].size()) {
//                 k += l1;matched = true;
//             }
//             else if(q[i].substr(k,l2) == s2 && k + l2 <= q[i].size()) {
//                 k += l2;matched = true;
//             }
//             else if(q[i].substr(k,l3) == s3 && k + l3 <= q[i].size()) {
//                 k += l3;matched = true;
//             }
//             if(!matched) match = false;
//         }

//         if(match && k == q[i].size()) cout << "ACC" << endl;
//         else cout << "WA" << endl;
//     }
// }


// int main(){
//     int n,m;
//     string s;
//     cin >> n >> m;
//     cin >> s;
//     int i = 0;

//     while(i <= s.size()){    
//         if(s[i] != '0' ){
//             i = i+2;
//         }
//         else{
//             // 检查左边：左边界或左边是'0'
//             bool left_ok = (i == 0) || (s[i-1] == '0');
            
//             // 检查右边：右边界或右边是'0'
//             bool right_ok = (i == s.size()-1) || (s[i+1] == '0');
            
//             // 至少有一边是'0'就可以修改
//             if(left_ok && right_ok) {
//                 s[i] = '1';
//                 --m;
//             }
//             i++;   
//         }
//         if(m == 0) break;
//     }

//     if(m != 0) cout << "false" << endl;
//     else cout << "true" << endl;
// }






int main(){
    int n,k;
    const int N = 10;
    cin >> n >> k;
    int a[N][N],s[N][N],temp[N][N] = {0};
    
    int i,j,m = 0;
    for(i = 0;i < n;i++){//读入矩阵
        for(j = 0;j < n;j++){
            cin >> a[i][j];
        }
    }

    for(i = 0;i < n;i++){
        temp[i][i] = 1;
    }

    while(k > 0){
        memset(s, 0, sizeof(s));
        for(i = 0;i < n;i++){
            for(j = 0;j < n;j++){
                for(m = 0;m < n;m++){
                    s[i][j] += temp[i][m]*a[m][j];
                }         
            }
        }
        memcpy(temp, s, sizeof(temp));
        --k;
    }
             
    for(i = 0;i < n;i++){
        for(j = 0;j < n;j++){
            cout << s[i][j] << " ";
        }
        cout << endl;
    }
}



