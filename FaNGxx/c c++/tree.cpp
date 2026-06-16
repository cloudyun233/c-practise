#include<bits/stdc++.h>

using namespace std;

//P4913 【深基16.例3】二叉树深度
struct BiTNode{
   int l,r;
}BiTree[1000000];
int height = 0;
void findlen(int id, int n){
    if(id == 0) return;
    if(n > height) height = n;
    findlen(BiTree[id].l, n+1);
    findlen(BiTree[id].r, n+1);
}
int main(){
    int m;
    cin >> m;
    for(int i = 0; i < n; i++){
        cin >> BiTree[i].l >> BiTree[i].r;
    }
    findlen(1, 1);
    cout << height << endl;
}