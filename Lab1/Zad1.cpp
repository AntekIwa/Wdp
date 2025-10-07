#include <stdio.h>
#include <stdlib.h>

#define MAXN 1000007
#define LOGN 22

long long st[LOGN][MAXN];
long long t[MAXN];
int lg[MAXN];
long long n;

long long gcd_ll(long long a, long long b) {
    while (b != 0) {
        long long tmp = a % b;
        a = b;
        b = tmp;
    }
    return a;
}

long long NWW(long long a, long long b){
    if(a == 0 || b == 0) return 0;
    long long g = gcd_ll(a,b);
    long long prod = a/g * b;
    if(prod >= (1LL << 33)) return 0;
    return prod;
}

void log_precomp(){
    lg[1]  = 0;
    for(int i = 2; i < MAXN; i++) lg[i] = lg[i/2] + 1;
}

void sparseTable_build(){
    for(int i = 1; i <= n; i++) st[0][i] = t[i];
    for(int i = 1; i < LOGN; i++){
        for(int j = 1; j + (1 << i) - 1 <= n; j++){
            st[i][j] = NWW(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
        }
    }
}

long long query(int l, int r){
    if(l > r) return 0;
    int i = lg[r - l + 1];
    return NWW(st[i][l], st[i][r - (1 << i) + 1]);
}

int find_max(int idx){
    int l = 0, r = idx - 1;
    int res = 0;
    while(l <= r){
        int mid = (l + r)/2;
        long long nww = query(idx - mid, idx - 1);
        if(nww == 0 || t[idx] % nww != 0) {
            r = mid - 1;
        } else {
            if(mid > res) res = mid;
            l = mid + 1;
        }
    }
    return res;
}

int main(){
    scanf("%lld", &n);
    for(int i = 1; i <= n; i++) scanf("%lld", &t[i]);

    log_precomp();
    sparseTable_build();

    int res = 0;
    for(int i = 1; i <= n; i++) {
        int cand = find_max(i);
        if(cand > res) res = cand;
    }

    printf("%d\n", res + 1);
    return 0;
}
