// vector for struct implementation
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

int global_q;

typedef struct{
    int a, b, r; //[a,b], r = a mod q
} Segment;

typedef struct{
    Segment* data;
    int size;
    int cap;
} vector;

typedef struct {
    unsigned q;   // różnica q dla zbioru
    vector segs;  // lista segmentów (r, a, b)
} zbior_ary;

void v_init(vector* v){
    v->data = NULL;
    v->size = 0;
    v->cap = 0;
}

static int next_cap(int cur_cap){
    if(cur_cap < 8) return 8;
    long long new_cap = cur_cap + (cur_cap/2) + 1;
    if(new_cap > 2147483640) new_cap = 2147483640;
    return (int)new_cap;
}

void v_reserve(vector* v, int need){
    if(need <= v->cap) return; 
    int new_cap = v->cap;
    while(new_cap < need) new_cap = next_cap(new_cap);
    Segment* p = (Segment*)realloc(v->data, (size_t)new_cap * sizeof(Segment));
    assert(p && "realloc failed");
    v->data = p;
    v->cap = new_cap;
}

void v_push(vector* v, Segment element){
    if(v->size == v->cap) v_reserve(v, v->size ? v->size*2 : 8);
    v->data[v->size] = element;
    v->size++;
}

void zbior_init(zbior_ary* Z, int q){
    Z->q = q;
    v_init(&Z->segs);
}

void v_push_merged(vector* v, Segment s){
    if(v->size > 0){
        Segment* last = &v->data[v->size - 1];
        if(last->r == s.r && s.a <= last->b + 1){
            if(s.b > last->b) last->b = s.b;
            return;
        }
    }
    v_push(v, s);
}

int comparator(Segment A, Segment B){
    if(A.r != B.r) return  (A.r < B.r) ? -1 : 1;
    if(A.a != B.a) return (A.a < B.a) ? -1 : 1;
    if(A.b != B.b) return (A.b < B.b) ? -1 : 1;
    return 0; //te same przedzialy
}
zbior_ary ciag_arytmetyczny(int a, int q, int b){
    if(!global_q) global_q = q;
    zbior_ary Z;
    zbior_init(&Z, q);
    int r = a%global_q;
    int t0 = (a - r) / (int)Z.q;
    int t1 = (b - r) / (int)Z.q;
    v_push(&Z.segs, (Segment){t0, t1, r});
    return Z;
}

zbior_ary singleton(int a){
    zbior_ary Z;
    zbior_init(&Z, global_q);
    int r = a%global_q;
    int t = (a - r) / Z.q;
    v_push(&Z.segs, (Segment){t,t,r});
    return Z;
}

zbior_ary suma(zbior_ary A, zbior_ary B){
    zbior_ary C;
    zbior_init(&C, global_q);
    int i = 0, j = 0;
    while(i < A.segs.size || j < B.segs.size){
        if(i < A.segs.size && j < B.segs.size){
            int comp = comparator(A.segs.data[i], B.segs.data[j]);
            if(comp < 0) v_push(&C.segs, A.segs.data[i++]);
            else if(comp > 0) v_push_merged(&C.segs, B.segs.data[j++]);
            else{
                v_push_merged(&C.segs, A.segs.data[i]);
                i++;
                j++;
            }
        }
        else if(i < A.segs.size) v_push_merged(&C.segs, A.segs.data[i++]);
        else v_push_merged(&C.segs, B.segs.data[j++]);
    }
    return C;
}

zbior_ary iloczyn(zbior_ary A, zbior_ary B){
    zbior_ary C;
    zbior_init(&C, global_q);
    int i = 0, j = 0;
    while(i < A.segs.size && j < B.segs.size){
        Segment a = A.segs.data[i];
        Segment b = B.segs.data[j];
        if(a.r < b.r) i++;
        else if(a.r > b.r) j++;
        else{
            int l = (a.a > b.a) ? a.a : b.a;
            int r = (a.b < b.b) ? a.b : b.b;
            if(l <= r) v_push(&C.segs, (Segment){l, r, a.r});
            if(a.b < b.b) i++;
            else j++;
        }
    }
    return C;
}

zbior_ary roznica(zbior_ary A, zbior_ary B){
    zbior_ary C;
    zbior_init(&C, global_q);
    int i = 0, j = 0;
    while(i < A.segs.size){
        Segment x = A.segs.data[i];
        while(j < B.segs.size && (x.r > B.segs.data[j].r || (x.r == B.segs.data[j].r && x.a > B.segs.data[j].b))) j++;
        int l = x.a, r = x.b;
        while(j < B.segs.size && x.r == B.segs.data[j].r && r >= B.segs.data[j].a){
            int br = B.segs.data[j].b, bl = B.segs.data[j].a;
            if(bl > l) v_push(&C.segs, (Segment){l, bl - 1, x.r});
            if(br + 1 > l) l = br + 1; // [bl, br] pokrywa sie jakos z [l, r], zatem nowe l = br + 1
            if(l > r) break;
            j++;
        }
        if(l <= r) v_push(&C.segs, (Segment){l, r, x.r});
        i++;
    }
    return C;
}

unsigned ary(zbior_ary A){
    return (unsigned)A.segs.size;
}

unsigned moc(zbior_ary A){
    unsigned suma = 0;
    int i = 0;
    while(i < A.segs.size){
        suma += A.segs.data[i].b - A.segs.data[i].a + 1;
        i++;
    }
    return suma;
}

bool nalezy(zbior_ary A, int x){
    int left = 0;
    int right = A.segs.size - 1;
    int reszta = x%global_q;
    x = (x - reszta)/global_q;
    int mid;
    while(left <= right){
        mid = (left + right)/2;
        Segment s = A.segs.data[mid];
        if (s.r <  reszta || (s.r == reszta && s.b < x)) left = mid + 1;
        else if(s.r > reszta || (s.r == reszta && s.a > x)) right = mid - 1;
        else return true;
    }
    return false;
}

void print_zbior(const char* name, zbior_ary Z) {
    printf("%s (q=%u):", name, Z.q);
    for (int i = 0; i < Z.segs.size; i++) {
        Segment s = Z.segs.data[i];
        long A = (long)s.r + (long)Z.q * s.a;
        long B = (long)s.r + (long)Z.q * s.b;
        printf(" [r=%u, t=[%d..%d], x=[%ld..%ld]]",
               s.r, s.a, s.b, A, B);
    }
    printf("\n");
}
int main(){
    zbior_ary A = ciag_arytmetyczny(1,3,13); // {1,4,7,10,13}
    print_zbior("A", A);
    zbior_ary B = ciag_arytmetyczny(4,3,22); // {4,7,10,13,16,19,22}
    print_zbior("B", B);
    zbior_ary U = suma(A,B);
    print_zbior("U = A ∪ B", U);
    zbior_ary I = iloczyn(A,B);
    print_zbior("I = A ∩ B", I);
    zbior_ary D = roznica(U, B);
    print_zbior("D = A \\ B", D);
    return 0;
}
