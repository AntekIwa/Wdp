#include "zbior_ary.h"
#include <stdlib.h>
#include <assert.h>

static int g_q = 0;

static void z_init(zbior_ary* z){
    z->data = NULL; 
    z->size = 0; 
    z->cap = 0;
}
static int next_cap(int c){
    if(c < 8) return 8; 
    long long n = c + c/2 + 1; 
    if(n > 2147483640) n = 2147483640; 
    return (int)n; 
}
static void z_reserve(zbior_ary* z, int need){
    if(need <= z->cap) return;
    int nc = z->cap; 
    while(nc < need) nc = next_cap(nc);
    Segment* p = (Segment*)realloc(z->data, (size_t)nc * sizeof(Segment));
    z->data = p; 
    z->cap = nc;
}
static void z_push(zbior_ary* z, Segment s){
    if(z->size == z->cap) z_reserve(z, z->size ? z->size*2 : 8);
    z->data[z->size++] = s;
}
static void z_push_merged(zbior_ary* z, Segment s){
    if(z->size > 0){
        Segment* last = &z->data[z->size-1];
        if(last->r == s.r && s.a <= last->b + 1){
            if(s.b > last->b) last->b = s.b;
            return;
        }
    }
    z_push(z, s);
}

static int cmp_seg(Segment A, Segment B){
    if(A.r != B.r) return A.r < B.r ? -1 : 1;
    if(A.a != B.a) return A.a < B.a ? -1 : 1;
    if(A.b != B.b) return A.b < B.b ? -1 : 1;
    return 0;
}

zbior_ary ciag_arytmetyczny(int a, int q, int b){
    if(!g_q) g_q = q;
    zbior_ary Z; 
    z_init(&Z);
    int r = a % g_q; 
    if(r < 0) r += g_q;
    int t0 = (a - r)/g_q, t1 = (b - r)/g_q; 
    if(t0 > t1){
        int t = t0;
        t0 = t1;
        t1 = t;
    }
    z_push(&Z, (Segment){t0, t1, r});
    return Z;
}

zbior_ary singleton(int a){
    zbior_ary Z; 
    z_init(&Z);
    int r = a%g_q; 
    if(r < 0) r += g_q;
    int t = (a - r)/g_q;
    z_push(&Z, (Segment){t, t, r});
    return Z;
}

bool nalezy(zbior_ary A, int x){
    if(A.size == 0) return false;
    int q = g_q;
    int r = x%q; 
    if(r < 0) r+=q;
    int t = (x - r)/q;
    int L = 0, R = A.size - 1;
    while(L <= R){
        int M = L + (R - L)/2;
        Segment s = A.data[M];
        if(s.r < r || (s.r == r && s.b < t)) L = M + 1;
        else if(s.r > r || (s.r == r && s.a > t)) R = M - 1;
        else return true;
    }
    return false;
}

unsigned moc(zbior_ary A){
    unsigned long long total = 0;
    for(int i = 0; i < A.size; i++) total += (unsigned long long)(A.data[i].b - A.data[i].a + 1);
    return (unsigned)total;
}

unsigned ary(zbior_ary A){ 
    return (unsigned)A.size; 
}
