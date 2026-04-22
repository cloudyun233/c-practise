#include<bits/stdc++.h>

using namespace std;

typedef struct m{
   int data;
}m[30];
double fun(int n){
   if(n == 1) return 1;
   else return n*n + fun(n-1);
}
int main(){
   cout << fun(3) << endl;

}