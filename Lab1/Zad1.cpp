#include <bits/stdc++.h>
using namespace std;

const int maxn = 1e6 + 7;
const int logn = 22;

long long st[logn][maxn];
long long t[maxn];
int lg[maxn];
int n;

long long NWW(long long a, long long b){
    if(a == -1 || b == -1) return -1;
    long long nwd = __gcd(a,b);
    long long nww = a * b / nwd;
    if(nww >= (1LL << 33)) nww = -1;
    return nww;
}
void log_precomp(){
    lg[1]  = 0;
    for(int i = 2; i < maxn; i++) lg[i] = lg[i/2] + 1;
}

void sparseTable_build(){
    for(int i = 1; i <= n; i++) st[0][i] = t[i];
    for(int i = 1; i < logn; i++){
        for(int j = 0; j + (1 << i) < maxn; j++){
            st[i][j] = NWW(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
        }
    }
}

long long query(int l, int r){
    if(l > r) return -1;
    int i = lg[r - l + 1];
    //cout << st[i][l] << " " << st[i][r - (1 << i) + 1] << endl;
    return NWW(st[i][l], st[i][r - (1 << i) + 1]);
}

int find_max(int i){
    int l = 0, r = i - 1;
    int res = 0;
    while(l <= r){
        int mid = (l + r)/2;
        long long nww = query(i - mid, i - 1);
        if(nww == -1 || t[i]%nww != 0) r = mid - 1;
        else{
            res = max(res, mid);
            l = mid + 1;
        }
    }
    return res;
}
int main(){
	cin >> n;
    for(int i = 1; i <= n; i++) cin >> t[i];
    sparseTable_build();
    log_precomp();
   // cout << query(1, 3) << endl;
   // cout << query(2,2) << endl;
    //cout << find_max(3) << endl;
    int res = 0;
    for(int i = 1; i <= n; i++) res = max(res, find_max(i));
    cout << res + 1;
}
