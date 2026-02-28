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

int main(){
	string str,substr;
	cin >> str >> substr;
	int p = 0;
	for(int i = 0; i < str.size(); i++){
        if(str[i] < str[p]) p = i;
    }
    str = str.insert(p+1,substr);
    cout << str << endl;
		
}